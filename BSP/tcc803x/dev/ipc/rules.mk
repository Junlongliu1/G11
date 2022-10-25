IPC_DIR := $(GET_LOCAL_DIR)

INCLUDE += -I$(IPC_DIR)
INCLUDE += -I$(IPC_DIR)/include

SRC += \
	$(IPC_DIR)/tcc_ipc_os.c \
	$(IPC_DIR)/tcc_ipc_mbox.c \
	$(IPC_DIR)/tcc_ipc_cmd.c \
	$(IPC_DIR)/tcc_ipc_ctl.c \
	$(IPC_DIR)/tcc_ipc_buffer.c
