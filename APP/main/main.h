/****************************************************************************
 *   FileName    : Main.h
 *   Description : Main Application Start Entry
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

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               GLOBALS present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  _MAIN_H_
#define  _MAIN_H_

/*
*********************************************************************************************************
*                                            GLOBAL DEFINITIONS
*********************************************************************************************************
*/
typedef struct
{
	// SDK Software version
	uint8 sw_major_version;
	uint8 sw_minor_version;
	uint8 sw_patch_version;
	uint8 sw_version_reserve;
} Sdk_VersionInfoType;

extern void cmain (void);
extern void TSK_APP(void);
extern void SDK_GetVersionInfo(Sdk_VersionInfoType * pVersion);
#endif //_MAIN_H_