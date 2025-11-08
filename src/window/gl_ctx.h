#ifndef GL_CTX_H
#define GL_CTX_H

#include <glad/egl.h>
#include <glad/gl.h>
#include "window/window.h"
#include "base.h"

typedef struct {
        EGLContext context;
        EGLDisplay display;
        EGLSurface surface;
} GLctx;

GLOBAL GLctx gl_ctx_create(NativeWindow* window);
GLOBAL void  gl_ctx_make_current(GLctx* ctx);
GLOBAL void  gl_ctx_vsync(GLctx* ctx);
GLOBAL void  gl_ctx_destroy(GLctx* ctx);
GLOBAL void  gl_ctx_swapbuffers(GLctx* ctx);
GLOBAL void  gl_ctx_make_current(GLctx* ctx);

#endif /* GL_CTX_H */
