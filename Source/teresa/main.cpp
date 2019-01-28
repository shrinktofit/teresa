
#include <node/node_api.h>
#include <iostream>
#include <memory>
#include <map>
#include <list>
#include <numeric>
#include <array>
#include "napi_utils.h"
#include "app.h"

node_ptr<teresa::webgl_canvas> create_canvas()
{
    return make_node_ptr<teresa::webgl_canvas>();
}

void destroy_canvas(node_ptr<teresa::webgl_canvas> canvas_)
{
    destroy_node_ptr(canvas_);
}

napi_value Init(napi_env env_, napi_value exports_)
{
    set_node_property(env_, exports_, u8"createCanvas", create_canvas);
    set_node_property(env_, exports_, u8"destroyCanvas", destroy_canvas);
    return exports_;
}

NAPI_MODULE(native_webgl, Init)
