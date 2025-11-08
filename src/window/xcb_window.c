#define KEY_COUNT (256)
INTERNAL Key keys[KEY_COUNT];
KeyCode      KEY_CODE_ESCAPE = 9;
KeyCode      KEY_CODE_W      = 25;
KeyCode      KEY_CODE_A      = 38;
KeyCode      KEY_CODE_S      = 39;
KeyCode      KEY_CODE_D      = 40;
KeyCode      KEY_CODE_SPACE  = 65;

INTERNAL xcb_intern_atom_cookie_t intern_cookie(xcb_connection_t* connection,
                                                uint8_t create_if_exists,
                                                const char* const name) {
        return xcb_intern_atom(connection, create_if_exists,
                               (uint16_t) strlen(name), name);
}

GLOBAL NativeWindow window_create(const char* title, uint16_t width,
                                  uint16_t height) {
        assert(title);

        LOG_INFO("creating xcb window");

        memset(keys, 0, sizeof(Key) * KEY_COUNT);

        NativeWindow window = {0};
        window.width        = width;
        window.height       = height;

        window.connection = xcb_connect(NULL, NULL);
        assert(window.connection);

        uint32_t attr =
            XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
            XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
            XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW |
            XCB_EVENT_MASK_STRUCTURE_NOTIFY;

        xcb_screen_t* screen =
            xcb_setup_roots_iterator(xcb_get_setup(window.connection)).data;

        window.handle = xcb_generate_id(window.connection);
        xcb_create_window(window.connection, XCB_COPY_FROM_PARENT,
                          window.handle, screen->root, 0, 0, window.width,
                          window.height, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                          screen->root_visual, XCB_CW_EVENT_MASK, &attr);
        xcb_map_window(window.connection, window.handle);

        xcb_intern_atom_cookie_t protocols_cookie =
            intern_cookie(window.connection, true, "WM_PROTOCOLS");
        xcb_intern_atom_cookie_t delete_window_cookie =
            intern_cookie(window.connection, 0, "WM_DELETE_WINDOW");
        xcb_intern_atom_cookie_t name_cookie =
            intern_cookie(window.connection, 0, "WM_NAME");

        window.wm_protocols =
            xcb_intern_atom_reply(window.connection, protocols_cookie, 0);
        window.wm_delete_window =
            xcb_intern_atom_reply(window.connection, delete_window_cookie, 0);
        window.wm_name =
            xcb_intern_atom_reply(window.connection, name_cookie, 0);

        xcb_change_property(window.connection, XCB_PROP_MODE_REPLACE,
                            window.handle, (*window.wm_protocols).atom, 4, 32,
                            1, &(*window.wm_delete_window).atom);

        xcb_change_property(window.connection, XCB_PROP_MODE_REPLACE,
                            window.handle, XCB_ATOM_WM_CLASS, XCB_ATOM_STRING,
                            8, (uint32_t) strlen(title), title);

        xcb_flush(window.connection);

        return window;
}

GLOBAL void window_destroy(NativeWindow* window) {
        assert(window);
        free(window->wm_protocols);
        free(window->wm_delete_window);
        free(window->wm_name);

        xcb_disconnect(window->connection);
}

GLOBAL void window_poll_events(NativeWindow* window) {
        xcb_generic_event_t* event = NULL;
        while ((event = xcb_poll_for_event(window->connection))) {
                switch (XCB_EVENT_RESPONSE_TYPE(event)) {
                        default: {
                        } break;

                        case XCB_CONFIGURE_NOTIFY: {
                                xcb_configure_notify_event_t* ce =
                                    (xcb_configure_notify_event_t*) event;
                                window->width  = ce->width;
                                window->height = ce->height;
                                glViewport(0, 0, window->width, window->height);
                        } break;

                        case XCB_CLIENT_MESSAGE: {
                                xcb_client_message_event_t* msg =
                                    (xcb_client_message_event_t*) event;
                                if (msg->data.data32[0] ==
                                    (*window->wm_delete_window).atom) {
                                        LOG_WARNING("wm_delete_window called");
                                        window->should_close = true;
                                }
                        } break;

                        case XCB_KEY_PRESS: {
                                xcb_key_press_event_t* kb =
                                    (xcb_key_press_event_t*) event;
                                keys[kb->detail].state = KEY_STATE_PRESSED;
                                keys[kb->detail].code  = kb->detail;
                        } break;

                        case XCB_KEY_RELEASE: {
                                xcb_key_release_event_t* kb =
                                    (xcb_key_release_event_t*) event;
                                keys[kb->detail].state = KEY_STATE_RELEASED;
                                keys[kb->detail].code  = kb->detail;
                        } break;
                }

                free(event);
        }
}

GLOBAL bool window_read_key(KeyCode key, KeyState state) {
        return keys[key].state == state;
}
