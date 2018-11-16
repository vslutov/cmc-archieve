#pragma once

#include "context.h"
#include "glsl.h"

#include <cinttypes>
#include <initializer_list>
#include <string>
#include <vector>

/**
 * @file
 * @author Lutov V. S. <vslutov@yandex.ru>
 * @brief OpenGL primitives.
 */

/**
 * VBO class.
 */
template<typename ValueT>
class VBO
{
private:
    GLuint vbo = 0;
    bool updated = false;

    std::vector<ValueT> data = std::vector<ValueT>();
public:
    /**
     * Basic construction.
     *
     * @param in_size buffer element count
     */
    VBO(ssize_t in_size);

    /**
     * Count of element.
     */
    ssize_t
    get_size() const;

    /**
     * Const operator for getting elements.
     */
    const ValueT &
    operator[](ssize_t index) const;

    /**
     * Unconst operator for setting elements.
     */
    ValueT &
    operator[](ssize_t index);

    void
    push_back(const ValueT &elem);

    /**
     * Get VBO index.
     */
    GLuint
    get_value();
};

/**
 * Any kind of shader.
 */
class Shader {
private:
    GLuint shader_id;
public:
    /**
     * Load shader from file.
     *
     * @param shader_filepath path to shader
     * @param shader_type type of shader
     */
    Shader(const std::string &shader_filepath,
           GLenum shader_type);

    /**
     * Run glDeleteShader
     */
    ~Shader();

    /**
     * Get shader id.
     */
    GLuint
    get_value() const;
};

class Texture {
private:
    GLuint texture_id;
public:
    Texture(const std::string &filepath);
    ~Texture();

    GLuint
    get_value() const;
};

/**
 * Draw program.
 */
class ShaderProgram {
private:
    GLuint program_id;
    std::vector<GLint> attributes, uniforms;
    std::vector<std::pair<GLenum, std::shared_ptr<Texture>>> textures;
    GLenum texture_id = 0;
public:
    /**
     * Constructor for building program from source.
     *
     * @param vertex_filepath path to vertex shader
     * @param fragment_filepath path to fragment shader
     */
    ShaderProgram(const std::string &vertex_filepath,
                  const std::string &fragment_filepath);

    /**
     * Constructor for only fragment shader.
     *
     * @param fragment_filepath path to fragment shader
     */
    ShaderProgram(const std::string &fragment_filepath);

    /**
     * Destructor.
     */
    ~ShaderProgram();

    /**
     * Get program id.
     */
    GLuint
    get_value() const;

    void
    set_uniform(const std::string &name, GLfloat value);

    void
    set_uniform(const std::string &name, const vec2 &value);

    void
    set_uniform(const std::string &name, const vec3 &value);

    void
    set_uniform(const std::string &name, std::shared_ptr<Texture> &value);

    /**
     * Set attribute to vec3 VBO.
     *
     * @param name shader input name
     * @param vbo pointer to VBO
     */
    void
    set_attrib(const std::string &name,
               std::shared_ptr<VBO<vec3>> &vbo);

    /**
     * glUseProgram.
     */
    void
    setup();

    /**
     * Unbind arrays and buffers.
     */
    void
    shutdown();
};

#include "ogl.hpp"
