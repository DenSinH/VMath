#pragma once

#include "Types.h"

#include <array>
#include <bit>
#include <tuple>


namespace vmath {

template<typename T, typename S>
concept Compatible = std::is_same_v<S, T> || std::is_same_v<std::make_unsigned_t<S>, std::make_unsigned_t<T>>;

#define VMATH_IOP_HELPER(_symbol) \
template<typename S> \
Vector<T, n>& operator _symbol ## =(S other) { \
    base = (*this _symbol other).base; \
    return *this; \
}

#define VMATH_BASIC_OPERATOR_HELPER(_symbol, _name) \
template<Compatible<T> S> \
Vector<std::common_type_t<S, T>, n> operator _symbol(const Vector<S, n>& other) const { \
    return Vector<std::common_type_t<S, T>, n>{type:: _name(base, other.base)}; \
} \

#define VMATH_BASIC_OPERATOR_MASK_HELPER(_name) \
template<Compatible<T> S> \
Vector<std::common_type_t<S, T>, n> mask_ ## _name(const Vector<S, n>& other, mask_t mask, const Vector<S, n>& src) const { \
    return Vector<std::common_type_t<S, T>, n>{type::mask_ ## _name(src.base, mask, base, other.base)}; \
} \
 \
template<Compatible<T> S> \
Vector<std::common_type_t<S, T>, n> mask_ ## _name(const Vector<S, n>& other, mask_t mask) const { \
    return mask_ ## _name(other, mask, *this); \
} \
 \
template<Compatible<T> S> \
Vector<std::common_type_t<S, T>, n> maskz_ ## _name(const Vector<S, n>& other, mask_t mask) const { \
    return Vector<std::common_type_t<S, T>, n>{type::maskz_ ## _name(mask, base, other.base)}; \
}

