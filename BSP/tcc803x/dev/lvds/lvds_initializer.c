/****************************************************************************************
 *   FileName    : lvds_initializer.c
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

#include <types.h>
#include <bsp.h>
#include "reg_phys.h"
#include "debug.h"
#include <lvds_wrap.h>
#include <lvds_phy.h>
#include "tcc_ipc_typedef.h"
#include "tcc_ipc.h"
#include "tcc_ipc_buffer.h"
#include "tcc_ipc_os.h"
#include "tcc_ipc_mbox.h"
#include "tcc_ipc_cmd.h"
#include "tcc_ipc_ctl.h"

#include <lvds_initializer.h> // ipc_lvds_ctrl


int32 lvds_wrap_ctrl(uint32 lvds_type, uint32 val, uint32 select, uint32 wrap_cmd)
{
	int32 ret = LVDS_OK;
	uint32 ts_mux_type;
	uint32 (*txout_sel0)[TXOUT_DATA_PER_LINE] = NULL;
	uint32 (*txout_sel1)[TXOUT_DATA_PER_LINE] = NULL;
	/*tm123xdhp90 dual LVDS panel output format*/
	uint32 tm123xdhp90_txout_sel0[TXOUT_MAX_LINE][TXOUT_DATA_PER_LINE] = {
		{TXOUT_G_D(0), TXOUT_R_D(5), TXOUT_R_D(4), TXOUT_R_D(3), TXOUT_R_D(2), TXOUT_R_D(1), TXOUT_R_D(0)},
		{TXOUT_B_D(1), TXOUT_B_D(0), TXOUT_G_D(5), TXOUT_G_D(4), TXOUT_G_D(3), TXOUT_G_D(2), TXOUT_G_D(1)},
		{TXOUT_DE, TXOUT_VS, TXOUT_HS, TXOUT_B_D(5), TXOUT_B_D(4), TXOUT_B_D(3), TXOUT_B_D(2)},
		{TXOUT_DUMMY, TXOUT_B_D(7), TXOUT_B_D(6), TXOUT_G_D(7), TXOUT_G_D(6), TXOUT_R_D(7), TXOUT_R_D(6)}
	};
	uint32 tm123xdhp90_txout_sel1[TXOUT_MAX_LINE][TXOUT_DATA_PER_LINE] = {
		{TXOUT_G_D(0), TXOUT_R_D(5), TXOUT_R_D(4), TXOUT_R_D(3), TXOUT_R_D(2), TXOUT_R_D(1), TXOUT_R_D(0)},
		{TXOUT_B_D(1), TXOUT_B_D(0), TXOUT_G_D(5), TXOUT_G_D(4), TXOUT_G_D(3), TXOUT_G_D(2), TXOUT_G_D(1)},
		{TXOUT_DE, TXOUT_DUMMY, TXOUT_DUMMY, TXOUT_B_D(5), TXOUT_B_D(4), TXOUT_B_D(3), TXOUT_B_D(2)},
		{TXOUT_DUMMY, TXOUT_B_D(7), TXOUT_B_D(6), TXOUT_G_D(7), TXOUT_G_D(6), TXOUT_R_D(7), TXOUT_R_D(6)}
	};
	/*fld0800 single LVDS panel output format*/
	uint32 fld0800_txout_sel0[TXOUT_MAX_LINE][TXOUT_DATA_PER_LINE] = {
		{TXOUT_G_D(0), TXOUT_R_D(5), TXOUT_R_D(4), TXOUT_R_D(3), TXOUT_R_D(2), TXOUT_R_D(1), TXOUT_R_D(0)},
		{TXOUT_B_D(1), TXOUT_B_D(0), TXOUT_G_D(5), TXOUT_G_D(4), TXOUT_G_D(3), TXOUT_G_D(2), TXOUT_G_D(1)},
		{TXOUT_DE, TXOUT_VS, TXOUT_HS, TXOUT_B_D(5), TXOUT_B_D(4), TXOUT_B_D(3), TXOUT_B_D(2)},
		{TXOUT_DUMMY, TXOUT_B_D(7), TXOUT_B_D(6), TXOUT_G_D(7), TXOUT_G_D(6), TXOUT_R_D(7), TXOUT_R_D(6)}
	};

	LVDS_DEBUG("%s in lvds_type = %d, val = %d, select = %d, wrap_cmd = %d\n",__func__, lvds_type, val, select, wrap_cmd);

	if(lvds_type == LVDS_TYPE_DUAL){
		txout_sel0 = tm123xdhp90_txout_sel0;
		txout_sel1 = tm123xdhp90_txout_sel1;
		ts_mux_type = TS_MUX_IDX0;
	}else if(lvds_type == LVDS_TYPE_SINGLE){
		txout_sel0 = fld0800_txout_sel0;
		ts_mux_type = val;
	}else{
		ret = -LVDS_ERR;
	}

	if(ret == LVDS_OK){
		switch(wrap_cmd){
			case LVDS_WRAP_CMD_SET:
				LVDS_WRAP_Set(lvds_type, val, select, txout_sel0, txout_sel1);
				break;
			case LVDS_WRAP_CMD_RESET_PHY:
				LVDS_WRAP_ResetPHY(ts_mux_type, 1);
				break;
			default:
				ret = -LVDS_ERR;
				break;
		}
	}
	LVDS_DEBUG("%s out. ret = %d\n", __func__, ret);
	return ret;
}

