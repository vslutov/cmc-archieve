#pragma once

#include "context.h"

#include <cmath>
#include <initializer_list>

/**
 * @file
 * @author Lutov V. S. <vslutov@yandex.ru>
 * @brief GLSL abstraction classes.
 */

/**
 * Abstract 3d vector.
 */
template<typename ValueT>
struct avec3 {
    ValueT x, y, z;

    /**
     * Default constrictor.
     *
     * x, y, z sets to zero.
     */
    avec3();

    /**
     * Create vector from coordinates.
     */
    avec3(ValueT in_x, ValueT in_y, ValueT in_z);

    /**
     * Make from initializer_list (C++11).
     *
     * Example:
     *
     *     vec3 a = {0, 0, 1};
     */
    avec3(const std::initializer_list<ValueT> &list);

    avec3<ValueT>
    operator+(const avec3<ValueT> &add) const;

    avec3<ValueT>
    operator-(const avec3<ValueT> &sub) const;

    avec3<ValueT>
    operator-() const;

    avec3<ValueT>
    operator*(ValueT mult) const;

    avec3<ValueT>
    operator/(ValueT divisor) const;

    ValueT
    len() const;

    avec3<ValueT>
    cross(const avec3<ValueT> &other) const;

    ValueT
    dot(const avec3<ValueT> &other) const;
};

/**
 * Float 3d vector.
 */
typedef avec3<GLfloat> vec3;

/**
 * Signed 3d vector.
 */
typedef avec3<GLint> ivec3;

/**
 * Unsigned 3d vector.
 */
typedef avec3<GLuint> uvec3;

typedef bool GLbool;

/**
 * Boolean 3d vector.
 */
typedef avec3<GLbool> bvec3;

/**
 * Abstract 2d vector.
 */
template<typename ValueT>
struct avec2 {
    ValueT x, y;

    /**
     * Default constrictor.
     *
     * x, y sets to zero.
     */
    avec2();

    /**
     * Create vector from coordinates.
     */
    avec2(ValueT in_x, ValueT in_y);

    /**
     * Make from initializer_list (C++11).
     *
     * Example:
     *
     *     vec2 a = {0, 0};
     */
    avec2(const std::initializer_list<ValueT> &list);

    avec2<ValueT>
    operator+(const avec2<ValueT> &add) const;

    avec2<ValueT>
    operator-(const avec2<ValueT> &sub) const;

    avec2<ValueT>
    operator*(ValueT mult) const;

    avec2<ValueT>
    operator/(ValueT divisor) const;
};

/**
 * Float 2d vector.
 */
typedef avec2<GLfloat> vec2;

/**
 * Signed 2d vector.
 */
typedef avec2<GLint> ivec2;

/**
 * Unsigned 2d vector.
 */
typedef avec2<GLuint> uvec2;

typedef bool GLbool;

/**
 * Boolean 2d vector.
 */
typedef avec2<GLbool> bvec2;

#include "glsl.hpp"