#define VMATH_SIGNED_OPERATOR_HELPER(_symbol, _name) \
template<Compatible<T> S> \
Vector<std::common_type_t<S, T>, n> operator _symbol(const Vector<S, n>& other) const { \
    if constexpr(!std::is_integral_v<T> || std::is_signed_v<T>) { \
        return Vector<std::common_type_t<S, T>, n>{type::_name(base, other.base)}; \
    } \
    else { \
        return Vector<std::common_type_t<S, T>, n>{type::u ## _name(base, other.base)}; \
    } \
}

#define VMATH_SIGNED_OPERATOR_MASK_HELPER(_name) \
template<Compatible<T> S> \
Vector<std::common_type_t<S, T>, n> mask_ ## _name(const Vector<S, n>& other, mask_t mask, const Vector<S, n>& src) const { \
    if constexpr(!std::is_integral_v<T> || std::is_signed_v<T>) { \
        return Vector<std::common_type_t<S, T>, n>{type::mask_ ## _name(src.base, mask, base, other.base)}; \
    } \
    else { \
        return Vector<std::common_type_t<S, T>, n>{type::mask_u ## _name(src.base, mask, base, other.base)}; \
    } \
} \
 \
template<Compatible<T> S> \
Vector<std::common_type_t<S, T>, n> mask_ ## _name(const Vector<S, n>& other, mask_t mask) const { \
    return mask_ ## _name(other, mask, *this); \
} \
 \
template<Compatible<T> S> \
Vector<std::common_type_t<S, T>, n> maskz_ ## _name(const Vector<S, n>& other, mask_t mask) const { \
    if constexpr(!std::is_integral_v<T> || std::is_signed_v<T>) { \
        return Vector<std::common_type_t<S, T>, n>{type::maskz_ ## _name(mask, base, other.base)}; \
    } \
    else { \
        return Vector<std::common_type_t<S, T>, n>{type::maskz_ ## _name(mask, base, other.base)}; \
    } \
}

namespace detail {

/*
 * For making an arbitrary type signed if possible
 * */
template<typename T>
struct identity { using type = T; };

template<typename T>
using try_make_signed =
typename std::conditional< std::is_integral<T>::value, std::make_signed<T>, identity<T>>::type;

template<typename T>
using try_make_signed_t = typename try_make_signed<T>::type;

/*
 * For creating a tuple with a fixed size, with given first elements
 * */
template<int n, typename T>
using copy_t = T;

template<typename T, typename... Args, size_t... Is, size_t... As>
auto fill_tuple_impl(std::index_sequence<Is...>, std::index_sequence<As...>, Args... args) {
    return std::tuple<typename detail::copy_t<Is, T>...>{args..., ((void)As, 0)...};
}

template<typename T, int n, typename... Args>
auto fill_tuple(Args... args) {
    return detail::fill_tuple_impl<T>(std::make_index_sequence<n>{}, std::make_index_sequence<n - sizeof...(Args)>{}, args...);
}

/*
 * For getting function argument types.
 * */
template<typename T> struct func { };
template<typename R, typename First, typename... Args>
struct func<R (*const) (First, Args...)> {
    using return_t = R;
    using first_t = First;
};

/*
 * For getting equal sized, integer argument types
 * */
template<typename T>
struct integral_size {
    using type = try_make_signed_t<T>;
};

template<>
struct integral_size<float> {
    using type = std::uint32_t;
};

template<>
struct integral_size<double> {
    using type = std::uint64_t;
};

template<typename T>
using integral_size_t = typename integral_size<T>::type;

}

template<typename T, size_t n>
struct Vector {
    static_assert(n * sizeof(T) * 8 <= 256);

    using sT = detail::try_make_signed_t<T>;
    static constexpr size_t base_n = (8 * sizeof(T) * n < 128) ? 128 / (8 * sizeof(T)) : 256 / (8 * sizeof(T));
    using type = vtype<sT, base_n>;
    using mask_t = std::uint32_t;

    vtype_t<sT, base_n> base;

    template<typename... Args>
    requires (sizeof...(Args) == n)
    Vector(Args... args) : base(std::apply(type::set, detail::fill_tuple<sT, base_n>((sT)args...))) {

    }

    Vector(T value) : base(type::set1(value)) {

    }

    Vector() : Vector(0) {

    }

    Vector(const vtype_t<sT, base_n>& other) : base(other) {

    }

    template<int imm8>
    requires (0 <= imm8) && (imm8 < n)
    T get() const {
        return std::bit_cast<T>(type::extract.template operator()<imm8>(base));
    }

    static Vector<T, n> load(const T* memory) {
        return type::load(reinterpret_cast<typename detail::func<decltype(type::load)>::first_t>(memory));
    }

    static Vector<T, n> loadu(const T* memory) {
        return type::loadu(memory);
    }

    void store(T* dest) const {
        if constexpr(std::is_integral_v<T>) {
            type::store(reinterpret_cast<vmath::vtype_t<sT, n>*>(dest), base);
        }
        else {
            type::store(dest, base);
        }
    }

    void storeu(T* dest) const {
        type::storeu(dest, base);
    }

    std::array<T, base_n> data() const {
        alignas(sizeof(T) * base_n) std::array<T, base_n> data;
        store(data.data());
        return data;
    }

    std::array<T, base_n> datau() const {
        std::array<T, base_n> data;
        store(data.data());
        return data;
    }

    template<Compatible<T> S>
    Vector<T, n> permutex2var(Vector<detail::integral_size_t<T>, n> permutation, Vector<S, n> other) const {
        return Vector<T, n>{type::permutex2var(base, permutation.base, other.base)};
    }

    Vector<T, n> permutexvar(Vector<detail::integral_size_t<T>, n> permutation) const {
        return Vector<T, n>{type::permutexvar(permutation.base, base)};
    }

    VMATH_BASIC_OPERATOR_HELPER(+, add)
    VMATH_BASIC_OPERATOR_MASK_HELPER(add)
    VMATH_BASIC_OPERATOR_HELPER(-, sub)
    VMATH_BASIC_OPERATOR_MASK_HELPER(sub)
    VMATH_SIGNED_OPERATOR_HELPER(*, mul)
    VMATH_SIGNED_OPERATOR_MASK_HELPER(mul)

    Vector<T, n> operator*(const T& other) const {
        if constexpr(!std::is_integral_v<T> || std::is_signed_v<T>) {
            return Vector<T, n>{type::mul(base, Vector<T, n>{other}.base)};
        }
        else {
            return Vector<T, n>{type::umul(base, Vector<T, n>{other}.base)};
        }
    }

    VMATH_BASIC_OPERATOR_HELPER(/, div)
    VMATH_BASIC_OPERATOR_MASK_HELPER(div)

    VMATH_BASIC_OPERATOR_HELPER(&, and_)
    VMATH_BASIC_OPERATOR_HELPER(|, or_)
    VMATH_BASIC_OPERATOR_HELPER(^, xor_)

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

    Vector<T, n> sqrt() const {
        return Vector<T, n>{type::sqrt(base)};
    }

    template<Compatible<T> S>
    Vector<T, n> mask_sqrt(mask_t mask, const Vector<S, n>& src) const {
        return Vector<T, n>{type::mask_sqrt(src.base, mask, base)};
    }

    template<Compatible<T> S>
    Vector<T, n> mask_sqrt(mask_t mask) const {
        return mask_sqrt(mask, *this);
    }

    template<Compatible<T> S>
    Vector<T, n> maskz_sqrt(mask_t mask) const {
        return Vector<T, n>{type::maskz_sqrt(mask, base)};
    }

    Vector<T, n> rsqrt() const {
        return Vector<T, n>{type::sqrt(base)};
    }

    template<Compatible<T> S>
    Vector<T, n> mask_rsqrt(mask_t mask, const Vector<S, n>& src) const {
        return Vector<T, n>{type::mask_sqrt(src.base, mask, base)};
    }

    template<Compatible<T> S>
    Vector<T, n> mask_rsqrt(mask_t mask) const {
        return mask_sqrt(mask, *this);
    }

    template<Compatible<T> S>
    Vector<T, n> maskz_rsqrt(mask_t mask) const {
        return Vector<T, n>{type::maskz_sqrt(mask, base)};
    }

    template<Compatible<T> S>
    Vector<T, n> andnot(const Vector<S, n>& other) const {
        return Vector<T, n>{type::andnot(base, other.base)};
    }

    template<Compatible<T> S>
    Vector<T, n> min(const Vector<S, n>& other) const {
        if constexpr(!std::is_integral_v<T> || std::is_signed_v<T>) {
            return Vector<T, n>{type::min(base, other.base)};
        }
        else {
            return Vector<T, n>{type::umin(base, other.base)};
        }
    }

    VMATH_SIGNED_OPERATOR_MASK_HELPER(min)

    Vector<T, n> min(T other) const {
        return min(Vector<T, n>{other});
    }

    template<Compatible<T> S>
    Vector<T, n> max(const Vector<S, n>& other) const {
        if constexpr(!std::is_integral_v<T> || std::is_signed_v<T>) {
            return Vector<T, n>{type::max(base, other.base)};
        }
        else {
            return Vector<T, n>{type::umax(base, other.base)};
        }
    }

    VMATH_SIGNED_OPERATOR_MASK_HELPER(max)

    Vector<T, n> max(T other) const {
        return max(Vector<T, n>{other});
    }

    template<Compatible<T> S>
    Vector<T, n> clamp(const Vector<S, n>& min, const Vector<S, n>& max) const {
        return min.max(this->min(Vector<T, n>{max}));
    }

    template<Compatible<T> S>
    Vector<T, n> mask_clamp(const Vector<S, n>& min, const Vector<S, n>& max, mask_t mask, const Vector<S, n>& src) const {
        return min.mask_max(this->min(Vector<T, n>{max}), mask, src);
    }

    template<Compatible<T> S>
    Vector<T, n> mask_clamp(const Vector<S, n>& min, const Vector<S, n>& max, mask_t mask) const {
        return min.mask_max(this->min(Vector<T, n>{max}), mask, *this);
    }

    template<Compatible<T> S>
    Vector<T, n> maskz_clamp(const Vector<S, n>& min, const Vector<S, n>& max, mask_t mask) const {
        return min.maskz_max(this->min(Vector<T, n>{max}), mask);
    }

    Vector<T, n> clamp(T min, T max) const {
        return clamp(Vector<T, n>{min}, Vector<T, n>{max});
    }

    template<Compatible<T> S>
    Vector<T, n> mask_clamp(T min, T max, mask_t mask, const Vector<S, n>& src) const {
        return mask_clamp(Vector<T, n>{min}, Vector<T, n>{max}, mask, src);
    }

    Vector<T, n> mask_clamp(T min, T max, mask_t mask) const {
        return mask_clamp(Vector<T, n>{min}, Vector<T, n>{max}, mask);
    }

    Vector<T, n> maskz_clamp(T min, T max, mask_t mask) const {
        return maskz_clamp(Vector<T, n>{min}, Vector<T, n>{max}, mask);
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

#undef VMATH_IOP_HELPER
#undef VMATH_BASIC_OPERATOR_HELPER
#undef VMATH_SIGNED_OPERATOR_HELPER
#undef VMATH_SIGNED_OPERATOR_MASK_HELPER

}