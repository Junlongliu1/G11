INCLUDE += -I$(TOPDIR)/SERVICE/IPC
INCLUDE += -I$(TOPDIR)/APP/rpc
INCLUDE += -I$(TOPDIR)/HAL/HAL_IPC
SRC += \
	$(TOPDIR)/SERVICE/IPC/tcc_ipc_protocol.c \
	$(TOPDIR)/SERVICE/IPC/IPC_Parser.c