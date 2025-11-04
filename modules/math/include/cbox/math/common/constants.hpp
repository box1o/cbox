#pragma once

#include "../detail/arithmetic.hpp"

#include <numbers>
#include <limits>


namespace cc {

template <arithmetic T>
inline constexpr T pi = std::numbers::pi_v<T>;

template <arithmetic T>
inline constexpr T two_pi = std::numbers::pi_v<T> * T{2};

template<arithmetic T>
inline constexpr T epsilon = std::numeric_limits<T>::epsilon();

template<arithmetic T>
inline constexpr T infinity = std::numeric_limits<T>::infinity();


}
