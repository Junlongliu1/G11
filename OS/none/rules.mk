OS_NONE_DIR := $(GET_LOCAL_DIR)

INCLUDE += -I$(OS_NONE_DIR)

ASMSRC += \
	$(OS_NONE_DIR)/GCC/none_os.S
