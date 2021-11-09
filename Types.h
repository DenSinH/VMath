#pragma once

#include <immintrin.h>
#include <cstdint>
#include <type_traits>
#include <memory>

namespace vmath {

namespace detail {

template<typename T>
concept unsigned_integral = std::is_integral_v<T> && std::is_unsigned_v<T>;

}

/*
 * https://www.cism.ucl.ac.be/Services/Formations/ICS/ics_2013.0.028/composerxe/Documentation/en_US/compiler_c/main_cls/GUID-014B2DB6-B363-4CEB-97EC-74CD6A018106.htm
 * */

template<typename T, size_t n>
struct vtype {
    using type = T;
};

#define _mm_slli_pi8 nullptr
#define _mm_srai_pi8 nullptr
#define _mm_srli_pi8 nullptr

#define TYPEM64(_size) template<> struct vtype<std::int ## _size ## _t, 64 / _size> { \
    using type = __m64;                                                     \
    static constexpr auto load = _mm_setr_pi ## _size; \
    static constexpr auto store = [](void* dest, const type& v) { std::memcpy(dest, &v, sizeof(type)); }; \
    static constexpr auto abs = _mm_abs_pi ## _size; \
    static constexpr auto add = _mm_add_pi ## _size; \
    static constexpr auto and_ = _m_pand; \
    static constexpr auto andnot = _m_pandn; \
    static constexpr auto xor_ = _m_pxor; \
    static constexpr auto sub = _mm_sub_pi ## _size; \
    static constexpr auto neg = _mm_sign_pi ## _size; \
    static constexpr auto shli = _mm_slli_pi ## _size; \
    static constexpr auto shrai = _mm_srai_pi ## _size; \
    static constexpr auto shrli = _mm_srli_pi ## _size; \
    static constexpr auto cmpeq = _mm_cmpeq_pi ## _size;                              \
    static constexpr auto cmpgt = _mm_cmpgt_pi ## _size;                              \
}

TYPEM64(8);
TYPEM64(16);
TYPEM64(32);

template<>
struct vtype<std::int8_t, 16> {
    using type = __m128i;
    static constexpr auto load = _mm_setr_epi8;
    static constexpr auto store = _mm_store_epi32;
    static constexpr auto abs = _mm_abs_epi8;
    static constexpr auto add = _mm_add_epi8;
};

template<>
struct vtype<std::int16_t, 8> {
    using type = __m128i;
    static constexpr auto load = _mm_setr_epi16;
    static constexpr auto store = _mm_store_epi32;
    static constexpr auto abs = _mm_abs_epi16;
    static constexpr auto add = _mm_add_epi16;
};

template<>
struct vtype<std::int32_t, 4> {
    using type = __m128i;
    static constexpr auto load = _mm_setr_epi32;
    static constexpr auto store = _mm_store_epi32;
    static constexpr auto abs = _mm_abs_epi32;
    static constexpr auto add = _mm_add_epi32;
};

template<>
struct vtype<std::int64_t, 2> {
    using type = __m128i;
    static constexpr auto load = _mm_setr_epi64;
    static constexpr auto store = _mm_store_epi64;
    static constexpr auto abs = _mm_abs_epi64;
    static constexpr auto add = _mm_add_epi64;
};

template<>
struct vtype<std::int8_t, 32> {
    using type = __m256i;
};

template<>
struct vtype<std::int16_t, 16> {
    using type = __m256i;
};

template<>
struct vtype<std::int32_t, 8> {
    using type = __m256i;
};

template<>
struct vtype<std::int64_t, 4> {
    using type = __m256i;
};

template<>
struct vtype<float, 8> {
    using type = __m256;
};

template<>
struct vtype<double, 4> {
    using type = __m256d;
};

template<>
struct vtype<float, 4> {
    using type = __m128;
};

template<>
struct vtype<double, 2> {
    using type = __m128d;
};

template<typename T, size_t n>
using vtype_t = typename vtype<T, n>::type;

}