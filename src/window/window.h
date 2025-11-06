#ifndef WINDOW_H
#define WINDOW_H

#if defined(WINDOW_XLIB)
typedef Window NativeWindowHandle;
#else
#error "can't create window, platform not supported"
#endif

typedef struct NativeWindow NativeWindow;
struct NativeWindow {
#if defined(WINDOW_XLIB)
        Display* display;
        Atom     wm_protocols;
        Atom     wm_delete_window;
        Atom     wm_name;
        Atom     wm_hints;
#endif
        const char*        title;
        uint16_t           width;
        uint16_t           height;
        bool               should_close;
        NativeWindowHandle handle;
};

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

GLOBAL NativeWindow window_create(const char* title, uint16_t width,
                                  uint16_t height);
GLOBAL void         window_destroy(NativeWindow* window);
GLOBAL void         window_poll_events(NativeWindow* window);
GLOBAL bool         window_read_key(KeyCode key, KeyState state);

#endif  // WINDOW_H
