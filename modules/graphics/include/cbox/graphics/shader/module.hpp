#pragma once
#include "cbox/core/core.hpp"
#include "cbox/graphics/shader/compiler.hpp"
#include "cbox/graphics/shader/reflection.hpp"
#include <string>
#include <vector>
#include <unordered_map>

namespace cc {

class ShaderModule {
  public:
    class Builder {
      public:
        Builder() = default;

        Builder& AddStage(ShaderStage stage, const std::filesystem::path& filepath);
        Builder& AddStage(ShaderStage stage, const std::vector<u32>& spirv);
        Builder& Reflect();

        ref<ShaderModule> Build();

      private:
        struct StageData {
            ShaderStage stage;
            std::vector<u32> spirv;
            std::string glsl;
            ShaderReflection reflection;
        };

        std::unordered_map<ShaderStage, StageData> stages_;
        bool reflect_{false};
    };

    ~ShaderModule();

    static Builder Create() {
        return Builder();
    }

    u32 GetProgramID() const noexcept {
        return program_id_;
    }
    const ShaderReflection& GetReflection(ShaderStage stage) const;

    void Bind() const;
    void Unbind() const;

    i32 GetUniformLocation(const std::string& name) const;
    i32 GetAttributeLocation(const std::string& name) const;

  private:
    ShaderModule() = default;

    u32 program_id_{0};
    std::unordered_map<ShaderStage, ShaderReflection> reflections_;
    mutable std::unordered_map<std::string, i32> uniform_location_cache_;
};

} // namespace cc
