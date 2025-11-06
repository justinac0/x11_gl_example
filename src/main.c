int main(void) {
        log_set_stream(stderr);

        NativeWindow window = window_create("xlib_gl_example", 1280, 720);
        GLctx        ctx    = gl_ctx_create(&window);

        // NOTE(justin):
        // https://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
        // triangle drawing code pulled from this GL tutorial. too lazy to do a
        // proper example
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
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
                glDrawArrays(GL_TRIANGLES, 0, 3);
                glDisableVertexAttribArray(0);

                gl_ctx_swapbuffers(&ctx);
                window_poll_events(&window);
        }

        gl_ctx_destroy(&ctx);
        window_destroy(&window);

        return 0;
}

