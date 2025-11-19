#include "buffer.hpp"
#include "cbox/core/core.hpp"
#include <format>
#include <glad/glad.h>
#include <stdexcept>

namespace cc {

static GLenum ToGLBufferType(BufferType type) {
    switch (type) {
    case BufferType::Vertex:
        return GL_ARRAY_BUFFER;
    case BufferType::Index:
        return GL_ELEMENT_ARRAY_BUFFER;
    case BufferType::Uniform:
        return GL_UNIFORM_BUFFER;
    case BufferType::Storage:
        return GL_SHADER_STORAGE_BUFFER;
    }
    return GL_ARRAY_BUFFER;
}

static GLenum ToGLBufferUsage(BufferUsage usage) {
    switch (usage) {
    case BufferUsage::Static:
        return GL_STATIC_DRAW;
    case BufferUsage::Dynamic:
        return GL_DYNAMIC_DRAW;
    case BufferUsage::Stream:
        return GL_STREAM_DRAW;
    }
    return GL_STATIC_DRAW;
}

GLBuffer::~GLBuffer() {
    if (buffer_id_ != 0) {
        glDeleteBuffers(1, &buffer_id_);
    }
}

auto GLBuffer::Create(BufferType type, BufferUsage usage, u32 size, const void* data)
    -> ref<GLBuffer> {
    if (size == 0) {
        throw std::runtime_error(
            std::format("Cannot create buffer of type {} with size 0", static_cast<u8>(type)));
    }

    auto buffer = ref<GLBuffer>(new GLBuffer());
    buffer->type_ = type;
    buffer->usage_ = usage;
    buffer->size_ = size;

    glGenBuffers(1, &buffer->buffer_id_);
    glBindBuffer(ToGLBufferType(type), buffer->buffer_id_);
    glBufferData(ToGLBufferType(type), size, data, ToGLBufferUsage(usage));
    glBindBuffer(ToGLBufferType(type), 0);

    return buffer;
}

void GLBuffer::Bind() const {
    glBindBuffer(ToGLBufferType(type_), buffer_id_);
}

void GLBuffer::Unbind() const {
    glBindBuffer(ToGLBufferType(type_), 0);
}

void GLBuffer::SetData(const void* data, u32 size, u32 offset) {
    glBindBuffer(ToGLBufferType(type_), buffer_id_);
    glBufferSubData(ToGLBufferType(type_), offset, size, data);
    glBindBuffer(ToGLBufferType(type_), 0);
}

} // namespace cc
