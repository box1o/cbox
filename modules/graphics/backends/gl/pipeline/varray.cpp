#include "varray.hpp"
#include "cbox/graphics/resources/buffer.hpp"
#include "cbox/core/core.hpp"
#include <glad/glad.h>

namespace cc {

static GLenum ToGLType(VertexFormat format) {
    switch (format) {
    case VertexFormat::Float:
    case VertexFormat::Float2:
    case VertexFormat::Float3:
    case VertexFormat::Float4:
        return GL_FLOAT;
    case VertexFormat::Int:
    case VertexFormat::Int2:
    case VertexFormat::Int3:
    case VertexFormat::Int4:
        return GL_INT;
    case VertexFormat::UInt:
    case VertexFormat::UInt2:
    case VertexFormat::UInt3:
    case VertexFormat::UInt4:
        return GL_UNSIGNED_INT;
    }
    return GL_FLOAT;
}

GLVertexArray::~GLVertexArray() {
    if (vao_id_ != 0) {
        glDeleteVertexArrays(1, &vao_id_);
    }
}

auto GLVertexArray::Create() -> result<ref<GLVertexArray>> {
    auto vao = ref<GLVertexArray>(new GLVertexArray());
    glGenVertexArrays(1, &vao->vao_id_);
    return ok(vao);
}

void GLVertexArray::Bind() const {
    glBindVertexArray(vao_id_);
}

void GLVertexArray::Unbind() const {
    glBindVertexArray(0);
}

void GLVertexArray::SetVertexBuffer(const ref<Buffer>& buffer, const VertexLayout& layout) {
    vertex_buffer_ = buffer;

    Bind();
    buffer->Bind();

    const auto& attributes = layout.GetAttributes();
    for (const auto& attr : attributes) {
        glEnableVertexAttribArray(attr.location);
        glVertexAttribPointer(attr.location, GetFormatComponentCount(attr.format),
                              ToGLType(attr.format), attr.normalized ? GL_TRUE : GL_FALSE,
                              layout.GetStride(),
                              reinterpret_cast<const void*>(static_cast<uintptr_t>(attr.offset)));
    }

    Unbind();
}

void GLVertexArray::SetIndexBuffer(const ref<Buffer>& buffer) {
    index_buffer_ = buffer;
    Bind();
    buffer->Bind();
    Unbind();
}

result<ref<GLVertexArray>> GLVertexArrayCreate() {
    return GLVertexArray::Create();
}

} // namespace cc
