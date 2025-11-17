#include "cbox/graphics/commands/render_pass.hpp"

namespace cc {

auto RenderPass::Builder::SetFramebuffer(const ref<Framebuffer>& framebuffer) -> Builder& {
    framebuffer_ = framebuffer;
    return *this;
}

auto RenderPass::Builder::SetClearColor(u32 index, const vec4f& color) -> Builder& {
    if (index < 8) {
        color_attachments_[index].clear_value = color;
    }
    return *this;
}

auto RenderPass::Builder::SetClearDepth(f32 depth) -> Builder& {
    depth_stencil_.clear_depth = depth;
    return *this;
}

auto RenderPass::Builder::SetClearStencil(u32 stencil) -> Builder& {
    depth_stencil_.clear_stencil = stencil;
    return *this;
}

auto RenderPass::Builder::SetColorLoadOp(u32 index, LoadOp op) -> Builder& {
    if (index < 8) {
        color_attachments_[index].load = op;
    }
    return *this;
}

auto RenderPass::Builder::SetDepthLoadOp(LoadOp op) -> Builder& {
    depth_stencil_.depth_load = op;
    return *this;
}

auto RenderPass::Builder::Build() -> ref<RenderPass> {
    auto pass = ref<RenderPass>(new RenderPass());
    pass->framebuffer_ = framebuffer_;
    
    for (u32 i = 0; i < 8; ++i) {
        pass->color_attachments_[i] = color_attachments_[i];
    }
    
    pass->depth_stencil_ = depth_stencil_;
    pass->color_attachment_count_ = color_attachment_count_;
    
    return pass;
}

}
