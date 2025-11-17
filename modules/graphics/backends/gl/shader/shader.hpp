#pragma once
#include "cbox/core/core.hpp"
#include "cbox/graphics/shader/compiler.hpp"
#include <string>
#include <vector>

namespace cc {

class GLShader {
public:
    static auto TranspileSPIRV(const std::vector<u32>& spirv, ShaderStage stage) -> result<std::string>;
    
    static auto CompileShader(const std::string& source, ShaderStage stage) -> result<u32>;
    
    static auto LinkProgram(const std::vector<u32>& shader_ids) -> result<u32>;
    
    static void DeleteShader(u32 shader_id);
    static void DeleteProgram(u32 program_id);
};

}
