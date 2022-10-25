/********************************************************************************************/
/** <!-- Copyright (C) Alpine Electronics, Inc. All rights reserved. ------------------------>
 *
 *	\file			gen_fn_mem.c
 *	\date			2010/02/10
 *	\author			Daniel Morgan (ALPINE)
 *	\model			ERATO-AV
 *	\description	Generic Library - Memory Manipulation Functions.
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
#define GEN_CPY_THRESHOLD		(96)
#define GEN_SET_THRESHOLD		(96)

#define GEN_ALIGNMASK_02BYTE	(0x00000001U)
#define GEN_ALIGNMASK_04BYTE	(0x00000003U)
#define GEN_ALIGNSIZE_02BYTE	(0x00000002U)
#define GEN_ALIGNSIZE_04BYTE	(0x00000004U)

#define GEN_SHIFT_01BIT			(1U)
#define GEN_SHIFT_02BIT			(2U)
#define GEN_SHIFT_05BIT			(5U)
#define GEN_SHIFT_06BIT			(6U)
#define GEN_SHIFT_08BIT			(8U)
#define GEN_SHIFT_16BIT			(16U)
#define GEN_SHIFT_24BIT			(24U)
#define GEN_SHIFTMASK_01BIT		(0x7FFFFFFFU)
#define GEN_SHIFTMASK_02BIT		(0x3FFFFFFFU)
#define GEN_SHIFTMASK_04BIT		(0x07FFFFFFU)
#define GEN_SHIFTMASK_06BIT		(0x03FFFFFFU)
#define GEN_MASK_SUB_16BIT		(0x0000001FU)
#define GEN_MASK_SUB_32BIT		(0x0000003FU)

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
static void nvdGen_MemSet1x( void *,
							 const Type_uByte,
							 const Type_uWord);

static void nvdGen_MemSet2x( void *,
							 const Type_uByte,
							 const Type_uWord);

static void nvdGen_MemSet4x( void *,
							 const Type_uByte,
							 const Type_uWord);

static void nvdGen_MemCpy1x( void *,
							 const void *,
							 const Type_uWord );
static void nvdGen_MemCpy2x( void *,
							 const void *,
							 const Type_uWord );
static void nvdGen_MemCpy4x( void *,
							 const void *,
							 const Type_uWord );

/********************************************************************************************/
/*							ROM Table Section												*/
/********************************************************************************************/

/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/
/********************************************************************************************/
/**	\function		wvdGen_MemSet
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Set n Bytes of Data to Target.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[out]		avdTgt_p - Target Data Pointer.
 *	\param[in]		aubData - Data Value to Set.
 *	\param[in]		auwSize - Data Size.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdGen_MemSet( void *				avdTgt_p,
					const Type_uByte	aubData,
					const Type_uWord	auwSize )
{
	Type_uWord  auwDestAddr;								/* <AUTO> Target Address		*/
	Type_uByte *aubDest_p;									/* <AUTO> Target Data Pointer	*/

	if (avdTgt_p != STD_NULL)
	{
		if (auwSize > GEN_SET_THRESHOLD)
		{
			aubDest_p = (Type_uByte *)avdTgt_p;
			auwDestAddr = (Type_uWord)aubDest_p;

			if ((auwDestAddr & GEN_ALIGNMASK_04BYTE) != GEN_ALIGNMASK_04BYTE)			/* 4byte align */
			{
				Type_uWord auwPreSize;
				Type_uWord auwPostSize;
				Type_uWord auwSetSize;

				auwPreSize = auwDestAddr & GEN_ALIGNMASK_04BYTE;

				if (auwPreSize > 0)
				{
					auwPreSize = GEN_ALIGNSIZE_04BYTE - auwPreSize;
				}
				else	/* No action required */
				{
					;
				}

				auwPostSize = (auwSize - auwPreSize) & GEN_ALIGNMASK_04BYTE;
				auwSetSize = auwSize - (auwPreSize + auwPostSize);

				nvdGen_MemSet1x( &aubDest_p[0],							aubData, auwPreSize );
				nvdGen_MemSet4x( &aubDest_p[auwPreSize],				aubData, auwSetSize );
				nvdGen_MemSet1x( &aubDest_p[auwPreSize + auwSetSize],	aubData, auwPostSize );
			}
			else if ((auwDestAddr & GEN_ALIGNMASK_02BYTE) != GEN_ALIGNMASK_02BYTE)		/* 2byte align */
			{
				Type_uWord auwPreSize;
				Type_uWord auwPostSize;
				Type_uWord auwSetSize;

				auwPreSize = auwDestAddr & GEN_ALIGNMASK_02BYTE;

				if (auwPreSize != 0)
				{
					aubDest_p[0] = aubData;
				}
				else	/* No action required */
				{
					;
				}

				auwPostSize = (auwSize - auwPreSize) & GEN_ALIGNMASK_02BYTE;
				auwSetSize = auwSize - (auwPreSize + auwPostSize);

				nvdGen_MemSet2x( &aubDest_p[auwPreSize], aubData, auwSetSize );

				if (auwPostSize != 0)
				{
					aubDest_p[auwPreSize + auwSetSize] = aubData;
				}
				else	/* No action required */
				{
					;
				}
			}
			else
			{
				nvdGen_MemSet1x( avdTgt_p, aubData, auwSize );
			}
		}
		else
		{
			nvdGen_MemSet1x( avdTgt_p, aubData, auwSize );
		}
	}
	else	/* No action required */
	{
		;
	}
}


