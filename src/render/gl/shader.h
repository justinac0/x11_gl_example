#ifndef GL_SHADER_H
#define GL_SHADER_H

#include "base.h"
#include <glad/gl.h>

INTERNAL char* read_file(char* filePath);
GLOBAL GLuint create_shader_program(char* vertexFilePath, char* fragmentFilePath);

#endif /* GL_SHADER_H */
