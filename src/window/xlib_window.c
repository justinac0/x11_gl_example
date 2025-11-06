#define KEY_COUNT (256)
INTERNAL Key keys[KEY_COUNT];
KeyCode      KEY_CODE_ESCAPE = 9;
KeyCode      KEY_CODE_W      = 25;
KeyCode      KEY_CODE_A      = 38;
KeyCode      KEY_CODE_S      = 39;
KeyCode      KEY_CODE_D      = 40;
KeyCode      KEY_CODE_SPACE  = 65;

GLOBAL NativeWindow window_create(const char* title, uint16_t width,
                                  uint16_t height) {
        assert(title);

        memset(keys, 0, sizeof(Key) * KEY_COUNT);

        NativeWindow window = {0};
        window.width        = width;
        window.height       = height;

        window.display = XOpenDisplay(NULL);
        assert(window.display);

        int     screen = DefaultScreen(window.display);
        Visual* visual = DefaultVisual(window.display, screen);
        Window  root   = RootWindow(window.display, screen);

        XSetWindowAttributes attr = {0};
        attr.event_mask = EnterWindowMask | LeaveWindowMask | KeyPressMask |
                          KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
                          StructureNotifyMask;

        window.handle = XCreateWindow(window.display, root, 0, 0, width, height,
                                      0, DefaultDepth(window.display, screen),
                                      InputOutput, visual, CWEventMask, &attr);

        XMapWindow(window.display, window.handle);

        // NOTE: setup Atoms
        // TODO(justin): check BadAlloc/BadValue from XInternAtom
        window.wm_protocols =
            XInternAtom(window.display, "WM_PROTOCOLS", False);
        window.wm_delete_window =
            XInternAtom(window.display, "WM_DELETE_WINDOW", False);
        window.wm_name  = XInternAtom(window.display, "WM_NAME", False);
        window.wm_hints = XInternAtom(window.display, "WM_HINTS", False);

        XSetWMProtocols(window.display, window.handle, &window.wm_delete_window,
                        1);

        XFlush(window.display);

        return window;
}

GLOBAL void window_destroy(NativeWindow* window) {
        assert(window);

        XCloseDisplay(window->display);
}

GLOBAL void window_poll_events(NativeWindow* window) {
        assert(window);

        int pending = XPending(window->display);
        if (pending == 0)
                return;

        XEvent e;
        XNextEvent(window->display, &e);
        switch (e.type) {
                default: {
                } break;

                case ClientMessage: {
                        if ((Atom) e.xclient.data.l[0] ==
                            window->wm_delete_window) {
                                LOG_WARNING("wm_delete_window called");
                                window->should_close = true;
                        }
                } break;

                case ConfigureNotify: {
                        window->width  = e.xconfigure.width;
                        window->height = e.xconfigure.height;
                        // NOTE: assumes GL context
                        glViewport(0, 0, window->width, window->height);
                } break;

                case KeyPress: {
                        keys[e.xkey.keycode].state = KEY_STATE_PRESSED;
                } break;

                case KeyRelease: {
                        keys[e.xkey.keycode].state = KEY_STATE_RELEASED;
                } break;
        }
}

GLOBAL bool window_read_key(KeyCode key, KeyState state) {
        return keys[key].state == state;
}
