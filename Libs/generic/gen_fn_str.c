/********************************************************************************************/
/** <!-- Copyright (C) Alpine Electronics, Inc. All rights reserved. ------------------------>
 *
 *	\file			gen_fn_str.c
 *	\date			2010/02/10
 *	\author			Daniel Morgan (ALPINE)
 *	\model			ERATO-AV
 *	\description	OS Framework - String Manipulation Functions.
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
#define WORD_DATA_MAX				0xFFFFFFFFU			/* Word Data Maximum Value			*/

#define GEN_NULL_C					0					/* NULL Character					*/
#define GEN_NULL_N					0					/* Numerical NULL					*/
#define LOWER_LETTER_MIN			0x61				/* Lowercase Letter min. 'a'		*/
#define LOWER_LETTER_MAX			0x7A				/* Lowercase Numeric max. 'z'		*/
#define UPPER_LETTER_MIN			0x41				/* Uppercase Numeric min. 'A'		*/
#define UPPER_LETTER_MAX			0x5A				/* Uppercase Numeric max. 'Z'		*/

/* Difference between Lower Case and Upper Case */
#define DIFF_LETTER_CASE			(LOWER_LETTER_MIN - UPPER_LETTER_MIN)

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
/**	\function		wvdGen_StrCpy
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Copy n Characters from Source to Target.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[out]		avdTgt_p - Target String Pointer.
 *	\param[in]		avdSrc_p - Source String Pointer.
 *	\param[in]		auwSize - String Size.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdGen_StrCpy( void				*avdTgt_p,
					const void			*avdSrc_p,
					const Type_uWord	auwSize )
{
	if ((avdTgt_p != STD_NULL) && (avdSrc_p != STD_NULL))
	{
		Type_uWord auwCounter;								/* <AUTO> Data Counter			*/
		Type_uWord auwCpySize;								/* <AUTO> Copy Size				*/
		Type_uByte *aubTgt_p;								/* <AUTO> Target String Pointer	*/
		const Type_uByte *aubSrc_p;							/* <AUTO> Source String Pointer	*/

		aubTgt_p = (Type_uByte *)(avdTgt_p);
		aubSrc_p = (const Type_uByte *)(avdSrc_p);

		if (auwSize == GEN_NULL_N)
		{
			auwCpySize = WORD_DATA_MAX;
		}
		else
		{
			auwCpySize = auwSize;
		}

		for (auwCounter = 0; ((auwCounter < auwCpySize) && (aubSrc_p[auwCounter] != GEN_NULL_C)); auwCounter++)
		{
			aubTgt_p[auwCounter] = aubSrc_p[auwCounter];
		}

		/* Fill remaining characters with GEN_NULL_C if copy size does not equal GEN_NULL_N */
		if (auwSize != GEN_NULL_N)
		{
			while (auwCounter < auwCpySize)
			{
				aubTgt_p[auwCounter] = GEN_NULL_C;
				auwCounter++;
			}
		}
		else	/* Set last character to GEN_NULL_C */
		{
			aubTgt_p[auwCounter] = GEN_NULL_C;
		}
	}
	else	/* No action required */
	{
		;
	}
}

/********************************************************************************************/
/**	\function		wvdGen_WCStrCpy
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Copy n Characters from Source to Target Wide Character Strings.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[out]		avdTgt_p - Target String Pointer.
 *	\param[in]		avdSrc_p - Source String Pointer.
 *	\param[in]		auwSize - String Size.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdGen_WCStrCpy( void				*avdTgt_p,
					  const void		*avdSrc_p,
					  const Type_uWord	auwSize )
{
	if ((avdTgt_p != STD_NULL) && (avdSrc_p != STD_NULL))
	{
		Type_uWord auwCounter;								/* <AUTO> Data Counter			*/
		Type_uWord auwCpySize;								/* <AUTO> Copy Size				*/
		Type_uHWord *auhTgt_p;								/* <AUTO> Target String Pointer	*/
		const Type_uHWord *auhSrc_p;						/* <AUTO> Source String Pointer	*/

		auhTgt_p = (Type_uHWord *)(avdTgt_p);
		auhSrc_p = (const Type_uHWord *)(avdSrc_p);

		if (auwSize == GEN_NULL_N)
		{
			auwCpySize = WORD_DATA_MAX;
		}
		else
		{
			auwCpySize = auwSize;
		}

		for (auwCounter = 0; ((auwCounter < auwCpySize) && (auhSrc_p[auwCounter] != GEN_NULL_C)); auwCounter++)
		{
			auhTgt_p[auwCounter] = auhSrc_p[auwCounter];
		}

		/* Fill remaining characters with GEN_NULL_C if copy size equals GEN_NULL_N */
		if (auwSize != GEN_NULL_N)
		{
			while (auwCounter < auwCpySize)
			{
				auhTgt_p[auwCounter] = GEN_NULL_C;
				auwCounter++;
			}
		}
		else	/* Set last character to GEN_NULL_C */
		{
			auhTgt_p[auwCounter] = GEN_NULL_C;
		}
	}
	else	/* No action required */
	{
		;
	}
}

