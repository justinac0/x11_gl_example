#include <X11/X.h>
#include <X11/Xlib.h>

#include "glad/egl.h"
#include "glad/gl.h"
#define KEY_COUNT (256)
static Key keys[KEY_COUNT];
#undef KEY_COUNT

KeyCode KEY_CODE_ESCAPE = 9;
KeyCode KEY_CODE_W      = 25;
KeyCode KEY_CODE_A      = 38;
KeyCode KEY_CODE_S      = 39;
KeyCode KEY_CODE_D      = 40;
KeyCode KEY_CODE_SPACE  = 65;

GLOBAL NativeWindow window_create(const char* title, uint16_t width,
                                  uint16_t height) {
        assert(title);

        NativeWindow window = {0};
        window.width        = width;
        window.height       = height;

        window.display = XOpenDisplay(NULL);
        assert(window.display);

        int     screen = DefaultScreen(window.display);
        Visual* visual = DefaultVisual(window.display, screen);
        Window  root   = RootWindow(window.display, screen);

        Colormap colormap =
            XCreateColormap(window.display, root, visual, AllocNone);

        XSetWindowAttributes attr = {0};
        attr.event_mask = EnterWindowMask | LeaveWindowMask | KeyPressMask |
                          KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
                          StructureNotifyMask;
        attr.colormap = colormap;

        window.handle =
            XCreateWindow(window.display, root, 0, 0, width, height, 0,
                          DefaultDepth(window.display, screen), InputOutput,
                          visual, CWColormap | CWEventMask, &attr);

        XFreeColormap(window.display, colormap);

        XMapWindow(window.display, window.handle);

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
                default:
                        break;
                case ConfigureNotify:
                        window->width  = e.xconfigure.width;
                        window->height = e.xconfigure.height;
                        // NOTE: assumes GL context
                        glViewport(0, 0, window->width, window->height);
                        break;
                case KeyPress:
                        keys[e.xkey.keycode].state = KEY_STATE_PRESSED;
                        break;
                case KeyRelease:
                        keys[e.xkey.keycode].state = KEY_STATE_RELEASED;
                        break;
        }
}

GLOBAL bool window_read_key(KeyCode key, KeyState state) {
        return keys[key].state == state;
}

