#pragma once
#include "cbox/core/core.hpp"
#include "cbox/math/math.hpp"
#include <unordered_map>
#include <variant>

namespace cc {

class ShaderModule;
class Texture;
class Sampler;

using UniformValue = std::variant<f32, i32, vec2f, vec3f, vec4f, mat3f, mat4f>;

class Material {
public:
    class Builder {
    public:
        Builder() = default;

        Builder& SetShader(const ref<ShaderModule>& shader);
        Builder& SetFloat(const std::string& name, f32 value);
        Builder& SetInt(const std::string& name, i32 value);
        Builder& SetVec2(const std::string& name, const vec2f& value);
        Builder& SetVec3(const std::string& name, const vec3f& value);
        Builder& SetVec4(const std::string& name, const vec4f& value);
        Builder& SetMat3(const std::string& name, const mat3f& value);
        Builder& SetMat4(const std::string& name, const mat4f& value);
        Builder& SetTexture(const std::string& name, u32 slot, const ref<Texture>& texture);
        Builder& SetSampler(u32 slot, const ref<Sampler>& sampler);

        ref<Material> Build();

    private:
        ref<ShaderModule> shader_;
        std::unordered_map<std::string, UniformValue> uniforms_;
        std::unordered_map<std::string, std::pair<u32, ref<Texture>>> textures_;
        std::unordered_map<u32, ref<Sampler>> samplers_;
    };

    static Builder Create() { return Builder(); }

    void Apply() const;

    const ref<ShaderModule>& GetShader() const noexcept { return shader_; }

    template<typename T>
    void SetUniform(const std::string& name, const T& value) {
        uniforms_[name] = value;
    }

    void SetTexture(const std::string& name, u32 slot, const ref<Texture>& texture) {
        textures_[name] = {slot, texture};
    }

    void SetSampler(u32 slot, const ref<Sampler>& sampler) {
        samplers_[slot] = sampler;
    }

private:
    Material() = default;

    ref<ShaderModule> shader_;
    std::unordered_map<std::string, UniformValue> uniforms_;
    std::unordered_map<std::string, std::pair<u32, ref<Texture>>> textures_;
    std::unordered_map<u32, ref<Sampler>> samplers_;

    friend class Builder;
};

}
