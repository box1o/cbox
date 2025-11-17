#include "cbox/graphics/pipeline/vlayout.hpp"


namespace cc {

u32 GetFormatSize(VertexFormat format) {
    switch (format) {
        case VertexFormat::Float:   return 4;
        case VertexFormat::Float2:  return 8;
        case VertexFormat::Float3:  return 12;
        case VertexFormat::Float4:  return 16;
        case VertexFormat::Int:     return 4;
        case VertexFormat::Int2:    return 8;
        case VertexFormat::Int3:    return 12;
        case VertexFormat::Int4:    return 16;
        case VertexFormat::UInt:    return 4;
        case VertexFormat::UInt2:   return 8;
        case VertexFormat::UInt3:   return 12;
        case VertexFormat::UInt4:   return 16;
    }
    return 0;
}

u32 GetFormatComponentCount(VertexFormat format) {
    switch (format) {
        case VertexFormat::Float:
        case VertexFormat::Int:
        case VertexFormat::UInt:    return 1;
        case VertexFormat::Float2:
        case VertexFormat::Int2:
        case VertexFormat::UInt2:   return 2;
        case VertexFormat::Float3:
        case VertexFormat::Int3:
        case VertexFormat::UInt3:   return 3;
        case VertexFormat::Float4:
        case VertexFormat::Int4:
        case VertexFormat::UInt4:   return 4;
    }
    return 0;
}

auto VertexLayout::Builder::Attribute(u32 location, VertexFormat format, u32 offset, bool normalized) 
    -> Builder& {
    attributes_.push_back({location, format, offset, normalized});
    return *this;
}

auto VertexLayout::Builder::Stride(u32 stride) -> Builder& {
    stride_ = stride;
    return *this;
}

VertexLayout VertexLayout::Builder::Build() {
    VertexLayout layout;
    layout.attributes_ = std::move(attributes_);
    layout.stride_ = stride_;
    return layout;
}

}
