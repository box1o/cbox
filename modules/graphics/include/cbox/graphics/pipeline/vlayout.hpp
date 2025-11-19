#pragma once
#include "cbox/core/core.hpp"
#include <string>
#include <vector>

namespace cc {

enum class VertexFormat : u8 {
    Float,
    Float2,
    Float3,
    Float4,
    Int,
    Int2,
    Int3,
    Int4,
    UInt,
    UInt2,
    UInt3,
    UInt4,
};

struct VertexAttribute {
    u32 location;
    VertexFormat format;
    u32 offset;
    bool normalized{false};
};

class VertexLayout {
  public:
    class Builder {
      public:
        Builder() = default;

        Builder& Attribute(u32 location, VertexFormat format, u32 offset, bool normalized = false);
        Builder& Stride(u32 stride);

        VertexLayout Build();

      private:
        std::vector<VertexAttribute> attributes_;
        u32 stride_{0};
    };

    VertexLayout() = default;

    static Builder Create() {
        return Builder();
    }

    const std::vector<VertexAttribute>& GetAttributes() const noexcept {
        return attributes_;
    }
    u32 GetStride() const noexcept {
        return stride_;
    }

  private:
    std::vector<VertexAttribute> attributes_;
    u32 stride_{0};

    friend class Builder;
};

u32 GetFormatSize(VertexFormat format);
u32 GetFormatComponentCount(VertexFormat format);

} // namespace cc
