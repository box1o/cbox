#pragma once 

#include "fwd.hpp"
#include "../detail/arithmetic.hpp"

#include <array>
#include <cmath>
#include <cassert>
#include <cstddef>


namespace cc {
template<arithmetic T>
class vec<2, T> {

public:
    static constexpr std::size_t size = 2;
    using value_type = T;

    union{
        struct { T x, y;};
        struct { T r, g;};
        std::array<T, 2> data_;
    };

    constexpr vec() : x(T{0}), y(T{0}) {}
    explicit constexpr vec(T scalar) noexcept : x(scalar), y(scalar) {}

    constexpr vec(T x_ , T y_ ) noexcept : x(x_), y(y_) {}

    template<arithmetic U>
    constexpr vec(const vec<2, U>& other) noexcept : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}


    constexpr T* data() noexcept { return data_.data(); }
    constexpr const T* data() const noexcept { return data_.data(); }

    constexpr T& operator[](std::size_t i)  noexcept {
        assert(i < 2);
        return data_[i];
    }

    constexpr const T& operator[](std::size_t i) const  noexcept {
        assert(i < 2);
        return data_[i];
    }

    constexpr vec& operator+=(const vec& other) noexcept {
        x += other.x; 
        y += other.y;
        return *this;
    }

    constexpr vec& operator-=(const vec& other) noexcept {
        x -= other.x; 
        y -= other.y;
        return *this;
    }

    constexpr vec& operator*=(const vec& other) noexcept {
        x *= other.x; 
        y *= other.y;
        return *this;
    }

    constexpr vec& operator*=(T scalar) noexcept {
        x *= scalar; 
        y *= scalar;
        return *this;
    }

    constexpr vec& operator/=(const vec& other) noexcept {
        x /= other.x; 
        y /= other.y;
        return *this;
    }

    constexpr vec& operator/=(T scalar) noexcept {
        x /= scalar; 
        y /= scalar;
        return *this;
    }

    friend constexpr vec operator+(const vec& a, const vec& b) noexcept {
        return {a.x + b.x, a.y + b.y};
    }

    friend constexpr vec operator-(const vec& a, const vec& b) noexcept {
        return {a.x - b.x, a.y - b.y};
    }

    friend constexpr vec operator*(const vec& a, const vec& b) noexcept {
        return {a.x * b.x, a.y * b.y};
    }

    friend constexpr vec operator*(const vec& v, T scalar) noexcept {
        return {v.x * scalar, v.y * scalar};
    }

    friend constexpr vec operator*(T scalar, const vec& v) noexcept {
        return v * scalar;
    }

    friend constexpr vec operator/(const vec& a, const vec& b) noexcept {
        return {a.x / b.x, a.y / b.y};
    }

    friend constexpr vec operator/(const vec& v, T scalar) noexcept {
        return {v.x / scalar, v.y / scalar};
    }

    constexpr vec operator-() const noexcept {
        return {-x, -y};
    }

    friend constexpr bool operator==(const vec& a, const vec& b) noexcept {
        return a.x == b.x && a.y == b.y;
    }

    constexpr T dot(const vec& other) const noexcept {
        return x * other.x + y * other.y;
    }

    constexpr T cross(const vec& other) const noexcept {
        return x * other.y - y * other.x;
    }

    constexpr T len_sq() const noexcept {
        return dot(*this);
    }

    T len() const noexcept {
        return std::sqrt(len_sq());
    }

    vec norm() const noexcept {
        T l = len();
        return l > T{} ? *this / l : vec{};
    }

};

}
