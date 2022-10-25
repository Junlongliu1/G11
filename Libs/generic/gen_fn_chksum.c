/********************************************************************************************/
/** <!-- Copyright (C) Alpine Electronics, Inc. All rights reserved. ------------------------>
 *
 *	\file			gen_fn_chksum.c
 *	\date			2010/02/10
 *	\author			Daniel Morgan (ALPINE)
 *	\model			ERATO-AV
 *	\description	Generic Library - Checksum Calculation Functions.
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
#define CRC_DATA_INIT_VAL			0xFFFFU				/* CRC Data Initialization			*/

/* CRC-16-CCITT Precalculated Modifier Data Macros */
#define CRC16_PRE_DATA001			0x0000U
#define CRC16_PRE_DATA002			0x1021U
#define CRC16_PRE_DATA003			0x2042U
#define CRC16_PRE_DATA004			0x3063U
#define CRC16_PRE_DATA005			0x4084U
#define CRC16_PRE_DATA006			0x50A5U
#define CRC16_PRE_DATA007			0x60C6U
#define CRC16_PRE_DATA008			0x70E7U
#define CRC16_PRE_DATA009			0x8108U
#define CRC16_PRE_DATA010			0x9129U
#define CRC16_PRE_DATA011			0xA14AU
#define CRC16_PRE_DATA012			0xB16BU
#define CRC16_PRE_DATA013			0xC18CU
#define CRC16_PRE_DATA014			0xD1ADU
#define CRC16_PRE_DATA015			0xE1CEU
#define CRC16_PRE_DATA016			0xF1EFU
#define CRC16_PRE_DATA017			0x1231U
#define CRC16_PRE_DATA018			0x0210U
#define CRC16_PRE_DATA019			0x3273U
#define CRC16_PRE_DATA020			0x2252U
#define CRC16_PRE_DATA021			0x52B5U
#define CRC16_PRE_DATA022			0x4294U
#define CRC16_PRE_DATA023			0x72F7U
#define CRC16_PRE_DATA024			0x62D6U
#define CRC16_PRE_DATA025			0x9339U
#define CRC16_PRE_DATA026			0x8318U
#define CRC16_PRE_DATA027			0xB37BU
#define CRC16_PRE_DATA028			0xA35AU
#define CRC16_PRE_DATA029			0xD3BDU
#define CRC16_PRE_DATA030			0xC39CU
#define CRC16_PRE_DATA031			0xF3FFU
#define CRC16_PRE_DATA032			0xE3DEU
#define CRC16_PRE_DATA033			0x2462U
#define CRC16_PRE_DATA034			0x3443U
#define CRC16_PRE_DATA035			0x0420U
#define CRC16_PRE_DATA036			0x1401U
#define CRC16_PRE_DATA037			0x64E6U
#define CRC16_PRE_DATA038			0x74C7U
#define CRC16_PRE_DATA039			0x44A4U
#define CRC16_PRE_DATA040			0x5485U
#define CRC16_PRE_DATA041			0xA56AU
#define CRC16_PRE_DATA042			0xB54BU
#define CRC16_PRE_DATA043			0x8528U
#define CRC16_PRE_DATA044			0x9509U
#define CRC16_PRE_DATA045			0xE5EEU
#define CRC16_PRE_DATA046			0xF5CFU
#define CRC16_PRE_DATA047			0xC5ACU
#define CRC16_PRE_DATA048			0xD58DU
#define CRC16_PRE_DATA049			0x3653U
#define CRC16_PRE_DATA050			0x2672U
#define CRC16_PRE_DATA051			0x1611U
#define CRC16_PRE_DATA052			0x0630U
#define CRC16_PRE_DATA053			0x76D7U
#define CRC16_PRE_DATA054			0x66F6U
#define CRC16_PRE_DATA055			0x5695U
#define CRC16_PRE_DATA056			0x46B4U
#define CRC16_PRE_DATA057			0xB75BU
#define CRC16_PRE_DATA058			0xA77AU
#define CRC16_PRE_DATA059			0x9719U
#define CRC16_PRE_DATA060			0x8738U
#define CRC16_PRE_DATA061			0xF7DFU
#define CRC16_PRE_DATA062			0xE7FEU
#define CRC16_PRE_DATA063			0xD79DU
#define CRC16_PRE_DATA064			0xC7BCU
#define CRC16_PRE_DATA065			0x48C4U
#define CRC16_PRE_DATA066			0x58E5U
#define CRC16_PRE_DATA067			0x6886U
#define CRC16_PRE_DATA068			0x78A7U
#define CRC16_PRE_DATA069			0x0840U
#define CRC16_PRE_DATA070			0x1861U
#define CRC16_PRE_DATA071			0x2802U
#define CRC16_PRE_DATA072			0x3823U
#define CRC16_PRE_DATA073			0xC9CCU
#define CRC16_PRE_DATA074			0xD9EDU
#define CRC16_PRE_DATA075			0xE98EU
#define CRC16_PRE_DATA076			0xF9AFU
#define CRC16_PRE_DATA077			0x8948U
#define CRC16_PRE_DATA078			0x9969U
#define CRC16_PRE_DATA079			0xA90AU
#define CRC16_PRE_DATA080			0xB92BU
#define CRC16_PRE_DATA081			0x5AF5U
#define CRC16_PRE_DATA082			0x4AD4U
#define CRC16_PRE_DATA083			0x7AB7U
#define CRC16_PRE_DATA084			0x6A96U
#define CRC16_PRE_DATA085			0x1A71U
#define CRC16_PRE_DATA086			0x0A50U
#define CRC16_PRE_DATA087			0x3A33U
#define CRC16_PRE_DATA088			0x2A12U
#define CRC16_PRE_DATA089			0xDBFDU
#define CRC16_PRE_DATA090			0xCBDCU
#define CRC16_PRE_DATA091			0xFBBFU
#define CRC16_PRE_DATA092			0xEB9EU
#define CRC16_PRE_DATA093			0x9B79U
#define CRC16_PRE_DATA094			0x8B58U
#define CRC16_PRE_DATA095			0xBB3BU
#define CRC16_PRE_DATA096			0xAB1AU
#define CRC16_PRE_DATA097			0x6CA6U
#define CRC16_PRE_DATA098			0x7C87U
#define CRC16_PRE_DATA099			0x4CE4U
#define CRC16_PRE_DATA100			0x5CC5U
#define CRC16_PRE_DATA101			0x2C22U
#define CRC16_PRE_DATA102			0x3C03U
#define CRC16_PRE_DATA103			0x0C60U
#define CRC16_PRE_DATA104			0x1C41U
#define CRC16_PRE_DATA105			0xEDAEU
#define CRC16_PRE_DATA106			0xFD8FU
#define CRC16_PRE_DATA107			0xCDECU
#define CRC16_PRE_DATA108			0xDDCDU
#define CRC16_PRE_DATA109			0xAD2AU
#define CRC16_PRE_DATA110			0xBD0BU
#define CRC16_PRE_DATA111			0x8D68U
#define CRC16_PRE_DATA112			0x9D49U
#define CRC16_PRE_DATA113			0x7E97U
#define CRC16_PRE_DATA114			0x6EB6U
#define CRC16_PRE_DATA115			0x5ED5U
#define CRC16_PRE_DATA116			0x4EF4U
#define CRC16_PRE_DATA117			0x3E13U
#define CRC16_PRE_DATA118			0x2E32U
#define CRC16_PRE_DATA119			0x1E51U
#define CRC16_PRE_DATA120			0x0E70U
#define CRC16_PRE_DATA121			0xFF9FU
#define CRC16_PRE_DATA122			0xEFBEU
#define CRC16_PRE_DATA123			0xDFDDU
#define CRC16_PRE_DATA124			0xCFFCU
#define CRC16_PRE_DATA125			0xBF1BU
#define CRC16_PRE_DATA126			0xAF3AU
#define CRC16_PRE_DATA127			0x9F59U
#define CRC16_PRE_DATA128			0x8F78U
#define CRC16_PRE_DATA129			0x9188U
#define CRC16_PRE_DATA130			0x81A9U
#define CRC16_PRE_DATA131			0xB1CAU
#define CRC16_PRE_DATA132			0xA1EBU
#define CRC16_PRE_DATA133			0xD10CU
#define CRC16_PRE_DATA134			0xC12DU
#define CRC16_PRE_DATA135			0xF14EU
#define CRC16_PRE_DATA136			0xE16FU
#define CRC16_PRE_DATA137			0x1080U
#define CRC16_PRE_DATA138			0x00A1U
#define CRC16_PRE_DATA139			0x30C2U
#define CRC16_PRE_DATA140			0x20E3U
#define CRC16_PRE_DATA141			0x5004U
#define CRC16_PRE_DATA142			0x4025U
#define CRC16_PRE_DATA143			0x7046U
#define CRC16_PRE_DATA144			0x6067U
#define CRC16_PRE_DATA145			0x83B9U
#define CRC16_PRE_DATA146			0x9398U
#define CRC16_PRE_DATA147			0xA3FBU
#define CRC16_PRE_DATA148			0xB3DAU
#define CRC16_PRE_DATA149			0xC33DU
#define CRC16_PRE_DATA150			0xD31CU
#define CRC16_PRE_DATA151			0xE37FU
#define CRC16_PRE_DATA152			0xF35EU
#define CRC16_PRE_DATA153			0x02B1U
#define CRC16_PRE_DATA154			0x1290U
#define CRC16_PRE_DATA155			0x22F3U
#define CRC16_PRE_DATA156			0x32D2U
#define CRC16_PRE_DATA157			0x4235U
#define CRC16_PRE_DATA158			0x5214U
#define CRC16_PRE_DATA159			0x6277U
#define CRC16_PRE_DATA160			0x7256U
#define CRC16_PRE_DATA161			0xB5EAU
#define CRC16_PRE_DATA162			0xA5CBU
#define CRC16_PRE_DATA163			0x95A8U
#define CRC16_PRE_DATA164			0x8589U
#define CRC16_PRE_DATA165			0xF56EU
#define CRC16_PRE_DATA166			0xE54FU
#define CRC16_PRE_DATA167			0xD52CU
#define CRC16_PRE_DATA168			0xC50DU
#define CRC16_PRE_DATA169			0x34E2U
#define CRC16_PRE_DATA170			0x24C3U
#define CRC16_PRE_DATA171			0x14A0U
#define CRC16_PRE_DATA172			0x0481U
#define CRC16_PRE_DATA173			0x7466U
#define CRC16_PRE_DATA174			0x6447U
#define CRC16_PRE_DATA175			0x5424U
#define CRC16_PRE_DATA176			0x4405U
#define CRC16_PRE_DATA177			0xA7DBU
#define CRC16_PRE_DATA178			0xB7FAU
#define CRC16_PRE_DATA179			0x8799U
#define CRC16_PRE_DATA180			0x97B8U
#define CRC16_PRE_DATA181			0xE75FU
#define CRC16_PRE_DATA182			0xF77EU
#define CRC16_PRE_DATA183			0xC71DU
#define CRC16_PRE_DATA184			0xD73CU
#define CRC16_PRE_DATA185			0x26D3U
#define CRC16_PRE_DATA186			0x36F2U
#define CRC16_PRE_DATA187			0x0691U
#define CRC16_PRE_DATA188			0x16B0U
#define CRC16_PRE_DATA189			0x6657U
#define CRC16_PRE_DATA190			0x7676U
#define CRC16_PRE_DATA191			0x4615U
#define CRC16_PRE_DATA192			0x5634U
#define CRC16_PRE_DATA193			0xD94CU
#define CRC16_PRE_DATA194			0xC96DU
#define CRC16_PRE_DATA195			0xF90EU
#define CRC16_PRE_DATA196			0xE92FU
#define CRC16_PRE_DATA197			0x99C8U
#define CRC16_PRE_DATA198			0x89E9U
#define CRC16_PRE_DATA199			0xB98AU
#define CRC16_PRE_DATA200			0xA9ABU
#define CRC16_PRE_DATA201			0x5844U
#define CRC16_PRE_DATA202			0x4865U
#define CRC16_PRE_DATA203			0x7806U
#define CRC16_PRE_DATA204			0x6827U
#define CRC16_PRE_DATA205			0x18C0U
#define CRC16_PRE_DATA206			0x08E1U
#define CRC16_PRE_DATA207			0x3882U
#define CRC16_PRE_DATA208			0x28A3U
#define CRC16_PRE_DATA209			0xCB7DU
#define CRC16_PRE_DATA210			0xDB5CU
#define CRC16_PRE_DATA211			0xEB3FU
#define CRC16_PRE_DATA212			0xFB1EU
#define CRC16_PRE_DATA213			0x8BF9U
#define CRC16_PRE_DATA214			0x9BD8U
#define CRC16_PRE_DATA215			0xABBBU
#define CRC16_PRE_DATA216			0xBB9AU
#define CRC16_PRE_DATA217			0x4A75U
#define CRC16_PRE_DATA218			0x5A54U
#define CRC16_PRE_DATA219			0x6A37U
#define CRC16_PRE_DATA220			0x7A16U
#define CRC16_PRE_DATA221			0x0AF1U
#define CRC16_PRE_DATA222			0x1AD0U
#define CRC16_PRE_DATA223			0x2AB3U
#define CRC16_PRE_DATA224			0x3A92U
#define CRC16_PRE_DATA225			0xFD2EU
#define CRC16_PRE_DATA226			0xED0FU
#define CRC16_PRE_DATA227			0xDD6CU
#define CRC16_PRE_DATA228			0xCD4DU
#define CRC16_PRE_DATA229			0xBDAAU
#define CRC16_PRE_DATA230			0xAD8BU
#define CRC16_PRE_DATA231			0x9DE8U
#define CRC16_PRE_DATA232			0x8DC9U
#define CRC16_PRE_DATA233			0x7C26U
#define CRC16_PRE_DATA234			0x6C07U
#define CRC16_PRE_DATA235			0x5C64U
#define CRC16_PRE_DATA236			0x4C45U
#define CRC16_PRE_DATA237			0x3CA2U
#define CRC16_PRE_DATA238			0x2C83U
#define CRC16_PRE_DATA239			0x1CE0U
#define CRC16_PRE_DATA240			0x0CC1U
#define CRC16_PRE_DATA241			0xEF1FU
#define CRC16_PRE_DATA242			0xFF3EU
#define CRC16_PRE_DATA243			0xCF5DU
#define CRC16_PRE_DATA244			0xDF7CU
#define CRC16_PRE_DATA245			0xAF9BU
#define CRC16_PRE_DATA246			0xBFBAU
#define CRC16_PRE_DATA247			0x8FD9U
#define CRC16_PRE_DATA248			0x9FF8U
#define CRC16_PRE_DATA249			0x6E17U
#define CRC16_PRE_DATA250			0x7E36U
#define CRC16_PRE_DATA251			0x4E55U
#define CRC16_PRE_DATA252			0x5E74U
#define CRC16_PRE_DATA253			0x2E93U
#define CRC16_PRE_DATA254			0x3EB2U
#define CRC16_PRE_DATA255			0x0ED1U
#define CRC16_PRE_DATA256			0x1EF0U

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
/*==========================================================================================*/
/*	CRC-16-CCITT Precalculated Modifier Data Table											*/
/*==========================================================================================*/
static const Type_uHWord nuhCrc16PreData_Tbl[] = {

	CRC16_PRE_DATA001,
	CRC16_PRE_DATA002,
	CRC16_PRE_DATA003,
	CRC16_PRE_DATA004,
	CRC16_PRE_DATA005,
	CRC16_PRE_DATA006,
	CRC16_PRE_DATA007,
	CRC16_PRE_DATA008,
	CRC16_PRE_DATA009,
	CRC16_PRE_DATA010,
	CRC16_PRE_DATA011,
	CRC16_PRE_DATA012,
	CRC16_PRE_DATA013,
	CRC16_PRE_DATA014,
	CRC16_PRE_DATA015,
	CRC16_PRE_DATA016,
	CRC16_PRE_DATA017,
	CRC16_PRE_DATA018,
	CRC16_PRE_DATA019,
	CRC16_PRE_DATA020,
	CRC16_PRE_DATA021,
	CRC16_PRE_DATA022,
	CRC16_PRE_DATA023,
	CRC16_PRE_DATA024,
	CRC16_PRE_DATA025,
	CRC16_PRE_DATA026,
	CRC16_PRE_DATA027,
	CRC16_PRE_DATA028,
	CRC16_PRE_DATA029,
	CRC16_PRE_DATA030,
	CRC16_PRE_DATA031,
	CRC16_PRE_DATA032,
	CRC16_PRE_DATA033,
	CRC16_PRE_DATA034,
	CRC16_PRE_DATA035,
	CRC16_PRE_DATA036,
	CRC16_PRE_DATA037,
	CRC16_PRE_DATA038,
	CRC16_PRE_DATA039,
	CRC16_PRE_DATA040,
	CRC16_PRE_DATA041,
	CRC16_PRE_DATA042,
	CRC16_PRE_DATA043,
	CRC16_PRE_DATA044,
	CRC16_PRE_DATA045,
	CRC16_PRE_DATA046,
	CRC16_PRE_DATA047,
	CRC16_PRE_DATA048,
	CRC16_PRE_DATA049,
	CRC16_PRE_DATA050,
	CRC16_PRE_DATA051,
	CRC16_PRE_DATA052,
	CRC16_PRE_DATA053,
	CRC16_PRE_DATA054,
	CRC16_PRE_DATA055,
	CRC16_PRE_DATA056,
	CRC16_PRE_DATA057,
	CRC16_PRE_DATA058,
	CRC16_PRE_DATA059,
	CRC16_PRE_DATA060,
	CRC16_PRE_DATA061,
	CRC16_PRE_DATA062,
	CRC16_PRE_DATA063,
	CRC16_PRE_DATA064,
	CRC16_PRE_DATA065,
	CRC16_PRE_DATA066,
	CRC16_PRE_DATA067,
	CRC16_PRE_DATA068,
	CRC16_PRE_DATA069,
	CRC16_PRE_DATA070,
	CRC16_PRE_DATA071,
	CRC16_PRE_DATA072,
	CRC16_PRE_DATA073,
	CRC16_PRE_DATA074,
	CRC16_PRE_DATA075,
	CRC16_PRE_DATA076,
	CRC16_PRE_DATA077,
	CRC16_PRE_DATA078,
	CRC16_PRE_DATA079,
	CRC16_PRE_DATA080,
	CRC16_PRE_DATA081,
	CRC16_PRE_DATA082,
	CRC16_PRE_DATA083,
	CRC16_PRE_DATA084,
	CRC16_PRE_DATA085,
	CRC16_PRE_DATA086,
	CRC16_PRE_DATA087,
	CRC16_PRE_DATA088,
	CRC16_PRE_DATA089,
	CRC16_PRE_DATA090,
	CRC16_PRE_DATA091,
	CRC16_PRE_DATA092,
	CRC16_PRE_DATA093,
	CRC16_PRE_DATA094,
	CRC16_PRE_DATA095,
	CRC16_PRE_DATA096,
	CRC16_PRE_DATA097,
	CRC16_PRE_DATA098,
	CRC16_PRE_DATA099,
	CRC16_PRE_DATA100,
	CRC16_PRE_DATA101,
	CRC16_PRE_DATA102,
	CRC16_PRE_DATA103,
	CRC16_PRE_DATA104,
	CRC16_PRE_DATA105,
	CRC16_PRE_DATA106,
	CRC16_PRE_DATA107,
	CRC16_PRE_DATA108,
	CRC16_PRE_DATA109,
	CRC16_PRE_DATA110,
	CRC16_PRE_DATA111,
	CRC16_PRE_DATA112,
	CRC16_PRE_DATA113,
	CRC16_PRE_DATA114,
	CRC16_PRE_DATA115,
	CRC16_PRE_DATA116,
	CRC16_PRE_DATA117,
	CRC16_PRE_DATA118,
	CRC16_PRE_DATA119,
	CRC16_PRE_DATA120,
	CRC16_PRE_DATA121,
	CRC16_PRE_DATA122,
	CRC16_PRE_DATA123,
	CRC16_PRE_DATA124,
	CRC16_PRE_DATA125,
	CRC16_PRE_DATA126,
	CRC16_PRE_DATA127,
	CRC16_PRE_DATA128,
	CRC16_PRE_DATA129,
	CRC16_PRE_DATA130,
	CRC16_PRE_DATA131,
	CRC16_PRE_DATA132,
	CRC16_PRE_DATA133,
	CRC16_PRE_DATA134,
	CRC16_PRE_DATA135,
	CRC16_PRE_DATA136,
	CRC16_PRE_DATA137,
	CRC16_PRE_DATA138,
	CRC16_PRE_DATA139,
	CRC16_PRE_DATA140,
	CRC16_PRE_DATA141,
	CRC16_PRE_DATA142,
	CRC16_PRE_DATA143,
	CRC16_PRE_DATA144,
	CRC16_PRE_DATA145,
	CRC16_PRE_DATA146,
	CRC16_PRE_DATA147,
	CRC16_PRE_DATA148,
	CRC16_PRE_DATA149,
	CRC16_PRE_DATA150,
	CRC16_PRE_DATA151,
	CRC16_PRE_DATA152,
	CRC16_PRE_DATA153,
	CRC16_PRE_DATA154,
	CRC16_PRE_DATA155,
	CRC16_PRE_DATA156,
	CRC16_PRE_DATA157,
	CRC16_PRE_DATA158,
	CRC16_PRE_DATA159,
	CRC16_PRE_DATA160,
	CRC16_PRE_DATA161,
	CRC16_PRE_DATA162,
	CRC16_PRE_DATA163,
	CRC16_PRE_DATA164,
	CRC16_PRE_DATA165,
	CRC16_PRE_DATA166,
	CRC16_PRE_DATA167,
	CRC16_PRE_DATA168,
	CRC16_PRE_DATA169,
	CRC16_PRE_DATA170,
	CRC16_PRE_DATA171,
	CRC16_PRE_DATA172,
	CRC16_PRE_DATA173,
	CRC16_PRE_DATA174,
	CRC16_PRE_DATA175,
	CRC16_PRE_DATA176,
	CRC16_PRE_DATA177,
	CRC16_PRE_DATA178,
	CRC16_PRE_DATA179,
	CRC16_PRE_DATA180,
	CRC16_PRE_DATA181,
	CRC16_PRE_DATA182,
	CRC16_PRE_DATA183,
	CRC16_PRE_DATA184,
	CRC16_PRE_DATA185,
	CRC16_PRE_DATA186,
	CRC16_PRE_DATA187,
	CRC16_PRE_DATA188,
	CRC16_PRE_DATA189,
	CRC16_PRE_DATA190,
	CRC16_PRE_DATA191,
	CRC16_PRE_DATA192,
	CRC16_PRE_DATA193,
	CRC16_PRE_DATA194,
	CRC16_PRE_DATA195,
	CRC16_PRE_DATA196,
	CRC16_PRE_DATA197,
	CRC16_PRE_DATA198,
	CRC16_PRE_DATA199,
	CRC16_PRE_DATA200,
	CRC16_PRE_DATA201,
	CRC16_PRE_DATA202,
	CRC16_PRE_DATA203,
	CRC16_PRE_DATA204,
	CRC16_PRE_DATA205,
	CRC16_PRE_DATA206,
	CRC16_PRE_DATA207,
	CRC16_PRE_DATA208,
	CRC16_PRE_DATA209,
	CRC16_PRE_DATA210,
	CRC16_PRE_DATA211,
	CRC16_PRE_DATA212,
	CRC16_PRE_DATA213,
	CRC16_PRE_DATA214,
	CRC16_PRE_DATA215,
	CRC16_PRE_DATA216,
	CRC16_PRE_DATA217,
	CRC16_PRE_DATA218,
	CRC16_PRE_DATA219,
	CRC16_PRE_DATA220,
	CRC16_PRE_DATA221,
	CRC16_PRE_DATA222,
	CRC16_PRE_DATA223,
	CRC16_PRE_DATA224,
	CRC16_PRE_DATA225,
	CRC16_PRE_DATA226,
	CRC16_PRE_DATA227,
	CRC16_PRE_DATA228,
	CRC16_PRE_DATA229,
	CRC16_PRE_DATA230,
	CRC16_PRE_DATA231,
	CRC16_PRE_DATA232,
	CRC16_PRE_DATA233,
	CRC16_PRE_DATA234,
	CRC16_PRE_DATA235,
	CRC16_PRE_DATA236,
	CRC16_PRE_DATA237,
	CRC16_PRE_DATA238,
	CRC16_PRE_DATA239,
	CRC16_PRE_DATA240,
	CRC16_PRE_DATA241,
	CRC16_PRE_DATA242,
	CRC16_PRE_DATA243,
	CRC16_PRE_DATA244,
	CRC16_PRE_DATA245,
	CRC16_PRE_DATA246,
	CRC16_PRE_DATA247,
	CRC16_PRE_DATA248,
	CRC16_PRE_DATA249,
	CRC16_PRE_DATA250,
	CRC16_PRE_DATA251,
	CRC16_PRE_DATA252,
	CRC16_PRE_DATA253,
	CRC16_PRE_DATA254,
	CRC16_PRE_DATA255,
	CRC16_PRE_DATA256

};

