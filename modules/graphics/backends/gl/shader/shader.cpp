#include "shader.hpp"
#include "cbox/core/core.hpp"
#include <glad/glad.h>
#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>

namespace cc {

static GLenum ToGLShaderType(ShaderStage stage) {
    switch (stage) {
        case ShaderStage::Vertex: return GL_VERTEX_SHADER;
        case ShaderStage::Fragment: return GL_FRAGMENT_SHADER;
        case ShaderStage::Geometry: return GL_GEOMETRY_SHADER;
        case ShaderStage::Compute: return GL_COMPUTE_SHADER;
        case ShaderStage::TessControl: return GL_TESS_CONTROL_SHADER;
        case ShaderStage::TessEvaluation: return GL_TESS_EVALUATION_SHADER;
    }
    return GL_VERTEX_SHADER;
}

auto GLShader::TranspileSPIRV(const std::vector<u32>& spirv, ShaderStage stage) -> result<std::string> {
    if (spirv.empty()) {
        return err(error_code::validation_invalid_state, "SPIR-V data is empty");
    }

    try {
        spirv_cross::CompilerGLSL glsl_compiler(spirv);
        
        spirv_cross::CompilerGLSL::Options options;
        options.version = 330;
        options.es = false;
        options.enable_420pack_extension = false;
        glsl_compiler.set_common_options(options);

        std::string glsl_source = glsl_compiler.compile();
        
        return ok(std::move(glsl_source));

    } catch (const std::exception& e) {
        return err(error_code::parse_invalid_format,
                   std::format("SPIR-V transpilation failed: {}", e.what()));
    }
}

auto GLShader::CompileShader(const std::string& source, ShaderStage stage) -> result<u32> {
    u32 shader_id = glCreateShader(ToGLShaderType(stage));
    
    const char* source_cstr = source.c_str();
    glShaderSource(shader_id, 1, &source_cstr, nullptr);
    glCompileShader(shader_id);

    GLint success = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        GLint log_length = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
        
        std::string info_log(log_length, '\0');
        glGetShaderInfoLog(shader_id, log_length, nullptr, info_log.data());
        
        glDeleteShader(shader_id);
        
        return err(error_code::parse_invalid_format,
                   std::format("Shader compilation failed:\n{}", info_log));
    }

    return ok(shader_id);
}

auto GLShader::LinkProgram(const std::vector<u32>& shader_ids) -> result<u32> {
    u32 program_id = glCreateProgram();

    for (u32 shader_id : shader_ids) {
        glAttachShader(program_id, shader_id);
    }

    glLinkProgram(program_id);

    GLint success = 0;
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);

    if (!success) {
        GLint log_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        std::string info_log(log_length, '\0');
        glGetProgramInfoLog(program_id, log_length, nullptr, info_log.data());

        glDeleteProgram(program_id);

        return err(error_code::parse_invalid_format,
                   std::format("Shader program linking failed:\n{}", info_log));
    }

    for (u32 shader_id : shader_ids) {
        glDetachShader(program_id, shader_id);
    }

    return ok(program_id);
}

void GLShader::DeleteShader(u32 shader_id) {
    glDeleteShader(shader_id);
}

void GLShader::DeleteProgram(u32 program_id) {
    glDeleteProgram(program_id);
}

}
