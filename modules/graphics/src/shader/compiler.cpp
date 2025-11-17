#include "cbox/graphics/shader/compiler.hpp"
#include "cbox/core/core.hpp"
#include <shaderc/shaderc.hpp>
#include <fstream>
#include <sstream>

namespace cc {

static shaderc_shader_kind ToShaderCKind(ShaderStage stage) {
    switch (stage) {
        case ShaderStage::Vertex: return shaderc_vertex_shader;
        case ShaderStage::Fragment: return shaderc_fragment_shader;
        case ShaderStage::Geometry: return shaderc_geometry_shader;
        case ShaderStage::Compute: return shaderc_compute_shader;
        case ShaderStage::TessControl: return shaderc_tess_control_shader;
        case ShaderStage::TessEvaluation: return shaderc_tess_evaluation_shader;
    }
    return shaderc_vertex_shader;
}

ShaderCompiler::~ShaderCompiler() {
    if (options_) {
        delete static_cast<shaderc::CompileOptions*>(options_);
    }
    if (compiler_) {
        delete static_cast<shaderc::Compiler*>(compiler_);
    }
}

auto ShaderCompiler::Create() -> result<ref<ShaderCompiler>> {
    auto compiler = ref<ShaderCompiler>(new ShaderCompiler());

    if (auto res = compiler->Initialize(); !res) {
        return err(res.error());
    }

    return ok(compiler);
}

auto ShaderCompiler::Initialize() -> result<void> {
    compiler_ = new shaderc::Compiler();
    options_ = new shaderc::CompileOptions();

    if (!static_cast<shaderc::Compiler*>(compiler_)->IsValid()) {
        return err(error_code::unknown_error, "Failed to initialize shaderc compiler");
    }

    log::Info("Shader compiler initialized");
    return ok();
}

auto ShaderCompiler::CompileFile(const std::filesystem::path& filepath, ShaderStage stage,
                                  const ShaderCompileOptions& options) -> result<std::vector<u32>> {
    if (!std::filesystem::exists(filepath)) {
        return err(error_code::file_not_found, 
                   std::format("Shader file not found: {}", filepath.string()));
    }

    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        return err(error_code::file_access_denied,
                   std::format("Failed to open shader file: {}", filepath.string()));
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    return CompileSource(source, filepath.filename().string(), stage, options);
}

auto ShaderCompiler::CompileSource(std::string_view source, std::string_view name, ShaderStage stage,
                                    const ShaderCompileOptions& options) -> result<std::vector<u32>> {
    auto* compiler = static_cast<shaderc::Compiler*>(compiler_);
    auto* compile_opts = static_cast<shaderc::CompileOptions*>(options_);

    compile_opts->SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
    compile_opts->SetAutoBindUniforms(true);
    compile_opts->SetAutoMapLocations(true);

    if (options.optimize) {
        compile_opts->SetOptimizationLevel(shaderc_optimization_level_performance);
    }

    if (options.debug_info) {
        compile_opts->SetGenerateDebugInfo();
    }

    if (options.warnings_as_errors) {
        compile_opts->SetWarningsAsErrors();
    }

    shaderc::SpvCompilationResult result = compiler->CompileGlslToSpv(
        source.data(), source.size(),
        ToShaderCKind(stage),
        name.data(),
        options.entry_point.c_str(),
        *compile_opts
    );

    if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
        return err(error_code::parse_invalid_format,
                   std::format("Shader compilation failed: {}", result.GetErrorMessage()));
    }

    std::vector<u32> spirv(result.cbegin(), result.cend());

    log::Info("Compiled shader '{}' to SPIR-V ({} bytes)", name, spirv.size() * 4);

    return ok(std::move(spirv));
}

}
