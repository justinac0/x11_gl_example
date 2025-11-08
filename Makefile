# TODO:
# helper commands for using meson
# [ ] Platform specific building

BUILD_DIR ?= build
EXECUTABLE ?= app

OPTIONS ?= xcb


export CC = clang
# export CXX ?= clang++

.PHONY: all
all: build $(OPTIONS) compile

.PHONY: compile
compile: | $(BUILD_DIR)
	meson compile -C $(BUILD_DIR)

.PHONY: run
run: $(BUILD_DIR)/$(EXECUTABLE)
	./$(BUILD_DIR)/$(EXECUTABLE)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: xlib
xlib: | $(BUILD_DIR)
	meson configure -Dwindow_system='xlib' $(BUILD_DIR)

.PHONY: xcb
xcb: | $(BUILD_DIR)
	meson configure -Dwindow_system='xcb' $(BUILD_DIR)

.PHONY: win32
win32: | $(BUILD_DIR)
	meson configure -Dwindow_system='win32' $(BUILD_DIR)

$(BUILD_DIR):
	meson setup $(BUILD_DIR) --unity=on
