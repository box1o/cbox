#include "command_buffer.hpp"
#include "cbox/graphics/commands/render_pass.hpp"
#include "cbox/graphics/pipeline/pipeline.hpp"
#include "cbox/graphics/shader/module.hpp"
#include "cbox/graphics/framebuffer/framebuffer.hpp"
#include "cbox/graphics/resources/buffer.hpp"
#include "cbox/graphics/resources/texture.hpp"
#include "cbox/graphics/resources/sampler.hpp"
#include "../pipeline/pipeline.hpp"
#include "cbox/core/core.hpp"
#include <glad/glad.h>

namespace cc {

GLCommandBuffer::~GLCommandBuffer() {}

auto GLCommandBuffer::Create() -> result<ref<GLCommandBuffer>> {
    auto cmd = ref<GLCommandBuffer>(new GLCommandBuffer());
    return ok(cmd);
}

void GLCommandBuffer::Begin() {
    recording_ = true;
}

void GLCommandBuffer::End() {
    recording_ = false;
}

void GLCommandBuffer::BeginRenderPass(const ref<RenderPass>& pass) {
    if (!pass || !pass->GetFramebuffer()) {
        return;
    }

    pass->GetFramebuffer()->Bind();

    const auto& color_attachments = pass->GetColorAttachments();
    const auto& depth_stencil = pass->GetDepthStencil();

    GLbitfield clear_mask = 0;

    if (color_attachments[0].load == LoadOp::Clear) {
        const auto& color = color_attachments[0].clear_value;
        glClearColor(color.x, color.y, color.z, color.w);
        clear_mask |= GL_COLOR_BUFFER_BIT;
    }

    if (depth_stencil.depth_load == LoadOp::Clear) {
        glClearDepth(depth_stencil.clear_depth);
        clear_mask |= GL_DEPTH_BUFFER_BIT;
    }

    if (depth_stencil.stencil_load == LoadOp::Clear) {
        glClearStencil(depth_stencil.clear_stencil);
        clear_mask |= GL_STENCIL_BUFFER_BIT;
    }

    if (clear_mask != 0) {
        glClear(clear_mask);
    }
}

void GLCommandBuffer::EndRenderPass() {}

void GLCommandBuffer::SetPipeline(const ref<PipelineState>& pipeline) {
    current_pipeline_ = std::static_pointer_cast<GLPipelineState>(pipeline);
    current_shader_ = pipeline->GetShader();
    current_pipeline_->Bind();
}

void GLCommandBuffer::SetVertexBuffer(const ref<Buffer>& buffer, u32 binding) {
    if (current_pipeline_ && current_pipeline_->GetVAO()) {
        //NOTE: VAO already bound by pipeline
    }
}

void GLCommandBuffer::SetIndexBuffer(const ref<Buffer>& buffer) {
    if (current_pipeline_ && current_pipeline_->GetVAO()) {
        //NOTE: VAO already bound by pipeline
    }
}

void GLCommandBuffer::SetUniformFloat(const std::string& name, f32 value) {
    if (!current_shader_)
        return;
    i32 loc = current_shader_->GetUniformLocation(name);
    if (loc >= 0) {
        glUniform1f(loc, value);
    }
}

void GLCommandBuffer::SetUniformInt(const std::string& name, i32 value) {
    if (!current_shader_)
        return;
    i32 loc = current_shader_->GetUniformLocation(name);
    if (loc >= 0) {
        glUniform1i(loc, value);
    }
}

void GLCommandBuffer::SetUniformVec2(const std::string& name, const vec2f& value) {
    if (!current_shader_)
        return;
    i32 loc = current_shader_->GetUniformLocation(name);
    if (loc >= 0) {
        glUniform2f(loc, value.x, value.y);
    }
}

void GLCommandBuffer::SetUniformVec3(const std::string& name, const vec3f& value) {
    if (!current_shader_)
        return;
    i32 loc = current_shader_->GetUniformLocation(name);
    if (loc >= 0) {
        glUniform3f(loc, value.x, value.y, value.z);
    }
}

void GLCommandBuffer::SetUniformVec4(const std::string& name, const vec4f& value) {
    if (!current_shader_)
        return;
    i32 loc = current_shader_->GetUniformLocation(name);
    if (loc >= 0) {
        glUniform4f(loc, value.x, value.y, value.z, value.w);
    }
}

void GLCommandBuffer::SetUniformMat3(const std::string& name, const mat3f& value) {
    if (!current_shader_)
        return;
    i32 loc = current_shader_->GetUniformLocation(name);
    if (loc >= 0) {
        glUniformMatrix3fv(loc, 1, GL_FALSE, &value[0][0]);
    }
}

void GLCommandBuffer::SetUniformMat4(const std::string& name, const mat4f& value) {
    if (!current_shader_)
        return;
    i32 loc = current_shader_->GetUniformLocation(name);
    if (loc >= 0) {
        glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
    }
}

void GLCommandBuffer::SetTexture(u32 slot, const ref<Texture>& texture) {
    if (texture) {
        texture->Bind(slot);
    }
}

void GLCommandBuffer::SetSampler(u32 slot, const ref<Sampler>& sampler) {
    if (sampler) {
        sampler->Bind(slot);
    }
}

void GLCommandBuffer::Draw(u32 vertex_count, u32 first_vertex) {
    if (!current_pipeline_)
        return;

    GLenum mode = GL_TRIANGLES;
    switch (current_pipeline_->GetTopology()) {
    case PrimitiveTopology::Points:
        mode = GL_POINTS;
        break;
    case PrimitiveTopology::Lines:
        mode = GL_LINES;
        break;
    case PrimitiveTopology::LineStrip:
        mode = GL_LINE_STRIP;
        break;
    case PrimitiveTopology::Triangles:
        mode = GL_TRIANGLES;
        break;
    case PrimitiveTopology::TriangleStrip:
        mode = GL_TRIANGLE_STRIP;
        break;
    case PrimitiveTopology::TriangleFan:
        mode = GL_TRIANGLE_FAN;
        break;
    }

    glDrawArrays(mode, first_vertex, vertex_count);
}

void GLCommandBuffer::DrawIndexed(u32 index_count, u32 first_index, i32 vertex_offset) {
    if (!current_pipeline_)
        return;

    GLenum mode = GL_TRIANGLES;
    switch (current_pipeline_->GetTopology()) {
    case PrimitiveTopology::Points:
        mode = GL_POINTS;
        break;
    case PrimitiveTopology::Lines:
        mode = GL_LINES;
        break;
    case PrimitiveTopology::LineStrip:
        mode = GL_LINE_STRIP;
        break;
    case PrimitiveTopology::Triangles:
        mode = GL_TRIANGLES;
        break;
    case PrimitiveTopology::TriangleStrip:
        mode = GL_TRIANGLE_STRIP;
        break;
    case PrimitiveTopology::TriangleFan:
        mode = GL_TRIANGLE_FAN;
        break;
    }

    void* offset = reinterpret_cast<void*>(static_cast<uintptr_t>(first_index * sizeof(u32)));
    glDrawElements(mode, index_count, GL_UNSIGNED_INT, offset);
}

void GLCommandBuffer::SetViewport(f32 x, f32 y, f32 width, f32 height) {
    glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width),
               static_cast<GLsizei>(height));
}

void GLCommandBuffer::SetScissor(i32 x, i32 y, u32 width, u32 height) {
    glScissor(x, y, width, height);
}

} // namespace cc
