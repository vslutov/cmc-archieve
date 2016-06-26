#pragma once

#include <GL/glew.h>
#include <functional>
#include <memory>
#include <vector>

/**
 * @file
 * @author Lutov V. S. <vslutov@yandex.ru>
 * @brief Context manager.
 *
 * Based on [OGLdev](http://ogldev.atspace.co.uk).
 */

/**
 * Context - windows, opengl context etc.
 */
class Context
{
private:
    /**
     * Private default constructor for singleton.
     */
    Context(int argc, char **argv);

    static std::shared_ptr<Context> singleton;
    GLuint program_id = 0;
    int mouse_x, mouse_y;
    bool mouse_b1;

public:
    /**
     * Add there render callbacks (runs every frame).
     */
    std::vector<std::function<void(Context &)>> render_callbacks =
        std::vector<std::function<void(Context &)>>();

    /**
     * Setup OpenGL, glut and glew.
     */
    static std::shared_ptr<Context>
    setup(int argc, char **argv);

    static std::shared_ptr<Context>
    get_singleton();

    /**
     * Main glut loop.
     */
    void
    main_loop();

    /**
     * Get window width.
     */
    GLfloat
    get_width() const;

    /**
     * Get window height.
     */
    GLfloat
    get_height() const;

    /**
     * Set program_id, if it is not set.
     */
    void
    set_pid(GLuint pid);

    GLfloat
    get_mouse_x() const;

    GLfloat
    get_mouse_y() const;

    bool
    get_mouse_b1() const;

    void
    set_mouse_pos(int x, int y);

    void
    set_mouse_b1(bool value);
};
