#include "cbox/graphics/commands/command_buffer.hpp"
#include "../../backends/gl/commands/command_buffer.hpp"

namespace cc {

auto CommandBuffer::Create() -> result<ref<CommandBuffer>> {
    return GLCommandBuffer::Create();
}

} // namespace cc
