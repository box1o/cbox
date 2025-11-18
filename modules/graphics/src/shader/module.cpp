#include "cbox/graphics/shader/module.hpp"
#include "cbox/graphics/shader/compiler.hpp"
#include "cbox/graphics/shader/reflection.hpp"
#include "../../backends/gl/shader/shader.hpp"
#include "cbox/core/core.hpp"
#include "glad/glad.h"
#include <format>

namespace cc {

ShaderModule::~ShaderModule() {
    if (program_id_ != 0) {
        GLShader::DeleteProgram(program_id_);
        log::Info("Shader module destroyed (program: {})", program_id_);
    }
}

auto ShaderModule::Builder::AddStage(ShaderStage stage, const std::filesystem::path& filepath) -> Builder& {
    auto compiler_result = ShaderCompiler::Create();
    if (!compiler_result) {
        log::Error("Failed to create shader compiler: {}", compiler_result.error().message());
        return *this;
    }

    auto spirv_result = compiler_result.value()->CompileFile(filepath, stage);
    if (!spirv_result) {
        log::Error("Failed to compile shader file '{}': {}", 
                   filepath.string(), spirv_result.error().message());
        return *this;
    }

    StageData data;
    data.stage = stage;
    data.spirv = std::move(spirv_result.value());

    stages_[stage] = std::move(data);

    return *this;
}

auto ShaderModule::Builder::AddStage(ShaderStage stage, const std::vector<u32>& spirv) -> Builder& {
    StageData data;
    data.stage = stage;
    data.spirv = spirv;

    stages_[stage] = std::move(data);

    return *this;
}

auto ShaderModule::Builder::Reflect() -> Builder& {
    reflect_ = true;
    return *this;
}

auto ShaderModule::Builder::Build() -> ref<ShaderModule> {
    if (stages_.empty()) {
        std::runtime_error("No shader stages added");
    }

    auto module = ref<ShaderModule>(new ShaderModule());

    std::vector<u32> shader_ids;

    for (auto& [stage, data] : stages_) {
        auto glsl_result = GLShader::TranspileSPIRV(data.spirv, stage);
        if (!glsl_result) {
            for (u32 id : shader_ids) {
                GLShader::DeleteShader(id);
            }
            auto msg = std::format("Failed to transpile SPIR-V to GLSL for stage {}", static_cast<u32>(stage));
            std::runtime_error(msg.c_str());
        }
        data.glsl = std::move(glsl_result.value());

        auto shader_result = GLShader::CompileShader(data.glsl, stage);
        if (!shader_result) {
            for (u32 id : shader_ids) {
                GLShader::DeleteShader(id);
            }
            auto msg = std::format("Failed to compile GLSL shader for stage {}", static_cast<u32>(stage));
            std::runtime_error(msg.c_str());
        }
        shader_ids.push_back(shader_result.value());

        if (reflect_) {
            auto reflector_result = ShaderReflector::Create();
            if (!reflector_result) {
                for (u32 id : shader_ids) {
                    GLShader::DeleteShader(id);
                }
                auto msg = std::format("Failed to create shader reflector: {}", reflector_result.error().message());
                std::runtime_error(msg.c_str());
            }

            auto reflection_result = reflector_result.value()->Reflect(data.spirv, stage);
            if (!reflection_result) {
                for (u32 id : shader_ids) {
                    GLShader::DeleteShader(id);
                }
                auto msg = std::format("Failed to reflect shader stage {}: {}", 
                                       static_cast<u32>(stage), reflection_result.error().message());
                std::runtime_error(msg.c_str());
            }

            module->reflections_[stage] = std::move(reflection_result.value());
        }
    }

    auto program_result = GLShader::LinkProgram(shader_ids);

    for (u32 id : shader_ids) {
        GLShader::DeleteShader(id);
    }

    if (!program_result) {
        auto msg = std::format("Failed to link shader program: {}", program_result.error().message());
        std::runtime_error(msg.c_str());
    }

    module->program_id_ = program_result.value();

    log::Info("Shader module created (program: {}, stages: {})", 
              module->program_id_, stages_.size());

    return module;
}

const ShaderReflection& ShaderModule::GetReflection(ShaderStage stage) const {
    static ShaderReflection empty;
    auto it = reflections_.find(stage);
    return it != reflections_.end() ? it->second : empty;
}

void ShaderModule::Bind() const {
    glUseProgram(program_id_);
}

void ShaderModule::Unbind() const {
    glUseProgram(0);
}

i32 ShaderModule::GetUniformLocation(const std::string& name) const {
    auto it = uniform_location_cache_.find(name);
    if (it != uniform_location_cache_.end()) {
        return it->second;
    }

    i32 location = glGetUniformLocation(program_id_, name.c_str());
    uniform_location_cache_[name] = location;

    return location;
}

i32 ShaderModule::GetAttributeLocation(const std::string& name) const {
    return glGetAttribLocation(program_id_, name.c_str());
}

}
