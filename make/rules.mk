#Build rules for Nucleo projects. See <board_name>_app.mk for board-specific stuff


# Add include paths, defines to cflags
CFLAGS += $(addprefix -I,$(INCLUDE))
CFLAGS += $(addprefix -D,$(DEFINES))

# Build list of .o files we need
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
ALL_OBJS := $(OBJS)
DEPS := $(OBJS:.o=.d)

DRIVER_OBJS := $(DRIVER_SRCS:%=$(BUILD_DIR)/drivers/%.o)
ALL_OBJS += $(DRIVER_OBJS)
DEPS += $(DRIVER_OBJS:.o=.d)

PLATFORM_OBJS := $(PLATFORM_SRCS:%=$(BUILD_DIR)/platform/%.o)
ALL_OBJS += $(PLATFORM_OBJS)
DEPS += $(PLATFORM_OBJS:.o=.d)

# Build rules
all: $(BUILD_DIR)/$(APP_NAME).hex
.PHONY: all


%.hex: %.elf
	$(OBJCOPY) -Oihex $< $@

.PHONY: copy

copy: $(BUILD_DIR)/$(APP_NAME).hex
	openocd -f board/st_nucleo_f0.cfg -c "program $< verify reset exit"


$(BUILD_DIR)/$(APP_NAME).elf: $(ALL_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(ALL_OBJS) -o $@ -Xlinker -Map=$@.map

$(BUILD_DIR)/drivers/%.o: $(DRIVER_PATH)/%
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/platform/%.o: $(PLATFORM_PATH)/%
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -rf $(BUILD_DIR)

-include $(DEPS)
