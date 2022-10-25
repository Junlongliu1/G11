LVDS_DIR := $(GET_LOCAL_DIR)

INCLUDE += -I$(LVDS_DIR)

SRC += \
	$(LVDS_DIR)/lvds_wrap.c \
	$(LVDS_DIR)/lvds_phy.c  \
	$(LVDS_DIR)/lvds_dualport_cluster.c \
	$(LVDS_DIR)/lvds_initializer.c
