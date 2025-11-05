int main(void) {
        NativeWindow window = window_create("xlib_gl_example", 1280, 720);

        while (!window.should_close) {
                window_poll_events(&window);
                if (window_read_key(KEY_CODE_ESCAPE, KEY_STATE_PRESSED)) {
                        window.should_close = true;
                }

                window_swapbuffers(&window);
        }

        window_destroy(&window);

        return 0;
}

