# \file           mk
# \date           2021/4/12
# \author         AOCHR Li Xiaoqiang
# \model          EDA
# \description    kline makefile

INCLUDE += -I$(TOPDIR)/APP/kline

SRC += $(wildcard $(TOPDIR)/APP/kline/*.c)