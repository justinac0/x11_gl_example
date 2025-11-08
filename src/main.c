#include "log/log.h"
#include "window/window.h"
#include "window/gl_ctx.h"
#include "render/gl/shader.h"
#include <glad/gl.h>
#include <cglm/cglm.h>

int main(void) {
        log_set_stream(stderr);

        NativeWindow window = window_create("xlib_gl_example", 640, 480);
        GLctx        ctx    = gl_ctx_create(&window);
        gl_ctx_make_current(&ctx);
        gl_ctx_vsync(&ctx);

        // NOTE(justin):
        // https://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
        // triangle drawing code pulled from this GL tutorial. too lazy to do a
        // proper example
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        static const GLfloat verts[] = {
            -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        };

        static const GLfloat colors[] = {
            1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
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

        mat4 model, perspective, view;
        glm_mat4_identity(model);
        glm_mat4_identity(perspective);
        glm_mat4_identity(view);

        glm_lookat((vec3){0, 0, 4}, (vec4){0, 0, 0}, (vec3){0, 1, 0}, view);

        glm_perspective_default((float) window.width / (float) window.height,
                                perspective);

        double i = 0;
        while (!window.should_close) {
                if (window_read_key(KEY_CODE_ESCAPE, KEY_STATE_PRESSED)) {
                        window.should_close = true;
                }

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glClearColor(0, 0.15, 0.4, 1);

                glm_spin(model, 0.01f, (vec3){0, 1, 0});

                glUseProgram(program);

                glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1,
                                   GL_FALSE, (float*) model);
                glUniformMatrix4fv(glGetUniformLocation(program, "perspective"),
                                   1, GL_FALSE, (float*) perspective);
                glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1,
                                   GL_FALSE, (float*) view);

                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glDrawArrays(GL_TRIANGLES, 0, 3);
                glDisableVertexAttribArray(0);

                perspective[3][0] = sinf(i);
                perspective[3][1] = cosf(i);

                i += 0.01f;

                gl_ctx_swapbuffers(&ctx);
                window_poll_events(&window);
        }

        gl_ctx_destroy(&ctx);
        window_destroy(&window);

        return 0;
}
