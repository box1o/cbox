#pragma once
#include "cbox/core/core.hpp"
#include <filesystem>

namespace cc {

enum class TextureFormat : u8 {
    R8,
    RG8,
    RGB8,
    RGBA8,
    R16F,
    RG16F,
    RGB16F,
    RGBA16F,
    R32F,
    RG32F,
    RGB32F,
    RGBA32F,
    Depth24Stencil8,
    Depth32F
};

enum class TextureType : u8 { Texture2D, TextureCube, Texture3D };

class Texture {
  public:
    virtual ~Texture() = default;

    virtual void Bind(u32 slot = 0) const = 0;
    virtual void Unbind() const = 0;

    virtual u32 GetWidth() const noexcept = 0;
    virtual u32 GetHeight() const noexcept = 0;
    virtual TextureFormat GetFormat() const noexcept = 0;
    virtual TextureType GetType() const noexcept = 0;

  protected:
    Texture() = default;
};

class Texture2D : public Texture {
  public:
    class Builder {
      public:
        Builder(u32 width, u32 height, TextureFormat format);
        explicit Builder(const std::filesystem::path& filepath);

        Builder& SetData(const void* data, u32 size);
        Builder& SetSRGB(bool srgb);
        Builder& GenerateMipmaps();

        ref<Texture2D> Build();

      private:
        u32 width_{0};
        u32 height_{0};
        TextureFormat format_{TextureFormat::RGBA8};
        const void* data_{nullptr};
        u32 data_size_{0};
        bool srgb_{false};
        bool generate_mipmaps_{false};
        std::filesystem::path filepath_;
    };

    static Builder Create(u32 width, u32 height, TextureFormat format = TextureFormat::RGBA8) {
        return Builder(width, height, format);
    }

    static Builder FromFile(const std::filesystem::path& filepath) {
        return Builder(filepath);
    }

  protected:
    Texture2D() = default;
};

} // namespace cc
