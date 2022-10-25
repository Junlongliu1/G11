/**********************************************************************************************************************
*	Include File Name			: ProEng.h
*	Date						: 2000.09.12
*	Author						: ?/APN
*	Model						: CDA7878A/J/R
*	Description					:
*
*	Pre-read File Name			:
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*	lizx : 0-11-13 10:57    special page function call added in.
*	#01		02.11.06	T.Ichikawa(apn)		Process ID  unsigned char -> unsigned short int
*
*
**********************************************************************************************************************/
#ifndef _PROENGIN_H_
#define _PROENGIN_H_

/**********************************************************************************************************************
*	Macro Definition Section
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------
*	Boolean definition
*-------------------------------------------------------------------------------------------------------------------*/
#define		TRUE					1
#define		FALSE					0

/*--------------------------------------------------------------------------------------------------------------------
*	Subroutine End Code
*-------------------------------------------------------------------------------------------------------------------*/
#define		PE_SEC0					0xfdU		/** Subroutine End Code 0 **/
#define		PE_SEC1					0xfeU		/** Subroutine End Code 1 **/
#define		PE_SEC_CONTINUE			0xffU		/** Same Stage Continue **/
/*--------------------------------------------------------------------------------------------------------------------
*	Porcess End Code
*-------------------------------------------------------------------------------------------------------------------*/
#define		PE_PEC0					0x7fU		/** Process End Code 0 **/
#define		PE_PEC1					0x7eU		/** Process End Code 1 **/

/*--------------------------------------------------------------------------------------------------------------------
*	Child Process Data
*-------------------------------------------------------------------------------------------------------------------*/
/***** #01 02.11.06 T.Ichikawa(apn) modify start *****/
/*	#define		PE_HAVE_CHILD			0x80	*/	/** Have Child Process **/
/*	#define		PE_NO_CHILD				0x00	*/	/** No Child Process **/
#define		PE_HAVE_CHILD			0x8000U		/** Have Child Process **/
#define		PE_NO_CHILD				0x0000U		/** No Child Process **/
/***** #01 02.11.06 T.Ichikawa(apn) modify end *****/

/*--------------------------------------------------------------------------------------------------------------------
*	On-time Execution Data
*-------------------------------------------------------------------------------------------------------------------*/
#define		PE_CONTINUE				0x80U		/** Have On-time Execute **/
#define		PE_EXIT					0x00U		/** No On-time Execute **/

/*--------------------------------------------------------------------------------------------------------------------
*	Invalid Return Value
*-------------------------------------------------------------------------------------------------------------------*/
#define		PE_INVALID_VALUE		0xffU		/** Memory Code for Return Value member **/

/*--------------------------------------------------------------------------------------------------------------------
*	Return value from Process Execution Function
*-------------------------------------------------------------------------------------------------------------------*/
#define		PERC_NO_EXECUTE_PROC	0xffU		/** No Process Execution **/
#define		PERC_STAGE_CONTINUE		0xfeU		/** Same Stage Continue **/

/*--------------------------------------------------------------------------------------------------------------------
*	Process Control Table Macro
*-------------------------------------------------------------------------------------------------------------------*/
#define		PE_MAKE_PROC_TABLE( CHILD ,ID , NEXT0 , RETURN0 , NEXT1 , RETURN1 )	{ (CHILD | ID) , NEXT0 , RETURN0 , NEXT1 , RETURN1 }
#define		SPE_MAKE_PROC_TABLE( CHILD ,ID , NEXT , RETURN )	{ (CHILD | ID) , NEXT , RETURN }

