#pragma once
#include "cbox/core/core.hpp"
#include "cbox/graphics/shader/compiler.hpp"
#include <string>
#include <vector>
#include <unordered_map>

namespace cc {

enum class UniformType : u8 {
    None = 0,
    Float,
    Vec2,
    Vec3,
    Vec4,
    Mat3,
    Mat4,
    Int,
    IVec2,
    IVec3,
    IVec4,
    Bool,
    Sampler2D,
    SamplerCube,
    Sampler3D
};

struct ShaderVertexAttribute {
    u32 location;
    std::string name;
    UniformType type;
    u32 offset;
};

struct UniformVariable {
    u32 binding;
    u32 location;
    std::string name;
    UniformType type;
    u32 size;
};

struct ShaderReflection {
    std::vector<ShaderVertexAttribute> attributes;
    std::vector<UniformVariable> uniforms;
    std::vector<UniformVariable> samplers;
    std::unordered_map<std::string, u32> uniform_locations;
    std::unordered_map<std::string, u32> sampler_bindings;
};

class ShaderReflector {
  public:
    ~ShaderReflector();

    static auto Create() -> result<ref<ShaderReflector>>;

    auto Reflect(const std::vector<u32>& spirv, ShaderStage stage) -> result<ShaderReflection>;

  private:
    ShaderReflector() = default;
};

} // namespace cc
