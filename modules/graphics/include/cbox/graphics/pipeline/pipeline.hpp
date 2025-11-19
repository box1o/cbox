#pragma once
#include "cbox/core/core.hpp"
#include "cbox/graphics/pipeline/vlayout.hpp"
#include "cbox/graphics/pipeline/rasterizer.hpp"
#include "cbox/graphics/pipeline/depth_stencil.hpp"
#include "cbox/graphics/pipeline/blend.hpp"

namespace cc {

class ShaderModule;

enum class PrimitiveTopology : u8 {
    Points,
    Lines,
    LineStrip,
    Triangles,
    TriangleStrip,
    TriangleFan
};

class PipelineState {
  public:
    class Builder {
      public:
        Builder() = default;

        Builder& SetShader(const ref<ShaderModule>& shader);
        Builder& SetVertexLayout(const VertexLayout& layout);
        Builder& SetTopology(PrimitiveTopology topology);
        Builder& SetRasterizer(const RasterizerState& state);
        Builder& SetDepthStencil(const DepthStencilState& state);
        Builder& SetBlend(const BlendState& state);

        result<ref<PipelineState>> Build();

      private:
        ref<ShaderModule> shader_;
        VertexLayout layout_;
        PrimitiveTopology topology_{PrimitiveTopology::Triangles};
        RasterizerState rasterizer_;
        DepthStencilState depth_stencil_;
        BlendState blend_;
    };

    virtual ~PipelineState() = default;

    static Builder Create() {
        return Builder();
    }

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    const ref<ShaderModule>& GetShader() const noexcept {
        return shader_;
    }
    PrimitiveTopology GetTopology() const noexcept {
        return topology_;
    }

  protected:
    PipelineState() = default;

    ref<ShaderModule> shader_;
    VertexLayout layout_;
    PrimitiveTopology topology_{PrimitiveTopology::Triangles};
    RasterizerState rasterizer_;
    DepthStencilState depth_stencil_;
    BlendState blend_;
};

} // namespace cc
