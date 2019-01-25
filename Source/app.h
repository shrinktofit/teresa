
#include "glfw_window.h"
#include "native_webgl.h"
#include "napi_utils.h"
#include <memory>
#include <thread>

namespace teresa
{
    class webgl_canvas
        :public node_compatible
    {
    public:
        webgl_canvas();

        void flush();

        void to_node(napi_env env_, napi_value object_) const
        {
            node_compatible::to_node(env_, object_);
            set_node_property(env_, object_, u8"flush", &webgl_canvas::flush);
        }
    private:
        std::unique_ptr<glfw_window> _displayWindow;
        std::unique_ptr<native_webgl> _nativeWebGL;
        int _flushCount = 0;
    };
}