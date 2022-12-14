/********************************************************************************************/
/** <!-- Copyright (C) Alpine Electronics, Inc. All rights reserved. ------------------------>
 *
 *	\file			gen_fn_bit_man.c
 *	\date			2010/02/10
 *	\author			Daniel Morgan (ALPINE)
 *	\model			ERATO-AV
 *	\description	Generic Library - Bit Manipulation Functions.
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
#define DECIMAL_7					7					/* Decimal 7						*/
#define MASK_BYTE_LSB				0x01				/* Mask uByte LSB					*/

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
/**	\function		wvdGen_ReorderBit8
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Reorder 8 Bit Data Array.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[out]		aubTgt_p - Target Data Pointer.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdGen_ReorderBit8( Type_uByte	*aubTgt_p )
{
	if (aubTgt_p != STD_NULL)
	{
		Type_sWord aswCounter;								/* <AUTO> Bit Counter			*/
		Type_uByte aubBaseVal;								/* <AUTO> Base Value			*/

		aubBaseVal = *aubTgt_p;
		*aubTgt_p = 0;

		for (aswCounter = DECIMAL_7; (aswCounter >= 0); aswCounter--)
		{
			*aubTgt_p += (((aubBaseVal >> aswCounter) & MASK_BYTE_LSB) << (DECIMAL_7 - aswCounter));
		}
	}
	else	/* No action required */
	{
		;
	}
}

/* End Of File */