/********************************************************************************************/
/**	\function		wvdGen_StrCpyCharToWChar
 *	\date			2010/02/19
 *	\author			Daniel Morgan
 *	\description	Copy n Characters from Source (Char) to Target (Wide Char) Strings.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[out]		avdTgt_p - Target String Pointer (Wide Character).
 *	\param[in]		avdSrc_p - Source String Pointer.
 *	\param[in]		auwSize - String Size.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdGen_StrCpyCharToWChar( void					*avdTgt_p,
							   const void			*avdSrc_p,
							   const Type_uWord		auwSize )
{
	if ((avdTgt_p != STD_NULL) && (avdSrc_p != STD_NULL))
	{
		Type_uWord auwCounter;								/* <AUTO> Data Counter			*/
		Type_uWord auwCpySize;								/* <AUTO> Copy Size				*/
		Type_uHWord *auhTgt_p;								/* <AUTO> Target String Pointer	*/
		const Type_uByte *aubSrc_p;							/* <AUTO> Source String Pointer	*/

		auhTgt_p = (Type_uHWord *)(avdTgt_p);
		aubSrc_p = (const Type_uByte *)(avdSrc_p);

		if (auwSize == GEN_NULL_N)
		{
			auwCpySize = WORD_DATA_MAX;
		}
		else
		{
			auwCpySize = auwSize;
		}

		for (auwCounter = 0; ((auwCounter < auwCpySize) && (aubSrc_p[auwCounter] != GEN_NULL_C)); auwCounter++)
		{
			auhTgt_p[auwCounter] = aubSrc_p[auwCounter];
		}

		/* Fill remaining characters with GEN_NULL_C if copy size equals GEN_NULL_N */
		if (auwSize != GEN_NULL_N)
		{
			while (auwCounter < auwCpySize)
			{
				auhTgt_p[auwCounter] = GEN_NULL_C;
				auwCounter++;
			}
		}
		else	/* Set last character to GEN_NULL_C */
		{
			auhTgt_p[auwCounter] = GEN_NULL_C;
		}
	}
	else	/* No action needed */
	{
		;
	}
}

/********************************************************************************************/
/**	\function		wvdGen_StrCpyWCharToChar
 *	\date			2010/02/19
 *	\author			Daniel Morgan
 *	\description	Copy n Characters from Source (Wide Char) to Target (Char) Strings.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[out]		avdTgt_p - Target String Pointer.
 *	\param[in]		avdSrc_p - Source String Pointer (Wide Character).
 *	\param[in]		auwSize - String Size.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdGen_StrCpyWCharToChar( void					*avdTgt_p,
							   const void			*avdSrc_p,
							   const Type_uWord		auwSize )
{
	if ((avdTgt_p != STD_NULL) && (avdSrc_p != STD_NULL))
	{
		Type_uWord auwCounter;								/* <AUTO> Data Counter			*/
		Type_uWord auwCpySize;								/* <AUTO> Copy Size				*/
		Type_uByte *aubTgt_p;								/* <AUTO> Target String Pointer	*/
		const Type_uHWord *auhSrc_p;						/* <AUTO> Source String Pointer	*/

		aubTgt_p = (Type_uByte *)(avdTgt_p);
		auhSrc_p = (const Type_uHWord *)(avdSrc_p);

		if (auwSize == GEN_NULL_N)
		{
			auwCpySize = WORD_DATA_MAX;
		}
		else
		{
			auwCpySize = auwSize;
		}

		for (auwCounter = 0; ((auwCounter < auwCpySize) && (auhSrc_p[auwCounter] != GEN_NULL_C)); auwCounter++)
		{
			aubTgt_p[auwCounter] = (Type_uByte)(auhSrc_p[auwCounter]);
		}

		/* Fill remaining characters with GEN_NULL_C if copy size equals GEN_NULL_N */
		if (auwSize != GEN_NULL_N)
		{
			while (auwCounter < auwCpySize)
			{
				aubTgt_p[auwCounter] = GEN_NULL_C;
				auwCounter++;
			}
		}
		else	/* Set last character to GEN_NULL_C */
		{
			aubTgt_p[auwCounter] = GEN_NULL_C;
		}
	}
	else	/* No action needed */
	{
		;
	}
}

