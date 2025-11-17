#include "cbox/graphics/resources/texture.hpp"
#include "../../backends/gl/resources/texture.hpp"

namespace cc {

Texture2D::Builder::Builder(u32 width, u32 height, TextureFormat format)
    : width_(width), height_(height), format_(format) {}

Texture2D::Builder::Builder(const std::filesystem::path& filepath)
    : filepath_(filepath) {}

auto Texture2D::Builder::SetData(const void* data, u32 size) -> Builder& {
    data_ = data;
    data_size_ = size;
    return *this;
}

auto Texture2D::Builder::SetSRGB(bool srgb) -> Builder& {
    srgb_ = srgb;
    return *this;
}

auto Texture2D::Builder::GenerateMipmaps() -> Builder& {
    generate_mipmaps_ = true;
    return *this;
}

auto Texture2D::Builder::Build() -> result<ref<Texture2D>> {
    if (!filepath_.empty()) {
        return GLTexture2D::CreateFromFile(filepath_, srgb_, generate_mipmaps_);
    }
    return GLTexture2D::Create(width_, height_, format_, data_, srgb_, generate_mipmaps_);
}

}
