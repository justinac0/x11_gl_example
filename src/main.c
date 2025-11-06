int main(void) {
        NativeWindow window = window_create("xlib_gl_example", 1280, 720);
        GLctx ctx = gl_ctx_create(&window.handle);

        while (!window.should_close) {
                window_poll_events(&window);
                if (window_read_key(KEY_CODE_ESCAPE, KEY_STATE_PRESSED)) {
                        window.should_close = true;
                }

                glClear(GL_COLOR_BUFFER_BIT);
                glClearColor(0.1, 0.2, 0.5, 1);

                gl_ctx_swapbuffers(&ctx);
        }

        gl_ctx_destroy(&ctx);
        window_destroy(&window);

        return 0;
}
