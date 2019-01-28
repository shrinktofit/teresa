#pragma once

#include <algorithm>
#include <map>
#include "glfw_window.h"

namespace teresa
{
    namespace impl
    {
        unsigned glfw_component_counter = 0;

        glfw_component::glfw_component()
        {
            if (!glfw_component_counter)
            {
                if (glfwInit() != GLFW_TRUE)
                    throw std::runtime_error("Couldn't initialize GLFW library.");

            }
            ++glfw_component_counter;
        }

        glfw_component::~glfw_component()
        {
            --glfw_component_counter;
            if (!glfw_component_counter)
                glfwTerminate();
        }

        namespace glfw
        {
            static std::map<GLFWwindow*, glfw_window*> _glfwWindowMap;

            static void framebuffer_resize_calback(GLFWwindow* window, int width, int height) {
                auto r = _glfwWindowMap.find(window);
                if (r == _glfwWindowMap.end()) {
                    return;
                }
                r->second->_on_framebuffer_resize(width, height);
            }
        }
    }

    glfw_window::glfw_window(width_type w, height_type h, const std::string & title, bool use_vulkan_)
        :_glfwWindow(nullptr)
    {
        // https://stackoverflow.com/questions/41379529/wat-does-the-vkcreateswapchainkhrinternal-drawable-creation-failed-means
        if (use_vulkan_) {
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        }
        _glfwWindow = glfwCreateWindow(static_cast<int>(w), static_cast<int>(h), title.c_str(), nullptr, nullptr);
        if (!_glfwWindow)
            throw std::runtime_error("Couldn't create GLFW window.");
        impl::glfw::_glfwWindowMap.insert({ _glfwWindow, this });
        glfwSetFramebufferSizeCallback(_glfwWindow, impl::glfw::framebuffer_resize_calback);
    }

    std::pair<glfw_window::width_type, glfw_window::height_type> glfw_window::size() const
    {
        int w = 0, h = 0;
        glfwGetWindowSize(_glfwWindow, &w, &h);
        return { static_cast<width_type>(w), static_cast<height_type>(h) };
    }

    std::pair<glfw_window::width_type, glfw_window::height_type> glfw_window::framebuffer_size() const
    {
        int w = 0, h = 0;
        glfwGetFramebufferSize(_glfwWindow, &w, &h);
        return { static_cast<width_type>(w), static_cast<height_type>(h) };
    }

    void glfw_window::make_current() const
    {
        glfwMakeContextCurrent(_glfwWindow);
    }

    void glfw_window::swap_buffers() const
    {
        glfwSwapBuffers(_glfwWindow);
    }

    unsigned glfw_window::width() const
    {
        return size().first;
    }

    unsigned glfw_window::height() const
    {
        return size().second;
    }

    void glfw_window::add_framebuffer_resize_callback(std::function<void(unsigned, unsigned)> callback_)
    {
        _framebufferResizeCallbacks.push_back(callback_);
    }

    bool glfw_window::should_close() const
    {
        return glfwWindowShouldClose(_glfwWindow);
    }

    void glfw_window::react() const
    {
        glfwPollEvents();
    }

    std::any glfw_window::create_native_surface(std::any arg) const
    {
        return nullptr;
    }

    glfw_window::~glfw_window()
    {
        glfwDestroyWindow(_glfwWindow);
        impl::glfw::_glfwWindowMap.erase(_glfwWindow);
        _glfwWindow = nullptr;
    }

    void glfw_window::_on_framebuffer_resize(int width_, int height_)
    {
        for (auto &x : _framebufferResizeCallbacks) {
            x(static_cast<unsigned>(width_), static_cast<unsigned>(height_));
        }
    }
}