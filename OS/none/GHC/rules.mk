BSP_UCOS_DIR := $(GET_LOCAL_DIR)

INCLUDE += -I$(BSP_UCOS_DIR)

#ASMSRC += 

SRC += \
    $(BSP_UCOS_DIR)/cpu_bsp.c \
    $(BSP_UCOS_DIR)/os_app_hooks.c

