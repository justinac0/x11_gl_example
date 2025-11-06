// NOTE:
// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDebugMessageCallback.xhtml
// callback only supported on OpenGL version >= 4.3
INTERNAL void gl_debug_callback_(GLenum source, GLenum type, GLuint id,
                                 GLenum severity, GLsizei length,
                                 const GLchar* message, const void* userParam) {
}

// NOTE: thanks to mmozeiko for the following gist, super helpful!
// ref: https://gist.github.com/mmozeiko/911347b5e3d998621295794e0ba334c4
GLOBAL GLctx gl_ctx_create(NativeWindowHandle* window_handle) {
        assert(window_handle);

        GLctx ctx = {0};

        {  // Load EGL
                int egl_version = gladLoaderLoadEGL(NULL);
                if (!egl_version) {
                        abort();
                }

                EGLint major, minor;
                major = GLAD_VERSION_MAJOR(egl_version);
                minor = GLAD_VERSION_MINOR(egl_version);

                printf("Loaded EGL %d.%d on first load.\n", major, minor);
                ctx.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
                if (ctx.display == EGL_NO_DISPLAY) {
                        abort();
                }

                if (!eglInitialize(ctx.display, NULL, NULL)) {
                        abort();
                }

                egl_version = gladLoaderLoadEGL(ctx.display);
                if (!egl_version) {
                        abort();
                }

                major = GLAD_VERSION_MAJOR(egl_version);
                minor = GLAD_VERSION_MINOR(egl_version);
                if (major < 1 || (major == 1 && minor < 5)) {
                        abort();
                }
                printf("Loaded EGL %d.%d on second load.\n", major, minor);
        }

        EGLBoolean ok = eglBindAPI(EGL_OPENGL_API);
        assert(ok);

        {  // Surface
           EGLConfig configs[64];
           EGLint config_count = ARRAY_LEN(configs);

           EGLint attr[] = {
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_CONFORMANT, EGL_OPENGL_BIT,
                EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
                EGL_COLOR_BUFFER_TYPE, EGL_RGB_BUFFER,

                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_DEPTH_SIZE, 24,
                EGL_STENCIL_SIZE, 8,

                EGL_SAMPLE_BUFFERS, 1,
                EGL_SAMPLES, 4,

                EGL_NONE,
           };

           if (!eglChooseConfig(ctx.display, attr, configs, config_count, &config_count) || config_count == 0) {
                abort();
           }

           for (EGLint i = 0; i < config_count; i++) {
                EGLAttrib attr[] = {
                        EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
                        // can have different frame buffer color formats
                        EGL_NONE,
                };

                ctx.surface = eglCreatePlatformWindowSurface(ctx.display, configs[i], window_handle, attr);
                if (ctx.surface != EGL_NO_SURFACE) {
                        break;
                }
           }

           if (ctx.surface == EGL_NO_SURFACE) {
                abort();
           }
        }

        {  // Context 
                EGLint attr[] = {
                        EGL_CONTEXT_MAJOR_VERSION, 4,
                        EGL_CONTEXT_MINOR_VERSION, 5,
                        EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
                        EGL_CONTEXT_OPENGL_DEBUG, GL_TRUE,
                        EGL_NONE,
                };

                ctx.context = eglCreateContext(ctx.display, EGL_NO_CONFIG_KHR, EGL_NO_CONTEXT, attr);
                if (ctx.context == EGL_NO_CONTEXT) {
                        abort();
                }
        }


        gl_ctx_make_current(&ctx);
        gladLoaderLoadGL();

        glDebugMessageCallback(&gl_debug_callback_, NULL);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        return ctx;
}

GLOBAL void gl_ctx_destroy(GLctx* ctx) {
        printf("cleanup");
        gladLoaderUnloadGL();
}

inline GLOBAL void gl_ctx_swapbuffers(GLctx* ctx) {
        eglSwapBuffers(ctx->display, ctx->surface);
}

inline GLOBAL void gl_ctx_make_current(GLctx* ctx) {
        EGLBoolean ok = eglMakeCurrent(ctx->display, ctx->surface, ctx->surface, ctx->context);
        assert(ok);
}
