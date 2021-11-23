#include <cstdio>
#include "../Types.h"
#include "../Vector.h"
#include "../Matrix.h"


int main() {
    // basic vector operations
    vmath::Vector<std::uint32_t, 4> v1{1, 2, 3, 4};
    vmath::Vector<std::uint32_t, 4> v2{4, 3, 2, 1};
    v1 += -v2;
    vmath::Vector<std::uint32_t, 8> w1{1, 2, 3, 4, 5, 6, 7, 8};
    vmath::Vector<std::uint32_t, 8> w2{5, 6, 7, 8, 4, 3, 2, 1};
    w1 += -w2;

    // special Vect128/Vect256 aliases and mask operations
    vmath::Vect256<double> y1{1, 2, 3, 4};
    vmath::Vect256<double> y2{5, 6, 7, 8};
    y1 /= y2 * y1 + y1;
    y1 *= 2;
    y1 = y1.sqrt().mask_clamp(0, 0.5, 0xc);
    std::printf("%d\n", v1.get<0>());
    std::printf("%d\n", w1.get<0>());
    std::printf("%f %f %f %f\n", y1.get<0>(), y1.get<1>(), y1.get<2>(), y1.get<3>());

    // non po2-sized vectors
    vmath::Vector<double, 3> e1{1, 2, 3};
    vmath::Vector<double, 3> e2{5, 6, 7};
    e1 += -e2 * e1;
    std::printf("%f\n", e1.get<0>());

    // only supported on AVX-512
//    e2 = e2.permutexvar({0, 1, 2});
//    std::printf("permuted: %f %f %f\n", e2.get<0>(), e2.get<1>(), e2.get<2>());

    // basic matrix operations
    vmath::Matrix<double, 3, 3> mat{
            {1, 0, 0},
            {1, 1, 0},
            {1, 1, 1}
    };

    mat *= mat * 3;
    mat += mat;
    std::printf("mat\n%f\n", mat.get<0, 0>());
    std::printf("%f\n", mat.get<1, 0>());
    std::printf("%f\n", mat.get<2, 0>());

    // matrix-vector multiplication
    e1 = mat * e1;
    std::printf("%f %f %f\n", e1.get<0>(), e1.get<1>(), e1.get<2>());

    // vector casting
    auto f1 = e1.convert<int32_t>().convert<int32_t>().convert<double>();
    std::printf("%f %f %f\n", f1.get<0>(), f1.get<1>(), f1.get<2>());

    // casting from __m256 to __m128 also works
    auto f2 = e1.convert<float>().convert<uint8_t>();
    std::printf("%d %d %d\n", f2.get<0>(), f2.get<1>(), f2.get<2>());
    std::printf("%d %d %d\n", (uint8_t)(-24), (uint8_t)(-108), (uint8_t)(-300));

    // reinterpreting vectors
    vmath::Vector<uint8_t, 8> uiv = {1, 2, 3, 4, 5, 6, 7, 8};
    auto uiv32 = uiv.reinterpret<uint32_t>();
    std::printf("%08x %08x\n", uiv32.get<0>(), uiv32.get<1>());

    return 0;
}