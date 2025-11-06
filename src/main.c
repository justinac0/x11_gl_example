int main(void) {
        log_set_stream(stderr);

        NativeWindow window = window_create("xlib_gl_example", 1280, 720);
        GLctx        ctx    = gl_ctx_create(&window);

        // NOTE(justin):
        // https://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
        // triangle drawing code pulled from this GL tutorial. too lazy to do a
        // proper example
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        static const GLfloat verts[] = {
            -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        };

        static const GLfloat colors[] = {
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f,
        };

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        GLuint cbo;
        glGenBuffers(1, &cbo);
        glBindBuffer(GL_ARRAY_BUFFER, cbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);

        GLint program = create_shader_program("shader.vert", "shader.frag");

        while (!window.should_close) {
                if (window_read_key(KEY_CODE_ESCAPE, KEY_STATE_PRESSED)) {
                        window.should_close = true;
                }

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glClearColor(0, 0, 0, 1);

                glUseProgram(program);
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glDrawArrays(GL_TRIANGLES, 0, 3);
                glDisableVertexAttribArray(0);

                gl_ctx_swapbuffers(&ctx);
                window_poll_events(&window);
        }

        gl_ctx_destroy(&ctx);
        window_destroy(&window);

        return 0;
}
