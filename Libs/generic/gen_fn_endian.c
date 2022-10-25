/********************************************************************************************/
/** <!-- Copyright (C) Alpine Electronics, Inc. All rights reserved. ------------------------>
 *
 *	\file			gen_fn_endian.c
 *	\date			2010/02/10
 *	\author			Daniel Morgan (ALPINE)
 *	\model			ERATO-AV
 *	\description	Generic Library - Endian Manipulation Functions.
 *	\version
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/

/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
/* Generic Library Header Files */
#include "gen_common.h"

/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
#define SHIFT_1BIT					1					/* Shift Single Bit					*/
#define SHIFT_2BIT					2					/* Shift Double Bit					*/
#define SHIFT_3BYTE					24					/* Shift 3 Bytes					*/

#define MASK_WORD_LOW_31BIT			0x7FFFFFFFU			/* Mask uWord Lower 31st Bit		*/
#define MASK_WORD_LOW_30BIT			0x3FFFFFFFU			/* Mask uWord Lower 30th Bit		*/
#define MASK_HWORD_LOW_BYTE			0x00FFU				/* Mask uHWord Lower Byte			*/
#define MASK_HWORD_HIGH_BYTE		0xFF00U				/* Mask uHWord Higher Byte			*/
#define MASK_WORD_HIGH_BYTE			0xFF000000U			/* Mask uWord Higher Byte			*/
#define MASK_WORD_MID_HIGH_BYTE		0x00FF0000U			/* Mask uWord Middle Higher Byte	*/
#define MASK_WORD_MID_LOW_BYTE		0x0000FF00U			/* Mask uWord Middle Lower Byte	*/
#define MASK_WORD_LOW_BYTE			0x000000FFU			/* Mask uWord Lower Byte			*/

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
/*							Global Variable Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							Static Variable Definition Section								*/
/********************************************************************************************/

/********************************************************************************************/
/*							Static Prototype Declaration Section							*/
/********************************************************************************************/

/********************************************************************************************/
/*							ROM Table Section												*/
/********************************************************************************************/

/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/
/********************************************************************************************/
/**	\function		wuhGen_ChgEndianUHWord
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Change Endian Half Word.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		auhSrc - Source Data.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			auhTarget - Target Data.
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
Type_uHWord wuhGen_ChgEndianUHWord( const Type_uHWord auhSrc )
{
	Type_uHWord auhTarget;									/* <AUTO> Target Data			*/


	auhTarget = (Type_uHWord)(((auhSrc << SHIFT_1BYTE) & MASK_HWORD_HIGH_BYTE) |
							  ((auhSrc >> SHIFT_1BYTE) & MASK_HWORD_LOW_BYTE));

	return auhTarget;
}

/********************************************************************************************/
/**	\function		wvdGen_ChgEndian16
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Change Endian 16 Bit Data Array.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[out]		avdTgt_p - Target Data Pointer.
 *	\param[in]		avdSrc_p - Source Data Pointer.
 *	\param[in]		auwSize - Data Size.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdGen_ChgEndian16( void *				avdTgt_p,
						 const void *		avdSrc_p,
						 const Type_uWord	auwSize )
{
	if ((avdTgt_p != STD_NULL) && (avdSrc_p != STD_NULL))
	{
		Type_uWord auwCounter;								/* <AUTO> Change Counter		*/
		Type_uWord auwIndex;								/* <AUTO> Index					*/
		Type_uWord auwChgCount;								/* <AUTO> Change Count			*/
		Type_uByte *aubTgt_p;								/* <AUTO> Target Data Pointer	*/
		const Type_uByte *aubSrc_p;							/* <AUTO> Source Data Pointer	*/

		auwChgCount = ((auwSize >> SHIFT_1BIT) & MASK_WORD_LOW_31BIT);
		aubTgt_p = (Type_uByte *)(avdTgt_p);
		aubSrc_p = (const Type_uByte *)(avdSrc_p);

		for (auwCounter = 0; (auwCounter < auwChgCount); auwCounter++)
		{
			auwIndex = (auwCounter * sizeof(Type_uHWord));
			aubTgt_p[auwIndex] = aubSrc_p[(auwIndex + 1)];
			aubTgt_p[(auwIndex + 1)] = aubSrc_p[auwIndex];
		}
	}
	else	/* No action required */
	{
		;
	}
}

/********************************************************************************************/
/**	\function		wuwGen_ChgEndianUWord
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Change Endian Word.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		auwSrc - Source Data.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			auwTgt - Target Data.
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
Type_uWord wuwGen_ChgEndianUWord( const Type_uWord auwSrc )
{
	Type_uWord auwTgt;										/* <AUTO> Target Data			*/


	auwTgt = (((auwSrc << SHIFT_3BYTE) & MASK_WORD_HIGH_BYTE) |
			  ((auwSrc << SHIFT_1BYTE) & MASK_WORD_MID_HIGH_BYTE) |
			  ((auwSrc >> SHIFT_1BYTE) & MASK_WORD_MID_LOW_BYTE) |
			  ((auwSrc >> SHIFT_3BYTE) & MASK_WORD_LOW_BYTE));

	return auwTgt;
}

/********************************************************************************************/
/**	\function		wvdGen_ChgEndian32
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Change Endian 32 Bit Data Array.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[out]		avdTgt_p - Target Data Pointer.
 *	\param[in]		avdSrc_p - Source Data Pointer.
 *	\param[in]		auwSize - Data Size.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdGen_ChgEndian32( void *				avdTgt_p,
						 const void *		avdSrc_p,
						 const Type_uWord	auwSize )
{
	if ((avdTgt_p != STD_NULL) && (avdSrc_p != STD_NULL))
	{
		Type_uWord auwCounter;								/* <AUTO> Change Counter		*/
		Type_uWord auwIndex;								/* <AUTO> Index					*/
		Type_uWord auwChgCount;								/* <AUTO> Change Count			*/
		Type_uByte *aubTgt_p;								/* <AUTO> Target Data Pointer	*/
		const Type_uByte *aubSrc_p;							/* <AUTO> Source Data Pointer	*/

		auwChgCount = ((auwSize >> SHIFT_2BIT) & MASK_WORD_LOW_30BIT);
		aubTgt_p = (Type_uByte *)(avdTgt_p);
		aubSrc_p = (const Type_uByte *)(avdSrc_p);

		for (auwCounter = 0; (auwCounter < auwChgCount); auwCounter++)
		{
			auwIndex = (auwCounter * sizeof(Type_uWord));
			aubTgt_p[auwIndex] = aubSrc_p[(auwIndex + sizeof(Type_uHWord) + sizeof(Type_uByte))];
			aubTgt_p[(auwIndex + sizeof(Type_uByte))] = aubSrc_p[(auwIndex + sizeof(Type_uHWord))];
			aubTgt_p[(auwIndex + sizeof(Type_uHWord))] = aubSrc_p[(auwIndex + sizeof(Type_uByte))];
			aubTgt_p[(auwIndex + sizeof(Type_uHWord) + sizeof(Type_uByte))] = aubSrc_p[auwIndex];
		}
	}
	else	/* No action required */
	{
		;
	}
}

/* End Of File */
