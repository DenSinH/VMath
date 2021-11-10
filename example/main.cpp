#include <cstdio>
#include "../Types.h"
#include "../Vector.h"


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
    y1 = y1.sqrt().clamp(0, 0.5);
    std::printf("%d\n", v1[0]);
    std::printf("%d\n", w1[0]);
    std::printf("%f\n", y1[0]);
    return 0;
}