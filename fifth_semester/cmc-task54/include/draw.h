#pragma once

#include "glsl.h"
#include "ogl.h"

#include <memory>

/**
 * @file
 * @author Lutov V. S. <vslutov@yandex.ru>
 * @brief Drawing functional objects.
 */

/**
 * Render program for each pixel.
 */
class Draw
{
private:
    std::shared_ptr<VBO<vec3>> vbo;
    std::shared_ptr<ShaderProgram> program;
    GLfloat camera_len;
    vec2 rotate = vec2(0, M_PI/20);
    vec2 mouse_pos = vec2();
    bool mouse_b1;
public:
    /**
     * Basic constructor.
     *
     * @param in_program program to draw.
     */
    Draw(const std::shared_ptr<ShaderProgram> &in_program,
         GLfloat in_camera_len);

    /**
     * Calls from Context.
     *
     * @param context program context
     */
    void
    operator()(Context &context);

    void
    update_rotate(Context &);

    vec2
    get_fov(const Context &) const;
};
