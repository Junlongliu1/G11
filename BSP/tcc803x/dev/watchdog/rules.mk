DEV_WDT_LOCAL_DIR := $(GET_LOCAL_DIR)

INCLUDE += -I$(DEV_WDT_LOCAL_DIR)/include

SRC += \
	$(DEV_WDT_LOCAL_DIR)/tcc_wdt.c

