#pragma once
#include "cbox/graphics/device/device.hpp"

namespace cc {

class GLDevice : public RenderDevice {
public:
    ~GLDevice() override;

    static auto Create() -> result<ref<GLDevice>>;

    void Clear(const ClearColor& color, f32 depth, u8 stencil) override;
    void SetViewport(u32 x, u32 y, u32 width, u32 height) override;
    void SetVSync(bool enabled) override;

    const DeviceCapabilities& GetCapabilities() const noexcept override { return capabilities_; }
    RenderAPI GetAPI() const noexcept override { return RenderAPI::OpenGL; }

private:
    GLDevice() = default;

    auto Initialize() -> result<void>;
    void QueryCapabilities();

    DeviceCapabilities capabilities_;
    bool initialized_{false};
};

}
