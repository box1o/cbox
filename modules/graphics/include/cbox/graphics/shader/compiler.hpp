#pragma once
#include "cbox/core/core.hpp"
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace cc {

enum class ShaderStage : u8 { Vertex, Fragment, Geometry, Compute, TessControl, TessEvaluation };

struct ShaderCompileOptions {
    bool optimize{true};
    bool debug_info{false};
    bool warnings_as_errors{false};
    std::string entry_point{"main"};
};

class ShaderCompiler {
  public:
    ~ShaderCompiler();

    static auto Create() -> result<ref<ShaderCompiler>>;

    auto CompileFile(const std::filesystem::path& filepath, ShaderStage stage,
                     const ShaderCompileOptions& options = {}) -> result<std::vector<u32>>;

    auto CompileSource(std::string_view source, std::string_view name, ShaderStage stage,
                       const ShaderCompileOptions& options = {}) -> result<std::vector<u32>>;

  private:
    ShaderCompiler() = default;
    auto Initialize() -> result<void>;

    void* compiler_{nullptr};
    void* options_{nullptr};
};

} // namespace cc
