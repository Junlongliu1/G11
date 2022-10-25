BSP_DIR := $(GET_LOCAL_DIR)

INCLUDE += -I$(BSP_DIR)

# BSP_NAME must be define in the config.mk files

include $(BSP_DIR)/$(BSP_NAME)/rules.mk