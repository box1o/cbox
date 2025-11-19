#pragma once
#include "cbox/core/core.hpp"
#include "cbox/graphics/types/types.hpp"

namespace cc {

struct DeviceCapabilities {
    u32 max_texture_size{0};
    u32 max_texture_units{0};
    u32 max_vertex_attributes{0};
    u32 max_uniform_locations{0};
    u32 max_color_attachments{0};
    f32 max_anisotropy{0.0f};
    bool supports_compute{false};
    bool supports_geometry_shader{false};
    bool supports_tessellation{false};
};

class RenderDevice {
  public:
    virtual ~RenderDevice() = default;

    virtual void Clear(const ClearColor& color, f32 depth, u8 stencil) = 0;
    virtual void SetViewport(u32 x, u32 y, u32 width, u32 height) = 0;
    virtual void SetVSync(bool enabled) = 0;

    virtual const DeviceCapabilities& GetCapabilities() const noexcept = 0;
    virtual RenderAPI GetAPI() const noexcept = 0;
};

} // namespace cc
