#include <cstdio>
#include "../Types.h"
#include "../Vector.h"


int main() {
    vmath::Vector<std::int8_t, 8> v1{1, 2, 3, 4, 5, 6, 7, 8};
    vmath::Vector<std::int8_t, 8> v2{1, 2, 3, 4, 5, 6, 7, 8};
    v1 += v2;
    vmath::Vector<std::int16_t, 8> w1{1, 2, 3, 4, 5, 6, 7, 8};
    vmath::Vector<std::int16_t, 8> w2{8, 2, 3, 4, 5, 6, 7, 8};
    w1 += w2;
    vmath::Vector<std::int16_t, 4> y1{1, 2, 3, 0xffff};
    y1 <<= 1;
    std::printf("%x\n", v1[0]);
    std::printf("%x\n", w1[0]);
    std::printf("%x\n", y1[3]);
    return 0;
}