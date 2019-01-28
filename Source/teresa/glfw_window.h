#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <any>

namespace teresa
{
    namespace impl
    {
        class glfw_component
        {
        public:
            glfw_component();

            ~glfw_component();
        };

        namespace glfw
        {
            static void framebuffer_resize_calback(GLFWwindow* window, int width, int height);
        }
    }

    class glfw_window
        :public impl::glfw_component
    {
        friend void impl::glfw::framebuffer_resize_calback(GLFWwindow* window, int width, int height);
    public:
        using width_type = unsigned;

        using height_type = unsigned;

        glfw_window(width_type w, height_type h, const std::string &title, bool use_vulkan_ = false);

        std::pair<width_type, height_type> size() const;

        std::pair<width_type, height_type> framebuffer_size() const;

        void make_current() const;

        void swap_buffers() const;

        bool should_close() const;

        void react() const;

        std::any create_native_surface(std::any) const;

        unsigned width() const;

        unsigned height() const;

        void add_framebuffer_resize_callback(std::function<void(unsigned, unsigned)> callback_);

        ~glfw_window();
    private:
        GLFWwindow * _glfwWindow;

        std::list<std::function<void(unsigned, unsigned)>> _framebufferResizeCallbacks;

        void _on_framebuffer_resize(int width_, int height_);
    };
}