# TODO:
#
# helper commands for using meson
# [x] setup builddir
# 2. config builddir (window_system option etc...)
# 3. compile targets (platform?)
# 4. run executable
# 5. cleanup builddir

# NOTE:
#
# unity build seems to break how linting works?
# TODO: fix this ^
# TODO: migrate code to doris


BUILD_DIR ?= build
EXECUTABLE ?= app

.PHONY: all
all: $(BUILD_DIR)/$(EXECUTABLE)

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
	meson configure -Dwindow_system='win32' $(BUILD_DIR)

$(BUILD_DIR)/$(EXECUTABLE): | $(BUILD_DIR)
	meson compile -C $(BUILD_DIR) # $(EXECUTABLE)

$(BUILD_DIR):
	meson setup $(BUILD_DIR)
