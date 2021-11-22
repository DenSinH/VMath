#pragma once

#include <immintrin.h>
#include <cstdint>
#include <type_traits>
#include <memory>

namespace vmath {

/*
 * https://www.cism.ucl.ac.be/Services/Formations/ICS/ics_2013.0.028/composerxe/Documentation/en_US/compiler_c/main_cls/GUID-014B2DB6-B363-4CEB-97EC-74CD6A018106.htm
 * */

template<typename T, size_t n>
struct vtype {

};

#define _vmath_mm_extract_epi(_size, _type, _prefix) []<int imm8>(_type vec) { return _prefix ## _extract_epi ## _size (vec, imm8); }

#define VMATH_INTEGRAL_TYPE_HELPER(_size, _type, _type_bits, _prefix) template<> struct vtype<std::int ## _size ## _t, _type_bits / _size> { \
    using type = _type; \
    static constexpr auto set = _prefix ## _setr_epi ## _size; \
    static constexpr auto set1 = _prefix ## _set1_epi ## _size; \
    static constexpr auto load = _prefix ## _load_si ## _type_bits; \
    static constexpr auto loadu = _prefix ## _loadu_si ## _type_bits; \
    static constexpr auto store = _prefix ## _store_si ## _type_bits; \
    static constexpr auto storeu = _prefix ## _storeu_epi ## _size; \
    static constexpr auto extract = _vmath_mm_extract_epi(_size, _type, _prefix); \
    static constexpr auto abs = _prefix ## _abs_epi ## _size; \
    static constexpr auto add = _prefix ## _add_epi ## _size; \
    static constexpr auto mask_add = _prefix ## _mask_add_epi ## _size; \
    static constexpr auto maskz_add = _prefix ## _maskz_add_epi ## _size; \
    static constexpr auto mul = _prefix ## _mullo_epi ## _size; \
    static constexpr auto mask_mul = _prefix ## _mask_mullo_epi ## _size; \
    static constexpr auto maskz_mul = _prefix ## _maskz_mullo_epi ## _size; \
    static constexpr auto umul = _prefix ## _mul_epu ## _size; \
    static constexpr auto mask_umul = _prefix ## _mask_mul_epu ## _size; \
    static constexpr auto maskz_umul = _prefix ## _maskz_mul_epu ## _size; \
    static constexpr auto div = nullptr; \
    static constexpr auto mask_div = nullptr; \
    static constexpr auto maskz_div = nullptr; \
    static constexpr auto and_ = _prefix ## _and_si ## _type_bits; \
    static constexpr auto andnot = _prefix ## _andnot_si ## _type_bits; \
    static constexpr auto xor_ = _prefix ## _xor_si ## _type_bits; \
    static constexpr auto or_ = _prefix ## _or_si ## _type_bits; \
    static constexpr auto sub = _prefix ## _sub_epi ## _size; \
    static constexpr auto mask_sub = _prefix ## _mask_sub_epi ## _size; \
    static constexpr auto maskz_sub = _prefix ## _maskz_sub_epi ## _size; \
    static constexpr auto shli = _prefix ## _slli_epi ## _size; \
    static constexpr auto shrai = _prefix ## _srai_epi ## _size; \
    static constexpr auto shrli = _prefix ## _srli_epi ## _size; \
    static constexpr auto shlv = _prefix ## _sllv_epi ## _size; \
    static constexpr auto shrav = _prefix ## _srav_epi ## _size; \
    static constexpr auto shrlv = _prefix ## _srlv_epi ## _size; \
    static constexpr auto cmpeq = _prefix ## _cmpeq_epi ## _size; \
    static constexpr auto cmpgt = _prefix ## _cmpgt_epi ## _size; \
    static constexpr auto min = _prefix ## _min_epi ## _size; \
    static constexpr auto mask_min = _prefix ## _mask_min_epi ## _size; \
    static constexpr auto maskz_min = _prefix ## _maskz_min_epi ## _size; \
    static constexpr auto mask_umin = _prefix ## _mask_min_epu ## _size; \
    static constexpr auto maskz_umin = _prefix ## _maskz_min_epu ## _size; \
    static constexpr auto max = _prefix ## _max_epi ## _size; \
    static constexpr auto mask_max = _prefix ## _mask_max_epi ## _size; \
    static constexpr auto maskz_max = _prefix ## _maskz_max_epi ## _size; \
    static constexpr auto umax = _prefix ## _max_epu ## _size; \
    static constexpr auto mask_umax = _prefix ## _mask_max_epu ## _size; \
    static constexpr auto maskz_umax = _prefix ## _maskz_max_epu ## _size; \
}

