#include "cbox/graphics/pipeline/pipeline.hpp"
#include "../../backends/gl/pipeline/pipeline.hpp"

namespace cc {

auto PipelineState::Builder::SetShader(const ref<ShaderModule>& shader) -> Builder& {
    shader_ = shader;
    return *this;
}

auto PipelineState::Builder::SetVertexLayout(const VertexLayout& layout) -> Builder& {
    layout_ = layout;
    return *this;
}

auto PipelineState::Builder::SetTopology(PrimitiveTopology topology) -> Builder& {
    topology_ = topology;
    return *this;
}

auto PipelineState::Builder::SetRasterizer(const RasterizerState& state) -> Builder& {
    rasterizer_ = state;
    return *this;
}

auto PipelineState::Builder::SetDepthStencil(const DepthStencilState& state) -> Builder& {
    depth_stencil_ = state;
    return *this;
}

auto PipelineState::Builder::SetBlend(const BlendState& state) -> Builder& {
    blend_ = state;
    return *this;
}

auto PipelineState::Builder::Build() -> result<ref<PipelineState>> {
    return GLPipelineState::Create(shader_, layout_, topology_,
                                    rasterizer_, depth_stencil_, blend_);
}

}
