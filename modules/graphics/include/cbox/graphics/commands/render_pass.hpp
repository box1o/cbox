#pragma once
#include "cbox/core/core.hpp"
#include "cbox/math/math.hpp"

namespace cc {

class Framebuffer;

enum class LoadOp : u8 {
    Load,
    Clear,
    DontCare
};

enum class StoreOp : u8 {
    Store,
    DontCare
};

struct ColorAttachment {
    LoadOp load{LoadOp::Clear};
    StoreOp store{StoreOp::Store};
    vec4f clear_value{0.0f, 0.0f, 0.0f, 1.0f};
};

struct DepthStencilAttachment {
    LoadOp depth_load{LoadOp::Clear};
    StoreOp depth_store{StoreOp::Store};
    LoadOp stencil_load{LoadOp::DontCare};
    StoreOp stencil_store{StoreOp::DontCare};
    f32 clear_depth{1.0f};
    u32 clear_stencil{0};
};

class RenderPass {
public:
    class Builder {
    public:
        Builder() = default;

        Builder& SetFramebuffer(const ref<Framebuffer>& framebuffer);
        Builder& SetClearColor(u32 index, const vec4f& color);
        Builder& SetClearDepth(f32 depth);
        Builder& SetClearStencil(u32 stencil);
        Builder& SetColorLoadOp(u32 index, LoadOp op);
        Builder& SetDepthLoadOp(LoadOp op);

        ref<RenderPass> Build();

    private:
        ref<Framebuffer> framebuffer_;
        ColorAttachment color_attachments_[8];
        DepthStencilAttachment depth_stencil_;
        u32 color_attachment_count_{1};
    };

    static Builder Create() { return Builder(); }

    const ref<Framebuffer>& GetFramebuffer() const noexcept { return framebuffer_; }
    const ColorAttachment* GetColorAttachments() const noexcept { return color_attachments_; }
    const DepthStencilAttachment& GetDepthStencil() const noexcept { return depth_stencil_; }
    u32 GetColorAttachmentCount() const noexcept { return color_attachment_count_; }

private:
    RenderPass() = default;

    ref<Framebuffer> framebuffer_;
    ColorAttachment color_attachments_[8];
    DepthStencilAttachment depth_stencil_;
    u32 color_attachment_count_{1};

    friend class Builder;
};

}
