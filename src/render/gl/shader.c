#include "shader.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

GLOBAL char* read_file(char* filePath, const char* mode) {
    FILE* fileStream = fopen(filePath, mode);
    if (!fileStream) {
        return NULL;
    }

    fseek(fileStream, SEEK_SET, SEEK_END);
    uint32_t length = ftell(fileStream);

    rewind(fileStream);

    char* buffer = (char*) calloc(length + 1, sizeof(char));
    assert(buffer);

    if (!buffer) {
        return NULL;
    }

    fread(buffer, sizeof(char), length, fileStream);
    fclose(fileStream);

    return buffer;
}

GLOBAL GLuint create_shader_program(char* vertexFilePath, char* fragmentFilePath) {
    char* vertexSource = read_file(vertexFilePath, "r");
    char* fragmentSource = read_file(fragmentFilePath, "r");

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    const GLchar *source = (const GLchar *)vertexSource;
    glShaderSource(vertexShader, 1, &source, 0);

    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        GLchar* infoLog = calloc(maxLength, sizeof(GLchar));
        assert(infoLog);

        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
        printf("[SHADER]: %s\n", infoLog);
        free(infoLog);

        glDeleteShader(vertexShader);

        return 0;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    source = (const GLchar *)fragmentSource;
    glShaderSource(fragmentShader, 1, &source, 0);

    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        GLchar* infoLog = calloc(maxLength, sizeof(GLchar));
        assert(infoLog);

        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
        printf("[SHADER]: %s\n", infoLog);
        free(infoLog);

        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);

        return 0;
    }

    GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        GLchar* infoLog = calloc(maxLength, sizeof(GLchar));
        assert(infoLog);

        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
        printf("[SHADER]: %s\n", infoLog);
        free(infoLog);

        glDeleteProgram(program);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return 0;
    }

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    free(vertexSource);
    free(fragmentSource);

    return program;
}