/********************************************************************************************/
/**	\function		nvdGen_MemSet1x
 *	\date			2011/04/06
 *	\author			Hiroaki Mitani (HAD)
 *	\description	Set n Bytes of Data to Target.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[out]		avdTgt_p - Target Data Pointer.
 *	\param[in]		aubData - Data Value to Set.
 *	\param[in]		auwSize - Data Size.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdGen_MemSet1x( void *				avdTgt_p,
							 const Type_uByte	aubData,
							 const Type_uWord	auwSize )
{
	if (avdTgt_p != STD_NULL)
	{
		Type_uWord auwCounter;								/* <AUTO> Data Counter			*/
		Type_uByte *aubTgt_p;								/* <AUTO> Target Data Pointer	*/

		aubTgt_p = (Type_uByte *)(avdTgt_p);

		for (auwCounter = 0; (auwCounter < auwSize); auwCounter++)
		{
			aubTgt_p[auwCounter] = aubData;
		}
	}
	else	/* No action required */
	{
		;
	}
}


/********************************************************************************************/
/**	\function		nvdGen_MemSet2x
 *	\date			2011/04/06
 *	\author			Hiroaki Mitani (HAD)
 *	\description	Set n Bytes of Data from Source to Target.
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
static void nvdGen_MemSet2x(	void *				avdTgt_p,
								const Type_uByte	aubData,
								const Type_uWord	auwSize )
{
	Type_uWord auwCountSize;
	Type_uWord auwSubCountSize;

	auwCountSize = (auwSize >> GEN_SHIFT_05BIT) & GEN_SHIFTMASK_04BIT;
	auwSubCountSize = ((auwSize & GEN_MASK_SUB_16BIT) >> GEN_SHIFT_01BIT) & GEN_SHIFTMASK_01BIT;

	if (avdTgt_p != STD_NULL)
	{
		Type_uWord auwCounter;								/* <AUTO> Data Counter			*/
		Type_uHWord *auhTgt_p;								/* <AUTO> Target Data Pointer	*/
		Type_uHWord auhSrc;									/* <AUTO> Source Data Pointer	*/

		auhTgt_p = (Type_uHWord *)(avdTgt_p);
		auhSrc	 = (aubData << GEN_SHIFT_08BIT) | aubData; 

		for (auwCounter = 0; (auwCounter < auwCountSize); auwCounter++)
		{
			*auhTgt_p = auhSrc;
			auhTgt_p++;
			*auhTgt_p = auhSrc;
			auhTgt_p++;
			*auhTgt_p = auhSrc;
			auhTgt_p++;
			*auhTgt_p = auhSrc;
			auhTgt_p++;
			*auhTgt_p = auhSrc;
			auhTgt_p++;
			*auhTgt_p = auhSrc;
			auhTgt_p++;
			*auhTgt_p = auhSrc;
			auhTgt_p++;
			*auhTgt_p = auhSrc;
			auhTgt_p++;
			*auhTgt_p = auhSrc;
			auhTgt_p++;
			*auhTgt_p = auhSrc;
			auhTgt_p++;
			*auhTgt_p = auhSrc;
			auhTgt_p++;
			*auhTgt_p = auhSrc;
			auhTgt_p++;
			*auhTgt_p = auhSrc;
			auhTgt_p++;
			*auhTgt_p = auhSrc;
			auhTgt_p++;
			*auhTgt_p = auhSrc;
			auhTgt_p++;
			*auhTgt_p = auhSrc;
			auhTgt_p++;
		}

		for (auwCounter = 0; (auwCounter < auwSubCountSize); auwCounter++)
		{
			*auhTgt_p = auhSrc;
			auhTgt_p++;
		}
	}
	else	/* No action required */
	{
		;
	}
}


