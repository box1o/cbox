#include "cbox/graphics/framebuffer/framebuffer.hpp"
#include "cbox/graphics/framebuffer/attachment.hpp"
#include "../../backends/gl/framebuffer/framebuffer.hpp"

namespace cc {

auto Framebuffer::Builder::SetSize(u32 width, u32 height) -> Builder& {
    width_ = width;
    height_ = height;
    return *this;
}

auto Framebuffer::Builder::AddColorAttachment(const Attachment& attachment) -> Builder& {
    color_attachments_.push_back(attachment);
    return *this;
}

auto Framebuffer::Builder::SetDepthAttachment(const Attachment& attachment) -> Builder& {
    depth_attachment_ = attachment;
    return *this;
}

auto Framebuffer::Builder::SetDepthStencilAttachment(const Attachment& attachment) -> Builder& {
    depth_stencil_attachment_ = attachment;
    return *this;
}

auto Framebuffer::Builder::Build() -> result<ref<Framebuffer>> {
    const Attachment* depth_ptr =
        depth_attachment_.has_value() ? &depth_attachment_.value() : nullptr;
    const Attachment* depth_stencil_ptr =
        depth_stencil_attachment_.has_value() ? &depth_stencil_attachment_.value() : nullptr;

    return GLFramebuffer::CreateOffscreen(width_, height_, color_attachments_, depth_ptr,
                                          depth_stencil_ptr);
}

} // namespace cc
