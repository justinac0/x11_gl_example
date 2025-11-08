#ifndef GL_CTX_H
#define GL_CTX_H

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
