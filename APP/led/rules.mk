# \file           mk
# \date           2021/9/16
# \author         AOCHR Li Yongnan
# \model          EDA
# \description    led makefile

INCLUDE += -I$(TOPDIR)/APP/led

SRC += $(wildcard $(TOPDIR)/APP/led/*.c)