/*--------------------------------------------------------------------------------------------------------------------
*	Standard Stage Macro
*-------------------------------------------------------------------------------------------------------------------*/
/**	Without On-time Execution Stage **/
#define	STAGE0		0   	/**	STAGE0		**/
#define	STAGE1		1   	/**	STAGE1		**/
#define	STAGE2		2   	/**	STAGE2		**/
#define	STAGE3		3   	/**	STAGE3		**/
#define	STAGE4		4   	/**	STAGE4		**/
#define	STAGE5		5		/**	STAGE5		**/
#define	STAGE6		6   	/**	STAGE6		**/
#define	STAGE7		7   	/**	STAGE7		**/
#define	STAGE8		8   	/**	STAGE8		**/
#define	STAGE9		9   	/**	STAGE9		**/
#define	STAGE10		10  	/**	STAGE10		**/
#define	STAGE11		11  	/**	STAGE11		**/
#define	STAGE12		12  	/**	STAGE12		**/
#define	STAGE13		13  	/**	STAGE13		**/
#define	STAGE14		14  	/**	STAGE14		**/
#define	STAGE15		15  	/**	STAGE15		**/
#define	STAGE16		16  	/**	STAGE16		**/
#define	STAGE17		17  	/**	STAGE17		**/
#define	STAGE18		18  	/**	STAGE18		**/
#define	STAGE19		19  	/**	STAGE19		**/
#define	STAGE20		20  	/**	STAGE20		**/
#define	STAGE21		21  	/**	STAGE21		**/
#define	STAGE22		22  	/**	STAGE22		**/
#define	STAGE23		23  	/**	STAGE23		**/
#define	STAGE24		24  	/**	STAGE24		**/
#define	STAGE25		25  	/**	STAGE25		**/
#define	STAGE26		26  	/**	STAGE26		**/
#define	STAGE27		27  	/**	STAGE27		**/
#define	STAGE28		28  	/**	STAGE28		**/
#define	STAGE29		29  	/**	STAGE29		**/
#define	STAGE30		30  	/**	STAGE30		**/
#define	STAGE31		31  	/**	STAGE31		**/
#define	STAGE32		32  	/**	STAGE32		**/
#define	STAGE33		33  	/**	STAGE33		**/
#define	STAGE34		34  	/**	STAGE34		**/
#define	STAGE35		35  	/**	STAGE35		**/
#define	STAGE36		36  	/**	STAGE36		**/
#define	STAGE37		37  	/**	STAGE37		**/
#define	STAGE38		38  	/**	STAGE38		**/
#define	STAGE39		39  	/**	STAGE39		**/
#define	STAGE40		40  	/**	STAGE40		**/
#define	STAGE41		41  	/**	STAGE41		**/
#define	STAGE42		42  	/**	STAGE42		**/
#define	STAGE43		43  	/**	STAGE43		**/
#define	STAGE44		44  	/**	STAGE44		**/
#define	STAGE45		45  	/**	STAGE45		**/
#define	STAGE46		46  	/**	STAGE46		**/
#define	STAGE47		47  	/**	STAGE47		**/
#define	STAGE48		48  	/**	STAGE48		**/
#define	STAGE49		49  	/**	STAGE49		**/
#define	STAGE50		50  	/**	STAGE50		**/
#define	STAGE51		51  	/**	STAGE51		**/
#define	STAGE52		52  	/**	STAGE52		**/
#define	STAGE53		53  	/**	STAGE53		**/
#define	STAGE54		54  	/**	STAGE54		**/
#define	STAGE55		55  	/**	STAGE55		**/
#define	STAGE56		56  	/**	STAGE56		**/
#define	STAGE57		57  	/**	STAGE57		**/
#define	STAGE58		58  	/**	STAGE58		**/
#define	STAGE59		59  	/**	STAGE59		**/
#define	STAGE60		60  	/**	STAGE60		**/
#define	STAGE61		61  	/**	STAGE61		**/
#define	STAGE62		62  	/**	STAGE62		**/
#define	STAGE63		63  	/**	STAGE63		**/
#define	STAGE64		64  	/**	STAGE64		**/
#define	STAGE65		65  	/**	STAGE65		**/
#define	STAGE66		66  	/**	STAGE66		**/
#define	STAGE67		67  	/**	STAGE67		**/
#define	STAGE68		68  	/**	STAGE68		**/
#define	STAGE69		69  	/**	STAGE69		**/
#define	STAGE70		70  	/**	STAGE70		**/
#define	STAGE71		71  	/**	STAGE71		**/
#define	STAGE72		72  	/**	STAGE72		**/
#define	STAGE73		73  	/**	STAGE73		**/
#define	STAGE74		74  	/**	STAGE74		**/
#define	STAGE75		75  	/**	STAGE75		**/
#define	STAGE76		76  	/**	STAGE76		**/
#define	STAGE77		77  	/**	STAGE77		**/
#define	STAGE78		78  	/**	STAGE78		**/
#define	STAGE79		79  	/**	STAGE79		**/
#define	STAGE80		80  	/**	STAGE80		**/
#define	STAGE81		81  	/**	STAGE81		**/
#define	STAGE82		82  	/**	STAGE82		**/
#define	STAGE83		83  	/**	STAGE83		**/
#define	STAGE84		84  	/**	STAGE84		**/
#define	STAGE85		85  	/**	STAGE85		**/
#define	STAGE86		86  	/**	STAGE86		**/
#define	STAGE87		87  	/**	STAGE87		**/
#define	STAGE88		88  	/**	STAGE88		**/
#define	STAGE89		89  	/**	STAGE89		**/
#define	STAGE90		90  	/**	STAGE90		**/
#define	STAGE91		91  	/**	STAGE91		**/
#define	STAGE92		92  	/**	STAGE92		**/
#define	STAGE93		93  	/**	STAGE93		**/
#define	STAGE94		94  	/**	STAGE94		**/
#define	STAGE95		95  	/**	STAGE95		**/
#define	STAGE96		96  	/**	STAGE96		**/
#define	STAGE97		97  	/**	STAGE97		**/
#define	STAGE98		98  	/**	STAGE98		**/
#define	STAGE99		99  	/**	STAGE99		**/
#define	STAGE100	100 	/**	STAGE100	**/
#define	STAGE101	101 	/**	STAGE101	**/
#define	STAGE102	102 	/**	STAGE102	**/
#define	STAGE103	103 	/**	STAGE103	**/
#define	STAGE104	104 	/**	STAGE104	**/
#define	STAGE105	105 	/**	STAGE105	**/
#define	STAGE106	106 	/**	STAGE106	**/
#define	STAGE107	107 	/**	STAGE107	**/
#define	STAGE108	108 	/**	STAGE108	**/
#define	STAGE109	109 	/**	STAGE109	**/
#define	STAGE110	110 	/**	STAGE110	**/
#define	STAGE111	111 	/**	STAGE111	**/
#define	STAGE112	112 	/**	STAGE112	**/
#define	STAGE113	113 	/**	STAGE113	**/
#define	STAGE114	114 	/**	STAGE114	**/
#define	STAGE115	115 	/**	STAGE115	**/
#define	STAGE116	116 	/**	STAGE116	**/
#define	STAGE117	117 	/**	STAGE117	**/
#define	STAGE118	118 	/**	STAGE118	**/
#define	STAGE119	119 	/**	STAGE119	**/
#define	STAGE120	120 	/**	STAGE120	**/
#define	STAGE121	121 	/**	STAGE121	**/
#define	STAGE122	122 	/**	STAGE122	**/
#define	STAGE123	123 	/**	STAGE123	**/
#define	STAGE124	124 	/**	STAGE124	**/
#define	STAGE125	125 	/**	STAGE125	**/

