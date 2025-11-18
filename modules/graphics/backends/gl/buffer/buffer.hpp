#pragma once
#include "cbox/graphics/resources/buffer.hpp"

namespace cc {

class GLBuffer : public Buffer {
public:
    ~GLBuffer() override;

    static auto Create(BufferType type, BufferUsage usage, u32 size, const void* data) 
    -> ref<GLBuffer>;

    void Bind() const override;
    void Unbind() const override;
    void SetData(const void* data, u32 size, u32 offset) override;

    u32 GetSize() const noexcept override { return size_; }
    BufferType GetType() const noexcept override { return type_; }
    BufferUsage GetUsage() const noexcept override { return usage_; }

    u32 GetBufferID() const noexcept { return buffer_id_; }

private:
    GLBuffer() = default;

    u32 buffer_id_{0};
    u32 size_{0};
    BufferType type_;
    BufferUsage usage_;
};

}