/********************************************************************************************/
/**	\function		wuwGen_StrLen
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Return String Length.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		avdSrc_p - Source String Pointer.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			auwStrLen - String Length.
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
Type_uWord wuwGen_StrLen( const void *avdSrc_p )
{
	Type_uWord auwStrLen;									/* <AUTO> String Length			*/


	auwStrLen = 0;

	if (avdSrc_p != STD_NULL)
	{
		const Type_uByte *aubSrc_p;							/* <AUTO> Source String Pointer	*/

		aubSrc_p = (const Type_uByte *)(avdSrc_p);

		while ((auwStrLen < WORD_DATA_MAX) && (aubSrc_p[auwStrLen] != GEN_NULL_C))
		{
			auwStrLen++;
		}
	}
	else	/* No action required */
	{
		;
	}

	return auwStrLen;
}

/********************************************************************************************/
/**	\function		wuwGen_WCStrLen
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Return Wide Character String Length.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		avdSrc_p - Source String Pointer.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			auwStrLen - String Length.
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
Type_uWord wuwGen_WCStrLen( const void *avdSrc_p )
{
	Type_uWord auwStrLen;									/* <AUTO> String Length			*/


	auwStrLen = 0;

	if (avdSrc_p != STD_NULL)
	{
		const Type_uHWord *auhSrc_p;						/* <AUTO> Source String Pointer	*/

		auhSrc_p = (const Type_uHWord *)(avdSrc_p);

		while ((auwStrLen < WORD_DATA_MAX) && (auhSrc_p[auwStrLen] != GEN_NULL_C))
		{
			auwStrLen++;
		}
	}
	else	/* No action required */
	{
		;
	}

	return auwStrLen;
}

