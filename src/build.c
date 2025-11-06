////////////////////////////////////
/// STD

#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

////////////////////////////////////
/// GLAD specific
#include "glad/egl.h"
#include "glad/gl.h"
#include "thirdparty/glad/src/egl.c"
#include "thirdparty/glad/src/gl.c"

////////////////////////////////////
/// PLATFORM
#if defined(WINDOW_XLIB)
#include <X11/Xlib.h>
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
#include "src/window/gl_ctx.h"
#include "src/window/gl_ctx.c"
#include "src/render/gl/shader.h"
#include "src/render/gl/shader.c"

#if defined(WINDOW_XLIB)
#include "src/window/xlib_window.c"
#endif

#include "main.c"
