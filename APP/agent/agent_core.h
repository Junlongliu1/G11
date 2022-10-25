/*******************************************************************************************/
/**<!-------Copyright (C) 2021 Alpine Electronics, Inc. All rights reserved. --------------->
 * 
 * \file					Agent_Core.h
 * \date					2021/11/01
 * \author					zhangbo
 * \model					EDA
 * \description				External declarations
 * \version					0.55
 *
 *********************************************************************************************
 * \par 	 				Revision History:
 *	<!-------------	No.		Date			Revised by		Details	------------------------->
 *					0001	2021/11/01		zhangbo		New
 ********************************************************************************************/


#ifndef _AGENT_CORE_H_
#define _AGENT_CORE_H_

/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/

#define	AGENT_CLEAR_UB					(0x00U)
#define	AGENT_CLEAR_UH					(0x0000U) 
#define	AGENT_CLEAR_UW					(0x00000000U) 
#define	AGENT_INDEX_0					0
#define	AGENT_INDEX_1					1
#define	AGENT_INDEX_2					2
#define	AGENT_INDEX_3					3
#define	AGENT_INDEX_4					4
#define	AGENT_INDEX_5					5
#define	AGENT_INDEX_6					6
#define	AGENT_INDEX_7					7
#define	AGENT_INDEX_8					8
#define	AGENT_INDEX_9					9
#define	AGENT_INDEX_10					10

/* Value */
#define AGENT_VALUE_0						(0U)
#define AGENT_VALUE_1						(1U)
#define AGENT_VALUE_3						(3U)


//TODO: to be change 
#define	AGENT_DATA_SIZE_MAX				(460U)

#define	AGENT_DATA_RAM			(0x00U)
#define AGENT_DATA_ROM			(0x01U)

#define AGENT_RETRY_CONT		(1U)
#define AGENT_RETRY_BUSY_CONT	(3U)


#define MODEL_SIZE  			(8U)



/********************************************************************************************/
/*							Type Definition Section											*/
/********************************************************************************************/
/* Debug message control */
//#define AGENT_DEBUG
#ifdef AGENT_DEBUG

#define AGENT_D(fmt, ...)	printf("(D)(%8d)[AGENT][%s:%d]"fmt"\r\n",OSAL_GetOSTimeTick(), __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define AGENT_E(fmt, ...)	printf("(E)(%8d)[AGENT][%s:%d]"fmt"\r\n",OSAL_GetOSTimeTick(), __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define AGENT_D(fmt, args...)	do {} while(0)
#define AGENT_E(fmt, args...)	do {} while(0)

#endif


/********************************************************************************************/
/*							Enumeration Type Definition Section								*/
/********************************************************************************************/
enum AGENT_STATE{
	AGENT_INITIALIZE = 0U,
	AGENT_STANDBYOUT,
	AGENT_PRESTANDBY,
	AGENT_STANDBYIN,
	AGENT_ENFORCESTANDBY
};

//Temp for complie
enum AGENT_EVENT_ID{
	/* R5 Event */
	AGENT_EV_MIN = 0U,
	AGENT_EV_DATA_SAVE,
	AGENT_EV_DATA_READ,
	AGENT_EV_MAX
};

/********************************************************************************************/
/*							Structure/Union Type Definition Section							*/
/********************************************************************************************/

typedef struct{
	Type_uHWord	uhReqMoudelEvt;
	Type_uHWord	uhStartPos;
	Type_uByte	ubWriteType;
	Type_uHWord	uhDataSize;
}AgentDataSave_st;

typedef struct{
	Type_uByte	ubPropertyID;
	Type_uHWord	uhReqMoudelEvt;
	Type_uHWord	uhStartPos;
	Type_uHWord	uhDataSize;
}AgentDataReadReq_st;


typedef struct{
	Type_uByte	ubPropertyID;
	Type_uHWord	uhReqMoudelEvt;
	Type_uHWord	uhStartPos;
	Type_uHWord	uhChangeSize;
	Type_uByte	ubData_a[AGENT_DATA_SIZE_MAX];
}AgentDataWriteReq_st;

typedef struct{
	Type_uByte	ubPropertyID;
	Type_uHWord	uhReqMoudelEvt;
	Type_uByte	ubResult;
	Type_uHWord	uhStartPos;
	Type_uHWord	uhSize;
	Type_uByte	ubData_a[AGENT_DATA_SIZE_MAX];
}AgentDataWriteRes_st;


typedef struct{
	Type_uByte	ubPropertyID;
	Type_uHWord	uhReqMoudelEvt;
	Type_uByte	ubResult;
	Type_uHWord	uhStartPos;
	Type_uHWord	uhDataSize;
	Type_uByte	ubData_a[AGENT_DATA_SIZE_MAX];
}AgentDataReadRes_st;




/********************************************************************************************/
/*							Extern Declaration												*/
/********************************************************************************************/

extern Type_uByte wubAgentStandbyFlag;
/********************************************************************************************/
/*							Global Function Prototype Declaration							*/
/********************************************************************************************/

#endif