/********************************************************************************************/
/**	\function		nvdGen_MemSet4x
 *	\date			2011/04/06
 *	\author			Hiroaki Mitani (HAD)
 *	\description	Set n Bytes of Data from Source to Target.
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
static void nvdGen_MemSet4x(	void *				avdTgt_p,
								const Type_uByte	aubData,
								const Type_uWord	auwSize )
{
	Type_uWord auwCountSize;
	Type_uWord auwSubCountSize;

	auwCountSize = (auwSize >> GEN_SHIFT_06BIT) & GEN_SHIFTMASK_06BIT;
	auwSubCountSize = ((auwSize & GEN_MASK_SUB_32BIT) >> GEN_SHIFT_02BIT) & GEN_SHIFTMASK_02BIT;

	if (avdTgt_p != STD_NULL)
	{
		Type_uWord auwCounter;								/* <AUTO> Data Counter			*/
		Type_uWord *auwTgt_p;								/* <AUTO> Target Data Pointer	*/
		Type_uWord auwSrc;									/* <AUTO> Source Data Pointer	*/

		auwTgt_p = (Type_uWord *)(avdTgt_p);
		auwSrc	 = (aubData << GEN_SHIFT_24BIT) | (aubData << GEN_SHIFT_16BIT) | (aubData << GEN_SHIFT_08BIT) | aubData; 

		for (auwCounter = 0; (auwCounter < auwCountSize); auwCounter++)
		{
			*auwTgt_p = auwSrc;
			auwTgt_p++;
			*auwTgt_p = auwSrc;
			auwTgt_p++;
			*auwTgt_p = auwSrc;
			auwTgt_p++;
			*auwTgt_p = auwSrc;
			auwTgt_p++;
			*auwTgt_p = auwSrc;
			auwTgt_p++;
			*auwTgt_p = auwSrc;
			auwTgt_p++;
			*auwTgt_p = auwSrc;
			auwTgt_p++;
			*auwTgt_p = auwSrc;
			auwTgt_p++;
			*auwTgt_p = auwSrc;
			auwTgt_p++;
			*auwTgt_p = auwSrc;
			auwTgt_p++;
			*auwTgt_p = auwSrc;
			auwTgt_p++;
			*auwTgt_p = auwSrc;
			auwTgt_p++;
			*auwTgt_p = auwSrc;
			auwTgt_p++;
			*auwTgt_p = auwSrc;
			auwTgt_p++;
			*auwTgt_p = auwSrc;
			auwTgt_p++;
			*auwTgt_p = auwSrc;
			auwTgt_p++;
		}

		for (auwCounter = 0; (auwCounter < auwSubCountSize); auwCounter++)
		{
			*auwTgt_p = auwSrc;
			auwTgt_p++;
		}

	}
	else	/* No action required */
	{
		;
	}
}


