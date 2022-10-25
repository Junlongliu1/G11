/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					Sensor_if.h
 * \date					2021/10/25
 * \author					Guanjiafu
 * \model					EDA
 * \description				External declarations
 * \version					0.55
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/10/25		Guanjiafu		New
 *
 ********************************************************************************************/
#ifndef SNR_IF_H
#define SNR_IF_H

/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include "apn_basic_type.h"
/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/

/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/
/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/

/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/

/********************************************************************************************/
/*							Global Function Prototype Declaration							*/
/********************************************************************************************/
extern void TSK_CYC_SNR(void);
extern void TSK_SNR(void);
extern Type_uByte wubSnr_StandbyOut(void);
extern Type_uByte wubSnr_StandbyIn(void);
extern Type_uByte wubSnr_EnforceStandby(void);
extern void wvdSnr_RpcCallback(const Type_uByte aubsrc, const Type_uHWord auhEventCode, const void* avdBuffer_p, const Type_uHWord	auwBufferSize);
extern Type_uHWord wuhSnr_GetTemp(void);
extern Type_uHWord wuhSnr_GetVsp(void);
extern Type_uByte wubSnr_GetILL(void);
extern Type_uByte wubSnr_GetTelMute(void);
#endif  /* #define SNR_IF_H */
