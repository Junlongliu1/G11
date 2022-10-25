
# add rules file

# APP/HAL
include $(TOPDIR)/APP/rules.mk
include $(TOPDIR)/SERVICE/rules.mk
include $(TOPDIR)/HAL/rules.mk

# BSP
include $(TOPDIR)/BSP/rules.mk

#OS
include $(TOPDIR)/OS/rules.mk

#OSAL
include $(TOPDIR)/OSAL/rules.mk

#Libs
include $(TOPDIR)/Libs/rules.mk

#OS_fw
include $(TOPDIR)/OS_fw/rules.mk