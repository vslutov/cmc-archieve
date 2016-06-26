#include "draw.h"

#include "helper.h"

#include <ctime>

Draw::Draw(const std::shared_ptr<ShaderProgram> &in_program,
           GLfloat in_camera_len) :
    vbo(std::make_shared<VBO<vec3>>(4)),
    program(in_program),
    camera_len(in_camera_len)
{
    (*vbo)[0] = {-1, -1, 0};
    (*vbo)[1] = {1, -1, 0};
    (*vbo)[2] = {-1, 1, 0};
    (*vbo)[3] = {1, 1, 0};
}

void
Draw::update_rotate(Context &context)
{
    if (!this->mouse_b1 && context.get_mouse_b1()) {
        this->mouse_b1 = true;
        this->mouse_pos = vec2(context.get_mouse_x() / context.get_width(),
                               context.get_mouse_y() / context.get_height());
    } else if (this->mouse_b1) {
        vec2 step = vec2(context.get_mouse_x() / context.get_width(),
                         context.get_mouse_y() / context.get_height());
        step = step - this->mouse_pos;
        vec2 fov = this->get_fov(context);
        this->rotate = this->rotate + vec2(step.x * fov.x, step.y * fov.y);
        if (this->rotate.y > M_PI * 0.48) {
            this->rotate.y = M_PI * 0.48;
        } else if (this->rotate.y < -M_PI * 0.48) {
            this->rotate.y = -M_PI * 0.48;
        }

        this->mouse_pos = this->mouse_pos + step;
    }

    if (!context.get_mouse_b1()) {
        this->mouse_b1 = false;
    }
}

vec2
Draw::get_fov(const Context &context) const
{
    vec2 window_size = vec2(context.get_width(), context.get_height());
    vec2 fov = window_size / std::max(window_size.x, window_size.y) * M_PI / 3;
    return fov;
}

void
Draw::operator()(Context &context)
{
    this->program->setup();
    float t = float(clock()) / (CLOCKS_PER_SEC * M_PI);

    this->update_rotate(context);

    float phi = t + this->rotate.x;
    float psi = -this->rotate.y;
    auto camera = vec3(sin(phi)*cos(psi), cos(phi)*cos(psi), sin(psi)) * this->camera_len;
    auto view = -camera / camera.len();

    program->set_uniform("camera", camera);
    program->set_uniform("view", view);

    vec2 fov = this->get_fov(context);
    vec3 dx = view.cross(vec3(0, sin(M_PI/6), cos(M_PI/6)));
    vec3 dy = view.cross(dx);
    dx = dx / dx.len() * view.len() * tan(fov.x / 2);
    dy = dy / dy.len() * view.len() * tan(fov.y / 2);
    this->program->set_uniform("dx", dx);
    this->program->set_uniform("dy", dy);

    this->program->set_attrib("vertex_location", this->vbo);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, this->vbo->get_size()); CHECK_GL_ERRORS;

    this->program->shutdown();
}
