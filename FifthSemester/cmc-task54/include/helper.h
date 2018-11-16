#pragma once

/**
 * @file
 * @author unknown
 * @brief Helper function for OpenGL debug.
 */

void
ThrowExceptionOnGLError(int line, const char *file);

#define CHECK_GL_ERRORS ThrowExceptionOnGLError(__LINE__,__FILE__)
