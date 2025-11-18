#include "cbox/graphics/swapchain/swapchain.hpp"
#include "cbox/graphics/window/window.hpp"
#include "cbox/graphics/context/context.hpp"
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

auto Swapchain::Create(const ref<Window>& window) -> ref<Swapchain> {
    if (!window) {
        std::runtime_error("Window is null");
    }

    glfwMakeContextCurrent(window->GetNativeWindow());

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::runtime_error("Failed to initialize GLAD");
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

    auto ctx = RendererContext::Get();
    if (ctx) {
        ctx->InitializeDevice();
    }

    auto swapchain = ref<Swapchain>(new Swapchain());
    swapchain->window_ = window;

    auto fb_result = GLFramebuffer::CreateDefault(window->GetWidth(), window->GetHeight());
    if (!fb_result) {
        std::runtime_error("Failed to create default framebuffer for swapchain");
    }
    swapchain->framebuffer_ = fb_result.value();
    swapchain->initialized_ = true;

    if (window->GetVSync()) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW); 

    log::Info("Swapchain created for window ({}x{})", window->GetWidth(), window->GetHeight());

    return swapchain;
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
