#include "cbox/graphics/resources/mesh.hpp"
#include "cbox/graphics/resources/buffer.hpp"
#include "cbox/math/math.hpp"
#include <cmath>

namespace cc {

auto Mesh::Builder::SetVertexBuffer(const ref<Buffer>& buffer) -> Builder& {
    vertex_buffer_ = buffer;
    return *this;
}

auto Mesh::Builder::SetIndexBuffer(const ref<Buffer>& buffer) -> Builder& {
    index_buffer_ = buffer;
    return *this;
}

auto Mesh::Builder::SetVertexLayout(const VertexLayout& layout) -> Builder& {
    layout_ = layout;
    return *this;
}

auto Mesh::Builder::SetPrimitiveType(PrimitiveType type) -> Builder& {
    primitive_type_ = type;
    return *this;
}

auto Mesh::Builder::SetVertexCount(u32 count) -> Builder& {
    vertex_count_ = count;
    return *this;
}

auto Mesh::Builder::SetIndexCount(u32 count) -> Builder& {
    index_count_ = count;
    return *this;
}

auto Mesh::Builder::Build() -> ref<Mesh> {
    auto mesh = ref<Mesh>(new Mesh());
    mesh->vertex_buffer_ = vertex_buffer_;
    mesh->index_buffer_ = index_buffer_;
    mesh->layout_ = layout_;
    mesh->primitive_type_ = primitive_type_;
    mesh->vertex_count_ = vertex_count_;
    mesh->index_count_ = index_count_;
    return mesh;
}

ref<Mesh> Mesh::CreateQuad() {
    struct Vertex {
        vec3f position;
        vec3f normal;
        vec2f texcoord;
    };

    Vertex vertices[] = {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}
    };

    u32 indices[] = {0, 1, 2, 2, 3, 0};

    auto vbo = Buffer::Create(BufferType::Vertex, BufferUsage::Static,
                              std::span<const Vertex>(vertices, 4)).value();
    auto ibo = Buffer::Create(BufferType::Index, BufferUsage::Static,
                              std::span<const u32>(indices, 6)).value();

    auto layout = VertexLayout::Create()
        .Attribute(0, VertexFormat::Float3, offsetof(Vertex, position))
        .Attribute(1, VertexFormat::Float3, offsetof(Vertex, normal))
        .Attribute(2, VertexFormat::Float2, offsetof(Vertex, texcoord))
        .Stride(sizeof(Vertex))
        .Build();

    return Mesh::Create()
        .SetVertexBuffer(vbo)
        .SetIndexBuffer(ibo)
        .SetVertexLayout(layout)
        .SetVertexCount(4)
        .SetIndexCount(6)
        .Build();
}

ref<Mesh> Mesh::CreateCube() {
    struct Vertex {
        vec3f position;
        vec3f normal;
        vec2f texcoord;
    };

    Vertex vertices[] = {
        {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}},

        {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}},

        {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},

        {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},

        {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},

        {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}}
    };

    u32 indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };

    auto vbo = Buffer::Create(BufferType::Vertex, BufferUsage::Static,
                              std::span<const Vertex>(vertices, 24)).value();
    auto ibo = Buffer::Create(BufferType::Index, BufferUsage::Static,
                              std::span<const u32>(indices, 36)).value();

    auto layout = VertexLayout::Create()
        .Attribute(0, VertexFormat::Float3, offsetof(Vertex, position))
        .Attribute(1, VertexFormat::Float3, offsetof(Vertex, normal))
        .Attribute(2, VertexFormat::Float2, offsetof(Vertex, texcoord))
        .Stride(sizeof(Vertex))
        .Build();

    return Mesh::Create()
        .SetVertexBuffer(vbo)
        .SetIndexBuffer(ibo)
        .SetVertexLayout(layout)
        .SetVertexCount(24)
        .SetIndexCount(36)
        .Build();
}

ref<Mesh> Mesh::CreateSphere(u32 segments, u32 rings) {
    struct Vertex {
        vec3f position;
        vec3f normal;
        vec2f texcoord;
    };

    std::vector<Vertex> vertices;
    std::vector<u32> indices;

    for (u32 ring = 0; ring <= rings; ++ring) {
        f32 theta = static_cast<f32>(ring) / static_cast<f32>(rings) * 3.14159f;
        f32 sin_theta = std::sin(theta);
        f32 cos_theta = std::cos(theta);

        for (u32 seg = 0; seg <= segments; ++seg) {
            f32 phi = static_cast<f32>(seg) / static_cast<f32>(segments) * 2.0f * 3.14159f;
            f32 sin_phi = std::sin(phi);
            f32 cos_phi = std::cos(phi);

            vec3f position{
                sin_theta * cos_phi,
                cos_theta,
                sin_theta * sin_phi
            };

            vertices.push_back({
                position * 0.5f,
                position,
                {static_cast<f32>(seg) / segments, static_cast<f32>(ring) / rings}
            });
        }
    }

    for (u32 ring = 0; ring < rings; ++ring) {
        for (u32 seg = 0; seg < segments; ++seg) {
            u32 current = ring * (segments + 1) + seg;
            u32 next = current + segments + 1;

            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);

            indices.push_back(current + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }

    auto vbo = Buffer::Create(BufferType::Vertex, BufferUsage::Static,
                              std::span<const Vertex>(vertices.data(), vertices.size())).value();
    auto ibo = Buffer::Create(BufferType::Index, BufferUsage::Static,
                              std::span<const u32>(indices.data(), indices.size())).value();

    auto layout = VertexLayout::Create()
        .Attribute(0, VertexFormat::Float3, offsetof(Vertex, position))
        .Attribute(1, VertexFormat::Float3, offsetof(Vertex, normal))
        .Attribute(2, VertexFormat::Float2, offsetof(Vertex, texcoord))
        .Stride(sizeof(Vertex))
        .Build();

    return Mesh::Create()
        .SetVertexBuffer(vbo)
        .SetIndexBuffer(ibo)
        .SetVertexLayout(layout)
        .SetVertexCount(static_cast<u32>(vertices.size()))
        .SetIndexCount(static_cast<u32>(indices.size()))
        .Build();
}

}
