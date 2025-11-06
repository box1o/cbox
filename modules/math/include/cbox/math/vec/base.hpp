#pragma once
#include "../detail/arithmetic.hpp"
#include <array>
#include <cassert>
#include <cstddef>


namespace cc {

//NOTE: Forward declaration
template<std::size_t N , arithmetic T >
requires(N >= 2)
class vec{
public:
    static constexpr std::size_t size = N;
    using value_type = T;


    constexpr vec() = default;

    explicit constexpr vec(T scalar) noexcept{ 
        data_.fill(scalar);
    };



    template<arithmetic... Args>
    requires(sizeof...(Args) == N)
    constexpr vec(Args... args) noexcept
        : data_{static_cast<T>(args)...} {}


    template<arithmetic U>
    constexpr vec(const vec<N, U>& other) noexcept{
        for (std::size_t i = 0 ; i < N ; i++){
                data_[i] = static_cast<T>(other[i]);
        }
    }


    constexpr T& operator[](std::size_t i) noexcept {
        assert(i < N);
        return data_[i];
    }


    constexpr const T& operator[](std::size_t i) const noexcept {
        assert(i < N);
        return data_[i];
    }

    constexpr T* data() noexcept { return data_.data();}
    constexpr const T* data() const noexcept { return data_.data();}

    









private:
    std::array<T, N> data_;


};
}
