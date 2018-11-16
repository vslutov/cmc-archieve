#pragma once

#include <initializer_list>
#include <algorithm>
#include <tuple>
#include <memory>
#include <iostream>
#include <string>
#include <type_traits>

/**
 * @file
 * @author unknown
 * @brief Define Matrix class.
 */

/**
 * Matrix class with usefull methods.
 */
template<typename ValueT>
class Matrix
{
public:
    /**
     * Number of rows.
     */
    const ssize_t n_rows;
    /**
     * Number of cols.
     */
    const ssize_t n_cols;

    /**
     * Construct matrix with row_count of rows and col_count of columns.
     * @param row_count is n_rows of future matrix
     * @param col_count is n_cols of future matrix
     */
    Matrix(ssize_t row_count = 0, ssize_t col_count = 0);

    /**
     * Construct and initialize matrix which consists of one row (C++11).
     *
     * Example:
     *
     *     Matrix<int> binomial = {1, 4, 6, 4, 1};
     *
     * @param list element list
     */
    Matrix(std::initializer_list<ValueT> list);

    /**
     * Construct and initialize matrix with 2d array (C++11).
     *
     * Example:
     *     Matrix<double> growing = { {0.1, 0.2, 0.3, 0.4},
     *                                {0.5, 0.6, 0.7, 0.8} };
     *
     * @param lists matrix of const
     */
    Matrix(std::initializer_list < std::initializer_list < ValueT >> lists);

    /**
     * Copy constructor.
     *
     * Be careful, this function just copies the pointer
     * to data and doesn't allocate any memory for data!
     */
    Matrix(const Matrix&);

    /**
     * Allocates memory and copies all values.
     *
     * @return copy of matrix
     */
    Matrix<ValueT> deep_copy() const;

    /**
     * Assignment operator.
     */
    const Matrix<ValueT> &operator = (const Matrix<ValueT> &m);

    /**
     * Move copy constructor (C++11).
     *
     * Needed when copy temporary object.
     */
    Matrix(Matrix &&);

    // Desctructor, yeah.
    ~Matrix();

    /**
     * Matrix indexing.
     *
     * Be careful, indexing starts at 0!
     *
     * Const for just taking value:
     *
     *     Matrix<int> a = {9, 8, 7};
     *     cout << a(0, 2); // 7
     *
     * @return element of matrix
     */
    const ValueT &operator() (ssize_t row, ssize_t col) const;

    /**
     * Non-const indexing for assignment.
     *
     * Example:
     *
     *     a(0, 1) = 3;
     *     cout << a; // 9 3 7
     */
    ValueT &operator() (ssize_t row, ssize_t col);

    /**
     * Matrix convolution.
     *
     * You give this function a unary operator. Operator _must_
     * have radius field and function
     * `operator()(const Matrix<ValueT> neighbourhood)`.
     * For every pixel of that matrix this function takes
     * neighbourhood of that pixel of size
     * `(2 * radius + 1) x (2 * radius + 1)`, applies operator to that
     * neighbourhood and writes result in a new matrix of the same size
     * Minimum radius is 0, operator will process only one pixel every time
     *
     * @param op unary operator
     * @return matrix of results of unary operator
     */
    template<typename UnaryMatrixOperator>
    // Function unary map returns a matrix of
    Matrix <
        // type which is returned
        typename std::result_of <
        // by operator applied to neighbourhood of pixel
        UnaryMatrixOperator(Matrix<ValueT>)
        > ::type
    >
    unary_map(const UnaryMatrixOperator &op) const;


    /**
     * Add mirror borders.
     *
     * @param kernel_vert_radius how many rows need to add up and down to matrix
     * @param kernel_hor_radius how many cols need ot add left and right to matrix
     * @return Matrix with borders
     */
    Matrix<ValueT> extra_borders(ssize_t kernel_vert_radius, ssize_t kernel_hor_radius) const;
    //transpond matrix
    Matrix<ValueT> trans() const;

    /**
     * Same, but unary operator is mutable.
     * If you take operator with mutable fields, you can
     * make statistic computations using unary map
     * (statistics like sum of pixel values or histograms of pixel values)
     */
    template<typename UnaryMatrixOperator>
    Matrix<typename std::result_of<UnaryMatrixOperator(Matrix<ValueT>)>::type>
        unary_map(UnaryMatrixOperator &op) const;

    /**
     * Get sumbmatrix of matrix.
     * Remember that indexing starts at 0!
     *
     * Example:
     *
     *     Matrix<int> a = { {1, 2, 3},
     *                       {4, 5, 6} };
     *     cout << a.submatrix(1, 1, 1, 2); // 5 6
     *
     * @param prow row of point, where to start slicing
     * @param pcol column of point, where to start slicing
     * @param rows length of submatrix in rows
     * @param cols length of submatrix in columns
     */
    const Matrix<ValueT> submatrix(ssize_t prow, ssize_t pcol,
        ssize_t rows, ssize_t cols) const;

private:
    // Stride - number of elements between two rows (needed for efficient
    // submatrix function without memory copy)
    const ssize_t stride;
    // First row and col, useful for taking submatrices. By default is (0, 0).
    const ssize_t pin_row, pin_col;
    // shared_ptr still has no support of c-style arrays and
    // <type[]> partial specialization like unique_ptr has.
    // works: unique_ptr<int[]>; doesn't: shared_ptr<int[]>.
    // so, for now we use shared_ptr just for counting links,
    // and work with raw pointer through get().
    std::shared_ptr<ValueT> _data;

    // Const cast for writing public const fields.
    template<typename T> inline T& make_rw(const T& val) const;
};

// Implementation of Matrix class
#include "matrix.hpp"
