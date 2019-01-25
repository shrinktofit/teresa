
#pragma once

#include <node_api.h>
#include <tuple>
#include <string>
#include <array>
#include <type_traits>
#include <functional>
#include <utility>

namespace std
{
    // This implementation requires that To is trivially default constructible
    template <class To, class From,
        typename = std::enable_if_t<(sizeof(To) == sizeof(From)) && std::is_trivially_copyable<From>::value && std::is_trivial<To>::value, To>>
        constexpr To bit_cast(const From &src) noexcept
    {
        To dst;
        std::memcpy(&dst, &src, sizeof(To));
        return dst;
    }
}

struct node_compatible
{
    constexpr static const char *native_handle_property_name = "__native_handle";

    void to_node(napi_env env_, napi_value object_) const;
};

extern std::list<node_compatible*> node_objects;

template <typename Ty>
class node_ptr
{
public:
    using value_type = Ty;

    node_ptr()
        :_ptr(nullptr)
    {

    }

    node_ptr(Ty *ptr_)
        :_ptr(ptr_)
    {

    }

    Ty* get()
    {
        return _ptr;
    }

    const Ty* get() const
    {
        return _ptr;
    }

    Ty* operator->()
    {
        return _ptr;
    }

    const Ty* operator->() const
    {
        return _ptr;
    }

    void reset()
    {
        _ptr = nullptr;
    }
private:
    Ty *_ptr;
};

template <typename Ty, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<node_compatible, Ty>>>
node_ptr<Ty> make_node_ptr(Args&& ...args)
{
    auto p = new Ty(std::forward<Args>(args)...);
    node_objects.push_back(p);
    return p;
}

template <typename Ty>
void destroy_node_ptr(node_ptr<Ty> ptr_)
{
    node_objects.remove(ptr_.get());
    ptr_.reset();
}

template <typename Ty>
struct is_node_ptr
    :public std::false_type
{

};

template <typename Ty>
struct is_node_ptr<node_ptr<Ty>>
    :public std::true_type
{

};

template <typename Ty>
constexpr bool is_node_ptr_v = is_node_ptr<Ty>::value;

struct global_napi_callback_data {
    std::function<napi_value(napi_env, napi_callback_info)> unpacker;
};

using global_napi_callback_data_t = global_napi_callback_data * ;

extern std::list<global_napi_callback_data_t*> global_napi_callback_datas;

napi_value global_napi_callback(napi_env env_, napi_callback_info callback_info_);

template <typename ...Tys, std::size_t ...Is>
void _read_node_function_args_impl(napi_env env_, napi_value *args_, std::tuple<Tys...> &result_, std::index_sequence<Is...>)
{
    using Result = std::tuple<Tys...>;
    auto extractOne = [&](napi_env env_, napi_value arg_, auto &ret_) {
        ret_ = read_node_value<std::decay_t<decltype(ret_)>>(env_, arg_);
    };
    (extractOne(env_, args_[Is], std::get<Is>(result_)), ...);
}

template <typename ...Args>
std::tuple<Args...> _read_node_function_args(napi_env env_, napi_callback_info callback_info_)
{
    std::array<napi_value, sizeof...(Args)> args = {};
    std::size_t argc = args.size();
    napi_value thisArg = nullptr;
    void *data = nullptr;
    napi_get_cb_info(env_, callback_info_, &argc, args.data(), &thisArg, &data);

    if (args.size() != argc) {
        napi_throw_error(env_, nullptr, "Unmatched arguments count.");
    }

    std::tuple<Args...> result;
    _read_node_function_args_impl(env_, args.data(), result, std::index_sequence_for<Args...>());
    return result;
}

template <typename ReturnTy, typename ...Args>
using _bound_fx_t = std::function<ReturnTy(Args...)>;

template <typename ReturnTy, typename ...Args>
using _bound_fx_getter_t = std::function<_bound_fx_t<ReturnTy, Args...>(napi_env, napi_callback_info)>;

template <typename ReturnTy, typename ...Args>
napi_value _create_node_function_like(napi_env env_, _bound_fx_getter_t<ReturnTy, Args...> bound_fx_getter_)
{
    auto invoker = [bound_fx_getter_](napi_env env_, napi_callback_info callback_info_) {
        auto boundFx = bound_fx_getter_(env_, callback_info_);
        auto args = _read_node_function_args<Args...>(env_, callback_info_);
        if constexpr (std::is_same_v<ReturnTy, void>) {
            std::apply(boundFx, std::move(args));
            return nullptr;
        }
        else {
            auto retval = std::apply(boundFx, std::move(args));
            return create_node_value(env_, retval);
        }
    };

    global_napi_callback_data_t data = new global_napi_callback_data{ invoker };
    napi_value result;
    auto status = napi_create_function(env_, nullptr, 0, global_napi_callback, std::bit_cast<void*>(data), &result);
    if (status != napi_ok) {
        napi_throw_error(env_, nullptr, "Unable to wrap native function.");
    }
    return result;
}