/**	Have On-time Execution Stage **/
#define	C_STAGE0	( PE_CONTINUE |	STAGE0	)		/**	STAGE0		**/
#define	C_STAGE1	( PE_CONTINUE |	STAGE1	)    	/**	STAGE1		**/
#define	C_STAGE2	( PE_CONTINUE |	STAGE2	)    	/**	STAGE2		**/
#define	C_STAGE3	( PE_CONTINUE |	STAGE3	)    	/**	STAGE3		**/
#define	C_STAGE4	( PE_CONTINUE |	STAGE4	)    	/**	STAGE4		**/
#define	C_STAGE5	( PE_CONTINUE |	STAGE5	)    	/**	STAGE5		**/
#define	C_STAGE6	( PE_CONTINUE |	STAGE6	)    	/**	STAGE6		**/
#define	C_STAGE7	( PE_CONTINUE |	STAGE7	)    	/**	STAGE7		**/
#define	C_STAGE8	( PE_CONTINUE |	STAGE8	)    	/**	STAGE8		**/
#define	C_STAGE9	( PE_CONTINUE |	STAGE9	)    	/**	STAGE9		**/
#define	C_STAGE10	( PE_CONTINUE |	STAGE10	)    	/**	STAGE10		**/
#define	C_STAGE11	( PE_CONTINUE |	STAGE11	)    	/**	STAGE11		**/
#define	C_STAGE12	( PE_CONTINUE |	STAGE12	)    	/**	STAGE12		**/
#define	C_STAGE13	( PE_CONTINUE |	STAGE13	)    	/**	STAGE13		**/
#define	C_STAGE14	( PE_CONTINUE |	STAGE14	)    	/**	STAGE14		**/
#define	C_STAGE15	( PE_CONTINUE |	STAGE15	)    	/**	STAGE15		**/
#define	C_STAGE16	( PE_CONTINUE |	STAGE16	)    	/**	STAGE16		**/
#define	C_STAGE17	( PE_CONTINUE |	STAGE17	)    	/**	STAGE17		**/
#define	C_STAGE18	( PE_CONTINUE |	STAGE18	)    	/**	STAGE18		**/
#define	C_STAGE19	( PE_CONTINUE |	STAGE19	)    	/**	STAGE19		**/
#define	C_STAGE20	( PE_CONTINUE |	STAGE20	)    	/**	STAGE20		**/
#define	C_STAGE21	( PE_CONTINUE |	STAGE21	)    	/**	STAGE21		**/
#define	C_STAGE22	( PE_CONTINUE |	STAGE22	)    	/**	STAGE22		**/
#define	C_STAGE23	( PE_CONTINUE |	STAGE23	)    	/**	STAGE23		**/
#define	C_STAGE24	( PE_CONTINUE |	STAGE24	)    	/**	STAGE24		**/
#define	C_STAGE25	( PE_CONTINUE |	STAGE25	)    	/**	STAGE25		**/
#define	C_STAGE26	( PE_CONTINUE |	STAGE26	)    	/**	STAGE26		**/
#define	C_STAGE27	( PE_CONTINUE |	STAGE27	)    	/**	STAGE27		**/
#define	C_STAGE28	( PE_CONTINUE |	STAGE28	)    	/**	STAGE28		**/
#define	C_STAGE29	( PE_CONTINUE |	STAGE29	)    	/**	STAGE29		**/
#define	C_STAGE30	( PE_CONTINUE |	STAGE30	)    	/**	STAGE30		**/
#define	C_STAGE31	( PE_CONTINUE |	STAGE31	)    	/**	STAGE31		**/
#define	C_STAGE32	( PE_CONTINUE |	STAGE32	)    	/**	STAGE32		**/
#define	C_STAGE33	( PE_CONTINUE |	STAGE33	)    	/**	STAGE33		**/
#define	C_STAGE34	( PE_CONTINUE |	STAGE34	)    	/**	STAGE34		**/
#define	C_STAGE35	( PE_CONTINUE |	STAGE35	)    	/**	STAGE35		**/
#define	C_STAGE36	( PE_CONTINUE |	STAGE36	)    	/**	STAGE36		**/
#define	C_STAGE37	( PE_CONTINUE |	STAGE37	)    	/**	STAGE37		**/
#define	C_STAGE38	( PE_CONTINUE |	STAGE38	)    	/**	STAGE38		**/
#define	C_STAGE39	( PE_CONTINUE |	STAGE39	)    	/**	STAGE39		**/
#define	C_STAGE40	( PE_CONTINUE |	STAGE40	)    	/**	STAGE40		**/
#define	C_STAGE41	( PE_CONTINUE |	STAGE41	)    	/**	STAGE41		**/
#define	C_STAGE42	( PE_CONTINUE |	STAGE42	)    	/**	STAGE42		**/
#define	C_STAGE43	( PE_CONTINUE |	STAGE43	)    	/**	STAGE43		**/
#define	C_STAGE44	( PE_CONTINUE |	STAGE44	)    	/**	STAGE44		**/
#define	C_STAGE45	( PE_CONTINUE |	STAGE45	)    	/**	STAGE45		**/
#define	C_STAGE46	( PE_CONTINUE |	STAGE46	)    	/**	STAGE46		**/
#define	C_STAGE47	( PE_CONTINUE |	STAGE47	)    	/**	STAGE47		**/
#define	C_STAGE48	( PE_CONTINUE |	STAGE48	)    	/**	STAGE48		**/
#define	C_STAGE49	( PE_CONTINUE |	STAGE49	)    	/**	STAGE49		**/
#define	C_STAGE50	( PE_CONTINUE |	STAGE50	)    	/**	STAGE50		**/
#define	C_STAGE51	( PE_CONTINUE |	STAGE51	)    	/**	STAGE51		**/
#define	C_STAGE52	( PE_CONTINUE |	STAGE52	)    	/**	STAGE52		**/
#define	C_STAGE53	( PE_CONTINUE |	STAGE53	)    	/**	STAGE53		**/
#define	C_STAGE54	( PE_CONTINUE |	STAGE54	)    	/**	STAGE54		**/
#define	C_STAGE55	( PE_CONTINUE |	STAGE55	)    	/**	STAGE55		**/
#define	C_STAGE56	( PE_CONTINUE |	STAGE56	)    	/**	STAGE56		**/
#define	C_STAGE57	( PE_CONTINUE |	STAGE57	)    	/**	STAGE57		**/
#define	C_STAGE58	( PE_CONTINUE |	STAGE58	)    	/**	STAGE58		**/
#define	C_STAGE59	( PE_CONTINUE |	STAGE59	)    	/**	STAGE59		**/
#define	C_STAGE60	( PE_CONTINUE |	STAGE60	)    	/**	STAGE60		**/
#define	C_STAGE61	( PE_CONTINUE |	STAGE61	)    	/**	STAGE61		**/
#define	C_STAGE62	( PE_CONTINUE |	STAGE62	)    	/**	STAGE62		**/
#define	C_STAGE63	( PE_CONTINUE |	STAGE63	)    	/**	STAGE63		**/
#define	C_STAGE64	( PE_CONTINUE |	STAGE64	)    	/**	STAGE64		**/
#define	C_STAGE65	( PE_CONTINUE |	STAGE65	)    	/**	STAGE65		**/
#define	C_STAGE66	( PE_CONTINUE |	STAGE66	)    	/**	STAGE66		**/
#define	C_STAGE67	( PE_CONTINUE |	STAGE67	)    	/**	STAGE67		**/
#define	C_STAGE68	( PE_CONTINUE |	STAGE68	)    	/**	STAGE68		**/
#define	C_STAGE69	( PE_CONTINUE |	STAGE69	)    	/**	STAGE69		**/
#define	C_STAGE70	( PE_CONTINUE |	STAGE70	)    	/**	STAGE70		**/
#define	C_STAGE71	( PE_CONTINUE |	STAGE71	)    	/**	STAGE71		**/
#define	C_STAGE72	( PE_CONTINUE |	STAGE72	)    	/**	STAGE72		**/
#define	C_STAGE73	( PE_CONTINUE |	STAGE73	)    	/**	STAGE73		**/
#define	C_STAGE74	( PE_CONTINUE |	STAGE74	)    	/**	STAGE74		**/
#define	C_STAGE75	( PE_CONTINUE |	STAGE75	)    	/**	STAGE75		**/
#define	C_STAGE76	( PE_CONTINUE |	STAGE76	)    	/**	STAGE76		**/
#define	C_STAGE77	( PE_CONTINUE |	STAGE77	)    	/**	STAGE77		**/
#define	C_STAGE78	( PE_CONTINUE |	STAGE78	)    	/**	STAGE78		**/
#define	C_STAGE79	( PE_CONTINUE |	STAGE79	)    	/**	STAGE79		**/
#define	C_STAGE80	( PE_CONTINUE |	STAGE80	)    	/**	STAGE80		**/
#define	C_STAGE81	( PE_CONTINUE |	STAGE81	)    	/**	STAGE81		**/
#define	C_STAGE82	( PE_CONTINUE |	STAGE82	)    	/**	STAGE82		**/
#define	C_STAGE83	( PE_CONTINUE |	STAGE83	)    	/**	STAGE83		**/
#define	C_STAGE84	( PE_CONTINUE |	STAGE84	)    	/**	STAGE84		**/
#define	C_STAGE85	( PE_CONTINUE |	STAGE85	)    	/**	STAGE85		**/
#define	C_STAGE86	( PE_CONTINUE |	STAGE86	)    	/**	STAGE86		**/
#define	C_STAGE87	( PE_CONTINUE |	STAGE87	)    	/**	STAGE87		**/
#define	C_STAGE88	( PE_CONTINUE |	STAGE88	)    	/**	STAGE88		**/
#define	C_STAGE89	( PE_CONTINUE |	STAGE89	)    	/**	STAGE89		**/
#define	C_STAGE90	( PE_CONTINUE |	STAGE90	)    	/**	STAGE90		**/
#define	C_STAGE91	( PE_CONTINUE |	STAGE91	)    	/**	STAGE91		**/
#define	C_STAGE92	( PE_CONTINUE |	STAGE92	)    	/**	STAGE92		**/
#define	C_STAGE93	( PE_CONTINUE |	STAGE93	)    	/**	STAGE93		**/
#define	C_STAGE94	( PE_CONTINUE |	STAGE94	)    	/**	STAGE94		**/
#define	C_STAGE95	( PE_CONTINUE |	STAGE95	)    	/**	STAGE95		**/
#define	C_STAGE96	( PE_CONTINUE |	STAGE96	)    	/**	STAGE96		**/
#define	C_STAGE97	( PE_CONTINUE |	STAGE97	)    	/**	STAGE97		**/
#define	C_STAGE98	( PE_CONTINUE |	STAGE98	)    	/**	STAGE98		**/
#define	C_STAGE99	( PE_CONTINUE |	STAGE99	)    	/**	STAGE99		**/
#define	C_STAGE100	( PE_CONTINUE |	STAGE100	)	/**	STAGE100	**/
#define	C_STAGE101	( PE_CONTINUE |	STAGE101	)	/**	STAGE101	**/
#define	C_STAGE102	( PE_CONTINUE |	STAGE102	)	/**	STAGE102	**/
#define	C_STAGE103	( PE_CONTINUE |	STAGE103	)	/**	STAGE103	**/
#define	C_STAGE104	( PE_CONTINUE |	STAGE104	)	/**	STAGE104	**/
#define	C_STAGE105	( PE_CONTINUE |	STAGE105	)	/**	STAGE105	**/
#define	C_STAGE106	( PE_CONTINUE |	STAGE106	)	/**	STAGE106	**/
#define	C_STAGE107	( PE_CONTINUE |	STAGE107	)	/**	STAGE107	**/
#define	C_STAGE108	( PE_CONTINUE |	STAGE108	)	/**	STAGE108	**/
#define	C_STAGE109	( PE_CONTINUE |	STAGE109	)	/**	STAGE109	**/
#define	C_STAGE110	( PE_CONTINUE |	STAGE110	)	/**	STAGE110	**/
#define	C_STAGE111	( PE_CONTINUE |	STAGE111	)	/**	STAGE111	**/
#define	C_STAGE112	( PE_CONTINUE |	STAGE112	)	/**	STAGE112	**/
#define	C_STAGE113	( PE_CONTINUE |	STAGE113	)	/**	STAGE113	**/
#define	C_STAGE114	( PE_CONTINUE |	STAGE114	)	/**	STAGE114	**/
#define	C_STAGE115	( PE_CONTINUE |	STAGE115	)	/**	STAGE115	**/
#define	C_STAGE116	( PE_CONTINUE |	STAGE116	)	/**	STAGE116	**/
#define	C_STAGE117	( PE_CONTINUE |	STAGE117	)	/**	STAGE117	**/
#define	C_STAGE118	( PE_CONTINUE |	STAGE118	)	/**	STAGE118	**/
#define	C_STAGE119	( PE_CONTINUE |	STAGE119	)	/**	STAGE119	**/
#define	C_STAGE120	( PE_CONTINUE |	STAGE120	)	/**	STAGE120	**/
#define	C_STAGE121	( PE_CONTINUE |	STAGE121	)	/**	STAGE121	**/
#define	C_STAGE122	( PE_CONTINUE |	STAGE122	)	/**	STAGE122	**/
#define	C_STAGE123	( PE_CONTINUE |	STAGE123	)	/**	STAGE123	**/
#define	C_STAGE124	( PE_CONTINUE |	STAGE124	)	/**	STAGE124	**/
#define	C_STAGE125	( PE_CONTINUE |	STAGE125	)	/**	STAGE125	**/

