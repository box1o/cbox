#include "cbox/graphics/scene/material.hpp"
#include "cbox/graphics/shader/module.hpp"
#include "cbox/graphics/resources/texture.hpp"
#include "cbox/graphics/resources/sampler.hpp"
#include <glad/glad.h>

namespace cc {

auto Material::Builder::SetShader(const ref<ShaderModule>& shader) -> Builder& {
    shader_ = shader;
    return *this;
}

auto Material::Builder::SetFloat(const std::string& name, f32 value) -> Builder& {
    uniforms_[name] = value;
    return *this;
}

auto Material::Builder::SetInt(const std::string& name, i32 value) -> Builder& {
    uniforms_[name] = value;
    return *this;
}

auto Material::Builder::SetVec2(const std::string& name, const vec2f& value) -> Builder& {
    uniforms_[name] = value;
    return *this;
}

auto Material::Builder::SetVec3(const std::string& name, const vec3f& value) -> Builder& {
    uniforms_[name] = value;
    return *this;
}

auto Material::Builder::SetVec4(const std::string& name, const vec4f& value) -> Builder& {
    uniforms_[name] = value;
    return *this;
}

auto Material::Builder::SetMat3(const std::string& name, const mat3f& value) -> Builder& {
    uniforms_[name] = value;
    return *this;
}

auto Material::Builder::SetMat4(const std::string& name, const mat4f& value) -> Builder& {
    uniforms_[name] = value;
    return *this;
}

auto Material::Builder::SetTexture(const std::string& name, u32 slot, const ref<Texture>& texture)
    -> Builder& {
    textures_[name] = {slot, texture};
    return *this;
}

auto Material::Builder::SetSampler(u32 slot, const ref<Sampler>& sampler) -> Builder& {
    samplers_[slot] = sampler;
    return *this;
}

auto Material::Builder::Build() -> ref<Material> {
    auto material = ref<Material>(new Material());
    material->shader_ = shader_;
    material->uniforms_ = uniforms_;
    material->textures_ = textures_;
    material->samplers_ = samplers_;
    return material;
}

void Material::Apply() const {
    if (!shader_)
        return;

    shader_->Bind();

    for (const auto& [name, value] : uniforms_) {
        i32 loc = shader_->GetUniformLocation(name);
        if (loc < 0)
            continue;

        std::visit(
            [loc](auto&& val) {
                using T = std::decay_t<decltype(val)>;
                if constexpr (std::is_same_v<T, f32>) {
                    glUniform1f(loc, val);
                } else if constexpr (std::is_same_v<T, i32>) {
                    glUniform1i(loc, val);
                } else if constexpr (std::is_same_v<T, vec2f>) {
                    glUniform2f(loc, val.x, val.y);
                } else if constexpr (std::is_same_v<T, vec3f>) {
                    glUniform3f(loc, val.x, val.y, val.z);
                } else if constexpr (std::is_same_v<T, vec4f>) {
                    glUniform4f(loc, val.x, val.y, val.z, val.w);
                } else if constexpr (std::is_same_v<T, mat3f>) {
                    glUniformMatrix3fv(loc, 1, GL_FALSE, &val[0][0]);
                } else if constexpr (std::is_same_v<T, mat4f>) {
                    glUniformMatrix4fv(loc, 1, GL_FALSE, &val[0][0]);
                }
            },
            value);
    }

    for (const auto& [name, tex_data] : textures_) {
        const auto& [slot, texture] = tex_data;
        if (texture) {
            texture->Bind(slot);
        }
    }

    for (const auto& [slot, sampler] : samplers_) {
        if (sampler) {
            sampler->Bind(slot);
        }
    }
}

} // namespace cc
