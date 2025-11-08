# x11_gl_example
I was curious how to create custom OpenGL contexts and load
them on to X based windows. Pretty new to all of this low-level
windowing code but hey, it works on my machine!

- Works with Xlib and XCB (P.S. XCB fix your documentation).

This is definitely not production ready, just hacked this
together in my free time.

## thirdparty
- glad: https://github.com/Dav1dde/glad
    - genereated all glad code through online generator, loader option enabled
- cglm: https://cglm.readthedocs.io/en/latest/index.html

## todo
- [ ] display GPU/GL info 
- [ ] correctly set window properties WM_PROTO etc...
- [ ] cleanup xcb and xlib code
- [ ] fix memory leaks fomr 'libgallium'?