/**********************************************************************************************************************
*	User-type Declaration Section
**********************************************************************************************************************/

/**********************************************************************************************************************
*	Structure Declaration Section
**********************************************************************************************************************/
/*--------------------------------------------------------------------------------------------------------------------
*	Process Buffer
*-------------------------------------------------------------------------------------------------------------------*/
typedef	struct {
		Type_uHWord	uhProcessID;		/**	Process ID **/
		Type_uByte	ubExecStageNum;		/** Executing Stage Number **/
} ProcBuf_ts;

/*--------------------------------------------------------------------------------------------------------------------
*	Process Control Table
*-------------------------------------------------------------------------------------------------------------------*/
typedef struct {
	Type_uHWord	uhIndex;			/** ID  **/
	Type_uByte	ubNextStageBy0;		/** Next Stage 0 **/
	Type_uByte	ubReturnValueBy0;	/** Return 0 **/
	Type_uByte	ubNextStageBy1;		/** Next Stage 1 **/
	Type_uByte	ubReturnValueBy1;	/** Return 1 **/
} ElementOfProcTbl_ts;

/*--------------------------------------------------------------------------------------------------------------------
*	Process Engine Control Block
*-------------------------------------------------------------------------------------------------------------------*/
typedef struct {
	Type_uByte					ubReqOfExecution;	/** Execution Requirement **/
	Type_uByte					ubNumOfActiveBuf;	/** Executing Process Buffer's layer **/
	ProcBuf_ts					*ProcBuf_p;			/** Pointer to Process Buffer **/
	const ElementOfProcTbl_ts	*const*ProcTblSAT_p;		/** Pointer to Process Table Address Table **/
	Type_uByte			(*const*SubSAT_p)( void );		/** Pointer to Subroutine Address Table **/
} ProcCTB_ts;

