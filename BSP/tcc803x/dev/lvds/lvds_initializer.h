/****************************************************************************************
 *   FileName    : lvds_initializer.h
 *   Description :
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

 This source code contains confidential information of Telechips.
 Any unauthorized use without a written permission of Telechips including not limited
 to re-distribution in source or binary form is strictly prohibited.
 This source code is provided AS IS and nothing contained in this source code
 shall constitute any express or implied warranty of any kind, including without limitation,
 any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent,
 copyright or other third party intellectual property right.
 No warranty is made, express or implied, regarding the information accuracy,
 completeness, or performance.
 In no event shall Telechips be liable for any claim, damages or other liability arising from,
 out of or in connection with this source code or the use in the source code.
 This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
 between Telechips and Company.
 *
 ****************************************************************************************/

#ifndef __LVDS_INITIALIZER_H__
#define __LVDS_INITIALIZER_H__

#include <types.h>

//#define LVDS_DEBUG
#ifdef LVDS_DEBUG
#define LVDS_DEBUG(fmt, args...)   printf(fmt, ## args)
#else
#define LVDS_DEBUG(fmt, args...)   do {} while(0)
#endif

#define LVDS_OK 0
#define LVDS_ERR 1

typedef enum {
	LVDS_WRAP_CMD_SET = 0,
	LVDS_WRAP_CMD_RESET_PHY,
	LVDS_WRAP_CMD_MAX,
} LVDS_WRAP_CMD;

typedef enum {
	LVDS_PHY_CMD_INIT = 0,
	LVDS_PHY_CMD_MAX,
} LVDS_PHY_CMD;

typedef enum {
	LVDS_CTRL_TYPE_WRAP = 0,
	LVDS_CTRL_TYPE_PHY,
} LVDS_CTRL_TYPE;

int32 lvds_wrap_ctrl(uint32 lvds_type, uint32 val, uint32 select, uint32 wrap_cmd);
int32 lvds_phy_ctrl(uint32 lvds_type, uint32 port_main, uint32 clk_freq, uint32 vcm, uint32 vsw, uint32 phy_cmd);
void ipc_lvds_ctrl(IPC_CH ch, const MboxMsgReg  * pMsg); //compiler warning : referenced by tcc_ipc_ctl.c
#endif
