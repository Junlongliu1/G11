SYS_PM_PATH := $(TOPDIR)/APP/PM

INCLUDE += -I$(SYS_PM_PATH)

include $(SYS_PM_PATH)/pmio/rules.mk
include $(SYS_PM_PATH)/power/rules.mk
include $(SYS_PM_PATH)/powerdrv/rules.mk
include $(SYS_PM_PATH)/powerwdt/rules.mk