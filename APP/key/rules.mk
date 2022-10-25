# \file           mk
# \date           2021/9/16
# \author         AOCHR Li Yongnan
# \model          EDA
# \description    key makefile

INCLUDE += -I$(TOPDIR)/APP/key

SRC += $(wildcard $(TOPDIR)/APP/key/*.c)