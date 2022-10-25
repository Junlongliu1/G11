/****************************************************************************
 *   FileName    : version.h
 *   Description : SDK version info
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

#ifndef  _VERSION_H_
#define  _VERSION_H_

#if defined(TCC803x)
#define SDK_MAJOR_VERSION	1
#define SDK_MINOR_VERSION	9
#define SDK_PATCH_VERSION	0
#else
#error "unknown chip set"
#endif

#endif //_VERSION_H_
