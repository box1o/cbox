#pragma once
#include "cbox/core/core.hpp"
#include "cbox/math/math.hpp"

namespace cc {

enum class RenderAPI : u8 { None = 0, OpenGL, Vulkan, DirectX12, Metal };

using ClearColor = vec4f;

} // namespace cc
