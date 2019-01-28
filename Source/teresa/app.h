
#include "glfw_window.h"
#include "native_webgl.h"
#include "napi_utils.h"
#include <memory>
#include <thread>

namespace webgl
{
    using enum_t = GLenum;
    using bool_t = GLboolean;
    using bitfield_t = GLbitfield;
    using clampf_t = GLfloat;
    using int_t = GLint;
    using uint_t = GLuint;
}

namespace teresa
{
    class webgl_canvas
        :public node_compatible
    {
    public:
        webgl_canvas();

        void flush();

        void bind_buffer()
        {

        }

        void to_node(napi_env env_, napi_value object_) const
        {
            node_compatible::to_node(env_, object_);
            _registerWebGL_1_0_properties(env_, object_);
            _registerWebGL_1_0_methods(env_, object_);
            // set_node_property(env_, object_, u8"activeTexture", );
            set_node_property(env_, object_, u8"flush", &webgl_canvas::flush);
        }
    private:
        std::unique_ptr<glfw_window> _displayWindow;
        std::unique_ptr<native_webgl> _nativeWebGL;
        int _flushCount = 0;

        void _registerWebGL_1_0_methods(napi_env env_, napi_value object_) const;

        void _registerWebGL_1_0_properties(napi_env env_, napi_value object_) const;

        // Flips the source data along its vertical axis if true.
        bool GL_UNPACK_FLIP_Y_WEBGL = false;

        // 	Multiplies the alpha channel into the other color channels
        bool GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL = false;


        bool GL_CONTEXT_LOST_WEBGL = false;


        webgl::enum_t GL_BROWSER_DEFAULT_WEBGL = 0x9244;

        // Default color space conversion or no color space conversion.
        webgl::enum_t GL_UNPACK_COLORSPACE_CONVERSION_WEBGL = GL_BROWSER_DEFAULT_WEBGL;
    };
}