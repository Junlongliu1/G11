
# add rules file
#none
ifeq ($(TARGET_RTOS), none)
include $(TOPDIR)/OS/none/rules.mk
endif
#uCOS
ifeq ($(TARGET_RTOS), ucos)
include $(TOPDIR)/OS/ucos/rules.mk
endif
#free RTOS
ifeq ($(TARGET_RTOS), freertos)
include $(TOPDIR)/OS/freertos/rules.mk
endif