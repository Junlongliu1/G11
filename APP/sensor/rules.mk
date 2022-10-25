# \file           mk
# \date           2021/9/16
# \author         AOCHR Li Yongnan
# \model          EDA
# \description    sensor makefile

INCLUDE += -I$(TOPDIR)/APP/sensor

SRC += $(wildcard $(TOPDIR)/APP/sensor/*.c)
