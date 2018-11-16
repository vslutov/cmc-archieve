#pragma once

#include <stdexcept>

template<typename ValueT>
avec3<ValueT>::avec3() :
    x(ValueT()),
    y(ValueT()),
    z(ValueT())
{
}

template<typename ValueT>
avec3<ValueT>::avec3(const std::initializer_list<ValueT> &list)
{
    ssize_t i = 0;
    ValueT data[3];
    for (auto &elem : list) {
        if (i >= 3) {
            std::string error = "too much elements in vec init " +
                std::to_string(list.size()) + " / 3";
            throw std::runtime_error(error);
        }

        data[i] = elem;
        ++ i;
    }

    if (i < 3) {
        std::string error = "too few elements in vec init " +
            std::to_string(list.size()) + " / 3";
        throw std::runtime_error(error);
    }

    this->x = data[0];
    this->y = data[1];
    this->z = data[2];
}

template<typename ValueT>
avec3<ValueT>::avec3(ValueT in_x, ValueT in_y, ValueT in_z) :
    x(in_x),
    y(in_y),
    z(in_z)
{
}

template<typename ValueT>
avec3<ValueT>
avec3<ValueT>::operator+(const avec3<ValueT> &add) const
{
    return avec3<ValueT>(this->x + add.x, this->y + add.y, this->z + add.z);
}

template<typename ValueT>
avec3<ValueT>
avec3<ValueT>::operator-() const
{
    return avec3<ValueT>(-this->x, -this->y, -this->z);
}

template<typename ValueT>
avec3<ValueT>
avec3<ValueT>::operator-(const avec3<ValueT> &sub) const
{
    return avec3<ValueT>(this->x - sub.x, this->y - sub.y, this->z - sub.z);
}

template<typename ValueT>
avec3<ValueT>
avec3<ValueT>::operator*(ValueT mult) const
{
    return avec3<ValueT>(this->x * mult, this->y * mult, this->z * mult);
}

template<typename ValueT>
avec3<ValueT>
avec3<ValueT>::operator/(ValueT div) const
{
    return avec3<ValueT>(this->x / div, this->y / div, this->z / div);
}

template<typename ValueT>
ValueT
avec3<ValueT>::len() const
{
    return sqrt(this->dot(*this));
}

template<typename ValueT>
avec3<ValueT>
avec3<ValueT>::cross(const avec3<ValueT> &other) const
{
    return avec3<ValueT>(this->y * other.z - this->z * other.y,
                         this->z * other.x - this->x * other.z,
                         this->x * other.y - this->y * other.x);
}

template<typename ValueT>
ValueT
avec3<ValueT>::dot(const avec3<ValueT> &other) const
{
    return this->x * other.x + this->y * other.y + this->z * other.z;
}


template<typename ValueT>
avec2<ValueT>::avec2() :
    x(ValueT()),
    y(ValueT())
{
}

template<typename ValueT>
avec2<ValueT>::avec2(const std::initializer_list<ValueT> &list)
{
    ssize_t i = 0;
    ValueT data[2];
    for (auto &elem : list) {
        if (i >= 2) {
            std::string error = "too much elements in vec init " +
                std::to_string(list.size()) + " / 2";
            throw std::runtime_error(error);
        }

        data[i] = elem;
        ++ i;
    }

    if (i < 2) {
        std::string error = "too few elements in vec init " +
            std::to_string(list.size()) + " / 2";
        throw std::runtime_error(error);
    }

    this->x = data[0];
    this->y = data[1];
}

template<typename ValueT>
avec2<ValueT>::avec2(ValueT in_x, ValueT in_y) :
    x(in_x),
    y(in_y)
{
}


template<typename ValueT>
avec2<ValueT>
avec2<ValueT>::operator+(const avec2<ValueT> &add) const
{
    return avec2<ValueT>(this->x + add.x, this->y + add.y);
}

template<typename ValueT>
avec2<ValueT>
avec2<ValueT>::operator-(const avec2<ValueT> &sub) const
{
    return avec2<ValueT>(this->x - sub.x, this->y - sub.y);
}

template<typename ValueT>
avec2<ValueT>
avec2<ValueT>::operator*(ValueT mult) const
{
    return avec2<ValueT>(this->x * mult, this->y * mult);
}

template<typename ValueT>
avec2<ValueT>
avec2<ValueT>::operator/(ValueT divisor) const
{
    return avec2<ValueT>(this->x / divisor, this->y / divisor);
}
