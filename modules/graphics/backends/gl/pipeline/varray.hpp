#pragma once
#include "cbox/core/core.hpp"
#include "cbox/graphics/pipeline/vlayout.hpp"

namespace cc {

class Buffer;

class GLVertexArray {
public:
    ~GLVertexArray();

    static auto Create() -> result<ref<GLVertexArray>>;

    void Bind() const;
    void Unbind() const;

    void SetVertexBuffer(const ref<Buffer>& buffer, const VertexLayout& layout);
    void SetIndexBuffer(const ref<Buffer>& buffer);

    u32 GetVAO() const noexcept { return vao_id_; }
    const ref<Buffer>& GetIndexBuffer() const noexcept { return index_buffer_; }

private:
    GLVertexArray() = default;

    u32 vao_id_{0};
    ref<Buffer> vertex_buffer_;
    ref<Buffer> index_buffer_;

    friend result<ref<GLVertexArray>> GLVertexArrayCreate();
};

result<ref<GLVertexArray>> GLVertexArrayCreate();

}
