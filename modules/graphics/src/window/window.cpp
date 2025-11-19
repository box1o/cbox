#include "cbox/graphics/window/window.hpp"
#include "cbox/core/core.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace cc {

static void GLFWErrorCallback(int error, const char* description) {
    log::Error("GLFW Error ({}): {}", error, description);
}

Window::~Window() {
    if (window_) {
        glfwDestroyWindow(window_);
        log::Info("Window destroyed");
    }

    window_count_--;
    if (window_count_ == 0) {
        glfwTerminate();
        log::Info("GLFW terminated");
    }
}

auto Window::Create(const WindowDesc& desc) -> ref<Window> {
    if (window_count_ == 0) {
        if (!glfwInit()) {
            std::runtime_error("Failed to initialize GLFW");
        }
        glfwSetErrorCallback(GLFWErrorCallback);
        log::Info("GLFW initialized");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, desc.resizable ? GLFW_TRUE : GLFW_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWwindow* window =
        glfwCreateWindow(static_cast<int>(desc.width), static_cast<int>(desc.height),
                         desc.name.c_str(), nullptr, nullptr);

    if (!window) {
        if (window_count_ == 0) {
            glfwTerminate();
        }
        throw std::runtime_error("Failed to create GLFW window");
    }

    auto win = ref<Window>(new Window());
    win->window_ = window;
    win->width_ = desc.width;
    win->height_ = desc.height;
    win->vsync_ = desc.vsync;
    win->should_close_ = false;

    window_count_++;

    glfwSetWindowUserPointer(window, win.get());

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* w, int width, int height) {
        auto* win_ptr = static_cast<Window*>(glfwGetWindowUserPointer(w));
        win_ptr->width_ = static_cast<u32>(width);
        win_ptr->height_ = static_cast<u32>(height);
    });

    log::Info("Window created: {} ({}x{})", desc.name, desc.width, desc.height);

    return win;
}

bool Window::ShouldClose() const noexcept {
    return should_close_ || glfwWindowShouldClose(window_);
}

void Window::PollEvents() noexcept {
    glfwPollEvents();
}

} // namespace cc
