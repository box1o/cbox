#include "cbox/graphics/framebuffer/attachment.hpp"

namespace cc {

Attachment::Builder::Builder(AttachmentType type) : type_(type) {}

auto Attachment::Builder::SetFormat(TextureFormat format) -> Builder& {
    format_ = format;
    return *this;
}

auto Attachment::Builder::SetTexture(const ref<Texture>& texture) -> Builder& {
    texture_ = texture;
    return *this;
}

Attachment Attachment::Builder::Build() {
    Attachment attachment;
    attachment.type_ = type_;
    attachment.format_ = format_;
    attachment.texture_ = texture_;
    return attachment;
}

}
