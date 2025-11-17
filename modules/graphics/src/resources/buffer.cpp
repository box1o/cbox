#include "cbox/graphics/resources/buffer.hpp"
#include "../../backends/gl/buffer/buffer.hpp"

namespace cc {

auto Buffer::Create(BufferType type, BufferUsage usage, u32 size, const void* data) 
    -> result<ref<Buffer>> {
    return GLBuffer::Create(type, usage, size, data);
}

}
