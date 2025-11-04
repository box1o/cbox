#pragma once

#include <type_traits>
namespace cc{


template<typename T>
concept arithmetic = std::is_arithmetic_v<T>;



template<arithmetic T , typename... Args>
T add(T first, Args... args) {
    return first + (args + ...);
} ;



}
