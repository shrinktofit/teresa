
#pragma once

#include <node/node_api.h>
#include <tuple>
#include <string>
#include <array>
#include <type_traits>
#include <functional>
#include <utility>

static_assert(sizeof(std::intptr_t) <= sizeof(std::int64_t));

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

template <typename Ty>
class node_optional
{
public:
    Ty* operator->()
    {
        return _ptr;
    }

    const Ty* operator->() const
    {
        return _ptr;
    }
};

struct array_buffer
{
    void *data = nullptr;
    std::size_t size = 0;
};

template <typename Ty>
struct typed_array
{
    using element_type = Ty;
    Ty *data = nullptr;
    std::size_t size = 0;
    std::size_t offset = 0;
    array_buffer underlying_buffer;
};

template <typename Ty>
bool check_typed_array_type(napi_typedarray_type dest_)
{
    switch (dest_)
    {
    case napi_typedarray_type::napi_int8_array:
        return std::is_same_v<Ty, std::int8_t>;
    case napi_typedarray_type::napi_uint8_array:
        return std::is_same_v<Ty, std::uint8_t>;
    case napi_typedarray_type::napi_int16_array:
        return std::is_same_v<Ty, std::int16_t>;
    case napi_typedarray_type::napi_uint16_array:
        return std::is_same_v<Ty, std::uint16_t>;
    case napi_typedarray_type::napi_int32_array:
        return std::is_same_v<Ty, std::int32_t>;
    case napi_typedarray_type::napi_uint32_array:
        return std::is_same_v<Ty, std::uint32_t>;
    case napi_typedarray_type::napi_float32_array:
        return std::is_same_v<Ty, float>;
    case napi_typedarray_type::napi_float64_array:
        return std::is_same_v<Ty, double>;
    case napi_typedarray_type::napi_uint8_clamped_array:
    case napi_typedarray_type::napi_bigint64_array:
    case napi_typedarray_type::napi_biguint64_array:
        // throw std::runtime_error("Type not supported.");
    default:
        break;
    }
    return false;
}

template <typename Ty>
struct is_typed_array
    :public std::false_type
{

};

template <typename Ty>
struct is_typed_array<typed_array<Ty>>
    :public std::true_type
{

};

template <typename Ty>
constexpr bool is_typed_array_v = is_typed_array<Ty>::value;

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
    auto status = napi_create_function(env_, nullptr, 0, global_napi_callback, static_cast<void*>(data), &result);
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
    else if constexpr (std::is_integral_v<Ty>) {
        std::int64_t i;
        napi_get_value_int64(env_, value_, &i);
        result = static_cast<Ty>(i);
    }
    else if constexpr (std::is_same_v<Ty, float>) {
        double v = 0;
        napi_get_value_double(env_, value_, &v);
        return static_cast<float>(v);
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
    else if constexpr (std::is_same_v<Ty, array_buffer>) {
        bool isArrayBuffer = false;
        napi_is_arraybuffer(env_, value_, &isArrayBuffer);
        if (!isArrayBuffer) {
            throw std::runtime_error("Type mismatch.");
        }
        napi_get_arraybuffer_info(env_, value_, &result.data, &result.size);
    }
    else if constexpr (is_typed_array_v<Ty>) {
        bool isTypedArray = false;
        napi_is_typedarray(env_, value_, &isTypedArray);
        if (!isTypedArray) {
            throw std::runtime_error("Type mismatch.");
        }
        napi_typedarray_type type = napi_typedarray_type::napi_int8_array;
        napi_value ab = nullptr;
        void *dataNoType = nullptr;
        napi_get_typedarray_info(env_, value_, &type, &result.size, &dataNoType, &ab, &result.offset);
        if (!check_typed_array_type<typename Ty::element_type>(type)) {
            throw std::runtime_error("Element type mismatch.");
        }
        result.data = reinterpret_cast<typename Ty::element_type*>(dataNoType);
        result.underlying_buffer = read_node_value<array_buffer>(env_, ab);
    }
    else if constexpr (std::is_pointer_v<Ty>) {
        std::int64_t i = 0;
        napi_get_value_int64(env_, value_, &i);
        result = reinterpret_cast<Ty>(static_cast<std::intptr_t>(i));
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
        napi_value tempi = nullptr;
        napi_create_uint32(env_, static_cast<std::uint32_t>(value_), &tempi);
        napi_coerce_to_bool(env_, tempi, &result);
    }
    else if constexpr (std::is_integral_v<Ty>) {
        napi_create_int64(env_, static_cast<std::int64_t>(value_), &result);
    }
    else if constexpr (std::is_same_v<Ty, double>) {
        napi_create_double(env_, value_, &result);
    }
    else if constexpr (std::is_same_v<Ty, std::string>) {
        napi_create_string_utf8(env_, value_.c_str(), value_.size(), &result);
    }
    else if constexpr (is_node_ptr_v<Ty>) {
        napi_create_object(env_, &result);
        value_->to_node(env_, result);
    }
    else if constexpr (std::is_function_v<Ty>) {
        result = _create_node_function(env_, value_);
    }
    else if constexpr (std::is_member_function_pointer_v<Ty>) {
        result = _create_node_method(env_, value_);
    }
    else if constexpr (std::is_same_v<Ty, array_buffer>) {
        napi_create_arraybuffer(env_, value_.size, &value.data, &result);
    }
    else if constexpr (std::is_pointer_v<Ty>) {
        using ElementType = std::remove_pointer_t<Ty>;
        if constexpr (std::is_function_v<ElementType>) {
            result = _create_node_function(env_, *value_);
        }
        else {
            auto i = static_cast<std::int64_t>(reinterpret_cast<std::intptr_t>(value_));
            napi_create_int64(env_, i, &result);
        }
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
