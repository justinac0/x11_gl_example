#ifndef WINDOW_H
#define WINDOW_H

#include "base.h"

#include <stdbool.h>
#include <stdint.h>

#if defined(WINDOW_XLIB)
#include <X11/X.h>
#include <X11/Xlib.h>
typedef Window NativeWindowHandle;
#elif defined(WINDOW_XCB)
#include <xcb/xcb.h>
#include <xcb/xcb_event.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>
typedef xcb_window_t NativeWindowHandle;
#else
// TODO(justin): support for xcb, wayland, winapi, ...
#error "platform not supported"
#endif

typedef enum {
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_COUNT,
} MouseButton;

typedef struct {
    uint16_t x, y;
    // TODO(mitch): macro for bitmap
    uint8_t buttons[(MOUSE_BUTTON_COUNT / 8) + (MOUSE_BUTTON_COUNT % 8 ? 1 : 0)];
} NativeMouse;

GLOBAL bool window_read_mouse_button(const NativeMouse* mouse, MouseButton button) {
    if (button >= MOUSE_BUTTON_COUNT) {
        return false;
    }

    return BITFIELD_CHECK(mouse->buttons, button) != 0;
}

typedef struct NativeWindow NativeWindow;
struct NativeWindow {
// TODO(justin): https://specifications.freedesktop.org/wm/1.5/index.html hints
// for WM to implement/lookat
#if defined(WINDOW_XLIB)
    Display* display;
    Atom     wm_protocols;
    Atom     wm_delete_window;
    Atom     wm_name;
    Atom     wm_hints;
#elif defined(WINDOW_XCB)
    xcb_connection_t*        display;
    xcb_intern_atom_reply_t* wm_protocols;
    xcb_intern_atom_reply_t* wm_delete_window;
    xcb_intern_atom_reply_t* wm_name;
    xcb_intern_atom_reply_t* wm_hints;
    const xcb_setup_t*       setup;
#endif
    const char*        title;
    uint16_t           width;
    uint16_t           height;
    NativeMouse        mouse;
    bool               should_close;
    NativeWindowHandle handle;
};

// XCB_EVENT_MASK_BUTTON_PRESS
// XCB_EVENT_MASK_BUTTON_RELEASE
// xcb_button_press_event_t https://www.x.org/archive/X11R7.5/doc/libxcb/tutorial/index.html

typedef enum {
    KEY_MOD_NONE,
    KEY_MOD_LSHIFT,
} KeyMod;

typedef enum {
    KEY_STATE_NONE,
    KEY_STATE_PRESSED,
    KEY_STATE_RELEASED,
} KeyState;

typedef uint8_t KeyCode;
extern KeyCode  KEY_CODE_ESCAPE;
extern KeyCode  KEY_CODE_W;
extern KeyCode  KEY_CODE_A;
extern KeyCode  KEY_CODE_S;
extern KeyCode  KEY_CODE_D;
extern KeyCode  KEY_CODE_SPACE;

typedef struct {
    KeyCode  code;
    KeyState state;
} Key;

GLOBAL NativeWindow window_create(const char* title, uint16_t width, uint16_t height);
GLOBAL void         window_destroy(NativeWindow* window);
GLOBAL void         window_poll_events(NativeWindow* window);
GLOBAL bool         window_read_key(const NativeWindow* window, KeyCode key, KeyState state);

#endif  // WINDOW_H
