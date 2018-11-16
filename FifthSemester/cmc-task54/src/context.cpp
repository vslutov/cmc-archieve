#include "context.h"

#include "helper.h"

#include <ctime>
#include <GL/freeglut.h>
#include <iostream>
#include <stdexcept>

static intmax_t MAX_COUNTER = 200;

std::shared_ptr<Context>
Context::singleton = std::shared_ptr<Context>();

static void
RenderCB()
{
    static intmax_t counter = 0;
    static clock_t start = clock();

    glClearColor(0, 0, 0, 0); CHECK_GL_ERRORS;
    glClear(GL_COLOR_BUFFER_BIT);       CHECK_GL_ERRORS;

    auto context = Context::get_singleton();

    for (const auto &cb : context->render_callbacks) {
        cb(*context);
    }

    glutSwapBuffers();                  CHECK_GL_ERRORS;

    ++ counter;
    if (counter == MAX_COUNTER) {
        double fps = MAX_COUNTER / (double(clock() - start) * 100 / CLOCKS_PER_SEC);
        std::cout << "FPS: " << fps << std::endl;
        counter = 0;
        start = clock();
    }
};

static void
KeyboardCB(unsigned char key, int, int)
{
    if (key == '\x1B') {
        exit(EXIT_SUCCESS);
    }
};

static void
IdleCB()
{
    glutPostRedisplay();
}

static void
MouseMotionCB(int x, int y)
{
    auto context = Context::get_singleton();
    context->set_mouse_pos(x, y);
}

static void
MouseCB(int button, int state, int x, int y)
{
    auto context = Context::get_singleton();
    if (button == GLUT_LEFT_BUTTON) {
        context->set_mouse_pos(x, y);
        context->set_mouse_b1(state == GLUT_DOWN);
    }
}

Context::Context(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitContextVersion(3, 0);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(-1, -1);
    glutCreateWindow("Black hole");

    glutDisplayFunc(RenderCB);
    glutKeyboardFunc(KeyboardCB);
    glutIdleFunc(IdleCB);
    glutMotionFunc(MouseMotionCB);
    glutMouseFunc(MouseCB);

    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        std::string error = reinterpret_cast<const char *>(glewGetErrorString(res));
        throw std::runtime_error(error);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   CHECK_GL_ERRORS;
}

std::shared_ptr<Context>
Context::setup(int argc, char **argv)
{
    Context::singleton = std::shared_ptr<Context>(new Context(argc, argv));
    return Context::singleton;
}


std::shared_ptr<Context>
Context::get_singleton()
{
    if (Context::singleton.use_count() == 0) {
        std::string error = "context doesn't setup";
        throw std::runtime_error(error);
    }

    return Context::singleton;
}

void
Context::main_loop()
{
    glutMainLoop();
}

GLfloat
Context::get_width() const
{
    return glutGet(GLUT_WINDOW_WIDTH);
}

GLfloat
Context::get_height() const
{
    return glutGet(GLUT_WINDOW_HEIGHT);
}

void
Context::set_pid(GLuint pid)
{
    if (pid != this->program_id) {
        glUseProgram(pid); CHECK_GL_ERRORS;
        this->program_id = pid;
    }
}

GLfloat
Context::get_mouse_x() const
{
    return this->mouse_x;
}

GLfloat
Context::get_mouse_y() const
{
    return this->mouse_y;
}

bool
Context::get_mouse_b1() const
{
    return this->mouse_b1;
}

void
Context::set_mouse_pos(int x, int y)
{
    this->mouse_x = x;
    this->mouse_y = y;
}

void
Context::set_mouse_b1(bool value)
{
    this->mouse_b1 = value;
}
