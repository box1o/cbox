#pragma once
#include "cbox/core/core.hpp"
#include "cbox/math/math.hpp"

namespace cc {

class RenderPass;
class PipelineState;
class Buffer;
class Texture;
class Sampler;

class CommandBuffer {
  public:
    virtual ~CommandBuffer() = default;

    static auto Create() -> result<ref<CommandBuffer>>;

    virtual void Begin() = 0;
    virtual void End() = 0;

    virtual void BeginRenderPass(const ref<RenderPass>& pass) = 0;
    virtual void EndRenderPass() = 0;

    virtual void SetPipeline(const ref<PipelineState>& pipeline) = 0;
    virtual void SetVertexBuffer(const ref<Buffer>& buffer, u32 binding = 0) = 0;
    virtual void SetIndexBuffer(const ref<Buffer>& buffer) = 0;

    virtual void SetUniformFloat(const std::string& name, f32 value) = 0;
    virtual void SetUniformInt(const std::string& name, i32 value) = 0;
    virtual void SetUniformVec2(const std::string& name, const vec2f& value) = 0;
    virtual void SetUniformVec3(const std::string& name, const vec3f& value) = 0;
    virtual void SetUniformVec4(const std::string& name, const vec4f& value) = 0;
    virtual void SetUniformMat3(const std::string& name, const mat3f& value) = 0;
    virtual void SetUniformMat4(const std::string& name, const mat4f& value) = 0;

    virtual void SetTexture(u32 slot, const ref<Texture>& texture) = 0;
    virtual void SetSampler(u32 slot, const ref<Sampler>& sampler) = 0;

    virtual void Draw(u32 vertex_count, u32 first_vertex = 0) = 0;
    virtual void DrawIndexed(u32 index_count, u32 first_index = 0, i32 vertex_offset = 0) = 0;

    virtual void SetViewport(f32 x, f32 y, f32 width, f32 height) = 0;
    virtual void SetScissor(i32 x, i32 y, u32 width, u32 height) = 0;

  protected:
    CommandBuffer() = default;
};

} // namespace cc
