#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"
extern "C"{
    void printGLString(const char *name, GLenum s) 
    {
        const char *v = (const char *) glGetString(s);
        LOGI("OpenGL error", "GL %s = %s\n", name, v);
    }
    void checkGlError(const char* op) 
    {
        for (GLint error = glGetError(); error; error
             = glGetError()) {
            LOGI("OpenGL error", "after %s() glError (0x%x)\n", op, error);
        }
    }
}
