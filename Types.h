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

#define INTEGRAL_TYPE_HELPER(_size, _type, _type_bits, _prefix) template<> struct vtype<std::int ## _size ## _t, _type_bits / _size> { \
    using type = _type; \
    static constexpr auto load = _prefix ## _setr_epi ## _size; \
    static constexpr auto load1 = _prefix ## _set1_epi ## _size; \
    static constexpr auto store = _prefix ## _store_si ## _type_bits; \
    static constexpr auto abs = _prefix ## _abs_epi ## _size; \
    static constexpr auto add = _prefix ## _add_epi ## _size; \
    static constexpr auto mul = _prefix ## _mullo_epi ## _size; \
    static constexpr auto umul = _prefix ## _mul_epu ## _size; \
    static constexpr auto div = nullptr; \
    static constexpr auto udiv = nullptr; \
    static constexpr auto and_ = _prefix ## _and_si ## _type_bits; \
    static constexpr auto andnot = _prefix ## _andnot_si ## _type_bits; \
    static constexpr auto xor_ = _prefix ## _xor_si ## _type_bits; \
    static constexpr auto or_ = _prefix ## _or_si ## _type_bits; \
    static constexpr auto sub = _prefix ## _sub_epi ## _size; \
    static constexpr auto shli = _prefix ## _slli_epi ## _size; \
    static constexpr auto shrai = _prefix ## _srai_epi ## _size; \
    static constexpr auto shrli = _prefix ## _srli_epi ## _size; \
    static constexpr auto shlv = _prefix ## _sllv_epi ## _size; \
    static constexpr auto shrav = _prefix ## _srav_epi ## _size; \
    static constexpr auto shrlv = _prefix ## _srlv_epi ## _size; \
    static constexpr auto cmpeq = _prefix ## _cmpeq_epi ## _size;                              \
    static constexpr auto cmpgt = _prefix ## _cmpgt_epi ## _size;                            \
}

#define _mm256_cmpeq_ps [](__m256 a, __m256 b) { return _mm256_cmp_ps(a, b, _CMP_EQ_OS); }
#define _mm256_cmpeq_pd [](__m256d a, __m256d b) { return _mm256_cmp_pd(a, b, _CMP_EQ_OS); }
#define _mm256_cmpgt_ps [](__m256 a, __m256 b) { return _mm256_cmp_ps(a, b, _CMP_GT_OS); }
#define _mm256_cmpgt_pd [](__m256d a, __m256d b) { return _mm256_cmp_pd(a, b, _CMP_GT_OS); }

#define FP_TYPE_HELPER(_base_type, _base_bits, _type, _type_bits, _prefix, _postfix) template<> struct vtype<_base_type, _type_bits / (8 * sizeof(_base_type))> { \
    using type = _type; \
    static constexpr auto load = _prefix ## _setr_ ## _postfix; \
    static constexpr auto load1 = _prefix ## _set1_ ## _postfix; \
    static constexpr auto store = _prefix ## _store_ ## _postfix; \
    static constexpr auto abs = [](type& v) { \
        __m ## _type_bits ## i minus1 = _prefix ## _set1_epi32(-1); \
        _type mask = _prefix ## _castsi ## _type_bits ## _ ## _postfix(_prefix ## _srli_epi ## _base_bits(minus1, 1)); \
        return _prefix ## _and_ ## _postfix(mask, v); \
    }; \
    static constexpr auto add = _prefix ## _add_ ## _postfix; \
    static constexpr auto mul = _prefix ## _mul_ ## _postfix; \
    static constexpr auto div = _prefix ## _div_ ## _postfix; \
    static constexpr auto sub = _prefix ## _sub_ ## _postfix; \
    static constexpr auto cmpeq = _prefix ## _cmpeq_ ## _postfix;                              \
    static constexpr auto cmpgt = _prefix ## _cmpgt_ ## _postfix;                            \
}

#define TYPEM128I_HELPER(_size) INTEGRAL_TYPE_HELPER(_size, __m128i, 128, _mm)
#define TYPEM256I_HELPER(_size) INTEGRAL_TYPE_HELPER(_size, __m256i, 256, _mm256)

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

#define TYPEM128(_size) TYPEM128I_HELPER(_size)

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

#define TYPEM256(_size) TYPEM256I_HELPER(_size)

TYPEM256(8);
TYPEM256(16);
TYPEM256(32);
TYPEM256(64);

FP_TYPE_HELPER(float, 32, __m128, 128, _mm, ps);
FP_TYPE_HELPER(double, 64, __m128d, 128, _mm, pd);
FP_TYPE_HELPER(float, 32, __m256, 256, _mm256, ps);
FP_TYPE_HELPER(double, 64, __m256d, 256, _mm256, pd);


template<typename T, size_t n>
using vtype_t = typename vtype<T, n>::type;

}