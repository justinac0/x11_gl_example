#ifndef GL_SHADER_H
#define GL_SHADER_H

#include "base.h"
#include <glad/gl.h>

// TODO(justin): move to os layer
GLOBAL char* read_file(char* filePath, const char* mode);
GLOBAL GLuint create_shader_program(char* vertexFilePath, char* fragmentFilePath);

#endif /* GL_SHADER_H */
