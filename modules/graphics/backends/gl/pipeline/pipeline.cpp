#include "pipeline.hpp"
#include "varray.hpp"
#include "cbox/graphics/shader/module.hpp"
#include "cbox/core/core.hpp"
#include <glad/glad.h>

namespace cc {

GLPipelineState::~GLPipelineState() {}

auto GLPipelineState::Create(const ref<ShaderModule>& shader, const VertexLayout& layout,
                             PrimitiveTopology topology, const RasterizerState& rasterizer,
                             const DepthStencilState& depth_stencil, const BlendState& blend)
    -> result<ref<GLPipelineState>> {
    if (!shader) {
        return err(error_code::validation_null_value, "Shader is null");
    }

    auto pipeline = ref<GLPipelineState>(new GLPipelineState());
    pipeline->shader_ = shader;
    pipeline->layout_ = layout;
    pipeline->topology_ = topology;
    pipeline->rasterizer_ = rasterizer;
    pipeline->depth_stencil_ = depth_stencil;
    pipeline->blend_ = blend;

    auto vao_result = GLVertexArray::Create();
    if (!vao_result) {
        return err(vao_result.error());
    }
    pipeline->vao_ = vao_result.value();

    return ok(pipeline);
}

void GLPipelineState::Bind() const {
    shader_->Bind();
    if (vao_) {
        vao_->Bind();
    }
    rasterizer_.Apply();
    depth_stencil_.Apply();
    blend_.Apply();
}

void GLPipelineState::Unbind() const {
    if (vao_) {
        vao_->Unbind();
    }
    shader_->Unbind();
}

} // namespace cc
