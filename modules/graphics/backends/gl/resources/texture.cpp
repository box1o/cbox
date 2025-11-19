#include "texture.hpp"
#include "cbox/core/core.hpp"
#include <glad/glad.h>
#include <stb_image.h>

namespace cc {

static GLenum ToGLInternalFormat(TextureFormat format, bool srgb) {
    switch (format) {
    case TextureFormat::R8:
        return GL_R8;
    case TextureFormat::RG8:
        return GL_RG8;
    case TextureFormat::RGB8:
        return srgb ? GL_SRGB8 : GL_RGB8;
    case TextureFormat::RGBA8:
        return srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8;
    case TextureFormat::R16F:
        return GL_R16F;
    case TextureFormat::RG16F:
        return GL_RG16F;
    case TextureFormat::RGB16F:
        return GL_RGB16F;
    case TextureFormat::RGBA16F:
        return GL_RGBA16F;
    case TextureFormat::R32F:
        return GL_R32F;
    case TextureFormat::RG32F:
        return GL_RG32F;
    case TextureFormat::RGB32F:
        return GL_RGB32F;
    case TextureFormat::RGBA32F:
        return GL_RGBA32F;
    case TextureFormat::Depth24Stencil8:
        return GL_DEPTH24_STENCIL8;
    case TextureFormat::Depth32F:
        return GL_DEPTH_COMPONENT32F;
    }
    return GL_RGBA8;
}

static GLenum ToGLFormat(TextureFormat format) {
    switch (format) {
    case TextureFormat::R8:
    case TextureFormat::R16F:
    case TextureFormat::R32F:
        return GL_RED;
    case TextureFormat::RG8:
    case TextureFormat::RG16F:
    case TextureFormat::RG32F:
        return GL_RG;
    case TextureFormat::RGB8:
    case TextureFormat::RGB16F:
    case TextureFormat::RGB32F:
        return GL_RGB;
    case TextureFormat::RGBA8:
    case TextureFormat::RGBA16F:
    case TextureFormat::RGBA32F:
        return GL_RGBA;
    case TextureFormat::Depth24Stencil8:
        return GL_DEPTH_STENCIL;
    case TextureFormat::Depth32F:
        return GL_DEPTH_COMPONENT;
    }
    return GL_RGBA;
}

static GLenum ToGLDataType(TextureFormat format) {
    switch (format) {
    case TextureFormat::R8:
    case TextureFormat::RG8:
    case TextureFormat::RGB8:
    case TextureFormat::RGBA8:
        return GL_UNSIGNED_BYTE;
    case TextureFormat::R16F:
    case TextureFormat::RG16F:
    case TextureFormat::RGB16F:
    case TextureFormat::RGBA16F:
    case TextureFormat::R32F:
    case TextureFormat::RG32F:
    case TextureFormat::RGB32F:
    case TextureFormat::RGBA32F:
        return GL_FLOAT;
    case TextureFormat::Depth24Stencil8:
        return GL_UNSIGNED_INT_24_8;
    case TextureFormat::Depth32F:
        return GL_FLOAT;
    }
    return GL_UNSIGNED_BYTE;
}

GLTexture2D::~GLTexture2D() {
    if (texture_id_ != 0) {
        glDeleteTextures(1, &texture_id_);
    }
}

auto GLTexture2D::Create(u32 width, u32 height, TextureFormat format, const void* data, bool srgb,
                         bool mipmaps) -> ref<GLTexture2D> {
    if (width == 0 || height == 0) {
        auto msg = std::format("Texture dimensions cannot be 0 ({}x{})", width, height);
        std::runtime_error(msg.c_str());
    }

    auto texture = ref<GLTexture2D>(new GLTexture2D());
    texture->width_ = width;
    texture->height_ = height;
    texture->format_ = format;

    glGenTextures(1, &texture->texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture->texture_id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    mipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, ToGLInternalFormat(format, srgb), width, height, 0,
                 ToGLFormat(format), ToGLDataType(format), data);

    if (mipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

auto GLTexture2D::CreateFromFile(const std::filesystem::path& filepath, bool srgb, bool mipmaps)
    -> ref<GLTexture2D> {
    if (!std::filesystem::exists(filepath)) {
        auto msg = std::format("Texture file not found: {}", filepath.string());
        std::runtime_error(msg.c_str());
    }

    stbi_set_flip_vertically_on_load(1);

    int width, height, channels;
    unsigned char* data = stbi_load(filepath.string().c_str(), &width, &height, &channels, 0);

    if (!data) {
        auto msg = std::format("Failed to load texture: {}", filepath.string());
        std::runtime_error(msg.c_str());
    }

    TextureFormat format;
    switch (channels) {
    case 1:
        format = TextureFormat::R8;
        break;
    case 2:
        format = TextureFormat::RG8;
        break;
    case 3:
        format = TextureFormat::RGB8;
        break;
    case 4:
        format = TextureFormat::RGBA8;
        break;
    default:
        stbi_image_free(data);
        auto msg =
            std::format("Unsupported texture channel count ({}): {}", channels, filepath.string());
        std::runtime_error(msg.c_str());
    }

    auto result = Create(width, height, format, data, srgb, mipmaps);

    stbi_image_free(data);

    if (result) {
        log::Info("Loaded texture: {} ({}x{}, {} channels)", filepath.filename().string(), width,
                  height, channels);
    }

    return result;
}

void GLTexture2D::Bind(u32 slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
}

void GLTexture2D::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace cc
