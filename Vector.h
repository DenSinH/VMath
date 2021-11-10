#pragma once

#include "Types.h"

#include <array>


namespace vmath {

template<typename T, typename S>
concept Compatible = std::is_same_v<S, T> || std::is_same_v<std::make_unsigned_t<S>, std::make_unsigned_t<T>>;

#define VMATH_IOP_HELPER(_symbol) \
    template<typename S> \
Vector<T, n>& operator _symbol ## =(S other) { \
    base = (*this _symbol other).base; \
    return *this; \
}

namespace detail {

template<typename T>
struct identity { using type = T; };

template<typename T>
using try_make_signed =
typename std::conditional<
        std::is_integral<T>::value,
        std::make_signed<T>,
        identity<T>
>::type;

template<typename T>
using try_make_signed_t = typename try_make_signed<T>::type;

}

template<typename T, size_t n>
struct Vector {
    using signed_T = detail::try_make_signed_t<T>;
    using type = vtype<signed_T, n>;

    vtype_t<signed_T, n> base;

    template<typename... Args>
    requires (sizeof...(Args) == n)
    Vector(Args... args) : base(type::load((signed_T)args...)) {

    }

    Vector(T value) : base(type::load1(value)) {

    }

    template<Compatible<T> S>
    Vector(const vtype_t<S, n>& other) : base(other.base) {

    }

    Vector(const vtype_t<signed_T, n>& other) : base(other) {

    }

    template<Compatible<T> S>
    Vector<std::common_type_t<S, T>, n> operator+(const Vector<S, n>& other) const {
        return Vector<std::common_type_t<S, T>, n>{type::add(base, other.base)};
    }

    template<Compatible<T> S>
    Vector<std::common_type_t<S, T>, n> operator-(const Vector<S, n>& other) const {
        return Vector<std::common_type_t<S, T>, n>{type::sub(base, other.base)};
    }

    template<Compatible<T> S>
    Vector<std::common_type_t<S, T>, n> operator*(const Vector<S, n>& other) const {
        if constexpr(!std::is_integral_v<T> || std::is_signed_v<T>) {
            return Vector<std::common_type_t<S, T>, n>{type::mul(base, other.base)};
        }
        else {
            return Vector<std::common_type_t<S, T>, n>{type::umul(base, other.base)};
        }
    }

    template<typename S>
    Vector<T, n> operator*(const S& other) const {
        if constexpr(!std::is_integral_v<T> || std::is_signed_v<T>) {
            return Vector<std::common_type_t<S, T>, n>{type::mul(base, Vector<T, n>{(T)other}.base)};
        }
        else {
            return Vector<std::common_type_t<S, T>, n>{type::umul(base, Vector<T, n>{(T)other}.base)};
        }
    }

    template<Compatible<T> S>
    Vector<std::common_type_t<S, T>, n> operator/(const Vector<S, n>& other) const {
        if constexpr(!std::is_integral_v<T> || std::is_signed_v<T>) {
            return Vector<std::common_type_t<S, T>, n>{type::div(base, other.base)};
        }
        else {
            return Vector<std::common_type_t<S, T>, n>{type::udiv(base, other.base)};
        }
    }

    template<Compatible<T> S>
    Vector<std::common_type_t<S, T>, n> operator&(const Vector<S, n>& other) const {
        return Vector<std::common_type_t<S, T>, n>{type::and_(base, other.base)};
    }

    template<Compatible<T> S>
    Vector<std::common_type_t<S, T>, n> operator|(const Vector<S, n>& other) const {
        return Vector<std::common_type_t<S, T>, n>{type::or_(base, other.base)};
    }

    template<Compatible<T> S>
    Vector<std::common_type_t<S, T>, n> operator^(const Vector<S, n>& other) const {
        return Vector<std::common_type_t<S, T>, n>{type::xor_(base, other.base)};
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

    template<Compatible<T> S>
    Vector<T, n> operator<<(const Vector<S, n>& other) const {
        return Vector<T, n>{type::shlv(base, other.base)};
    }

    template<Compatible<T> S>
    Vector<T, n> operator>>(const Vector<S, n>& other) const {
        if constexpr(std::is_signed_v<T>) {
            return Vector<std::common_type_t<S, T>, n>{type::shrav(base, other.base)};
        }
        else {
            return Vector<std::common_type_t<S, T>, n>{type::shrlv(base, other.base)};
        }
    }

    template<Compatible<T> S>
    Vector<T, n> operator>(const Vector<S, n>& other) const {
        return Vector<T, n>{type::cmpgt(base, other.base)};
    }

    template<Compatible<T> S>
    Vector<T, n> operator==(const Vector<S, n>& other) const {
        return Vector<T, n>{type::cmpeq(base, other.base)};
    }

    template<Compatible<T> S>
    Vector<T, n> operator>=(const Vector<S, n>& other) const {
        return (*this > other) | (*this == other);
    }

    template<Compatible<T> S>
    Vector<T, n> operator!=(const Vector<S, n>& other) const {
        return !(*this == other);
    }

    template<Compatible<T> S>
    Vector<T, n> operator<=(const Vector<S, n>& other) const {
        return !(*this > other);
    }

    template<Compatible<T> S>
    Vector<T, n> operator<(const Vector<S, n>& other) const {
        return (*this <= other).andnot(*this == other);
    }

    Vector<T, n> operator-() const {
        return Vector<T, n>{0} - *this;
    }

    Vector<T, n> operator!() const {
        return Vector<T, n>{-1}.andnot(*this);
    }

    Vector<T, n> abs() const { return Vector<T, n>{type::abs(base)}; }
    Vector<T, n> sqrt() const { return Vector<T, n>{type::sqrt(base)}; }

    template<Compatible<T> S>
    Vector<T, n> andnot(const Vector<S, n>& other) const {
        return Vector<T, n>{type::andnot(base, other.base)};
    }

    void store(T* dest) const {
        if constexpr(std::is_integral_v<T>) {
            type::store(reinterpret_cast<vmath::vtype_t<signed_T, n>*>(dest), base);
        }
        else {
            type::store(dest, base);
        }
    }

    void storeu(T* dest) const { type::storeu(dest, base); }

    std::array<T, n> data() const {
        constexpr size_t alignment = sizeof(T) * n == 16 ? 16 : 32;
        alignas(alignment) std::array<T, n> data;
        store(data.data());
        return data;
    }

    std::array<T, n> datau() const {
        std::array<T, n> data;
        store(data.data());
        return data;
    }

    T operator[](size_t index) const {
        return data()[index];
    }

    VMATH_IOP_HELPER(+)
    VMATH_IOP_HELPER(-)
    VMATH_IOP_HELPER(*)
    VMATH_IOP_HELPER(/)
    VMATH_IOP_HELPER(<<)
    VMATH_IOP_HELPER(>>)
    VMATH_IOP_HELPER(&)
    VMATH_IOP_HELPER(|)
    VMATH_IOP_HELPER(^)
};

template<typename T, size_t n, typename S>
Vector<T, n> operator*(const S& other, const Vector<T, n>& v) {
    return v * other;
}

template<typename T>
using Vect128 = Vector<T, 128 / (8 * sizeof(T))>;
template<typename T>
using Vect256 = Vector<T, 256 / (8 * sizeof(T))>;

}