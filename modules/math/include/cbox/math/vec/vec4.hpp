
#pragma once
#include "fwd.hpp"
#include <array>
#include <cassert>
#include <cmath>

namespace cc {

template<arithmetic T>
class vec<4, T> {
public:
    static constexpr std::size_t size = 4;
    using value_type = T;

    union {
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
        std::array<T, 4> data_;
    };

    constexpr vec() : x(T{}), y(T{}), z(T{}), w(T{}) {}
    explicit constexpr vec(T scalar) noexcept : x(scalar), y(scalar), z(scalar), w(scalar) {}
    constexpr vec(T x_, T y_, T z_, T w_) noexcept : x(x_), y(y_), z(z_), w(w_) {}
    constexpr vec(const vec<2, T>& xy_, T z_, T w_) noexcept : x(xy_.x), y(xy_.y), z(z_), w(w_) {}
    constexpr vec(const vec<3, T>& xyz_, T w_) noexcept : x(xyz_.x), y(xyz_.y), z(xyz_.z), w(w_) {}

    template<arithmetic U>
    constexpr vec(const vec<4, U>& other) noexcept 
        : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)), 
          z(static_cast<T>(other.z)), w(static_cast<T>(other.w)) {}

    constexpr T& operator[](std::size_t i) noexcept {
        assert(i < 4);
        return data_[i];
    }

    constexpr const T& operator[](std::size_t i) const noexcept {
        assert(i < 4);
        return data_[i];
    }

    constexpr T* data() noexcept { return data_.data(); }
    constexpr const T* data() const noexcept { return data_.data(); }

    constexpr vec& operator+=(const vec& other) noexcept {
        x += other.x; 
        y += other.y; 
        z += other.z; 
        w += other.w;
        return *this;
    }

    constexpr vec& operator-=(const vec& other) noexcept {
        x -= other.x; 
        y -= other.y; 
        z -= other.z; 
        w -= other.w;
        return *this;
    }

    constexpr vec& operator*=(const vec& other) noexcept {
        x *= other.x; 
        y *= other.y; 
        z *= other.z; 
        w *= other.w;
        return *this;
    }

    constexpr vec& operator*=(T scalar) noexcept {
        x *= scalar; 
        y *= scalar; 
        z *= scalar; 
        w *= scalar;
        return *this;
    }

    constexpr vec& operator/=(const vec& other) noexcept {
        x /= other.x; 
        y /= other.y; 
        z /= other.z; 
        w /= other.w;
        return *this;
    }

    constexpr vec& operator/=(T scalar) noexcept {
        x /= scalar; 
        y /= scalar; 
        z /= scalar; 
        w /= scalar;
        return *this;
    }

    friend constexpr vec operator+(const vec& a, const vec& b) noexcept {
        return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
    }

    friend constexpr vec operator-(const vec& a, const vec& b) noexcept {
        return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
    }

    friend constexpr vec operator*(const vec& a, const vec& b) noexcept {
        return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
    }

    friend constexpr vec operator*(const vec& v, T scalar) noexcept {
        return {v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar};
    }

    friend constexpr vec operator*(T scalar, const vec& v) noexcept {
        return v * scalar;
    }

    friend constexpr vec operator/(const vec& a, const vec& b) noexcept {
        return {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
    }

    friend constexpr vec operator/(const vec& v, T scalar) noexcept {
        return {v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar};
    }

    constexpr vec operator-() const noexcept {
        return {-x, -y, -z, -w};
    }

    friend constexpr bool operator==(const vec& a, const vec& b) noexcept {
        return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
    }

    constexpr T dot(const vec& other) const noexcept {
        return x * other.x + y * other.y + z * other.z + w * other.w;
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

    constexpr vec<2, T> xy() const noexcept { return {x, y}; }
    constexpr vec<2, T> xz() const noexcept { return {x, z}; }
    constexpr vec<2, T> yz() const noexcept { return {y, z}; }
    constexpr vec<3, T> xyz() const noexcept { return {x, y, z}; }
};

}
