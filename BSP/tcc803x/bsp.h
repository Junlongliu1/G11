/****************************************************************************
 *   FileName    : bsp.h
 *   Description : Board Support Package for MICOM
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
#ifndef _BSP_H_
#define _BSP_H_

// For Dolphin +
#define TCC803x_BD_VER_0_1	30 ///number start from 30 for Dolphin+
#define TCC803x_BD_VER_0_3	31 // EVB 0.3

#define TCC_EVM_BD_VERSION (TCC803x_BD_VER_0_3)

//KEY TYPE
#define KEYPAD_MANY_PUSHKEY 0x0
#define KEYPAD_ONLY_ROTARY	0x1

#ifdef TCC803x_CS
//for TCC8030, TCC8035, TCC8036 Safety Enabled Package
//#define SDM_SAFETY_EN
#endif

//#define SDRAM_1G
#define SDRAM_512

//#define SRAM_DEVICE_SHARE_CONFIG_BASE (0xF0009FD0UL)
//#define SRAM_BOOT_SEQ_CHECK_BASE (0xF0009FF8UL)
#define SRAM_EXCEPTION_SAVE_BASE (0xF000B1F0UL) // -- 0xF000_B200 (16bytes)

#define  BSP_FALSE                                         0u
#define  BSP_TRUE                                          1u

#define  BSP_DISABLED                                      0u
#define  BSP_ENABLED                                       1u

#define  BSP_NO                                            0u
#define  BSP_YES                                           1u

#define  BSP_FAIL                                          0u
#define  BSP_OK                                            1u

#define  BSP_NULL                                          0

#define	BSP_BIT_31								0x80000000ul
#define	BSP_BIT_30								0x40000000ul
#define	BSP_BIT_29								0x20000000ul
#define	BSP_BIT_28								0x10000000ul
#define	BSP_BIT_27								0x08000000ul
#define	BSP_BIT_26								0x04000000ul
#define	BSP_BIT_25								0x02000000ul
#define	BSP_BIT_24								0x01000000ul
#define	BSP_BIT_23								0x00800000ul
#define	BSP_BIT_22								0x00400000ul
#define	BSP_BIT_21								0x00200000ul
#define	BSP_BIT_20								0x00100000ul
#define	BSP_BIT_19								0x00080000ul
#define	BSP_BIT_18								0x00040000ul
#define	BSP_BIT_17								0x00020000ul
#define	BSP_BIT_16								0x00010000ul
#define	BSP_BIT_15								0x00008000ul
#define	BSP_BIT_14								0x00004000ul
#define	BSP_BIT_13								0x00002000ul
#define	BSP_BIT_12								0x00001000ul
#define	BSP_BIT_11								0x00000800ul
#define	BSP_BIT_10								0x00000400ul
#define	BSP_BIT_09								0x00000200ul
#define	BSP_BIT_08								0x00000100ul
#define	BSP_BIT_07								0x00000080ul
#define	BSP_BIT_06								0x00000040ul
#define	BSP_BIT_05								0x00000020ul
#define	BSP_BIT_04								0x00000010ul
#define	BSP_BIT_03								0x00000008ul
#define	BSP_BIT_02								0x00000004ul
#define	BSP_BIT_01								0x00000002ul
#define	BSP_BIT_00								0x00000001ul
#define BSP_BIT_NONE								0x00000000ul

#define  BSP_BIT_SET(val, mask)                 ((val) = ((val) | (mask)))
#define  BSP_BIT_CLR(val, mask)                 ((val) = ((val) & ~(mask)))

#define  BSP_BIT_SET_32(val, mask)              BSP_BIT_SET((val), (mask))
#define  BSP_BIT_CLR_32(val, mask)              BSP_BIT_CLR((val), (mask))

#ifndef NOP_DELAY
#ifdef __GNU_C__
#define NOP_DELAY()	__asm__ ("nop")
#else
#define NOP_DELAY()	asm("nop")
#endif
#endif

#define BSP_ERR_NONE                (0)		// ok
#define BSP_ERR_INIT                (-1)	// initialization error
#define BSP_ERR_INVALID_HANDLE      (-2)//  invalid handle
#define BSP_ERR_INVALID_PARAMETER   (-3)	// passed invalid parameter
#define BSP_ERR_NOT_SUPPORT         (-9)	// not supported
#define BSP_ERR_NO_ACK             	(-10)	// ack error
#define BSP_ERR_TIMEOUT      		(-11)	// timed out error
#define BSP_ERR_NO_DATA             (-12)	// no data
#define BSP_ERR_UNDEF_STATE         (-13)	// not defined state




extern void _start(void);
extern void arm_reserved(void); //QAC : reference .asm function in C module
extern void init_arm(void);
extern void BSP_Init(void);
extern void BSP_Undef_Abort_Exception_Hook(uint32 except_id, uint32 dumy,uint32 LR);
extern void System_Halt(void);
extern void ARM_Set_VBAR(uint32 addr);
extern void BSP_usDelay(uint32 usdelay);
#endif
