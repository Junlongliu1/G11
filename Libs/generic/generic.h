/********************************************************************************************/
/** <!-- Copyright (C) Alpine Electronics, Inc. All rights reserved. ------------------------>
 *
 *	\file			generic.h
 *	\date			2010/02/10
 *	\author			Daniel Morgan (ALPINE)
 *	\model			ERATO-AV
 *	\description	Generic Library - Header File for External Components.
 *	\version
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/

#ifndef GENERIC_H
#define GENERIC_H

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************************/
/*							Include File Section											*/
/********************************************************************************************/
/* External Header Files */
#include "apn_basic_type.h"

/********************************************************************************************/
/*							Macro Definition Section										*/
/********************************************************************************************/
#define GEN_CMP_EQUAL				0						/*!< Compare Result is Equal	*/
#define GEN_CMP_INVALID				(-1)					/*!< Invalid Compare			*/


#ifndef NULL_PTR
#define NULL_PTR ((void *) 0)
#endif

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
/* Set n Bytes of Data */
extern void wvdGen_MemSet( void *,
						   const Type_uByte,
						   const Type_uWord );

/* Copy n Bytes of Data */
extern void wvdGen_MemCpy( void *,
						   const void *,
						   const Type_uWord );

/* Move n Bytes of Data */
extern void wvdGen_MemMove( void *,
							const void *,
							const Type_uWord );

/* Compare n Bytes of Data */
extern Type_sWord wswGen_MemCmp( const void *,
								 const void *,
								 const Type_uWord );

/* Copy n Characters */
extern void wvdGen_StrCpy( void *,
						   const void *,
						   const Type_uWord );

/* Copy n Characters of a Wide Character String */
extern void wvdGen_WCStrCpy( void *,
							 const void *,
							 const Type_uWord );

/* Copy n Characters of a Char to Wide Char String */
extern void wvdGen_StrCpyCharToWChar( void *,
									  const void *,
									  const Type_uWord );

/* Copy n Characters of a Wide Char to Char String */
extern void wvdGen_StrCpyWCharToChar( void *,
									  const void *,
									  const Type_uWord );

/* Return String Length */
extern Type_uWord wuwGen_StrLen( const void * );

/* Return String Lenght of a Wide Character String */
extern Type_uWord wuwGen_WCStrLen( const void * );

/* Compare n Characters */
extern Type_sWord wswGen_StrCmp( const void *,
								 const void *,
								 const Type_uWord );

/* Compare n Characters of a Wide Character String */
extern Type_sWord wswGen_WCStrCmp( const void *,
								   const void *,
								   const Type_uWord );

/* Compare n Characters (Case Insensitive) */
extern Type_sWord wswGen_StrICmp( const void *,
								  const void *,
								  const Type_uWord );

/* Compare n Characters of a Wide Character String (Case Insensitive) */
extern Type_sWord wswGen_WCStrICmp( const void *,
									const void *,
									const Type_uWord );

/* Compare n Characters of a Char to Wide Char String */
extern Type_sWord wswGen_StrCmpCharToWChar( const void *,
											const void *,
											const Type_uWord );

/* Concatenates Strings */
extern void wvdGen_StrCat( void *,
						   const void *,
						   const Type_uWord );

/* Concatenates Wide Character Strings */
extern void wvdGen_WCStrCat( void *,
							 const void *,
							 const Type_uWord );

/* Change n Upper Case Characters to Lower Case */
extern void wvdGen_StrLwr( void *,
						   const void *,
						   const Type_uWord );

/* Change n Lower Case Characters to Upper Case */
extern void wvdGen_StrUpr( void *,
						   const void *,
						   const Type_uWord );

/* Change Endian Half Word */
extern Type_uHWord wuhGen_ChgEndianUHWord( const Type_uHWord );

/* Change Endian 16 Bit Array */
extern void wvdGen_ChgEndian16( void *,
								const void *,
								const Type_uWord );

/* Change Endian Word */
extern Type_uWord wuwGen_ChgEndianUWord( const Type_uWord );

/* Change Endian 32 Bit Array */
extern void wvdGen_ChgEndian32( void *,
								const void *,
								const Type_uWord );

/* Reorder Bits in a Single Byte */
extern void wvdGen_ReorderBit8( Type_uByte * );

/* Calculate CRC-16-CCITT of Input Data */
extern Type_uHWord wuhGen_CalcCrc16( const void *,
									 const Type_uWord );

extern Type_sWord CSL_StringToHex(const Type_sByte *pucData);
extern	Type_uWord CSL_StringToDec(const Type_uByte *pucData);


#ifdef __cplusplus
}
#endif

#endif /* GENERIC_H */

/* End of File */
