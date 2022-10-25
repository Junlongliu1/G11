OS_FREERTOS_DIR := $(GET_LOCAL_DIR)

INCLUDE += -I$(OS_FREERTOS_DIR)
INCLUDE += -I$(OS_FREERTOS_DIR)/include
INCLUDE += -I$(OS_FREERTOS_DIR)/portable/GCC/ARM_CA7
INCLUDE += -I$(OS_FREERTOS_DIR)/portable/MemMang

SRC += \
	$(OS_FREERTOS_DIR)/tasks.c \
	$(OS_FREERTOS_DIR)/list.c \
	$(OS_FREERTOS_DIR)/queue.c \
	$(OS_FREERTOS_DIR)/event_groups.c \
	$(OS_FREERTOS_DIR)/timers.c

include $(OS_FREERTOS_DIR)/portable/rules.mk