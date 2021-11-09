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

};

//#define _mm_slli_pi8 nullptr
//#define _mm_srai_pi8 nullptr
//#define _mm_srli_pi8 nullptr
//
//#define TYPEM64(_size) template<> struct vtype<std::int ## _size ## _t, 64 / _size> { \
//    using type = __m64;                                                     \
//    static constexpr auto load = _mm_setr_pi ## _size; \
//    static constexpr auto store = [](void* dest, const type& v) { std::memcpy(dest, &v, sizeof(type)); }; \
//    static constexpr auto abs = _mm_abs_pi ## _size; \
//    static constexpr auto add = _mm_add_pi ## _size; \
//    static constexpr auto and_ = _m_pand; \
//    static constexpr auto andnot = _m_pandn; \
//    static constexpr auto xor_ = _m_pxor; \
//    static constexpr auto sub = _mm_sub_pi ## _size; \
//    static constexpr auto neg = _mm_sign_pi ## _size; \
//    static constexpr auto shli = _mm_slli_pi ## _size; \
//    static constexpr auto shrai = _mm_srai_pi ## _size; \
//    static constexpr auto shrli = _mm_srli_pi ## _size; \
//    static constexpr auto cmpeq = _mm_cmpeq_pi ## _size;                              \
//    static constexpr auto cmpgt = _mm_cmpgt_pi ## _size;                              \
//}
//
//TYPEM64(8);
//TYPEM64(16);
//TYPEM64(32);

#define _mm_slli_epi8 nullptr
#define _mm_srai_epi8 nullptr
#define _mm_srli_epi8 nullptr
#define _mm_sllv_epi8 nullptr
#define _mm_srav_epi8 nullptr
#define _mm_srlv_epi8 nullptr
#define _mm_mullo_epi8 nullptr
#define _mm_mul_epu8 nullptr
#define _mm_mul_epu16 nullptr
#define _mm_mul_epu64 nullptr

#define TYPEM128(_size) template<> struct vtype<std::int ## _size ## _t, 128 / _size> { \
    using type = __m128i;                                                     \
    static constexpr auto load = _mm_setr_epi ## _size; \
    static constexpr auto load1 = _mm_set1_epi ## _size; \
    static constexpr auto store = _mm_store_si128; \
    static constexpr auto abs = _mm_abs_epi ## _size; \
    static constexpr auto add = _mm_add_epi ## _size; \
    static constexpr auto imul = _mm_mullo_epi ## _size; \
    static constexpr auto umul = _mm_mul_epu ## _size; \
    static constexpr auto idiv = nullptr; \
    static constexpr auto udiv = nullptr; \
    static constexpr auto and_ = _mm_and_si128; \
    static constexpr auto andnot = _mm_andnot_si128; \
    static constexpr auto xor_ = _mm_xor_si128; \
    static constexpr auto or_ = _mm_or_si128; \
    static constexpr auto sub = _mm_sub_epi ## _size; \
    static constexpr auto shli = _mm_slli_epi ## _size; \
    static constexpr auto shrai = _mm_srai_epi ## _size; \
    static constexpr auto shrli = _mm_srli_epi ## _size; \
    static constexpr auto shlv = _mm_sllv_epi ## _size; \
    static constexpr auto shrav = _mm_srav_epi ## _size; \
    static constexpr auto shrlv = _mm_srlv_epi ## _size; \
    static constexpr auto cmpeq = _mm_cmpeq_epi ## _size;                              \
    static constexpr auto cmpgt = _mm_cmpgt_epi ## _size;                              \
}

TYPEM128(8);
TYPEM128(16);
TYPEM128(32);
TYPEM128(64);

#define _mm256_slli_epi8 nullptr
#define _mm256_srai_epi8 nullptr
#define _mm256_srli_epi8 nullptr
#define _mm256_sllv_epi8 nullptr
#define _mm256_srav_epi8 nullptr
#define _mm256_srlv_epi8 nullptr
#define _mm256_mullo_epi8 nullptr
#define _mm256_mul_epu8 nullptr
#define _mm256_mul_epu16 nullptr
#define _mm256_mul_epu64 nullptr
#define _mm256_setr_epi64 _mm256_setr_epi64x
#define _mm256_set1_epi64 _mm256_set1_epi64x

#define TYPEM256(_size) template<> struct vtype<std::int ## _size ## _t, 256 / _size> { \
    using type = __m256i;                                                     \
    static constexpr auto load = _mm256_setr_epi ## _size; \
    static constexpr auto load1 = _mm256_set1_epi ## _size; \
    static constexpr auto store = _mm256_store_si256; \
    static constexpr auto abs = _mm256_abs_epi ## _size; \
    static constexpr auto add = _mm256_add_epi ## _size; \
    static constexpr auto imul = _mm256_mullo_epi ## _size; \
    static constexpr auto umul = _mm256_mul_epu ## _size; \
    static constexpr auto idiv = nullptr; \
    static constexpr auto udiv = nullptr; \
    static constexpr auto and_ = _mm256_and_si256; \
    static constexpr auto andnot = _mm256_andnot_si256; \
    static constexpr auto xor_ = _mm256_xor_si256; \
    static constexpr auto or_ = _mm256_or_si256; \
    static constexpr auto sub = _mm256_sub_epi ## _size; \
    static constexpr auto shli = _mm256_slli_epi ## _size; \
    static constexpr auto shrai = _mm256_srai_epi ## _size; \
    static constexpr auto shrli = _mm256_srli_epi ## _size; \
    static constexpr auto shlv = _mm256_sllv_epi ## _size; \
    static constexpr auto shrav = _mm256_srav_epi ## _size; \
    static constexpr auto shrlv = _mm256_srlv_epi ## _size; \
    static constexpr auto cmpeq = _mm256_cmpeq_epi ## _size;                              \
    static constexpr auto cmpgt = _mm256_cmpgt_epi ## _size;                              \
}

TYPEM256(8);
TYPEM256(16);
TYPEM256(32);
TYPEM256(64);

template<>
struct vtype<float, 4> {
    using type = __m128;
};

template<>
struct vtype<double, 2> {
    using type = __m128d;
};

template<>
struct vtype<float, 8> {
    using type = __m256;
};

template<>
struct vtype<double, 4> {
    using type = __m256d;
};

template<typename T, size_t n>
using vtype_t = typename vtype<T, n>::type;

}