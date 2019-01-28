
#include "napi_utils.h"

std::list<node_compatible*> node_objects;
std::list<global_napi_callback_data_t*> global_napi_callback_datas;

void node_compatible::to_node(napi_env env_, napi_value object_) const
{
    set_node_property(env_, object_, native_handle_property_name, this);
}

napi_value global_napi_callback(napi_env env_, napi_callback_info callback_info_)
{
    void *dataraw = nullptr;
    napi_get_cb_info(env_, callback_info_, nullptr, nullptr, nullptr, &dataraw);

    auto data = static_cast<global_napi_callback_data_t>(dataraw);
    auto retval = data->unpacker(env_, callback_info_);
    return retval;
}
