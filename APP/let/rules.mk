# \file           mk
# \date           2021/4/12
# \author         AOCHR Li Xiaoqiang
# \model          EDA
# \description    let makefile

INCLUDE += -I$(TOPDIR)/APP/let

SRC += $(wildcard $(TOPDIR)/APP/let/*.c)