#pragma once
#include "cbox/graphics/pipeline/pipeline.hpp"

namespace cc {

class GLVertexArray;

class GLPipelineState : public PipelineState {
  public:
    ~GLPipelineState() override;

    static auto Create(const ref<ShaderModule>& shader, const VertexLayout& layout,
                       PrimitiveTopology topology, const RasterizerState& rasterizer,
                       const DepthStencilState& depth_stencil, const BlendState& blend)
        -> result<ref<GLPipelineState>>;

    void Bind() const override;
    void Unbind() const override;

    const ref<GLVertexArray>& GetVAO() const noexcept {
        return vao_;
    }

  private:
    GLPipelineState() = default;

    ref<GLVertexArray> vao_;
};

} // namespace cc