#define VMATH_FP_TYPE_HELPER(_base_type, _base_bits, _type, _type_bits, _prefix, _postfix) template<> struct vtype<_base_type, _type_bits / (8 * sizeof(_base_type))> { \
    using type = _type; \
    static constexpr auto set = _prefix ## _setr_ ## _postfix; \
    static constexpr auto set1 = _prefix ## _set1_ ## _postfix; \
    static constexpr auto load = _prefix ## _load_si ## _type_bits; \
    static constexpr auto loadu = _prefix ## _loadu_si ## _type_bits; \
    static constexpr auto store = _prefix ## _store_ ## _postfix; \
    static constexpr auto storeu = _prefix ## _storeu_ ## _postfix; \
    static constexpr auto extract = _vmath_mm_extract_epi(_base_bits, __m ## _type_bits ## i, _prefix); \
    static constexpr auto abs = [](type& v) { \
        __m ## _type_bits ## i minus1 = _prefix ## _set1_epi32(-1); \
        _type mask = _prefix ## _castsi ## _type_bits ## _ ## _postfix(_prefix ## _srli_epi ## _base_bits(minus1, 1)); \
        return _prefix ## _and_ ## _postfix(mask, v); \
    }; \
    static constexpr auto add = _prefix ## _add_ ## _postfix; \
    static constexpr auto mask_add = _prefix ## _mask_add_ ## _postfix; \
    static constexpr auto maskz_add = _prefix ## _maskz_add_ ## _postfix; \
    static constexpr auto mul = _prefix ## _mul_ ## _postfix; \
    static constexpr auto mask_mul = _prefix ## _mask_mul_ ## _postfix; \
    static constexpr auto maskz_mul = _prefix ## _maskz_mul_ ## _postfix; \
    static constexpr auto div = _prefix ## _div_ ## _postfix; \
    static constexpr auto mask_div = _prefix ## _mask_div_ ## _postfix; \
    static constexpr auto maskz_div = _prefix ## _maskz_div_ ## _postfix; \
    static constexpr auto sub = _prefix ## _sub_ ## _postfix; \
    static constexpr auto mask_sub = _prefix ## _mask_sub_ ## _postfix; \
    static constexpr auto maskz_sub = _prefix ## _maskz_sub_ ## _postfix; \
    static constexpr auto cmpeq = _prefix ## _cmpeq_ ## _postfix; \
    static constexpr auto cmpgt = _prefix ## _cmpgt_ ## _postfix; \
    static constexpr auto sqrt = _prefix ## _sqrt_ ## _postfix; \
    static constexpr auto mask_sqrt = _prefix ## _mask_sqrt_ ## _postfix; \
    static constexpr auto maskz_sqrt = _prefix ## _maskz_sqrt_ ## _postfix; \
    static constexpr auto rsqrt = _prefix ## _rsqrt14_ ## _postfix; \
    static constexpr auto mask_rsqrt = _prefix ## _mask_rsqrt14_ ## _postfix; \
    static constexpr auto maskz_rsqrt = _prefix ## _maskz_rsqrt14_ ## _postfix; \
    static constexpr auto min = _prefix ## _min_ ## _postfix; \
    static constexpr auto mask_min = _prefix ## _mask_min_ ## _postfix; \
    static constexpr auto maskz_min = _prefix ## _maskz_min_ ## _postfix; \
    static constexpr auto max = _prefix ## _max_ ## _postfix; \
    static constexpr auto mask_max = _prefix ## _mask_max_ ## _postfix; \
    static constexpr auto maskz_max = _prefix ## _maskz_max_ ## _postfix; \
}

