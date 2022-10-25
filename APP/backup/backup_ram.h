#ifndef EEPROM_DRV_CORE_H_
#define EEPROM_DRV_CORE_H_

/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
#include <osal.h>
#include "debug.h"
/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
#define BACKUP_USED_SIZE	   512
#define BACKUP_W_SIZE		   4

#define BACKUP_CLEAR		   0


/* Debug message control */
#if (DEBUG_ENABLE)
#define BACKUP_D(fmt, ...)	printf("(D)(%8d)[Backup][%s:%d]"fmt"\r\n",OSAL_GetOSTimeTick(), __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define BACKUP_E(fmt, ...)	printf("(E)(%8d)[Backup][%s:%d]"fmt"\r\n",OSAL_GetOSTimeTick(), __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define BACKUP_D(fmt, args...)
#define BACKUP_E(fmt, args...)
#endif

#define H_BYTE_IN_WORD(h)		((Type_uByte)(((h) >> 8)  & 0x00FF))
#define L_BYTE_IN_WORD(h)		((Type_uByte)((h)         & 0x00FF))

#define BACKUP_RAM_USB_SIZE			(440UL)	/*usb*/
#define BACKUP_ADDR_USB					(0x0000)	/* usb addr*/

enum{
	BACKUP_RAM_STATUS_STANDY         = (1U),
	BACKUP_RAM_STATUS_IDLE,
	BACKUP_RAM_STATUS_BUSY
};


/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/

/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/
typedef struct
{
	Type_uHWord uhFirstAddr;
	Type_uHWord	uhDatalength;
}BackupMapType_st;



/********************************************************************************************/
/*							Global Function Prototype Declaration							*/
/********************************************************************************************/
extern Type_uByte wubBackup_WriteData(Type_uHWord auhAddr, Type_uHWord auhLength, Type_uByte* aubOutBuf_p);
extern Type_uByte wubBackup_ReadData(Type_uHWord auhAddr, Type_uHWord auhLength, Type_uByte* aubOutBuf_p);
extern void wubBackup_ReadAllData(void);
extern Type_uByte wubBuckupRam_StandbyOut(void);
extern Type_uByte wubBuckupRam_StandbyIn(void);
extern Type_uByte wubBuckupRam_EnforceStandby(void);
extern Type_uByte wubBackup_AllClear(void);


#endif

