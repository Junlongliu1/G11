/********************************************************************************************/
/** <!-- Copyright (C) Alpine, Inc. All rights reserved. ------------------------>
 *
 *  \file           Backup_ram.c
 *  \date           2021/11/11
 *  \author         zhangbo
 *  \model          EDA
 *  \description
 *  <!----- No.     Date        Revised by      Details --------------------------------->
 *  <       01      2021/11/11  zhangbo
 ********************************************************************************************/

/********************************************************************************************/
/*                          Include File Section                                            */
/********************************************************************************************/
#include "os_framework.h"
#include "backup_ram.h"
#include "generic.h"
#define PMIO_VA_BACKUP_RAM_ADDR         (0x1B937000UL)
#define BYTE0_IN_WORD(h)               ((Type_uByte)( (h)        & 0x00FF))
#define BYTE1_IN_WORD(h)               ((Type_uByte)(((h) >> 8)  & 0x00FF))
#define BYTE2_IN_WORD(h)               ((Type_uByte)(((h) >> 16)  & 0x00FF))
#define BYTE3_IN_WORD(h)               ((Type_uByte)(((h) >> 24)  & 0x00FF))


static Type_uByte nubBuckupRam_State		= BACKUP_RAM_STATUS_STANDY;


/*PMIO va: Size*/
#define PMIO_VA_BACKUP_RAM_SIZE         (1024UL) // 1 Kbyte Backup RAM
#define SAL_WriteReg(v, a)              (*((volatile uint32 *)(a)) = (uint32)(v))
#define SAL_ReadReg(a)                  (*(volatile uint32 *)(a))

static Type_uByte nubBackupRamMap[BACKUP_USED_SIZE];

static const BackupMapType_st nstBackupMapTbl[] =
{
    {BACKUP_ADDR_USB,          		 BACKUP_RAM_USB_SIZE}
};

static Type_uByte nubBackup_CheckParam(Type_uHWord auhBackupAddr, Type_uHWord aubLength);
static Type_uWord nuwPMIO_GetBackupRam(Type_uWord	auwAddr);
static Type_uByte nubPMIO_SetBackupRam(Type_uWord	auwAddr,Type_uWord	auwVal);

/**********************************************************************************************
Function Name   : nubBackup_CheckParam
Date            :
Author          :
Description     : Extern Write Interface
Argument Code   : Type_uHWord auhAddr
                  Type_uHWord auhLength
                  Type_uByte* aubInBuf_p[IN]
Return Code     : BACKUP_ERR_PAR
                  BACKUP_OK
---------------------------------------------------------------------------------------------
Revision History
No  Date        Revised by          Description

**********************************************************************************************/
static Type_uByte nubBackup_CheckParam(Type_uHWord auhBackupAddr, Type_uHWord auhLength)
{
	Type_uByte aubCnt;
	Type_uByte aubRet = RES_NG;

	if ((auhBackupAddr > BACKUP_USED_SIZE -1) || (auhLength == STD_NULL) || (auhLength > BACKUP_USED_SIZE) || (auhBackupAddr + auhLength > BACKUP_USED_SIZE))
	{	 
		aubRet = RES_NG;
	}	 
	for (aubCnt = 0; aubCnt < sizeof(nstBackupMapTbl)/sizeof(BackupMapType_st); aubCnt++)
	{	 
		if ((auhBackupAddr >= nstBackupMapTbl[aubCnt].uhFirstAddr)
			&& ((auhBackupAddr + auhLength) <= (nstBackupMapTbl[aubCnt].uhFirstAddr + nstBackupMapTbl[aubCnt].uhDatalength )))
		{	  
			aubRet = RES_OK;
			break;
		}	 
	}	 
	return aubRet;
}


/**********************************************************************************************
Function Name   : wubBackupReadAllData
Date            : 
Author          : 
Description     : read all data
Argument Code   : void
Return Code     : void
---------------------------------------------------------------------------------------------
Revision History
No  Date        Revised by          Description

**********************************************************************************************/
void wubBackup_ReadAllData(void)
{
	wvdGen_MemSet(nubBackupRamMap, BACKUP_CLEAR, sizeof(nubBackupRamMap));
    for(Type_uHWord auhAddress = STD_NULL;auhAddress <  BACKUP_USED_SIZE; auhAddress += BACKUP_W_SIZE){
        *((Type_uWord*)(&nubBackupRamMap[auhAddress])) = nuwPMIO_GetBackupRam(PMIO_VA_BACKUP_RAM_ADDR + auhAddress);
    }
}

/**********************************************************************************************
Function Name   : wubBackup_AllClear
Date            : 
Author          : 
Description     : all Clear
Argument Code   : void
Return Code     : BACKUP_ERR_PAR
                  		RES_OK
---------------------------------------------------------------------------------------------
Revision History
No  Date        Revised by          Description

**********************************************************************************************/
Type_uByte wubBackup_AllClear(void)
{
	Type_uByte aubRet = RES_OK;

	Type_uWord  auwClearData;
	auwClearData = 0xFFFFFFFF;

	for(Type_uHWord auhAddress = STD_NULL;auhAddress <  PMIO_VA_BACKUP_RAM_SIZE; auhAddress += BACKUP_W_SIZE){
		aubRet = nubPMIO_SetBackupRam((PMIO_VA_BACKUP_RAM_ADDR + auhAddress),auwClearData);
		if( RES_OK != aubRet){
			return aubRet;
		}
  	 }
	 return 	aubRet;
}

