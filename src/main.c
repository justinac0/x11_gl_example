// TODO(justin): move shader code somewhere else, this code was pulled from a seperate project
#include "log/log.h"
char* read_file(char* filePath) {
    FILE* fileStream = fopen(filePath, "r");
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

GLuint create_shader_program(char* vertexFilePath, char* fragmentFilePath) {
    char* vertexSource = read_file(vertexFilePath);
    char* fragmentSource = read_file(fragmentFilePath);

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

int main(void) {
        log_set_stream(stderr);

        NativeWindow window = window_create("xlib_gl_example", 1280, 720);
        GLctx        ctx    = gl_ctx_create(&window);

        // NOTE: https://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
        // triangle drawing code pulled from this GL tutorial. too lazy to do a proper example
        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);
        static const GLfloat g_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        };
        GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
                     g_vertex_buffer_data, GL_STATIC_DRAW);

        GLint program = create_shader_program("shader.vert", "shader.frag");

        while (!window.should_close) {
                if (window_read_key(KEY_CODE_ESCAPE, KEY_STATE_PRESSED)) {
                        window.should_close = true;
                }

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glClearColor(0.1, 0.2, 0.5, 1);

                glUseProgram(program);
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
                glVertexAttribPointer(
                    0,  // attribute 0. No particular reason for 0, but must
                        // match the layout in the shader.
                    3,  // size
                    GL_FLOAT,  // type
                    GL_FALSE,  // normalized?
                    0,         // stride
                    (void*) 0  // array buffer offset
                );
                // Draw the triangle !
                glDrawArrays(GL_TRIANGLES, 0,
                             3);  // Starting from vertex 0; 3 vertices total ->
                                  // 1 triangle
                glDisableVertexAttribArray(0);

                gl_ctx_swapbuffers(&ctx);
                window_poll_events(&window);
        }

        gl_ctx_destroy(&ctx);
        window_destroy(&window);

        return 0;
}
