#include <cstdio>
#include "../Types.h"
#include "../Vector.h"
#include "../Matrix.h"


int main() {
    vmath::Vector<std::uint32_t, 4> v1{1, 2, 3, 4};
    vmath::Vector<std::uint32_t, 4> v2{4, 3, 2, 1};
    v1 += -v2;
    vmath::Vector<std::uint32_t, 8> w1{1, 2, 3, 4, 5, 6, 7, 8};
    vmath::Vector<std::uint32_t, 8> w2{5, 6, 7, 8, 4, 3, 2, 1};
    w1 += -w2;
    vmath::Vect256<double> y1{1, 2, 3, 4};
    vmath::Vect256<double> y2{5, 6, 7, 8};
    y1 /= y2 * y1 + y1;
    y1 *= 2;
    y1 = y1.sqrt().mask_clamp(0, 0.5, 0xc);
    std::printf("%d\n", v1.get<0>());
    std::printf("%d\n", w1.get<0>());
    std::printf("%f %f %f %f\n", y1.get<0>(), y1.get<1>(), y1.get<2>(), y1.get<3>());

    vmath::Vector<double, 3> e1{1, 2, 3};
    vmath::Vector<double, 3> e2{5, 6, 7};
    e1 += -e2 * e1;
    std::printf("%f\n", e1.get<0>());

    vmath::Matrix<double, 3, 3> mat{
            {1, 0, 0},
            {1, 1, 0},
            {1, 1, 1}
    };

    mat *= mat * 3;

    std::printf("mat\n%f\n", mat.get<0, 0>());
    std::printf("%f\n", mat.get<1, 0>());
    std::printf("%f\n", mat.get<2, 0>());

    return 0;
}