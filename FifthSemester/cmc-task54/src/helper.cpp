#include "helper.h"

#include "context.h"

#include <string>
#include <stdexcept>

void
ThrowExceptionOnGLError(int line, const char *file)
{
    static char errMsg[512];

    GLenum gl_error = glGetError();

    if(gl_error != GL_NO_ERROR) {

        switch(gl_error)
        {
        case GL_INVALID_ENUM:
            sprintf(errMsg, "GL_INVALID_ENUM file %s line %d\n", file, line);
            break;

        case GL_INVALID_VALUE:
            sprintf(errMsg, "GL_INVALID_VALUE file %s line %d\n",  file, line);
            break;

        case GL_INVALID_OPERATION:
            sprintf(errMsg, "GL_INVALID_OPERATION file %s line %d\n",  file, line);
            break;

        case GL_STACK_OVERFLOW:
            sprintf(errMsg, "GL_STACK_OVERFLOW file %s line %d\n",  file, line);
            break;

        case GL_STACK_UNDERFLOW:
            sprintf(errMsg, "GL_STACK_UNDERFLOW file %s line %d\n",  file, line);
            break;

        case GL_OUT_OF_MEMORY:
            sprintf(errMsg, "GL_OUT_OF_MEMORY file %s line %d\n",  file, line);
            break;

        case GL_TABLE_TOO_LARGE:
            sprintf(errMsg, "GL_TABLE_TOO_LARGE file %s line %d\n",  file, line);
            break;

        case GL_NO_ERROR:
            break;

        default:
            sprintf(errMsg, "Unknown error @ file %s line %d\n",  file, line);
            break;
        }

        throw std::runtime_error(errMsg);
    }
}
