#ifndef LAB_08_MATRIX_H
#define LAB_08_MATRIX_H

#include <cstdio>
#include <cstddef>
#include <iostream>

class Matrix {
public:
    Matrix(std::size_t r, std::size_t c);

    Matrix(const Matrix &other);
    Matrix(Matrix &&other) noexcept;

    Matrix &operator=(const Matrix &other);
    Matrix &operator=(Matrix &&other) noexcept;

    void swap(Matrix &other);

    std::size_t rows() const;
    std::size_t cols() const;

    void set(std::size_t i, std::size_t j, int val);
    int get(std::size_t i, std::size_t j) const;

    int *operator[](std::size_t row);
    const int *operator[](std::size_t row) const;

    friend Matrix operator*(const Matrix &a, const Matrix &b);
    friend Matrix operator+(Matrix a, const Matrix &b);
    friend Matrix operator-(Matrix a, const Matrix &b);

    friend bool operator==(const Matrix &a, const Matrix &b);
    friend bool operator!=(const Matrix &a, const Matrix &b);

    Matrix &operator+=(const Matrix &other);
    Matrix &operator-=(const Matrix &other);
    Matrix &operator*=(const Matrix &other);

    friend std::ostream &operator<<(std::ostream &out, const Matrix &mat);

    ~Matrix();

private:
    static int **alloc_2d_array(std::size_t r, std::size_t c);

    std::size_t _rows = 0;
    std::size_t _cols = 0;
    int **_data = nullptr;
};

#endif