/********************************************************************************************/
/**	\function		wswGen_StrCmp
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Compare n Characters between 2 Sources.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		avdSrc1_p - Source 1 String Pointer.
 *	\param[in]		avdSrc2_p - Source 2 String Pointer.
 *	\param[in]		auwSize - String Size.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			aswCmpResult - Compare Result.
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
Type_sWord wswGen_StrCmp( const void		*avdSrc1_p,
						  const void		*avdSrc2_p,
						  const Type_uWord	auwSize )
{
	Type_sWord aswCmpResult;								/* <AUTO> Compare Result		*/


	if ((avdSrc1_p != STD_NULL) && (avdSrc2_p != STD_NULL))
	{
		Type_uWord auwCounter;								/* <AUTO> Data Counter			*/
		Type_uWord auwCmpSize;								/* <AUTO> Compare Size			*/
		const Type_uByte *aubSrc1_p;						/* <AUTO> Source 1 Str Pointer	*/
		const Type_uByte *aubSrc2_p;						/* <AUTO> Source 2 Str Pointer	*/

		aswCmpResult = GEN_CMP_EQUAL;
		aubSrc1_p = (const Type_uByte *)(avdSrc1_p);
		aubSrc2_p = (const Type_uByte *)(avdSrc2_p);

		if (auwSize == GEN_NULL_N)
		{
			auwCmpSize = WORD_DATA_MAX;
		}
		else
		{
			auwCmpSize = auwSize;
		}

		for (auwCounter = 0; ((auwCounter < auwCmpSize) && (aswCmpResult == GEN_CMP_EQUAL) &&
				((aubSrc1_p[auwCounter] != GEN_NULL_C) || (aubSrc2_p[auwCounter] != GEN_NULL_C))); auwCounter++)
		{
			if (aubSrc1_p[auwCounter] != aubSrc2_p[auwCounter])
			{
				aswCmpResult = ((Type_sWord)(aubSrc1_p[auwCounter]) - (Type_sWord)(aubSrc2_p[auwCounter]));
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

/********************************************************************************************/
/**	\function		wswGen_WCStrCmp
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Compare n Characters between 2 Wide Character String Sources.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		avdSrc1_p - Source 1 String Pointer.
 *	\param[in]		avdSrc2_p - Source 2 String Pointer.
 *	\param[in]		auwSize - String Size.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			aswCmpResult - Compare Result.
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
Type_sWord wswGen_WCStrCmp( const void *		avdSrc1_p,
							const void *		avdSrc2_p,
							const Type_uWord	auwSize )
{
	Type_sWord aswCmpResult;								/* <AUTO> Compare Result		*/


	if ((avdSrc1_p != STD_NULL) && (avdSrc2_p != STD_NULL))
	{
		Type_uWord auwCounter;								/* <AUTO> Data Counter			*/
		Type_uWord auwCmpSize;								/* <AUTO> Compare Size			*/
		const Type_uHWord *auhSrc1_p;						/* <AUTO> Source 1 Str Pointer	*/
		const Type_uHWord *auhSrc2_p;						/* <AUTO> Source 2 Str Pointer	*/

		aswCmpResult = GEN_CMP_EQUAL;
		auhSrc1_p = (const Type_uHWord *)(avdSrc1_p);
		auhSrc2_p = (const Type_uHWord *)(avdSrc2_p);

		if (auwSize == GEN_NULL_N)
		{
			auwCmpSize = WORD_DATA_MAX;
		}
		else
		{
			auwCmpSize = auwSize;
		}

		for (auwCounter = 0; ((auwCounter < auwCmpSize) && (aswCmpResult == GEN_CMP_EQUAL) &&
				((auhSrc1_p[auwCounter] != GEN_NULL_C) || (auhSrc2_p[auwCounter] != GEN_NULL_C))); auwCounter++)
		{
			if (auhSrc1_p[auwCounter] != auhSrc2_p[auwCounter])
			{
				aswCmpResult = ((Type_sWord)(auhSrc1_p[auwCounter]) - (Type_sWord)(auhSrc2_p[auwCounter]));
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

/********************************************************************************************/
/**	\function		wswGen_StrICmp
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Compare n Characters between 2 Sources (Case Insensitive).
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		avdSrc1_p - Source 1 String Pointer.
 *	\param[in]		avdSrc2_p - Source 2 String Pointer.
 *	\param[in]		auwSize - String Size.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			aswCmpResult - Compare Result.
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
Type_sWord wswGen_StrICmp( const void			*avdSrc1_p,
						   const void			*avdSrc2_p,
						   const Type_uWord		auwSize )
{
	Type_sWord aswCmpResult;								/* <AUTO> Compare Result		*/


	if ((avdSrc1_p != STD_NULL) && (avdSrc2_p != STD_NULL))
	{
		Type_uWord auwCounter;								/* <AUTO> Data Counter			*/
		Type_uWord auwCmpSize;								/* <AUTO> Compare Size			*/
		Type_uByte aubChar1;								/* <AUTO> Source 1 Character	*/
		Type_uByte aubChar2;								/* <AUTO> Source 2 Character	*/
		const Type_uByte *aubSrc1_p;						/* <AUTO> Source 1 Str Pointer	*/
		const Type_uByte *aubSrc2_p;						/* <AUTO> Source 2 Str Pointer	*/

		aswCmpResult = GEN_CMP_EQUAL;
		aubSrc1_p = (const Type_uByte *)(avdSrc1_p);
		aubSrc2_p = (const Type_uByte *)(avdSrc2_p);

		if (auwSize == GEN_NULL_N)
		{
			auwCmpSize = WORD_DATA_MAX;
		}
		else
		{
			auwCmpSize = auwSize;
		}

		for (auwCounter = 0; ((auwCounter < auwCmpSize) && (aswCmpResult == GEN_CMP_EQUAL) &&
				((aubSrc1_p[auwCounter] != GEN_NULL_C) || (aubSrc2_p[auwCounter] != GEN_NULL_C))); auwCounter++)
		{
			aubChar1 = aubSrc1_p[auwCounter];
			aubChar2 = aubSrc2_p[auwCounter];

			if ((aubChar1 >= UPPER_LETTER_MIN) && (aubChar1 <= UPPER_LETTER_MAX))
			{
				aubChar1 = (Type_uByte)(aubChar1 + DIFF_LETTER_CASE);
			}
			else	/* No action required */
			{
				;
			}

			if ((aubChar2 >= UPPER_LETTER_MIN) && (aubChar2 <= UPPER_LETTER_MAX))
			{
				aubChar2 = (Type_uByte)(aubChar2 + DIFF_LETTER_CASE);
			}
			else	/* No action required */
			{
				;
			}

			if (aubChar1 != aubChar2)
			{
				aswCmpResult = ((Type_sWord)(aubChar1) - (Type_sWord)(aubChar2));
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

/********************************************************************************************/
/**	\function		wswGen_WCStrICmp
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Compare n Characters between 2 Wide Character String Sources (Case Insensitive).
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		avdSrc1_p - Source 1 String Pointer.
 *	\param[in]		avdSrc2_p - Source 2 String Pointer.
 *	\param[in]		auwSize - String Size.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			aswCmpResult - Compare Result.
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
Type_sWord wswGen_WCStrICmp( const void			*avdSrc1_p,
							 const void			*avdSrc2_p,
							 const Type_uWord	auwSize )
{
	Type_sWord aswCmpResult;								/* <AUTO> Compare Result		*/


	if ((avdSrc1_p != STD_NULL) && (avdSrc2_p != STD_NULL))
	{
		Type_uWord auwCounter;								/* <AUTO> Data Counter			*/
		Type_uWord auwCmpSize;								/* <AUTO> Compare Size			*/
		Type_uHWord auhChar1;								/* <AUTO> Source 1 Character	*/
		Type_uHWord auhChar2;								/* <AUTO> Source 2 Character	*/
		const Type_uHWord *auhSrc1_p;						/* <AUTO> Source 1 Str Pointer	*/
		const Type_uHWord *auhSrc2_p;						/* <AUTO> Source 2 Str Pointer	*/

		aswCmpResult = GEN_CMP_EQUAL;
		auhSrc1_p = (const Type_uHWord *)(avdSrc1_p);
		auhSrc2_p = (const Type_uHWord *)(avdSrc2_p);

		if (auwSize == GEN_NULL_N)
		{
			auwCmpSize = WORD_DATA_MAX;
		}
		else
		{
			auwCmpSize = auwSize;
		}

		for (auwCounter = 0; ((auwCounter < auwCmpSize) && (aswCmpResult == GEN_CMP_EQUAL) &&
				((auhSrc1_p[auwCounter] != GEN_NULL_C) || (auhSrc2_p[auwCounter] != GEN_NULL_C))); auwCounter++)
		{
			auhChar1 = auhSrc1_p[auwCounter];
			auhChar2 = auhSrc2_p[auwCounter];

			if ((auhChar1 >= UPPER_LETTER_MIN) && (auhChar1 <= UPPER_LETTER_MAX))
			{
				auhChar1 = (Type_uHWord)(auhChar1 + DIFF_LETTER_CASE);
			}
			else	/* No action required */
			{
				;
			}

			if ((auhChar2 >= UPPER_LETTER_MIN) && (auhChar2 <= UPPER_LETTER_MAX))
			{
				auhChar2 = (Type_uHWord)(auhChar2 + DIFF_LETTER_CASE);
			}
			else	/* No action required */
			{
				;
			}

			if (auhChar1 != auhChar2)
			{
				aswCmpResult = ((Type_sWord)(auhChar1) - (Type_sWord)(auhChar2));
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

/********************************************************************************************/
/**	\function		wswGen_StrCmpCharToWChar
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Compare n Characters between Char and Wide Char String Sources.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[in]		avdSrc1_p - Source 1 String Pointer (Char).
 *	\param[in]		avdSrc2_p - Source 2 String Pointer (Wide Char).
 *	\param[in]		auwSize - String Size.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			aswCmpResult - Compare Result.
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
Type_sWord wswGen_StrCmpCharToWChar( const void *		avdSrc1_p,
									 const void *		avdSrc2_p,
									 const Type_uWord	auwSize )
{
	Type_sWord aswCmpResult;								/* <AUTO> Compare Result		*/


	if ((avdSrc1_p != STD_NULL) && (avdSrc2_p != STD_NULL))
	{
		Type_uWord auwCounter;								/* <AUTO> Data Counter			*/
		Type_uWord auwCmpSize;								/* <AUTO> Compare Size			*/
		const Type_uByte *aubSrc1_p;						/* <AUTO> Source 1 Str Pointer	*/
		const Type_uHWord *auhSrc2_p;						/* <AUTO> Source 2 Str Pointer	*/

		aswCmpResult = GEN_CMP_EQUAL;
		aubSrc1_p = (const Type_uByte *)(avdSrc1_p);
		auhSrc2_p = (const Type_uHWord *)(avdSrc2_p);

		if (auwSize == GEN_NULL_N)
		{
			auwCmpSize = WORD_DATA_MAX;
		}
		else
		{
			auwCmpSize = auwSize;
		}

		for (auwCounter = 0; ((auwCounter < auwCmpSize) && (aswCmpResult == GEN_CMP_EQUAL) &&
				((aubSrc1_p[auwCounter] != GEN_NULL_C) || (auhSrc2_p[auwCounter] != GEN_NULL_C))); auwCounter++)
		{
			if (aubSrc1_p[auwCounter] != (Type_uByte)(auhSrc2_p[auwCounter]))
			{
				aswCmpResult = ((Type_sWord)(aubSrc1_p[auwCounter]) - (Type_sWord)(auhSrc2_p[auwCounter]));
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

/********************************************************************************************/
/**	\function		wvdGen_StrCat
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Concatenate n Characters from Source to Target.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[out]		avdTgt_p - Target String Pointer.
 *	\param[in]		avdSrc_p - Source String Pointer.
 *	\param[in]		auwSize - String Size.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			aswCmpResult - Compare Result.
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdGen_StrCat( void				*avdTgt_p,
					const void			*avdSrc_p,
					const Type_uWord	auwSize )
{
	if ((avdTgt_p != STD_NULL) && (avdSrc_p != STD_NULL))
	{
		Type_uWord auwCatSize;								/* <AUTO> Concatenate Size		*/
		Type_uWord auwTgtStrLen;							/* <AUTO> Target String Length	*/
		Type_uWord auwSrcStrLen;							/* <AUTO> Source String Length	*/
		Type_uByte *aubTgt_p;								/* <AUTO> Target String Pointer	*/
		const Type_uByte *aubSrc_p;							/* <AUTO> Source String Pointer	*/

		aubTgt_p = (Type_uByte *)(avdTgt_p);
		aubSrc_p = (const Type_uByte *)(avdSrc_p);
		auwTgtStrLen = wuwGen_StrLen( aubTgt_p );
		auwSrcStrLen = wuwGen_StrLen( aubSrc_p );

		if ((auwSrcStrLen < auwSize) || (auwSize == GEN_NULL_N))
		{
			auwCatSize = auwSrcStrLen;
		}
		else
		{
			auwCatSize = auwSize;
		}

		wvdGen_StrCpy( &aubTgt_p[auwTgtStrLen], aubSrc_p, auwCatSize );

		/* Set last character to GEN_NULL_C */
		aubTgt_p[auwTgtStrLen + auwCatSize] = GEN_NULL_C;
	}
	else	/* No action required */
	{
		;
	}
}

/********************************************************************************************/
/**	\function		wvdGen_WCStrCat
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Concatenate n Characters from Source to Target Wide Character Strings.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[out]		avdTgt_p - Target String Pointer.
 *	\param[in]		avdSrc_p - Source String Pointer.
 *	\param[in]		auwSize - String Size.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			aswCmpResult - Compare Result.
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdGen_WCStrCat( void				*avdTgt_p,
					  const void		*avdSrc_p,
					  const Type_uWord	auwSize )
{
	if ((avdTgt_p != STD_NULL) && (avdSrc_p != STD_NULL))
	{
		Type_uWord auwCatSize;								/* <AUTO> Concatenate Size		*/
		Type_uWord auwTgtStrLen;							/* <AUTO> Target String Length	*/
		Type_uWord auwSrcStrLen;							/* <AUTO> Source String Length	*/
		Type_uHWord *auhTgt_p;								/* <AUTO> Target String Pointer	*/
		const Type_uHWord *auhSrc_p;						/* <AUTO> Source String Pointer	*/

		auhTgt_p = (Type_uHWord *)(avdTgt_p);
		auhSrc_p = (const Type_uHWord *)(avdSrc_p);
		auwTgtStrLen = wuwGen_WCStrLen( auhTgt_p );
		auwSrcStrLen = wuwGen_WCStrLen( auhSrc_p );

		if ((auwSrcStrLen < auwSize) || (auwSize == GEN_NULL_N))
		{
			auwCatSize = auwSrcStrLen;
		}
		else
		{
			auwCatSize = auwSize;
		}

		wvdGen_WCStrCpy( &auhTgt_p[auwTgtStrLen], auhSrc_p, auwCatSize );

		/* Set last character to GEN_NULL_C */
		auhTgt_p[auwTgtStrLen + auwCatSize] = GEN_NULL_C;
	}
	else	/* No action required */
	{
		;
	}
}

/********************************************************************************************/
/**	\function		wvdGen_StrLwr
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Change n Upper Case Characters to Lower Case.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[out]		avdTgt_p - Target String Pointer.
 *	\param[in]		avdSrc_p - Source String Pointer.
 *	\param[in]		auwSize - String Size.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
void wvdGen_StrLwr( void				*avdTgt_p,
					const void			*avdSrc_p,
					const Type_uWord	auwSize )
{
	if ((avdTgt_p != STD_NULL) && (avdSrc_p != STD_NULL))
	{
		Type_uWord auwCounter;								/* <AUTO> Data Counter			*/
		Type_uByte *aubTgt_p;								/* <AUTO> Target String Pointer	*/
		const Type_uByte *aubSrc_p;							/* <AUTO> Source String Pointer	*/

		aubTgt_p = (Type_uByte *)(avdTgt_p);
		aubSrc_p = (const Type_uByte *)(avdSrc_p);

		for (auwCounter = 0; (auwCounter < auwSize); auwCounter++)
		{
			if ((aubSrc_p[auwCounter] >= UPPER_LETTER_MIN) && (aubSrc_p[auwCounter] <= UPPER_LETTER_MAX))
			{
				aubTgt_p[auwCounter] = (aubSrc_p[auwCounter] + DIFF_LETTER_CASE);
			}
			else
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
/**	\function		wvdGen_StrUpr
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Change n Lower Case Characters to Upper Case.
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
void wvdGen_StrUpr( void				*avdTgt_p,
					const void			*avdSrc_p,
					const Type_uWord	auwSize )
{
	if ((avdTgt_p != STD_NULL) && (avdSrc_p != STD_NULL))
	{
		Type_uWord auwCounter;								/* <AUTO> Data Counter			*/
		Type_uByte *aubTgt_p;								/* <AUTO> Target String Pointer	*/
		const Type_uByte *aubSrc_p;							/* <AUTO> Source String Pointer	*/

		aubTgt_p = (Type_uByte *)(avdTgt_p);
		aubSrc_p = (const Type_uByte *)(avdSrc_p);

		for (auwCounter = 0; (auwCounter < auwSize); auwCounter++)
		{
			if ((aubSrc_p[auwCounter] >= LOWER_LETTER_MIN) && (aubSrc_p[auwCounter] <= LOWER_LETTER_MAX))
			{
				aubTgt_p[auwCounter] = (aubSrc_p[auwCounter] - DIFF_LETTER_CASE);
			}
			else
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

Type_sWord CSL_StringToHex(const Type_sByte *pucData)
{
    Type_sWord hex = -1;
    Type_uWord count = 0UL;
    Type_uWord i;

    if (pucData != NULL_PTR)
    {
    	count = wuwGen_StrLen(pucData);

        for (i = 0UL ; i < count; i++)
        {
            if((pucData[i] >= '0') && (pucData[i] <= '9'))
            {
                hex = (((hex * 16) + pucData[i]) - '0');
            }
            else if ((pucData[i] >= 'A') && (pucData[i] <= 'F'))
            {
                hex = ((((hex * 16) + *pucData) - 'A') + 10);
            }
            else if ((pucData[i] >= 'a') && (pucData[i] <= 'f'))
            {
                hex = ((((hex * 16) + pucData[i]) - 'a') + 10);
            }
            else
            {
            	;
            }

        }
    }

    return hex;
}

Type_uWord CSL_StringToDec(const Type_uByte *pucData)
{
    Type_uWord hex = 0;
    Type_uWord count = 0UL;
    Type_uWord i;

    if (pucData != NULL_PTR)
    {
    	count = wuwGen_StrLen(pucData);

        for (i = 0UL ; i < count; i++)
        {
            if((pucData[i] >= '0') && (pucData[i] <= '9'))
            {
                hex = hex*10 + (pucData[i] - '0');
            }
            else
            {
            	;
            }

        }
    }

    return hex;
}


/* End Of File */
