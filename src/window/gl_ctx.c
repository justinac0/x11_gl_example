// NOTE: thanks to mmozeiko for the following gist, super helpful!
// ref: https://gist.github.com/mmozeiko/911347b5e3d998621295794e0ba334c4
GLOBAL GLctx gl_ctx_create(void) {
        GLctx ctx = {0};

        {  // load EGL
                int egl_version = gladLoaderLoadEGL(NULL);
                if (!egl_version) {
                        abort();
                }

                EGLint major, minor;
                major = GLAD_VERSION_MAJOR(egl_version);
                minor = GLAD_VERSION_MINOR(egl_version);

                printf("Loaded EGL %d.%d on first load.\n", major, minor);
                // init egl
                // window->egl_display =
                // eglGetPlatformDisplay(EGL_PLATFORM_X11_KHR, window->display,
                // NULL);
                ctx.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
                // eglGetDisplay((EGLNativeDisplayType) window->display);
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

        {  // eglSurface creation
           // eglCreateWindowSurface();
        }

        {  // eglContext
           // eglCreateContext();
        }

        return ctx;
}

inline GLOBAL void gl_ctx_swapbuffers(GLctx* ctx) {
        eglSwapBuffers(ctx->display, ctx->surface);
}

