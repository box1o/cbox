#pragma once
#include "cbox/graphics/framebuffer/framebuffer.hpp"

namespace cc {

class Window;
class Attachment;

class GLFramebuffer : public Framebuffer {
public:
    ~GLFramebuffer() override;

    static auto CreateDefault(u32 width, u32 height) -> result<ref<GLFramebuffer>>;
    
    static auto CreateOffscreen(u32 width, u32 height,
                                const std::vector<Attachment>& color_attachments,
                                const Attachment* depth_attachment,
                                const Attachment* depth_stencil_attachment)
        -> result<ref<GLFramebuffer>>;

    u32 GetWidth() const noexcept override { return width_; }
    u32 GetHeight() const noexcept override { return height_; }
    u32 GetFramebufferID() const noexcept override { return framebuffer_id_; }
    
    void Bind() const override;
    void Unbind() const override;

    ref<Texture> GetColorTexture(u32 index = 0) const override;
    ref<Texture> GetDepthTexture() const override;

    void Resize(u32 width, u32 height);

private:
    GLFramebuffer() = default;

    u32 framebuffer_id_{0};
    u32 width_{0};
    u32 height_{0};
    std::vector<ref<Texture>> color_textures_;
    ref<Texture> depth_texture_;
    bool is_default_{true};
};

}
