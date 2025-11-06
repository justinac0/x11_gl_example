#ifndef GL_SHADER_H
#define GL_SHADER_H

INTERNAL char* read_file(char* filePath);
GLOBAL GLuint create_shader_program(char* vertexFilePath, char* fragmentFilePath);

#endif /* GL_SHADER_H */
