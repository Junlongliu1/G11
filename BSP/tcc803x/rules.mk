BSP_TCC803X_DIR := $(GET_LOCAL_DIR)


INCLUDE += -I$(BSP_TCC803X_DIR)

SRC += \
	$(BSP_TCC803X_DIR)/bsp.c \
	$(BSP_TCC803X_DIR)/bsp_os.c \
	$(BSP_TCC803X_DIR)/tcc_ckc.c \
	$(BSP_TCC803X_DIR)/clock.c \
	$(BSP_TCC803X_DIR)/div64.c \
	$(BSP_TCC803X_DIR)/gpio.c \
	$(BSP_TCC803X_DIR)/i2c.c \
	$(BSP_TCC803X_DIR)/uart.c \
	$(BSP_TCC803X_DIR)/udma.c \
	$(BSP_TCC803X_DIR)/debug.c \
        $(BSP_TCC803X_DIR)/fmu.c \
	$(BSP_TCC803X_DIR)/fwug.c

include $(BSP_TCC803X_DIR)/ARM/rules.mk

include $(BSP_TCC803X_DIR)/dev/adc/rules.mk
include $(BSP_TCC803X_DIR)/dev/watchdog/rules.mk
include $(BSP_TCC803X_DIR)/dev/timer/rules.mk
include $(BSP_TCC803X_DIR)/dev/ictc/rules.mk
include $(BSP_TCC803X_DIR)/dev/pwm/rules.mk
include $(BSP_TCC803X_DIR)/dev/pm/rules.mk
include $(BSP_TCC803X_DIR)/dev/ipc/rules.mk
include $(BSP_TCC803X_DIR)/dev/reboot/rules.mk
#include $(BSP_TCC803X_DIR)/dev/lvds/rules.mk
