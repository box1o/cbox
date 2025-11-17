#pragma once
#include "cbox/graphics/commands/command_buffer.hpp"

namespace cc {

class ShaderModule;
class GLPipelineState;

class GLCommandBuffer : public CommandBuffer {
public:
    ~GLCommandBuffer() override;

    static auto Create() -> result<ref<GLCommandBuffer>>;

    void Begin() override;
    void End() override;

    void BeginRenderPass(const ref<RenderPass>& pass) override;
    void EndRenderPass() override;

    void SetPipeline(const ref<PipelineState>& pipeline) override;
    void SetVertexBuffer(const ref<Buffer>& buffer, u32 binding) override;
    void SetIndexBuffer(const ref<Buffer>& buffer) override;

    void SetUniformFloat(const std::string& name, f32 value) override;
    void SetUniformInt(const std::string& name, i32 value) override;
    void SetUniformVec2(const std::string& name, const vec2f& value) override;
    void SetUniformVec3(const std::string& name, const vec3f& value) override;
    void SetUniformVec4(const std::string& name, const vec4f& value) override;
    void SetUniformMat3(const std::string& name, const mat3f& value) override;
    void SetUniformMat4(const std::string& name, const mat4f& value) override;

    void SetTexture(u32 slot, const ref<Texture>& texture) override;
    void SetSampler(u32 slot, const ref<Sampler>& sampler) override;

    void Draw(u32 vertex_count, u32 first_vertex) override;
    void DrawIndexed(u32 index_count, u32 first_index, i32 vertex_offset) override;

    void SetViewport(f32 x, f32 y, f32 width, f32 height) override;
    void SetScissor(i32 x, i32 y, u32 width, u32 height) override;

private:
    GLCommandBuffer() = default;

    ref<ShaderModule> current_shader_;
    ref<GLPipelineState> current_pipeline_;
    bool recording_{false};
};

}