/********************************************************************************************/
/*							Function Definition Section										*/
/********************************************************************************************/
/********************************************************************************************/
/**	\function		wuhGen_CalcCrc16
 *	\date			2010/02/10
 *	\author			Daniel Morgan
 *	\description	Calculate CRC-16-CCITT of Input Data.
 *
 *	<!-------------	Argument Code ----------------------------------------------------------->
 *	\param[out]		avdData_p - Input Data Pointer.
 *	\param[out]		auwSize - Data Size.
 *
 *	<!-------------	Return Code ------------------------------------------------------------->
 *	\return			auhCrcData - CRC-16-CCITT Data.
 *
 *********************************************************************************************
 *	\par	Revision History:
 *	<!-----	No.		Date		Revised by		Details	------------------------------------->
 *
 ********************************************************************************************/
Type_uHWord wuhGen_CalcCrc16( const void *		avdData_p,
							  const Type_uWord	auwSize )
{
	Type_uHWord auhCrcData;									/* <AUTO> CRC-16-CCITT Data		*/


	auhCrcData = CRC_DATA_INIT_VAL;

	if (avdData_p != STD_NULL)
	{
		Type_uHWord auhTempData;							/* <AUTO> Temporary Data		*/
		Type_uWord auwDataCounter;							/* <AUTO> Data Counter			*/
		const Type_uByte *aubData_p;						/* <AUTO> Data Pointer			*/

		aubData_p = (const Type_uByte *)(avdData_p);

		for (auwDataCounter = 0; (auwDataCounter < auwSize); auwDataCounter++)
		{
			auhTempData = (Type_uHWord)((auhCrcData >> SHIFT_1BYTE) ^ aubData_p[auwDataCounter]);
			auhCrcData = (Type_uHWord)((Type_uHWord)(auhCrcData << SHIFT_1BYTE) ^ nuhCrc16PreData_Tbl[auhTempData]);
		}
	}
	else	/* No action required */
	{
		;
	}

	return auhCrcData;
}

/* End Of File */
