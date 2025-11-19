#pragma once
#include "cbox/core/core.hpp"
#include "cbox/graphics/resources/texture.hpp"

namespace cc {

enum class AttachmentType : u8 { Color, Depth, Stencil, DepthStencil };

class Attachment {
  public:
    class Builder {
      public:
        Builder(AttachmentType type);

        Builder& SetFormat(TextureFormat format);
        Builder& SetTexture(const ref<Texture>& texture);

        Attachment Build();

      private:
        AttachmentType type_;
        TextureFormat format_{TextureFormat::RGBA8};
        ref<Texture> texture_;
    };

    static Builder CreateColor(TextureFormat format = TextureFormat::RGBA8) {
        return Builder(AttachmentType::Color).SetFormat(format);
    }

    static Builder CreateDepth(TextureFormat format = TextureFormat::Depth24Stencil8) {
        return Builder(AttachmentType::Depth).SetFormat(format);
    }

    static Builder CreateDepthStencil() {
        return Builder(AttachmentType::DepthStencil).SetFormat(TextureFormat::Depth24Stencil8);
    }

    AttachmentType GetType() const noexcept {
        return type_;
    }
    TextureFormat GetFormat() const noexcept {
        return format_;
    }
    const ref<Texture>& GetTexture() const noexcept {
        return texture_;
    }

  private:
    Attachment() = default;

    AttachmentType type_;
    TextureFormat format_;
    ref<Texture> texture_;

    friend class Builder;
};

} // namespace cc
