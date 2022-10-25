INCLUDE += -I$(TOPDIR)/HAL

SRC 	+= \
	$(TOPDIR)/HAL/HAL_API.c \
	$(TOPDIR)/HAL/HAL_devices.c

include $(TOPDIR)/HAL/HAL_TIMER/rules.mk
include $(TOPDIR)/HAL/HAL_CONSOLE/rules.mk
include $(TOPDIR)/HAL/HAL_POWER_MANAGER/rules.mk
include $(TOPDIR)/HAL/HAL_IPC/rules.mk
include $(TOPDIR)/HAL/HAL_ADC_KEY/rules.mk
include $(TOPDIR)/HAL/HAL_I2C_KEY/rules.mk
include $(TOPDIR)/HAL/HAL_GPIO_KEY/rules.mk
include $(TOPDIR)/HAL/HAL_LED/rules.mk
include $(TOPDIR)/HAL/HAL_SNR/rules.mk
include $(TOPDIR)/HAL/HAL_EEP/rules.mk
include $(TOPDIR)/HAL/HAL_LINETOOL/rules.mk
include $(TOPDIR)/HAL/HAL_KLINEDRV/rules.mk