/********************************************************************************************/
/**	\function		wvdGen_MemCpy
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Copy n Bytes of Data from Source to Target.
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
void wvdGen_MemCpy( void *				avdTgt_p,
					const void *		avdSrc_p,
					const Type_uWord	auwSize )
{
	Type_uWord  auwSrcAddr;									/* <AUTO> Source Address		*/
	Type_uWord  auwDestAddr;								/* <AUTO> Target Address		*/
	Type_uByte *aubSrc_p;									/* <AUTO> Source Data Pointer	*/
	Type_uByte *aubDest_p;									/* <AUTO> Target Data Pointer	*/

	if ((avdTgt_p != STD_NULL) && (avdSrc_p != STD_NULL))
	{
		if (auwSize > GEN_CPY_THRESHOLD)
		{
			aubSrc_p = (Type_uByte *)avdSrc_p;
			aubDest_p = (Type_uByte *)avdTgt_p;
			auwSrcAddr = (Type_uWord)aubSrc_p;
			auwDestAddr = (Type_uWord)aubDest_p;

			if (((auwDestAddr & GEN_ALIGNMASK_04BYTE) == (auwSrcAddr & GEN_ALIGNMASK_04BYTE)))			/* 4byte align */
			{
				Type_uWord auwPreSize;
				Type_uWord auwPostSize;
				Type_uWord auwCopySize;

				auwPreSize = auwSrcAddr & GEN_ALIGNMASK_04BYTE;

				if (auwPreSize > 0)
				{
					auwPreSize = GEN_ALIGNSIZE_04BYTE - auwPreSize;
				}
				else	/* No action required */
				{
					;
				}

				auwPostSize = (auwSize - auwPreSize) & GEN_ALIGNMASK_04BYTE;
				auwCopySize = auwSize - (auwPreSize + auwPostSize);

				nvdGen_MemCpy1x( &aubDest_p[0], &aubSrc_p[0], auwPreSize );
				nvdGen_MemCpy4x( &aubDest_p[auwPreSize], &aubSrc_p[auwPreSize], auwCopySize );
				nvdGen_MemCpy1x( &aubDest_p[auwPreSize + auwCopySize], &aubSrc_p[auwPreSize + auwCopySize], auwPostSize );
			}
			else if (((auwDestAddr & GEN_ALIGNMASK_02BYTE) == (auwSrcAddr & GEN_ALIGNMASK_02BYTE)))		/* 2byte align */
			{
				Type_uWord auwPreSize;
				Type_uWord auwPostSize;
				Type_uWord auwCopySize;

				auwPreSize = auwSrcAddr & GEN_ALIGNMASK_02BYTE;

				if (auwPreSize != 0)
				{
					aubDest_p[0] = aubSrc_p[0];
				}
				else	/* No action required */
				{
					;
				}

				auwPostSize = (auwSize - auwPreSize) & GEN_ALIGNMASK_02BYTE;
				auwCopySize = auwSize - (auwPreSize + auwPostSize);

				nvdGen_MemCpy2x( &aubDest_p[auwPreSize], &aubSrc_p[auwPreSize], auwCopySize );

				if (auwPostSize != 0)
				{
					aubDest_p[auwPreSize + auwCopySize] = aubSrc_p[auwPreSize + auwCopySize];
				}
				else	/* No action required */
				{
					;
				}
			}
			else
			{
				nvdGen_MemCpy1x( avdTgt_p, avdSrc_p, auwSize );
			}
		}
		else
		{
			nvdGen_MemCpy1x( avdTgt_p, avdSrc_p, auwSize );
		}
	}
	else	/* No action required */
	{
		;
	}
}

/********************************************************************************************/
/**	\function		nvdGen_MemCpy1x
 *	\date			2010/10/13
 *	\author			Masato Minami (HAD)
 *	\description	Copy n Bytes of Data from Source to Target.
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
static void nvdGen_MemCpy1x( void *				avdTgt_p,
							 const void *		avdSrc_p,
							 const Type_uWord	auwSize )
{
	Type_uWord auwCounter;									/* <AUTO> Data Counter			*/
	Type_uByte *aubTgt_p;									/* <AUTO> Target Data Pointer	*/
	const Type_uByte *aubSrc_p;								/* <AUTO> Source Data Pointer	*/

	aubTgt_p = (Type_uByte *)(avdTgt_p);
	aubSrc_p = (const Type_uByte *)(avdSrc_p);

	for (auwCounter = 0; auwCounter < auwSize; auwCounter++)
	{
		*aubTgt_p = *aubSrc_p;
		aubTgt_p++;
		aubSrc_p++;
	}
}

