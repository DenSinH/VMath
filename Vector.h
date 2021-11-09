#pragma once

#include "Types.h"

#include <array>


namespace vmath {

template<typename T, size_t n>
struct Vector {
    using type = vtype<T, n>;
    vtype_t<T, n> base;

    template<typename... Args>
    requires (sizeof...(Args) == n)
    Vector(Args... args) : base(type::load(args...)) {

    }

    Vector(const vtype_t<T, n>& other) : base(other) {

    }

    template<typename S>
    requires (std::is_same_v<S, T> || std::is_same_v<std::make_unsigned_t<S>, std::make_unsigned_t<T>>)
    Vector<std::common_type_t<S, T>, n> operator+(const Vector<S, n>& other) const {
        return Vector<std::common_type_t<S, T>, n>{type::add(base, other.base)};
    }

    template<typename S>
    requires (std::is_same_v<S, T> || std::is_same_v<std::make_unsigned_t<S>, std::make_unsigned_t<T>>)
    Vector<std::common_type_t<S, T>, n> operator-(const Vector<S, n>& other) const {
        return Vector<std::common_type_t<S, T>, n>{type::sub(base, other.base)};
    }

    template<typename S>
    Vector<T, n> operator<<(S other) const {
        return Vector<T, n>{type::shli(base, other)};
    }

    template<typename S>
    Vector<T, n> operator>>(S other) const {
        if constexpr(std::is_signed_v<T>) {
            return Vector<std::common_type_t<S, T>, n>{type::shrai(base, other)};
        }
        else {
            return Vector<std::common_type_t<S, T>, n>{type::shrli(base, other)};
        }
    }

    Vector<T, n> operator-() const {
        return Vector<T, n>{type::neg(base)};
    }

    std::array<T, n> data() const {
        constexpr size_t alignment = sizeof(T) * n == 16 ? 16 : 32;
        alignas(alignment) std::array<T, n> data;
        type::store(data.data(), base);
        return data;
    }

    T operator[](size_t index) const {
        return data()[index];
    }

    template<typename S>
    Vector<T, n>& operator+=(S other) {
        base = (*this + other).base;
        return *this;
    }

    template<typename S>
    Vector<T, n>& operator-=(S other) {
        base = (*this - other).base;
        return *this;
    }

    template<typename S>
    Vector<T, n>& operator<<=(S other) {
        base = (*this << other).base;
        return *this;
    }

    template<typename S>
    Vector<T, n>& operator>>=(S other) {
        base = (*this >> other).base;
        return *this;
    }
};


}