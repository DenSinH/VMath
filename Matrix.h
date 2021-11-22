#pragma once

#include "Vector.h"


namespace vmath {

template<typename T, size_t n, size_t m>
struct Matrix {
    // use columns so that we can simply do columnwise multiplication with scalars
    // and vector addition for matrix vector products
    // in this case, we do not need to account for non-po2 vectors in a special way.
    std::array<Vector<T, n>, m> columns{};

    template<size_t... s>
    requires ((s <= m) && ...) && (sizeof...(s) <= n)
    constexpr Matrix(const T (&...list)[s]) {
        // copy input data
        std::array<std::array<T, m>, n> data{};
        auto pos = data.data();
        ((std::copy(list, list + s, pos->data()), pos++), ...);

        // transpose data
        alignas(sizeof(T) * Vector<T, n>::base_n) std::array<std::array<T, Vector<T, n>::base_n>, m> transpose{};
        for (int j = 0; j < m; j++) {
            for (int i = 0; i < n; i++) {
                transpose[j][i] = data[i][j];
            }
        }

        // assign columns
        for (int j = 0; j < m; j++) {
            columns[j] = Vector<T, n>::load(transpose[j].data());
        }
    }

    template<size_t i, size_t j>
    T get() const {
        return columns[j].template get<i>();
    }

    Vector<T, n> operator*(const Vector<T, m>& other) const {
        Vector<T, n> result{};
        auto v = other.data();
        for (int i = 0; i < m; i++) {
            result += v[i] * columns[i];
        }
        return result;
    }

    template<size_t l>
    Matrix<T, n, l> operator*(const Matrix<T, m, l>& other) const {
        Matrix<T, n, l> result{};
        for (int i = 0; i < l; i++) {
            result.columns[i] = *this * other.columns[i];
        }
        return result;
    }

    Matrix<T, n, m> operator+(const Matrix<T, n, m>& other) const {
        Matrix<T, n, m> result = *this;
        result += other;
        return result;
    }

    template<typename S>
    requires (std::is_arithmetic_v<S>)
    Matrix<T, n, m> operator*(const S& other) const {
        Matrix<T, n, m> result{};
        for (int i = 0; i < m; i++) {
            result.columns[i] = other * columns[i];
        }
        return result;
    }

    template<typename S>
    requires (std::is_arithmetic_v<S>)
    Matrix<T, n, m>& operator*=(const S& other) {
        for (int i = 0; i < m; i++) {
            columns[i] *= other;
        }
        return *this;
    }

    template<size_t l>
    requires (l == n) && (m == n)
    Matrix<T, n, l> operator*=(const Matrix<T, m, l>& other) {
        for (int i = 0; i < l; i++) {
            columns[i] = *this * other.columns[i];
        }
        return *this;
    }

    Matrix<T, n, m>& operator+=(Matrix<T, n, m>& other) {
        for (int i = 0; i < m; i++) {
            columns[i] += other.columns[i];
        }
        return *this;
    }
};

template<typename S, typename T, size_t n, size_t m>
requires (std::is_arithmetic_v<S>)
Matrix<T, n, m> operator*(const S& other, const Matrix<T, n, m>& mat) {
    return mat * other;
}

}