/********************************************************************************************/
/**	\function		nvdGen_MemCpy2x
 *	\date			2010/10/13
 *	\author			Masato Minami (HAD)
 *	\description	memory copy (2byte alignment)
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdGen_MemCpy2x(	void *				avdTgt_p,
								const void *		avdSrc_p,
								const Type_uWord	auwSize )
{
	Type_uWord auwCountSize;
	Type_uWord auwSubCountSize;

	auwCountSize = (auwSize >> GEN_SHIFT_05BIT) & GEN_SHIFTMASK_04BIT;
	auwSubCountSize = ((auwSize & GEN_MASK_SUB_16BIT) >> GEN_SHIFT_01BIT) & GEN_SHIFTMASK_01BIT;

	if ((avdTgt_p != STD_NULL)
		&& (avdSrc_p != STD_NULL))
	{
		Type_uWord auwCounter;								/* <AUTO> Data Counter			*/
		Type_uHWord *auhTgt_p;								/* <AUTO> Target Data Pointer	*/
		Type_uHWord *auhSrc_p;								/* <AUTO> Source Data Pointer	*/

		auhTgt_p = (Type_uHWord *)(avdTgt_p);
		auhSrc_p = (Type_uHWord *)(avdSrc_p);

		for (auwCounter = 0; (auwCounter < auwCountSize); auwCounter++)
		{
			*auhTgt_p = *auhSrc_p;
			auhTgt_p++;
			auhSrc_p++;
			*auhTgt_p = *auhSrc_p;
			auhTgt_p++;
			auhSrc_p++;
			*auhTgt_p = *auhSrc_p;
			auhTgt_p++;
			auhSrc_p++;
			*auhTgt_p = *auhSrc_p;
			auhTgt_p++;
			auhSrc_p++;
			*auhTgt_p = *auhSrc_p;
			auhTgt_p++;
			auhSrc_p++;
			*auhTgt_p = *auhSrc_p;
			auhTgt_p++;
			auhSrc_p++;
			*auhTgt_p = *auhSrc_p;
			auhTgt_p++;
			auhSrc_p++;
			*auhTgt_p = *auhSrc_p;
			auhTgt_p++;
			auhSrc_p++;
			*auhTgt_p = *auhSrc_p;
			auhTgt_p++;
			auhSrc_p++;
			*auhTgt_p = *auhSrc_p;
			auhTgt_p++;
			auhSrc_p++;
			*auhTgt_p = *auhSrc_p;
			auhTgt_p++;
			auhSrc_p++;
			*auhTgt_p = *auhSrc_p;
			auhTgt_p++;
			auhSrc_p++;
			*auhTgt_p = *auhSrc_p;
			auhTgt_p++;
			auhSrc_p++;
			*auhTgt_p = *auhSrc_p;
			auhTgt_p++;
			auhSrc_p++;
			*auhTgt_p = *auhSrc_p;
			auhTgt_p++;
			auhSrc_p++;
			*auhTgt_p = *auhSrc_p;
			auhTgt_p++;
			auhSrc_p++;
		}

		for (auwCounter = 0; (auwCounter < auwSubCountSize); auwCounter++)
		{
			*auhTgt_p = *auhSrc_p;
			auhTgt_p++;
			auhSrc_p++;
		}
	}
	else	/* No action required */
	{
		;
	}
}

/********************************************************************************************/
/**	\function		nvdGen_MemCpy4x
 *	\date			2010/10/13
 *	\author			Masato Minami (HAD)
 *	\description	memory copy (4byte alignment)
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
static void nvdGen_MemCpy4x(	void *				avdTgt_p,
								const void *		avdSrc_p,
								const Type_uWord	auwSize )
{
	Type_uWord auwCountSize;
	Type_uWord auwSubCountSize;

	auwCountSize = (auwSize >> GEN_SHIFT_06BIT) & GEN_SHIFTMASK_06BIT;
	auwSubCountSize = ((auwSize & GEN_MASK_SUB_32BIT) >> GEN_SHIFT_02BIT) & GEN_SHIFTMASK_02BIT;

	if ((avdTgt_p != STD_NULL) && (avdSrc_p != STD_NULL))
	{
		Type_uWord auwCounter;								/* <AUTO> Data Counter			*/
		Type_uWord *auwTgt_p;								/* <AUTO> Target Data Pointer	*/
		Type_uWord *auwSrc_p;								/* <AUTO> Source Data Pointer	*/

		auwTgt_p = (Type_uWord *)(avdTgt_p);
		auwSrc_p = (Type_uWord *)(avdSrc_p);

		for (auwCounter = 0; (auwCounter < auwCountSize); auwCounter++)
		{
			*auwTgt_p = *auwSrc_p;
			auwTgt_p++;
			auwSrc_p++;
			*auwTgt_p = *auwSrc_p;
			auwTgt_p++;
			auwSrc_p++;
			*auwTgt_p = *auwSrc_p;
			auwTgt_p++;
			auwSrc_p++;
			*auwTgt_p = *auwSrc_p;
			auwTgt_p++;
			auwSrc_p++;
			*auwTgt_p = *auwSrc_p;
			auwTgt_p++;
			auwSrc_p++;
			*auwTgt_p = *auwSrc_p;
			auwTgt_p++;
			auwSrc_p++;
			*auwTgt_p = *auwSrc_p;
			auwTgt_p++;
			auwSrc_p++;
			*auwTgt_p = *auwSrc_p;
			auwTgt_p++;
			auwSrc_p++;
			*auwTgt_p = *auwSrc_p;
			auwTgt_p++;
			auwSrc_p++;
			*auwTgt_p = *auwSrc_p;
			auwTgt_p++;
			auwSrc_p++;
			*auwTgt_p = *auwSrc_p;
			auwTgt_p++;
			auwSrc_p++;
			*auwTgt_p = *auwSrc_p;
			auwTgt_p++;
			auwSrc_p++;
			*auwTgt_p = *auwSrc_p;
			auwTgt_p++;
			auwSrc_p++;
			*auwTgt_p = *auwSrc_p;
			auwTgt_p++;
			auwSrc_p++;
			*auwTgt_p = *auwSrc_p;
			auwTgt_p++;
			auwSrc_p++;
			*auwTgt_p = *auwSrc_p;
			auwTgt_p++;
			auwSrc_p++;
		}

		for (auwCounter = 0; (auwCounter < auwSubCountSize); auwCounter++)
		{
			*auwTgt_p = *auwSrc_p;
			auwTgt_p++;
			auwSrc_p++;
		}
	}
	else	/* No action required */
	{
		;
	}
}

