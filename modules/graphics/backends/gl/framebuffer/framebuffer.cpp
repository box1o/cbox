#include "framebuffer.hpp"
#include "cbox/graphics/framebuffer/attachment.hpp"
#include "cbox/graphics/resources/texture.hpp"
#include "../resources/texture.hpp"
#include "cbox/core/core.hpp"
#include <glad/glad.h>
#include <stdexcept>

namespace cc {

GLFramebuffer::~GLFramebuffer() {
    if (framebuffer_id_ != 0 && !is_default_) {
        glDeleteFramebuffers(1, &framebuffer_id_);
    }
}

auto GLFramebuffer::CreateDefault(u32 width, u32 height) -> result<ref<GLFramebuffer>> {
    auto fb = ref<GLFramebuffer>(new GLFramebuffer());
    fb->width_ = width;
    fb->height_ = height;
    fb->is_default_ = true;
    fb->framebuffer_id_ = 0;

    return ok(fb);
}

auto GLFramebuffer::CreateOffscreen(u32 width, u32 height,
                                    const std::vector<Attachment>& color_attachments,
                                    const Attachment* depth_attachment,
                                    const Attachment* depth_stencil_attachment)
-> result<ref<GLFramebuffer>> {
    if (width == 0 || height == 0) {
        return err(error_code::validation_invalid_state, "Framebuffer dimensions cannot be 0");
    }

    auto fb = ref<GLFramebuffer>(new GLFramebuffer());
    fb->width_ = width;
    fb->height_ = height;
    fb->is_default_ = false;

    glGenFramebuffers(1, &fb->framebuffer_id_);
    glBindFramebuffer(GL_FRAMEBUFFER, fb->framebuffer_id_);

    for (u32 i = 0; i < color_attachments.size(); ++i) {
        const auto& attachment = color_attachments[i];

        if (!attachment.GetTexture()) {
            auto tex_result = Texture2D::Create(width, height, attachment.GetFormat()).Build();
            if (!tex_result) {
                glDeleteFramebuffers(1, &fb->framebuffer_id_);
                std::runtime_error("Failed to create color attachment texture for framebuffer");
            }
            fb->color_textures_.push_back(tex_result);
        } else {
            fb->color_textures_.push_back(attachment.GetTexture());
        }

        auto gl_tex = std::dynamic_pointer_cast<GLTexture2D>(fb->color_textures_[i]);
        if (gl_tex) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, 
                                   GL_TEXTURE_2D, 
                                   gl_tex->GetTextureID(),
                                   0);
        }
    }

    if (depth_stencil_attachment) {
        auto tex_result = Texture2D::Create(width, height, depth_stencil_attachment->GetFormat()).Build();
        if (!tex_result) {
            glDeleteFramebuffers(1, &fb->framebuffer_id_);
            std::runtime_error("Failed to create depth-stencil attachment texture for framebuffer");
        }
        fb->depth_texture_ = tex_result;

        auto gl_tex = std::dynamic_pointer_cast<GLTexture2D>(fb->depth_texture_);
        if (gl_tex) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                   GL_TEXTURE_2D,
                                   gl_tex->GetTextureID(),
                                   0);
        }
    } else if (depth_attachment) {
        auto tex_result = Texture2D::Create(width, height, depth_attachment->GetFormat()).Build();
        if (!tex_result) {
            glDeleteFramebuffers(1, &fb->framebuffer_id_);
            std::runtime_error("Failed to create depth attachment texture for framebuffer");
        }
        fb->depth_texture_ = tex_result;

        auto gl_tex = std::dynamic_pointer_cast<GLTexture2D>(fb->depth_texture_);
        if (gl_tex) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                   GL_TEXTURE_2D,
                                   gl_tex->GetTextureID(),
                                   0);
        }
    }

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        glDeleteFramebuffers(1, &fb->framebuffer_id_);
        return err(error_code::unknown_error, "Framebuffer is incomplete");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    log::Info("Created offscreen framebuffer ({}x{}, {} color attachments)", 
              width, height, color_attachments.size());

    return ok(fb);
}

void GLFramebuffer::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id_);
    glViewport(0, 0, width_, height_);
}

void GLFramebuffer::Unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ref<Texture> GLFramebuffer::GetColorTexture(u32 index) const {
    if (index < color_textures_.size()) {
        return color_textures_[index];
    }
    return nullptr;
}

ref<Texture> GLFramebuffer::GetDepthTexture() const {
    return depth_texture_;
}

void GLFramebuffer::Resize(u32 width, u32 height) {
    width_ = width;
    height_ = height;
}

}
