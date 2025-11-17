#pragma once
#include "cbox/core/core.hpp"
#include <span>

namespace cc {

enum class BufferType : u8 {
    Vertex,
    Index,
    Uniform,
    Storage
};

enum class BufferUsage : u8 {
    Static,
    Dynamic,
    Stream
};

class Buffer {
public:
    virtual ~Buffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void SetData(const void* data, u32 size, u32 offset = 0) = 0;
    
    virtual u32 GetSize() const noexcept = 0;
    virtual BufferType GetType() const noexcept = 0;
    virtual BufferUsage GetUsage() const noexcept = 0;

    template<typename T>
    void SetData(std::span<const T> data, u32 offset = 0) {
        SetData(data.data(), static_cast<u32>(data.size() * sizeof(T)), offset);
    }

    static auto Create(BufferType type, BufferUsage usage, u32 size, const void* data = nullptr) 
        -> result<ref<Buffer>>;

    template<typename T>
    static auto Create(BufferType type, BufferUsage usage, std::span<const T> data) 
        -> result<ref<Buffer>> {
        return Create(type, usage, static_cast<u32>(data.size() * sizeof(T)), data.data());
    }

protected:
    Buffer() = default;
};

}
