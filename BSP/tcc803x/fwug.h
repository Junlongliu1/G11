/****************************************************************************
 *   FileName    : fwug.h
 *   Description :
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

#ifndef _FWUG_H_
#define _FWUG_H_

#define FWUG_CMD_UPDATE_START   0x01


#if defined(TCC803x)
#define SNOR_BASEADDR	0x08000000
#define SRAM1_BASEADDR	0x07000000
#else //TCC803x_ES
#define SNOR_BASEADDR	0xD0000000
#define SRAM1_BASEADDR	0xC1000000
#endif

#define SNOR_SIGNATURE	0x524F4E53
#define SNOR_SECTION_MAX_COUNT	31
#define SNOR_ROM_INFO_OFFSET	0x100

/* SNOR information */
typedef struct snor_section_info {
    uint32 section_id;
    uint32 offset;
    uint32 section_size;
    uint32 image_size;
} snor_section_info_t;

typedef struct snor_rom_info {
    uint32        signature; /* SNOR signature */
    uint32        rom_size;
    uint32        rom_crc;
    uint32        section_info_cnt;
    snor_section_info_t section_info[31];
} snor_rom_info_t; /* total 512byte */

enum SNOR_SECTION_ID {
    SNOR_BOOT_HEADER_ID = 1,
    SNOR_BL1_BINARY_0_ID,
    SNOR_BL1_BINARY_1_ID,
    SNOR_CM4_BINARY_0_ID,
    SNOR_CM4_BINARY_1_ID,   /* 5 */
    SNOR_UPDATE_BINARY_0_ID,
    SNOR_UPDATE_BINARY_1_ID,
    SNOR_MICOM_HEADER_0_ID,
    SNOR_MICOM_HEADER_1_ID,
    SNOR_MICOM_BINARY_0_ID, /* 10 */
    SNOR_MICOM_BINARY_1_ID,
};


/* update binary information */
typedef struct _update_fw_header_x {
    uint32 address; /* target address */
    uint32 size;
    uint32 crc;
} update_fw_header_x;


extern void FWUG_Start(uint32 cmd);
extern void FWUG_CopyAndJump(void);

#endif	/* _FWUG_H_ */
