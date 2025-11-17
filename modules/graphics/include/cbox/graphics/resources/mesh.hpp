#pragma once
#include "cbox/core/core.hpp"
#include "cbox/graphics/pipeline/vlayout.hpp"

namespace cc {

class Buffer;

enum class PrimitiveType : u8 {
    Triangles,
    Lines,
    Points
};

class Mesh {
public:
    class Builder {
    public:
        Builder() = default;

        Builder& SetVertexBuffer(const ref<Buffer>& buffer);
        Builder& SetIndexBuffer(const ref<Buffer>& buffer);
        Builder& SetVertexLayout(const VertexLayout& layout);
        Builder& SetPrimitiveType(PrimitiveType type);
        Builder& SetVertexCount(u32 count);
        Builder& SetIndexCount(u32 count);

        ref<Mesh> Build();

    private:
        ref<Buffer> vertex_buffer_;
        ref<Buffer> index_buffer_;
        VertexLayout layout_;
        PrimitiveType primitive_type_{PrimitiveType::Triangles};
        u32 vertex_count_{0};
        u32 index_count_{0};
    };

    static Builder Create() { return Builder(); }

    static ref<Mesh> CreateQuad();
    static ref<Mesh> CreateCube();
    static ref<Mesh> CreateSphere(u32 segments = 32, u32 rings = 16);

    const ref<Buffer>& GetVertexBuffer() const noexcept { return vertex_buffer_; }
    const ref<Buffer>& GetIndexBuffer() const noexcept { return index_buffer_; }
    const VertexLayout& GetLayout() const noexcept { return layout_; }
    PrimitiveType GetPrimitiveType() const noexcept { return primitive_type_; }
    u32 GetVertexCount() const noexcept { return vertex_count_; }
    u32 GetIndexCount() const noexcept { return index_count_; }
    bool IsIndexed() const noexcept { return index_buffer_ != nullptr; }

private:
    Mesh() = default;

    ref<Buffer> vertex_buffer_;
    ref<Buffer> index_buffer_;
    VertexLayout layout_;
    PrimitiveType primitive_type_{PrimitiveType::Triangles};
    u32 vertex_count_{0};
    u32 index_count_{0};

    friend class Builder;
};

}