/**********************************************************************************************
Function Name   : wubBackup_ReadData
Date            :
Author          :
Description     : Extern Write Interface
Argument Code   : Type_uHWord auhAddr
                  Type_uHWord auhLength
                  Type_uByte* aubInBuf_p[IN]
Return Code     : BACKUP_ERR_PAR
                  BACKUP_OK
---------------------------------------------------------------------------------------------
Revision History
No  Date        Revised by          Description

**********************************************************************************************/
Type_uByte wubBackup_ReadData(Type_uHWord auhAddr, Type_uHWord auhLength, Type_uByte* aubOutBuf_p)
{
    // check addr and length
    if (RES_OK == nubBackup_CheckParam(auhAddr, auhLength))
    {
        if (aubOutBuf_p != NULL)
        {
            wvdGen_MemCpy((void*)aubOutBuf_p, (void*)&nubBackupRamMap[auhAddr], auhLength);
        }
        else
        {
            return RES_NG;
        }
    }
    else
    {
        return RES_NG;
    }
    return RES_OK;
}
/**********************************************************************************************
Function Name   : wubBackup_WriteData
Date            :
Author          :
Description     : Extern Write Interface
Argument Code   : Type_uHWord auhAddr
                  Type_uHWord auhLength
                  Type_uByte* aubInBuf_p[IN]
Return Code     : BACKUP_ERR_PAR
                  BACKUP_OK
---------------------------------------------------------------------------------------------
Revision History
No  Date        Revised by          Description

**********************************************************************************************/
Type_uByte wubBackup_WriteData(Type_uHWord auhAddr, Type_uHWord auhLength, Type_uByte* aubInBuf_p)
{
    Type_uHWord auhStaPos = BACKUP_CLEAR;
	Type_uByte 	aubRet = RES_NG;
	Type_uHWord auhEndPos = BACKUP_CLEAR;
		
	aubRet = nubBackup_CheckParam(auhAddr, auhLength);

	if ( (aubInBuf_p != NULL)&&( RES_OK == aubRet )){
		if (nubBuckupRam_State == BACKUP_RAM_STATUS_IDLE)	/* State IDLE */
        {
			nubBuckupRam_State = BACKUP_RAM_STATUS_BUSY;
            wvdGen_MemCpy((void*)&nubBackupRamMap[auhAddr], (void*)aubInBuf_p, auhLength);
			auhStaPos = auhAddr - (auhAddr % BACKUP_W_SIZE); //Start address
			auhEndPos = ((auhAddr + auhLength) / BACKUP_W_SIZE )*BACKUP_W_SIZE;//End address
            for( ; (auhStaPos <= auhEndPos)&&(RES_OK == aubRet); auhStaPos += BACKUP_W_SIZE){
                aubRet = nubPMIO_SetBackupRam(PMIO_VA_BACKUP_RAM_ADDR + auhStaPos,*((Type_uWord*) &nubBackupRamMap[auhStaPos]));
            }
			nubBuckupRam_State = BACKUP_RAM_STATUS_IDLE;
			
		}else{
			aubRet = RES_BUSY;
		}
	}else{
		 aubRet = RES_NG;
	}
			
    return aubRet;
}
/********************************************************************************************/
/**	\function		wubBuckupRam_StandbyOut
 *	\date			2021/12/20
 *	\author			xulei
 *	\description	ACC on
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			RES_OK
 *					RES_NG
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/12/20		xulei		New
 ********************************************************************************************/
Type_uByte wubBuckupRam_StandbyOut(void)
{
	nubBuckupRam_State = BACKUP_RAM_STATUS_BUSY;		/* Module State Unload */
	wubBackup_ReadAllData();
	nubBuckupRam_State = BACKUP_RAM_STATUS_IDLE;
	
	return RES_OK;
}
/********************************************************************************************/
/**	\function		wubLed_StandbyIn
 *	\date			2021/12/20
 *	\author			xulei
 *	\description	ACC off
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			RES_OK
 *					RES_NG
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		liyongnan		New
 ********************************************************************************************/
Type_uByte wubBuckupRam_StandbyIn(void)
{
	nubBuckupRam_State = BACKUP_RAM_STATUS_STANDY;

	return RES_OK;
}
/********************************************************************************************/
/**	\function		wubBuckupRam_EnforceStandby
 *	\date			2021/12/20
 *	\author			xulei
 *	\description	Batt off
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			RES_OK
 *					RES_NG
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		xulei		New
 ********************************************************************************************/
Type_uByte wubBuckupRam_EnforceStandby(void)
{
	nubBuckupRam_State = BACKUP_RAM_STATUS_STANDY;

	return RES_OK;
}
/********************************************************************************************/
/**	\function		nubPMIO_SetBackupRam
 *	\date			2021/12/20
 *	\author			xulei
 *	\description	Batt off
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			RES_OK
 *					RES_NG
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/12/21		xulei		New
 ********************************************************************************************/
static Type_uByte nubPMIO_SetBackupRam(Type_uWord	auwAddr,Type_uWord	auwVal)
{
	Type_uByte aubRet;

	aubRet = RES_OK;

	if(auwAddr < PMIO_VA_BACKUP_RAM_ADDR)
    {
		aubRet = RES_NG;
    }

	if(auwAddr > (PMIO_VA_BACKUP_RAM_ADDR + PMIO_VA_BACKUP_RAM_SIZE))
    {
		aubRet = RES_NG;
    }

	if(aubRet == RES_OK)
    {
		SAL_WriteReg(auwVal, auwAddr);
    }

	return aubRet;
}
/********************************************************************************************/
/**	\function		nuwPMIO_GetBackupRam
 *	\date			2021/12/20
 *	\author			xulei
 *	\description	
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		none
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			data
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/09/16		xulei		New
 ********************************************************************************************/

static Type_uWord nuwPMIO_GetBackupRam(Type_uWord	auwAddr)
{
	return SAL_ReadReg(auwAddr);
}

