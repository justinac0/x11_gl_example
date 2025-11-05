#ifndef GL_CTX_H
#define GL_CTX_H

typedef struct {
        EGLContext context;
        EGLDisplay display;
        EGLSurface surface;
} GLctx;

GLOBAL GLctx gl_ctx_create(void);
GLOBAL void gl_ctx_swapbuffers(GLctx* ctx);

#endif /* GL_CTX_H */