#define VMATH_TYPEM128I_HELPER(_size) VMATH_INTEGRAL_TYPE_HELPER(_size, __m128i, 128, _mm)
#define VMATH_TYPEM256I_HELPER(_size) VMATH_INTEGRAL_TYPE_HELPER(_size, __m256i, 256, _mm256)

#define _mm_slli_epi8 nullptr
#define _mm_srai_epi8 nullptr
#define _mm_srli_epi8 nullptr
#define _mm_sllv_epi8 nullptr
#define _mm_srav_epi8 nullptr
#define _mm_srlv_epi8 nullptr
#define _mm_mullo_epi8 nullptr
#define _mm_mask_mullo_epi8 nullptr
#define _mm_maskz_mullo_epi8 nullptr
#define _mm_mul_epu8 nullptr
#define _mm_mask_mul_epu8 nullptr
#define _mm_maskz_mul_epu8 nullptr
#define _mm_mul_epu16 nullptr
#define _mm_mask_mul_epu16 nullptr
#define _mm_maskz_mul_epu16 nullptr
#define _mm_mul_epu64 nullptr
#define _mm_mask_mul_epu64 nullptr
#define _mm_maskz_mul_epu64 nullptr


VMATH_TYPEM128I_HELPER(8);
VMATH_TYPEM128I_HELPER(16);
VMATH_TYPEM128I_HELPER(32);
VMATH_TYPEM128I_HELPER(64);

#define _mm256_slli_epi8 nullptr
#define _mm256_srai_epi8 nullptr
#define _mm256_srli_epi8 nullptr
#define _mm256_sllv_epi8 nullptr
#define _mm256_srav_epi8 nullptr
#define _mm256_srlv_epi8 nullptr
#define _mm256_mullo_epi8 nullptr
#define _mm256_mask_mullo_epi8 nullptr
#define _mm256_maskz_mullo_epi8 nullptr
#define _mm256_mul_epu8 nullptr
#define _mm256_mask_mul_epu8 nullptr
#define _mm256_maskz_mul_epu8 nullptr
#define _mm256_mul_epu16 nullptr
#define _mm256_mask_mul_epu16 nullptr
#define _mm256_maskz_mul_epu16 nullptr
#define _mm256_mul_epu64 nullptr
#define _mm256_mask_mul_epu64 nullptr
#define _mm256_maskz_mul_epu64 nullptr
#define _mm256_setr_epi64 _mm256_setr_epi64x
#define _mm256_set1_epi64 _mm256_set1_epi64x

VMATH_TYPEM256I_HELPER(8);
VMATH_TYPEM256I_HELPER(16);
VMATH_TYPEM256I_HELPER(32);
VMATH_TYPEM256I_HELPER(64);


#define _mm256_cmpeq_ps [](__m256 a, __m256 b) { return _mm256_cmp_ps(a, b, _CMP_EQ_OS); }
#define _mm256_cmpeq_pd [](__m256d a, __m256d b) { return _mm256_cmp_pd(a, b, _CMP_EQ_OS); }
#define _mm256_cmpgt_ps [](__m256 a, __m256 b) { return _mm256_cmp_ps(a, b, _CMP_GT_OS); }
#define _mm256_cmpgt_pd [](__m256d a, __m256d b) { return _mm256_cmp_pd(a, b, _CMP_GT_OS); }

VMATH_FP_TYPE_HELPER(float, 32, __m128, 128, _mm, ps);
VMATH_FP_TYPE_HELPER(double, 64, __m128d, 128, _mm, pd);
VMATH_FP_TYPE_HELPER(float, 32, __m256, 256, _mm256, ps);
VMATH_FP_TYPE_HELPER(double, 64, __m256d, 256, _mm256, pd);


template<typename T, size_t n>
using vtype_t = typename vtype<T, n>::type;

}

#undef VMATH_FP_TYPE_HELPER
#undef VMATH_INTEGRAL_TYPE_HELPER
#undef VMATH_TYPEM128I_HELPER
#undef VMATH_TYPEM256I_HELPER