int32 lvds_phy_ctrl(uint32 lvds_type, uint32 port_main, uint32 clk_freq, uint32 vcm, uint32 vsw, uint32 phy_cmd)
{
	int32 ret = LVDS_OK;
	uint32 upsample_ratio = 0;
	uint32 ref_cnt = 0;
	uint32 lvds_port_main = 0;
	uint32 lvds_port_sub = 0;
	uint32 *lvds_lane_main = NULL;
	uint32 *lvds_lane_sub = NULL;
	/*tm123xdhp90 dual LVDS panel output format*/
	uint32 tm123xdhp90_lane_main[5] = {LVDS_PHY_DATA3_LANE, LVDS_PHY_CLK_LANE, LVDS_PHY_DATA2_LANE, LVDS_PHY_DATA1_LANE, LVDS_PHY_DATA0_LANE};
	uint32 tm123xdhp90_lane_sub[5] = {LVDS_PHY_DATA0_LANE, LVDS_PHY_DATA1_LANE, LVDS_PHY_DATA2_LANE, LVDS_PHY_CLK_LANE, LVDS_PHY_DATA3_LANE};
	/*fld0800 dual LVDS panel output format*/
	uint32 fld0800_lane_main[5] = {LVDS_PHY_DATA0_LANE, LVDS_PHY_DATA1_LANE, LVDS_PHY_DATA2_LANE, LVDS_PHY_CLK_LANE, LVDS_PHY_DATA3_LANE};

	LVDS_DEBUG("%s in. lvds_type = %d, port_main = %d, clk_freq = %d, vcm = %d, vsw = %d, phy_cmd = %d\n"
		,__func__,lvds_type, port_main, clk_freq, vcm, vsw, phy_cmd);

	if(lvds_type == LVDS_TYPE_DUAL){
		lvds_port_main = LVDS_PHY_PORT_D0;
		lvds_port_sub = LVDS_PHY_PORT_D1;
		lvds_lane_main = tm123xdhp90_lane_main;
		lvds_lane_sub = tm123xdhp90_lane_sub;
	}else if(lvds_type == LVDS_TYPE_SINGLE){
		lvds_port_main = port_main;
		lvds_port_sub = LVDS_PHY_PORT_MAX;
		lvds_lane_main = fld0800_lane_main;
	}else{
		ret = -LVDS_ERR;
	}
	
	if(ret == LVDS_OK){
		upsample_ratio = LVDS_PHY_GetUpsampleRatio(lvds_port_main, lvds_port_sub, clk_freq);
		ref_cnt = LVDS_PHY_GetRefCnt(lvds_port_main, lvds_port_sub, clk_freq, upsample_ratio);
		switch(phy_cmd){
			case LVDS_PHY_CMD_INIT:
				LVDS_PHY_Init(lvds_type, lvds_port_main, lvds_port_sub, upsample_ratio, ref_cnt, vcm, vsw, lvds_lane_main, lvds_lane_sub);
				break;
			default:
				ret = -LVDS_ERR;
				break;
		}
	}
	LVDS_DEBUG("%s out. ret = %d\n", __func__, ret);
	return ret;
}