/*--------------------------------------------------------------------------------------------------------------------
*	Process Control Table (simple version)
*-------------------------------------------------------------------------------------------------------------------*/
typedef struct {
	Type_uHWord		uhIndex;			/** ID  **/
	Type_uByte		ubNextStage;		/** Next Stage **/
	Type_uByte		ubReturnValue;		/** Return Value **/
} ElementOfSProcTbl_ts;

/*--------------------------------------------------------------------------------------------------------------------
*	Process Engine Structure (simple version)
*-------------------------------------------------------------------------------------------------------------------*/
typedef struct {
	Type_uByte					ubReqOfExecution;	/** Execution Requirement **/
	Type_uByte					ubNumOfActiveBuf;	/** Executing Process Buffer's layer **/
	ProcBuf_ts					*ProcBuf_p;			/** Pointer to Process Buffer **/
	const ElementOfSProcTbl_ts	*const*ProcTblSAT_p;		/** Pointer to Process Table Address Table **/
	Type_uByte			(*const*SubSAT_p)( void );		/** Pointer to Subroutine Address Table **/
} SProcCTB_ts;


/**********************************************************************************************************************
*	Global Variable Extern Declaration Section
**********************************************************************************************************************/

/**********************************************************************************************************************
*	Global Function Prototype Declaration Section
**********************************************************************************************************************/
Type_uByte		wubPESetUp( ProcCTB_ts 	*astProcCTB_p ,
							ProcBuf_ts *astProcBuf_p ,
							const ElementOfProcTbl_ts *const*astProcTblSAT_p ,
							Type_uByte (*const*astSubSAT_p)( void ) );
