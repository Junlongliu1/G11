/****************************************************************************
 *   FileName    : ictc.c
 *   Description : to use the ICTC for Telechips chipset
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved
This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited to re-
distribution in source or binary form is strictly prohibited.
This source code is provided "AS IS" and nothing contained in this source code shall
constitute any express or implied warranty of any kind, including without limitation, any warranty
of merchantability, fitness for a particular purpose or non-infringement of any patent, copyright
or other third party intellectual property right. No warranty is made, express or implied,
regarding the information's accuracy, completeness, or performance.
In no event shall Telechips be liable for any claim, damages or other liability arising from, out of
or in connection with this source code or the use in the source code.
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
between Telechips and Company.
*
****************************************************************************/
#ifndef __ICTC_H__
#define __ICTC_H__

#include "apn_basic_type.h"

#ifndef ENABLE
#define ENABLE 1
#endif
#ifndef DISABLE
#define DISABLE 0
#endif

typedef void (*ictc_userCb)(int32 ch, int32 period, int32 duty);

#define FLT_CNT_FULL_IRQ		(1<<0)
#define TO_CNT_FULL_IRQ			(1<<1)
#define FEDGE_CNT_FULL_IRQ		(1<<2)
#define CMP_DT_ERR_IRQ			(1<<3)
#define CMP_PRD_ERR_IRQ			(1<<4)
#define PD_CMP_CNT_FULL_IRQ		(1<<5)
#define REDGE_NOTI_IRQ			(1<<6)
#define FEDGE_NOTI_IRQ			(1<<7)

#define EN_CTRL				0x00
#define MODE_CTRL			0x04
#define IRQ_CTRL			0x08
#define TIME_OUT			0x0c
#define RISING_EDGE			0x10
#define FALLING_EDGE		0x14
#define PRD_CMP_RND			0x18
#define DUTY_CMP_RND		0x1c
#define FALLING_EDGE_CNT	0x20
#define PRD_CNT_PVL			0x30
#define	PRD_CNT_PPVL		0x34 
#define DT_CNT_PVL			0x38
#define DT_CNT_PPVL			0x3c
#define FEDGE_DET_CNT_CVL	0x40
#define FEDGE_DET_CNT_PVL	0x44
#define REDGE_TS_CNT		0x48
#define FEDGE_TS_CNT		0x4c


void ictc_set_TimeOutCount_Flag(Type_uByte ubFlag);
Type_uByte ictc_get_TimeOutCount_Flag(void);
int32 ictc_get_fedge_det_cnt_cvl(uint32 ch);
int32 ictc_get_fedge_det_cnt_pvl(uint32 ch);
int32 ictc_get_period_pvl_count(uint32 ch);
int32 ictc_get_period_ppvl_count(uint32 ch);
int32 ictc_get_duty_pvl_count(uint32 ch);
int32 ictc_get_duty_ppvl_count(uint32 ch);
int32 ictc_set_clk(uint32 ch, uint32 divide_val);
void ictc_set_port(uint32 ch, uint32 gpio_num);
void ictc_set_filter_fmode(uint32 ch, uint32 filtermode);
void ictc_set_filter_rmode(uint32 ch, uint32 filtermode);
//void ictc_clear_counter(uint32 ch); //QAC
//void ictc_clear_interrupt(uint32 ch);//QAC
void ictc_set_interrupt(uint32 ch, uint32 irq_en);
void ictc_set_to_vl(uint32 ch, uint32 value);
void ictc_set_redge_vl(uint32 ch, uint32 value);
void ictc_set_fedge_vl(uint32 ch, uint32 value);
void ictc_set_prd_rnd_vl(uint32 ch, uint32 value);
void ictc_set_dt_rnd_vl(uint32 ch, uint32 value);
void ictc_reset(uint32 ch);
//void ictc_set_tclk(uint32 ch);//QAC
void ictc_start(uint32 ch);
//void ictc_stop(uint32 ch);//QAC
void ictc_init(void);
void ictc_set_callback(uint32 ch, ictc_userCb cb);
#endif
