#include "cbox/graphics/shader/reflection.hpp"
#include "cbox/core/core.hpp"
#include <spirv.hpp>
#include <spirv_cross.hpp>

namespace cc {

static UniformType SPIRTypeToUniformType(const spirv_cross::SPIRType& type) {
    if (type.basetype == spirv_cross::SPIRType::Float) {
        if (type.vecsize == 1 && type.columns == 1) return UniformType::Float;
        if (type.vecsize == 2 && type.columns == 1) return UniformType::Vec2;
        if (type.vecsize == 3 && type.columns == 1) return UniformType::Vec3;
        if (type.vecsize == 4 && type.columns == 1) return UniformType::Vec4;
        if (type.vecsize == 3 && type.columns == 3) return UniformType::Mat3;
        if (type.vecsize == 4 && type.columns == 4) return UniformType::Mat4;
    } else if (type.basetype == spirv_cross::SPIRType::Int) {
        if (type.vecsize == 1 && type.columns == 1) return UniformType::Int;
        if (type.vecsize == 2 && type.columns == 1) return UniformType::IVec2;
        if (type.vecsize == 3 && type.columns == 1) return UniformType::IVec3;
        if (type.vecsize == 4 && type.columns == 1) return UniformType::IVec4;
    } else if (type.basetype == spirv_cross::SPIRType::Boolean) {
        return UniformType::Bool;
    } else if (type.basetype == spirv_cross::SPIRType::Image) {
        if (type.image.dim == spv::Dim2D) return UniformType::Sampler2D;
        if (type.image.dim == spv::DimCube) return UniformType::SamplerCube;
        if (type.image.dim == spv::Dim3D) return UniformType::Sampler3D;
    }

    return UniformType::None;
}

static u32 GetUniformSize(UniformType type) {
    switch (type) {
        case UniformType::Float: return 4;
        case UniformType::Vec2: return 8;
        case UniformType::Vec3: return 12;
        case UniformType::Vec4: return 16;
        case UniformType::Mat3: return 36;
        case UniformType::Mat4: return 64;
        case UniformType::Int: return 4;
        case UniformType::IVec2: return 8;
        case UniformType::IVec3: return 12;
        case UniformType::IVec4: return 16;
        case UniformType::Bool: return 4;
        default: return 0;
    }
}

ShaderReflector::~ShaderReflector() {
}

auto ShaderReflector::Create() -> result<ref<ShaderReflector>> {
    auto reflector = ref<ShaderReflector>(new ShaderReflector());
    return ok(reflector);
}

auto ShaderReflector::Reflect(const std::vector<u32>& spirv, ShaderStage stage) -> result<ShaderReflection> {
    if (spirv.empty()) {
        return err(error_code::validation_invalid_state, "SPIR-V data is empty");
    }

    ShaderReflection reflection;

    try {
        spirv_cross::Compiler compiler(spirv);
        spirv_cross::ShaderResources resources = compiler.get_shader_resources();

        if (stage == ShaderStage::Vertex) {
            for (const auto& input : resources.stage_inputs) {
                ShaderVertexAttribute attr{};
                attr.name = input.name;
                attr.location = compiler.get_decoration(input.id, spv::DecorationLocation);
                
                const auto& type = compiler.get_type(input.type_id);
                attr.type = SPIRTypeToUniformType(type);
                attr.offset = 0;

                reflection.attributes.push_back(attr);
            }
        }

        for (const auto& uniform : resources.uniform_buffers) {
            const auto& type = compiler.get_type(uniform.type_id);
            u32 binding = compiler.get_decoration(uniform.id, spv::DecorationBinding);

            for (u32 i = 0; i < type.member_types.size(); ++i) {
                const auto& member_type = compiler.get_type(type.member_types[i]);
                std::string member_name = compiler.get_member_name(uniform.type_id, i);

                UniformVariable var{};
                var.name = member_name;
                var.binding = binding;
                var.location = i;
                var.type = SPIRTypeToUniformType(member_type);
                var.size = GetUniformSize(var.type);

                reflection.uniforms.push_back(var);
                reflection.uniform_locations[var.name] = var.location;
            }
        }

        for (const auto& sampler : resources.sampled_images) {
            UniformVariable var{};
            var.name = sampler.name;
            var.binding = compiler.get_decoration(sampler.id, spv::DecorationBinding);
            var.location = var.binding;
            
            const auto& type = compiler.get_type(sampler.type_id);
            var.type = SPIRTypeToUniformType(type);
            var.size = 0;

            reflection.samplers.push_back(var);
            reflection.sampler_bindings[var.name] = var.binding;
        }

        log::Info("Shader reflection:");
        log::Info("  Attributes: {}", reflection.attributes.size());
        log::Info("  Uniforms: {}", reflection.uniforms.size());
        log::Info("  Samplers: {}", reflection.samplers.size());

    } catch (const std::exception& e) {
        return err(error_code::parse_invalid_format, 
                   std::format("SPIR-V reflection failed: {}", e.what()));
    }

    return ok(std::move(reflection));
}

}
