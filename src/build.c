////////////////////////////////////
/// STD
#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

////////////////////////////////////
/// GLAD specific
#include "thirdparty/glad/include/glad/egl.h"
#include "thirdparty/glad/src/egl.c"
#include "thirdparty/glad/include/glad/gl.h"
#include "thirdparty/glad/src/gl.c"

////////////////////////////////////
/// PLATFORM
#if defined(WINDOW_XLIB)
#include <X11/Xlib.h>
#elif defined(WINDOW_XCB)
#include <xcb/xcb.h>
#include <xcb/xcb_event.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xproto.h>
#else
// TODO(justin): support for xcb, wayland, winapi, ...
#error "platform not supported"
#endif

////////////////////////////////////
/// X11_gl_example specific
#include "src/base.h"
#include "src/log/log.h"
#include "src/log/log.c"

#include "src/window/window.h"

#if defined(WINDOW_XLIB)
#include "src/window/xlib_window.c"
#elif defined(WINDOW_XCB)
#include "src/window/xcb_window.c"
#endif

#include "src/window/gl_ctx.h"
#include "src/window/gl_ctx.c"
#include "src/render/gl/shader.h"
#include "src/render/gl/shader.c"

#include <cglm/cglm.h>
#include "main.c"
