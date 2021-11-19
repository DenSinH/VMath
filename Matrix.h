#pragma once

#include "Vector.h"


namespace vmath {

template<typename T, size_t n>
struct Matrix {
    // use columns so that we can simply do columnwise multiplication with scalars
    // and vector addition for matrix vector products
    std::array<Vector<T, n>, n> columns;

    

};

}