template <typename ReturnTy, typename ...Args>
napi_value _create_node_function(napi_env env_, ReturnTy (*fx_)(Args...))
{
    std::function<ReturnTy(Args...)> fx(fx_);

    _bound_fx_getter_t<ReturnTy, Args...> boundFxGetter =
        [fx](napi_env env_, napi_callback_info callback_info_)
    {
        return fx;
    };

    return _create_node_function_like(env_, boundFxGetter);
}

template <typename ThisTy, typename ReturnTy, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<node_compatible, ThisTy>>>
napi_value _create_node_method(napi_env env_, ReturnTy (ThisTy::*fx_)(Args...))
{
    std::function<ReturnTy(ThisTy*, Args...)> fx(fx_);

    _bound_fx_getter_t<ReturnTy, Args...> boundFxGetter =
        [fx](napi_env env_, napi_callback_info callback_info_)
    {
        napi_value thisArg = nullptr;
        napi_get_cb_info(env_, callback_info_, nullptr, nullptr, &thisArg, nullptr);
        if (!thisArg) {
            napi_throw_error(env_, nullptr, "Missing this pointer.");
        }
        ThisTy *this_ = nullptr;
        read_node_property<ThisTy*>(env_, thisArg, this_, node_compatible::native_handle_property_name);
        return [this_, fx](Args&& ...args) {
            return fx(this_, std::forward<Args>(args)...);
        };
    };

    return _create_node_function_like(env_, boundFxGetter);
}

template <typename Ty>
Ty read_node_value(napi_env env_, napi_value value_)
{
    Ty result;
    if constexpr (std::is_same_v<Ty, std::int64_t>) {
        napi_get_value_int64(env_, value_, &result);
    }
    else if constexpr (std::is_same_v<Ty, std::int32_t>) {
        napi_get_value_int32(env_, value_, &result);
    }
    else if constexpr (std::is_same_v<Ty, std::uint32_t>) {
        napi_get_value_uint32(env_, value_, &result);
    }
    else if constexpr (std::is_same_v<Ty, bool>) {
        napi_get_value_bool(env_, value_, &result);
    }
    else if constexpr (std::is_same_v<Ty, double>) {
        napi_get_value_double(env_, value_, &result);
    }
    else if constexpr (std::is_same_v<Ty, std::string>) {
        std::vector<char> buf(512);
        std::size_t actualLength = 0;
        napi_get_value_string_utf8(env_, value_, buf.data(), buf.size(), &actualLength, &result);
        result.assign(buf.data(), buf.data() + actualLength);
    }
    else if constexpr (is_node_ptr_v<Ty>)
    {
        using ThisTy = typename Ty::value_type;
        ThisTy *this_ = nullptr;
        read_node_property<ThisTy*>(env_, value_, this_, node_compatible::native_handle_property_name);
        result = Ty(this_);
    }
    else if constexpr (std::is_pointer_v<Ty>) {
        std::int64_t i = 0;
        napi_get_value_int64(env_, value_, &i);
        result = std::bit_cast<Ty>(i);
    }
    else
    {
        static_assert(false, "Unknown type.");
    }
    return result;
}

template <typename Ty>
void read_node_property(napi_env env_, napi_value value_, Ty &result_, const char *property_name_)
{
    std::string propertyName = property_name_;
    napi_value keyName;
    napi_create_string_utf8(env_, propertyName.c_str(), propertyName.size(), &keyName);
    napi_value property = nullptr;
    napi_get_property(env_, value_, keyName, &property);
    result_ = read_node_value<Ty>(env_, property);
}

template <typename Ty>
napi_value create_node_value(napi_env env_, const Ty &value_)
{
    napi_value result = nullptr;
    if constexpr (std::is_same_v<Ty, std::int64_t>) {
        napi_create_int64(env_, value_, &result);
    }
    else if constexpr (std::is_same_v<Ty, std::int32_t>) {
        napi_create_int32(env_, value_, &result);
    }
    else if constexpr (std::is_same_v<Ty, std::uint32_t>) {
        napi_create_uint32(env_, value_, &result);
    }
    else if constexpr (std::is_same_v<Ty, bool>) {
        static_assert(false, "Unknown type.");
    }
    else if constexpr (std::is_same_v<Ty, double>) {
        napi_create_double(env_, value_, &result);
    }
    else if constexpr (std::is_same_v<Ty, std::string>) {
        napi_create_string_utf8(env_, value_.c_str(), value_.size(), &result);
    }
    else if constexpr (is_node_ptr_v<Ty>)
    {
        napi_create_object(env_, &result);
        value_->to_node(env_, result);
    }
    else if constexpr (std::is_pointer_v<Ty>) {
        auto i = std::bit_cast<std::int64_t>(value_);
        napi_create_int64(env_, i, &result);
    }
    else if constexpr (std::is_function_v<Ty>) {
        result = _create_node_function(env_, value_);
    }
    else if constexpr (std::is_member_function_pointer_v<Ty>) {
        result = _create_node_method(env_, value_);
    }
    else
    {
        static_assert(false, "Unknown type.");
    }
    return result;
}

template <typename Ty>
void set_node_property(napi_env env_, napi_value object_, const char *property_name_, const Ty &value_)
{
    napi_set_named_property(env_, object_, property_name_, create_node_value(env_, value_));
}
