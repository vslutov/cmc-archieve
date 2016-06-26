#pragma once

#include <memory>
#include <stdexcept>
#include <string>

template<typename ValueT>
VBO<ValueT>::VBO(ssize_t in_size) :
    updated(false),
    data(in_size)
{
}

template<typename ValueT>
ssize_t
VBO<ValueT>::get_size() const
{
    return this->data.size();
}

template<typename ValueT>
const ValueT &
VBO<ValueT>::operator[](ssize_t index) const
{
    if (0 <= index && index < static_cast<ssize_t>(this->data.size())) {
        return this->data[index];
    } else {
        std::string error = "VBO index error: " + std::to_string(index) +
                            " / " + std::to_string(this->data.size());
        throw std::runtime_error(error);
    }
}

template<typename ValueT>
ValueT &
VBO<ValueT>::operator[](ssize_t index)
{
    if (0 <= index && index < static_cast<ssize_t>(this->data.size())) {
        this->updated = false;
        return this->data[index];
    } else {
        std::string error = "VBO index error: " + std::to_string(index) +
                            " / " + std::to_string(this->data.size());
        throw std::runtime_error(error);
    }
}

template<typename ValueT>
void
VBO<ValueT>::push_back(const ValueT &elem)
{
    this->data.push_back(elem);
    this->updated = false;
}

template<typename ValueT>
GLuint
VBO<ValueT>::get_value()
{
    if (!(this->updated)) {
        bool have_initilized;

        if (this->vbo == 0) {
 	        glGenBuffers(1, &(this->vbo));
            have_initilized = false;
        } else {
            have_initilized = true;
        }

        ssize_t elem_size = sizeof(ValueT);
        ssize_t count = this->get_size();
        auto flat_data = new ValueT[count];

        for (ssize_t i = 0; i < count; ++ i) {
            flat_data[i] = this->data[i];
        }

	    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	    if (have_initilized) {
            glBufferData(GL_ARRAY_BUFFER,
                         elem_size * count,
                         flat_data,
                         GL_DYNAMIC_DRAW);
        } else {
            glBufferData(GL_ARRAY_BUFFER,
                         elem_size * count,
                         flat_data,
                         GL_DYNAMIC_DRAW);
        }
	    glBindBuffer(GL_ARRAY_BUFFER, 0);

        delete[] flat_data;

        this->updated = true;
    }

    return this->vbo;
}

