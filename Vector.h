#pragma once

#include "Types.h"

#include <array>


namespace vmath {

template<typename T, typename S>
concept Compatible = std::is_same_v<S, T> || std::is_same_v<std::make_unsigned_t<S>, std::make_unsigned_t<T>>;

#define IOP(_symbol) \
    template<typename S> \
Vector<T, n>& operator _symbol ## =(S other) { \
    base = (*this _symbol other).base; \
    return *this; \
}

template<typename T, size_t n>
struct Vector {
    using signed_T = std::conditional_t<std::is_integral_v<T>, std::make_signed_t<T>, T>;
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
        if constexpr(std::is_signed_v<T>) {
            return Vector<std::common_type_t<S, T>, n>{type::imul(base, other.base)};
        }
        else {
            return Vector<std::common_type_t<S, T>, n>{type::umul(base, other.base)};
        }
    }

    template<Compatible<T> S>
    Vector<std::common_type_t<S, T>, n> operator/(const Vector<S, n>& other) const {
        if constexpr(std::is_signed_v<T>) {
            return Vector<std::common_type_t<S, T>, n>{type::idiv(base, other.base)};
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

    Vector<T, n> operator-() const {
        return Vector<T, n>{0} - *this;
    }

    std::array<T, n> data() const {
        constexpr size_t alignment = sizeof(T) * n == 16 ? 16 : 32;
        alignas(alignment) std::array<T, n> data;
        type::store(reinterpret_cast<vmath::vtype_t<signed_T, n>*>(data.data()), base);
        return data;
    }

    T operator[](size_t index) const {
        return data()[index];
    }

    IOP(+)
    IOP(-)
    IOP(*)
    IOP(/)
    IOP(<<)
    IOP(>>)
    IOP(&)
    IOP(|)
    IOP(^)
};


}