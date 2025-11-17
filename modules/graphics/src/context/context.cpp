#include "cbox/graphics/context/context.hpp"
#include "../../backends/gl/device/device.hpp"
#include "cbox/core/core.hpp"
#include <stdexcept>

namespace cc {

RendererContext::~RendererContext() {
    if (device_) {
        device_.reset();
    }
}

auto RendererContext::Builder::Build() -> ref<RendererContext>{
    if (RendererContext::instance_) {
        log::Warn("Renderer context already exists, returning existing instance");
        return RendererContext::instance_;
    }

    if (api_ != RenderAPI::OpenGL) {
        std::runtime_error("Only OpenGL is currently supported");
    }

    auto ctx = ref<RendererContext>(new RendererContext());
    ctx->api_ = api_;
    ctx->vsync_ = vsync_;
    ctx->msaa_samples_ = msaa_samples_;

    RendererContext::instance_ = ctx;

    log::Info("Renderer context created with API: OpenGL");
    log::Info("  VSync: {}", vsync_ ? "Enabled" : "Disabled");
    if (msaa_samples_ > 0) {
        log::Info("  MSAA: {}x", msaa_samples_);
    }

    return ctx;
}

void RendererContext::InitializeDevice() {
    if (device_) return;

    auto device_result = GLDevice::Create();
    if (!device_result) {
        log::Error("Failed to create render device: {}", device_result.error().message());
        return;
    }

    device_ = device_result.value();
}

void RendererContext::Shutdown() {
    if (instance_) {
        instance_->device_.reset();
        instance_.reset();
        log::Info("Renderer context shutdown complete");
    }
}

}
