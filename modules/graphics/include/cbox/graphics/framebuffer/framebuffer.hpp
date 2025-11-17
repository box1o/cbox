#pragma once
#include "cbox/core/core.hpp"
#include "cbox/graphics/framebuffer/attachment.hpp"
#include <optional>

namespace cc {

class Texture;

class Framebuffer {
public:
    class Builder {
    public:
        Builder() = default;

        Builder& SetSize(u32 width, u32 height);
        Builder& AddColorAttachment(const Attachment& attachment);
        Builder& SetDepthAttachment(const Attachment& attachment);
        Builder& SetDepthStencilAttachment(const Attachment& attachment);

        result<ref<Framebuffer>> Build();

    private:
        u32 width_{0};
        u32 height_{0};
        std::vector<Attachment> color_attachments_;
        std::optional<Attachment> depth_attachment_;
        std::optional<Attachment> depth_stencil_attachment_;
    };

    virtual ~Framebuffer() = default;

    static Builder Create(u32 width, u32 height) {
        return Builder().SetSize(width, height);
    }

    virtual u32 GetWidth() const noexcept = 0;
    virtual u32 GetHeight() const noexcept = 0;
    virtual u32 GetFramebufferID() const noexcept = 0;
    
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual ref<Texture> GetColorTexture(u32 index = 0) const = 0;
    virtual ref<Texture> GetDepthTexture() const = 0;

protected:
    Framebuffer() = default;
};

}