/********************************************************************************************/
/**	\function		wvdGen_MemMove
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Move n Bytes of Data from Source to Target.
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
void wvdGen_MemMove( void *				avdTgt_p,
					 const void *		avdSrc_p,
					 const Type_uWord	auwSize )
{
	if ((avdTgt_p != STD_NULL) && (avdSrc_p != STD_NULL))
	{
		Type_uWord auwCounter;								/* <AUTO> Data Counter			*/
		Type_uByte *aubTgt_p;								/* <AUTO> Target Data Pointer	*/
		const Type_uByte *aubSrc_p;							/* <AUTO> Source Data Pointer	*/

		aubTgt_p = (Type_uByte *)(avdTgt_p);
		aubSrc_p = (const Type_uByte *)(avdSrc_p);

		if ((aubSrc_p < aubTgt_p) && (aubTgt_p < (&aubSrc_p[auwSize])))
		{
			/* Copy Data - backward */
			for (auwCounter = auwSize; auwCounter > 0; auwCounter--)
			{
				aubTgt_p[(auwCounter - 1)] = aubSrc_p[(auwCounter - 1)];
			}
		}
		else
		{
			/* Copy Data - forward */
			for (auwCounter = 0; auwCounter < auwSize; auwCounter++)
			{
				aubTgt_p[auwCounter] = aubSrc_p[auwCounter];
			}
		}
	}
	else	/* No action required */
	{
		;
	}
}

/********************************************************************************************/
/**	\function		wswGen_MemCmp
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Compare n Bytes of Data between 2 Sources.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		avdSrc1_p - Source 1 Data Pointer.
 *	\param[in]		avdSrc2_p - Source 2 Data Pointer.
 *	\param[in]		auwSize - Data Size.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			aswCmpResult - Compare Result.
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
Type_sWord wswGen_MemCmp( const void *		avdSrc1_p,
						  const void *		avdSrc2_p,
						  const Type_uWord	auwSize )
{
	Type_sWord aswCmpResult;								/* <AUTO> Compare Result		*/


	if ((avdSrc1_p != STD_NULL) && (avdSrc2_p != STD_NULL))
	{
		Type_uWord auwCounter;								/* <AUTO> Data Counter			*/
		const Type_uByte *aubSrc1_p;						/* <AUTO> Source 1 Data Pointer	*/
		const Type_uByte *aubSrc2_p;						/* <AUTO> Source 2 Data Pointer	*/

		aswCmpResult = GEN_CMP_EQUAL;
		aubSrc1_p = (const Type_uByte *)(avdSrc1_p);
		aubSrc2_p = (const Type_uByte *)(avdSrc2_p);

		for (auwCounter = 0; (auwCounter < auwSize) && (aswCmpResult == GEN_CMP_EQUAL); auwCounter++)
		{
			if (aubSrc1_p[auwCounter] != aubSrc2_p[auwCounter])
			{
				aswCmpResult = ((Type_sWord)aubSrc1_p[auwCounter] - (Type_sWord)aubSrc2_p[auwCounter]);
			}
			else	/* No action required */
			{
				;
			}
		}
	}
	else
	{
		aswCmpResult = GEN_CMP_INVALID;
	}

	return aswCmpResult;
}

/* End Of File */
