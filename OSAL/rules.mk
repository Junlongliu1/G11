OSAL_DIR := $(GET_LOCAL_DIR)

INCLUDE += -I$(TOPDIR)/OSAL

ASMSRC += \
	$(OSAL_DIR)/osal.S


ifeq ($(TARGET_RTOS), freertos)
include $(OSAL_DIR)/osal_freertos/rules.mk
endif
