#include "cbox/graphics/swapchain/swapchain.hpp"
#include "cbox/graphics/window/window.hpp"
#include "cbox/graphics/context/context.hpp"
#include "cbox/graphics/framebuffer/framebuffer.hpp"
#include "../../backends/gl/framebuffer/framebuffer.hpp"
#include "cbox/core/core.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace cc {

Swapchain::~Swapchain() {
    framebuffer_.reset();
    window_.reset();
    log::Info("Swapchain destroyed");
}

auto Swapchain::Create(const ref<Window>& window) -> result<ref<Swapchain>> {
    if (!window) {
        return err(error_code::validation_null_value, "Window is null");
    }

    glfwMakeContextCurrent(window->GetNativeWindow());

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return err(error_code::unknown_error, "Failed to initialize GLAD");
    }

    log::Info("OpenGL loaded");
    log::Info("  Vendor:   {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    log::Info("  Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    log::Info("  Version:  {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity,
                              GLsizei length, const GLchar* message, const void* userParam) {
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;

        if (severity == GL_DEBUG_SEVERITY_HIGH) {
            log::Error("OpenGL: {}", message);
        } else if (severity == GL_DEBUG_SEVERITY_MEDIUM) {
            log::Warn("OpenGL: {}", message);
        }
    }, nullptr);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    auto ctx = RendererContext::Get();
    if (ctx) {
        ctx->InitializeDevice();
    }

    auto swapchain = ref<Swapchain>(new Swapchain());
    swapchain->window_ = window;

    auto fb_result = GLFramebuffer::CreateDefault(window->GetWidth(), window->GetHeight());
    if (!fb_result) {
        return err(fb_result.error());
    }
    swapchain->framebuffer_ = fb_result.value();
    swapchain->initialized_ = true;

    if (window->GetVSync()) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    log::Info("Swapchain created for window ({}x{})", window->GetWidth(), window->GetHeight());

    return ok(swapchain);
}

void Swapchain::Clear(const ClearColor& color, f32 depth) {
    glClearColor(color.r, color.g, color.b, color.a);
    glClearDepth(depth);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Swapchain::Present() {
    glfwSwapBuffers(window_->GetNativeWindow());
}

void Swapchain::Present(const ref<CommandBuffer>& cmd) {
    glfwSwapBuffers(window_->GetNativeWindow());
}

u32 Swapchain::GetWidth() const noexcept {
    return window_->GetWidth();
}

u32 Swapchain::GetHeight() const noexcept {
    return window_->GetHeight();
}

}
