#include "sampler.hpp"
#include "cbox/core/core.hpp"
#include <glad/glad.h>

namespace cc {

static GLenum ToGLFilter(Filter filter) {
    switch (filter) {
        case Filter::Nearest: return GL_NEAREST;
        case Filter::Linear:  return GL_LINEAR;
    }
    return GL_LINEAR;
}

static GLenum ToGLMipmapFilter(Filter min, MipmapMode mipmap) {
    if (min == Filter::Nearest) {
        return mipmap == MipmapMode::Nearest ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_LINEAR;
    } else {
        return mipmap == MipmapMode::Nearest ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_LINEAR;
    }
}

static GLenum ToGLWrapMode(WrapMode mode) {
    switch (mode) {
        case WrapMode::Repeat:         return GL_REPEAT;
        case WrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
        case WrapMode::ClampToEdge:    return GL_CLAMP_TO_EDGE;
        case WrapMode::ClampToBorder:  return GL_CLAMP_TO_BORDER;
    }
    return GL_REPEAT;
}

GLSampler::~GLSampler() {
    if (sampler_id_ != 0) {
        glDeleteSamplers(1, &sampler_id_);
    }
}

auto GLSampler::Create(Filter min, Filter mag, MipmapMode mipmap,
                       WrapMode wrap_u, WrapMode wrap_v, WrapMode wrap_w,
                       f32 anisotropy, const vec4f& border_color)
    -> result<ref<GLSampler>> {
    auto sampler = ref<GLSampler>(new GLSampler());

    glGenSamplers(1, &sampler->sampler_id_);
    glSamplerParameteri(sampler->sampler_id_, GL_TEXTURE_MIN_FILTER, ToGLMipmapFilter(min, mipmap));
    glSamplerParameteri(sampler->sampler_id_, GL_TEXTURE_MAG_FILTER, ToGLFilter(mag));
    glSamplerParameteri(sampler->sampler_id_, GL_TEXTURE_WRAP_S, ToGLWrapMode(wrap_u));
    glSamplerParameteri(sampler->sampler_id_, GL_TEXTURE_WRAP_T, ToGLWrapMode(wrap_v));
    glSamplerParameteri(sampler->sampler_id_, GL_TEXTURE_WRAP_R, ToGLWrapMode(wrap_w));

    if (anisotropy > 1.0f) {
        glSamplerParameterf(sampler->sampler_id_, GL_TEXTURE_MAX_ANISOTROPY, anisotropy);
    }

    f32 border[4] = {border_color.x, border_color.y, border_color.z, border_color.w};
    glSamplerParameterfv(sampler->sampler_id_, GL_TEXTURE_BORDER_COLOR, border);

    return ok(sampler);
}

void GLSampler::Bind(u32 slot) const {
    glBindSampler(slot, sampler_id_);
}

}
