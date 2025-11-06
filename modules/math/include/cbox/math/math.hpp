#pragma once

// IWYU pragma: begin_exports

#include "detail/arithmetic.hpp"
#include "common/constants.hpp"
#include "common/functions.hpp"

//NOTE: vec
#include "vec/fwd.hpp"
#include "vec/base.hpp"
#include "vec/format.hpp"
#include "vec/vec2.hpp"
#include "vec/vec3.hpp"
#include "vec/vec4.hpp"
#include <cstdint>

// IWYU pragma: end_exports


namespace cc{

template<arithmetic T = std::uint32_t> using vec2 = vec<2, T>;
template<arithmetic T = std::uint32_t> using vec3 = vec<3, T>;
template<arithmetic T = std::uint32_t> using vec4 = vec<4, T>;

using vec2f = vec2<float>;
using vec3f = vec3<float>;
using vec4f = vec4<float>;

using vec2d = vec2<double>;
using vec3d = vec3<double>;
using vec4d = vec4<double>;

using vec2i = vec2<int>;
using vec3i = vec3<int>;
using vec4i = vec4<int>;

using vec2u = vec2<unsigned>;
using vec3u = vec3<unsigned>;
using vec4u = vec4<unsigned>;


}
