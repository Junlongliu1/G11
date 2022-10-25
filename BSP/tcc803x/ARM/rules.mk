BSP_ARM_DIR := $(GET_LOCAL_DIR)

INCLUDE += -I$(BSP_ARM_DIR)

ASMSRC += \
	$(BSP_ARM_DIR)/GCC/startup.S \
	$(BSP_ARM_DIR)/GCC/vector.S \
	$(BSP_ARM_DIR)/GCC/vfp_init.S \
	$(BSP_ARM_DIR)/GCC/arm_a.S \
	$(BSP_ARM_DIR)/GCC/cache.S

SRC += \
	$(BSP_ARM_DIR)/bsp_int.c \
	$(BSP_ARM_DIR)/bsp_os_timer.c \
	$(BSP_ARM_DIR)/mpu.c