void			wvdPESetReq( ProcCTB_ts *astProcCTB_p );
void			wvdPEResetReq( ProcCTB_ts *astProcCTB_p );
void			wvdPESetProcess( Type_uHWord auhProcID , ProcCTB_ts *astProcCTB_p );
void			wvdPEResetProcess( ProcCTB_ts *astProcCTB_p);
Type_uByte 		wubPERestart( ProcCTB_ts *astProcCTB_p );
Type_uByte		wubPEIsExecuteing( ProcCTB_ts *astProcCTB_p );
Type_uByte		wubPEExecute( ProcCTB_ts *astProcCTB_p );
Type_uByte		wubPEExecuteOnNow( ProcCTB_ts *astProcCTB_p );
Type_uByte		wubSPESetUp( SProcCTB_ts 	*astProcCTB_p ,
							 ProcBuf_ts *astProcBuf_p ,
							 const ElementOfSProcTbl_ts *const*astProcTblSAT_p ,
							 Type_uByte (*const*astSubSAT_p)( void ) );
void			wvdSPESetReq( SProcCTB_ts *astProcCTB_p );
void			wvdSPEResetReq( SProcCTB_ts *astProcCTB_p );
void			wvdSPESetProcess( Type_uHWord auhProcID , SProcCTB_ts *astProcCTB_p );
void			wvdSPEResetProcess( SProcCTB_ts *astProcCTB_p);
Type_uByte 		wubSPERestart( SProcCTB_ts *astProcCTB_p );
Type_uByte		wubSPEIsExecuteing( SProcCTB_ts *astProcCTB_p );
Type_uByte		wubSPEExecute( SProcCTB_ts *astProcCTB_p );
Type_uByte		wubSPEExecuteOnNow( SProcCTB_ts *astProcCTB_p );

#endif
