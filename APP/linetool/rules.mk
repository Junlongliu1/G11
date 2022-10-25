# \file           mk
# \date           2021/10/22
# \author         AOCHR Liu jie
# \model          EDA
# \description    linetool makefile

APP_LINETOOL_PATH := $(MCU_BSP_BUILD_CURDIR)

# Paths
VPATH += $(APP_LINETOOL_PATH)

# Includes
INCLUDES += -I$(APP_LINETOOL_PATH)
# Sources
SRC += $(wildcard $(TOPDIR)/APP/linetool/*.c)