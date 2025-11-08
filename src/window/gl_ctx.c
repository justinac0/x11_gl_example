// NOTE:
// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDebugMessageCallback.xhtml
// callback only supported on OpenGL version >= 4.3
INTERNAL void gl_debug_callback_(GLenum source, GLenum type, GLuint id,
                                 GLenum severity, GLsizei length,
                                 const GLchar* message, const void* userParam) {
        LOG_DEBUG("gl_debug_callback: %s", message);
}

// NOTE: thanks to mmozeiko for the following gist, super helpful!
// ref: https://gist.github.com/mmozeiko/911347b5e3d998621295794e0ba334c4
GLOBAL GLctx gl_ctx_create(NativeWindow* window) {
        assert(window);

        GLctx ctx = {0};

        {  // Load EGL
                int egl_version = gladLoaderLoadEGL(NULL);
                if (!egl_version) {
                        abort();
                }

                EGLint major, minor;
                major = GLAD_VERSION_MAJOR(egl_version);
                minor = GLAD_VERSION_MINOR(egl_version);

                ctx.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
                if (ctx.display == EGL_NO_DISPLAY) {
                        LOG_FATAL("failed to get EGL display");
                }

                if (!eglInitialize(ctx.display, NULL, NULL)) {
                        LOG_FATAL("failed to initialize EGL");
                }

                egl_version = gladLoaderLoadEGL(ctx.display);
                if (!egl_version) {
                        LOG_FATAL("failed to EGL with glad");
                }

                major = GLAD_VERSION_MAJOR(egl_version);
                minor = GLAD_VERSION_MINOR(egl_version);
                if (major < 1 || (major == 1 && minor < 5)) {
                        LOG_FATAL("EGL version 1.5 required, got EGL v%d.%d",
                                  major, minor);
                }
                LOG_INFO("EGL Version: %d.%d", major, minor);
        }

        EGLBoolean ok = eglBindAPI(EGL_OPENGL_API);
        assert(ok);

        {  // Surface
                EGLConfig configs[64];
                EGLint    config_count = ARRAY_LEN(configs);

                // NOTE:
                // https://registry.khronos.org/EGL/sdk/docs/man/html/eglChooseConfig.xhtml
                EGLint attr[] = {
                    EGL_SURFACE_TYPE,
                    EGL_WINDOW_BIT,
                    EGL_CONFORMANT,
                    EGL_OPENGL_BIT,
                    EGL_RENDERABLE_TYPE,
                    EGL_OPENGL_BIT,
                    EGL_COLOR_BUFFER_TYPE,
                    EGL_RGB_BUFFER,

                    EGL_RED_SIZE,
                    8,
                    EGL_GREEN_SIZE,
                    8,
                    EGL_BLUE_SIZE,
                    8,
                    EGL_ALPHA_SIZE,
                    8,
                    EGL_DEPTH_SIZE,
                    24,
                    EGL_STENCIL_SIZE,
                    8,
                    EGL_NONE,
                    /*
                    EGL_SAMPLE_BUFFERS,
                    1,
                    EGL_SAMPLES,
                    4,
                    */
                };

                if (!eglChooseConfig(ctx.display, attr, configs, config_count,
                                     &config_count) ||
                    config_count == 0) {
                        LOG_FATAL("EGL failed to choose config");
                }

                for (EGLint i = 0; i < config_count; i++) {
                        EGLAttrib attr[] = {
                            EGL_RENDER_BUFFER,
                            EGL_BACK_BUFFER,
                            EGL_NONE,
                        };

                        ctx.surface = eglCreatePlatformWindowSurface(
                            ctx.display, configs[i], &window->handle, attr);
                        if (ctx.surface != EGL_NO_SURFACE) {
                                break;
                        }
                }

                if (ctx.surface == EGL_NO_SURFACE) {
                        LOG_INFO(
                            "failed to create a platform specfic window "
                            "surface");
                }
        }

        {  // Context
                EGLint attr[] = {
                    EGL_CONTEXT_MAJOR_VERSION,
                    4,
                    EGL_CONTEXT_MINOR_VERSION,
                    5,
                    EGL_CONTEXT_OPENGL_PROFILE_MASK,
                    EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
                    EGL_CONTEXT_OPENGL_DEBUG,
                    GL_TRUE,
                    EGL_NONE,
                };

                ctx.context = eglCreateContext(ctx.display, EGL_NO_CONFIG_KHR,
                                               EGL_NO_CONTEXT, attr);
                if (ctx.context == EGL_NO_CONTEXT) {
                        LOG_INFO("failed to create EGL context");
                }
        }

        return ctx;
}

GLOBAL void gl_ctx_destroy(GLctx* ctx) {
        assert(ctx);

        gladLoaderUnloadGL();
        eglDestroyContext(ctx->display, ctx->context);
        eglDestroySurface(ctx->display, ctx->surface);
}

inline GLOBAL void gl_ctx_vsync(GLctx* ctx) {
        int        enabled = 1;
        EGLBoolean ok      = eglSwapInterval(ctx->display, enabled);
        if (ok != True) {
                LOG_FATAL("failed to enable vsync");
        }
}

inline GLOBAL void gl_ctx_make_current(GLctx* ctx) {
        assert(ctx);
        EGLBoolean ok = eglMakeCurrent(ctx->display, ctx->surface, ctx->surface,
                                       ctx->context);
        if (ok != True) {
                LOG_FATAL("failed to make egl context current");
        }

        {  // Load gl funcs and enable debugger
                int gl_version = gladLoaderLoadGL();
                UNUSED(gl_version);

                // ref:
                // https://stackoverflow.com/questions/68784829/how-print-out-the-opengl-version-of-a-gpu-and-put-it-in-a-string-in-opengl-and-c#68784846
                LOG_INFO("OpenGL Vendor: %s", glGetString(GL_VENDOR));
                LOG_INFO("OpenGL Renderer: %s", glGetString(GL_RENDERER));
                LOG_INFO("OpenGL Version: %s", glGetString(GL_VERSION));
                LOG_INFO("OpenGL Shading Language Version: %s",
                         glGetString(GL_SHADING_LANGUAGE_VERSION));

                glDebugMessageCallback(&gl_debug_callback_, NULL);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        }
}

inline GLOBAL void gl_ctx_swapbuffers(GLctx* ctx) {
        eglSwapBuffers(ctx->display, ctx->surface);
}