#ifdef TCC_IPC_SDK_V180_LVDS
void ipc_lvds_ctrl(IPC_CH ch, const MboxMsgReg  * pMsg)
{
	if(pMsg != NULL)
	{
		IPC_UINT32 cmd[8];

		cmd[0] = pMsg->reg[0]; /* u-boot */
		cmd[1] = pMsg->reg[1]; /* lvds ctrl type */
		cmd[2] = pMsg->reg[2];
		cmd[3] = pMsg->reg[3];
		cmd[4] = pMsg->reg[4];
		cmd[5] = pMsg->reg[5];
		cmd[6] = pMsg->reg[6];
		cmd[7] = pMsg->reg[7];

		if(cmd[1] == LVDS_CTRL_TYPE_WRAP){ //lvds_wrap
			//seqID = cmd[1];
			lvds_wrap_ctrl(cmd[2],cmd[3],cmd[4],cmd[7]);
		}else if(cmd[1] == LVDS_CTRL_TYPE_PHY){ //lvds_phy
			//seqID = cmd[1];
			lvds_phy_ctrl(cmd[2],cmd[3],cmd[4],cmd[5],cmd[6],cmd[7]);
		}else{
			//seqID = 5; //LVDS fail
			LVDS_DEBUG("%s, unknown lvds ctrl type\n", __func__);
		}
	}
}
#else
void ipc_lvds_ctrl(IPC_CH ch, const MboxMsgReg  * pMsg)
{
  if(pMsg != NULL)
  {
    IPC_UINT32 cmd[8];
		int i;
    IPC_INT32 ret;
		IPC_UINT32 size = 0;
		IPC_UCHAR * buffer = NULL;

    cmd[0] = pMsg->reg[0]; /* u-boot */
    cmd[1] = pMsg->reg[1];
    cmd[2] = pMsg->reg[2]; /* lvds ctrl type */
    cmd[3] = pMsg->reg[3];
    cmd[4] = pMsg->reg[4];
    cmd[5] = pMsg->reg[5];
    cmd[6] = pMsg->reg[6];
    cmd[7] = pMsg->reg[7];

		for(i = 0 ; i <= 7 ; i++){
			LVDS_DEBUG("%s cmd[%d] = %x\n", __func__,i, cmd[i]);
		}

    if(cmd[2] == LVDS_CTRL_TYPE_WRAP){
			lvds_wrap_ctrl(cmd[3],cmd[4],cmd[5],cmd[7]);
			ret = wswipc_send_sys_ctrl(ch, (Type_uWord*)cmd, buffer, size, IPC_O_NONACK);

			if(ret < IPC_SUCCESS) {
				LVDS_DEBUG("%s, U-BOOT WRITE - ERROR\n", __func__);
			}
    }else if(cmd[2] == LVDS_CTRL_TYPE_PHY){
      uint32 vcm = (cmd[6] >> 16) & 0xfff; //up to 4095
      uint32 vsw = cmd[6] & 0xfff; //up to 4095
      lvds_phy_ctrl(cmd[3],cmd[4],cmd[5],vcm,vsw,cmd[7]);

			ret = wswipc_send_sys_ctrl(ch, (Type_uWord*)cmd, buffer, size, IPC_O_NONACK);
			if(ret < IPC_SUCCESS) {
				LVDS_DEBUG("%s, U-BOOT WRITE - ERROR\n", __func__);
			}
    }else{
			LVDS_DEBUG("%s, unknown lvds ctrl type\n", __func__);
    }
  }
}
#endif
