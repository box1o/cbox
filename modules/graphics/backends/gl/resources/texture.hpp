#pragma once
#include "cbox/graphics/resources/texture.hpp"

namespace cc {

class GLTexture2D : public Texture2D {
public:
    ~GLTexture2D() override;

    static auto Create(u32 width, u32 height, TextureFormat format,
                       const void* data, bool srgb, bool mipmaps)
        -> result<ref<GLTexture2D>>;

    static auto CreateFromFile(const std::filesystem::path& filepath, bool srgb, bool mipmaps)
        -> result<ref<GLTexture2D>>;

    void Bind(u32 slot = 0) const override;
    void Unbind() const override;

    u32 GetWidth() const noexcept override { return width_; }
    u32 GetHeight() const noexcept override { return height_; }
    TextureFormat GetFormat() const noexcept override { return format_; }
    TextureType GetType() const noexcept override { return TextureType::Texture2D; }

    u32 GetTextureID() const noexcept { return texture_id_; }

private:
    GLTexture2D() = default;

    u32 texture_id_{0};
    u32 width_{0};
    u32 height_{0};
    TextureFormat format_{TextureFormat::RGBA8};
};

}
