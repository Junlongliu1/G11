/*********************************************************************************************************************
*	File Name		: ProEng.c
*	Date			: 2000.09.12
*	Author			: ?/APN
*	Model			: CDA7878A/J/R
*	Description		: Process Engine
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*
**********************************************************************************************************************/


/**********************************************************************************************************************
*	Include File Section
**********************************************************************************************************************/
#include "apn_basic_type.h"
#include	"ProEng.h"


/**********************************************************************************************************************
*	Macro Definition Section
**********************************************************************************************************************/
/*--------------------------------------------------------------------------------------------------------------------
*		No exectuing process, used by ProcCTB_ts.ucNumOfActiveBuf
*-------------------------------------------------------------------------------------------------------------------*/
#define		NO_EXECUTE_PROCESS		0xffU
/*--------------------------------------------------------------------------------------------------------------------
*		Child Process Mask Data
*-------------------------------------------------------------------------------------------------------------------*/
#define		NOT_PE_HAVE_CHILD_MASK	0x7fffU		/** Child Process Bit Mask **/

/*--------------------------------------------------------------------------------------------------------------------
*		On-time executing Mask Data
*-------------------------------------------------------------------------------------------------------------------*/
#define		NOT_PE_CONTINUE_MASK	0x7fU		/** On-time executing Bit Mask **/

/**********************************************************************************************************************
*	User-type Declaration Section
**********************************************************************************************************************/
/**********************************************************************************************************************
*	Structure Declaration Section
**********************************************************************************************************************/
/**********************************************************************************************************************
*	Global Variable Definition Section
**********************************************************************************************************************/
/**********************************************************************************************************************
*	File Static Variable Definition Section
**********************************************************************************************************************/
/**********************************************************************************************************************
*	Prototype Declaration Section
**********************************************************************************************************************/
/**********************************************************************************************************************
*	Debug Switch Definition Section
**********************************************************************************************************************/

/**********************************************************************************************************************
*	Function Name				: wucPESetUp
*	Date						: 2000.09.12
*	Description					: Process Control Block Setup
*	Return Value				: 0 (current)
*								: Reference No. (future)
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	:
*	Called by					:
*	Author						:
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*
*
**********************************************************************************************************************/
Type_uByte	wubPESetUp( ProcCTB_ts 	*astProcCTB_p ,
						ProcBuf_ts 	*astProcBuf_p ,
						const ElementOfProcTbl_ts *const*astProcTblSAT_p ,
						Type_uByte (*const*astSubSAT_p)(void) )

{
	astProcCTB_p->ubReqOfExecution = FALSE;					/** Execute Requirement **/
	astProcCTB_p->ubNumOfActiveBuf = NO_EXECUTE_PROCESS;	/** Executing Process Buffer's Layer **/
	astProcCTB_p->ProcBuf_p = astProcBuf_p;					/** Pointer to Process Buffer Header **/
	astProcCTB_p->ProcTblSAT_p = astProcTblSAT_p;			/** Pointer to Process Table Address Table **/
	astProcCTB_p->SubSAT_p = astSubSAT_p;					/** Pointer to Subroutine Address Table **/


	return 0;
}


/**********************************************************************************************************************
*	Function Name				: wvdPESetReq
*	Date						: 2000.09.12
*	Description					: Set Process Execution Requirement only
*								  For modules having more than one processes, use this function with
*								  wucPEExecute when call up Process Execute
*	Return Value				:
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	:
*	Called by					:
*	Author						:
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*
*
**********************************************************************************************************************/
void	wvdPESetReq( ProcCTB_ts *astProcCTB_p )
{
	if ( astProcCTB_p->ubNumOfActiveBuf != NO_EXECUTE_PROCESS ) {
		astProcCTB_p->ubReqOfExecution = TRUE;
	}
}

/**********************************************************************************************************************
*	Function Name				: wvdPEResetReq
*	Date						: 2000.09.12
*	Description					: Clear Process Execution Requirement only
*	Return Value				:
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	:
*	Called by					:
*	Author						:
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*
*
**********************************************************************************************************************/
void	wvdPEResetReq( ProcCTB_ts *astProcCTB_p )
{
	astProcCTB_p->ubReqOfExecution = FALSE;
}

/**********************************************************************************************************************
*	Function Name				: wvdPESetProcess
*	Date						:
*
*	Description					: Set Process execution requirement.
*								  Interrupt executing process and set new process.
*	Return Value				:
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	:
*	Called by					:
*	Author						:
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*
*
**********************************************************************************************************************/
void	wvdPESetProcess( Type_uHWord auhProcID , ProcCTB_ts *astProcCTB_p )
{

	ProcBuf_ts *astProcBuf_p;

	astProcCTB_p->ubReqOfExecution = TRUE;			/** Execution Requirement **/
	astProcCTB_p->ubNumOfActiveBuf = 0;				/** Executing Process Buffer's Layer **/

	astProcBuf_p = astProcCTB_p->ProcBuf_p;
	astProcBuf_p->uhProcessID = auhProcID;			/* Process Set */
	astProcBuf_p->ubExecStageNum = 0;				/* Executing Stage Number */
}

/**********************************************************************************************************************
*	Function Name				: wvdPEResetProcess
*	Date						:
*	Description					: Force exit of Process
*	Return Value				:
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	:
*	Called by					:
*	Author						:
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*
*
**********************************************************************************************************************/
void	wvdPEResetProcess( ProcCTB_ts *astProcCTB_p )
{
	astProcCTB_p->ubReqOfExecution = FALSE;					/** Execution Requirement **/
	astProcCTB_p->ubNumOfActiveBuf = NO_EXECUTE_PROCESS;	/** Executing Process Buffer's Layer **/
}

/**********************************************************************************************************************
*	Function Name				: wucPERestart
*	Date						:
*	Description					: Initialize executing process, return to initial status.
*								  That is, re-execute start.
*	Return Value				: Re-executed: TRUE
								  Not re-executed: FALSE
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	:
*	Called by					:
*	Author						:
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*
*
**********************************************************************************************************************/
Type_uByte wubPERestart( ProcCTB_ts *astProcCTB_p )
{
	Type_uByte aubReturn = FALSE;

	if ( astProcCTB_p->ubNumOfActiveBuf != NO_EXECUTE_PROCESS ) {
		wvdPESetProcess( (astProcCTB_p->ProcBuf_p)->uhProcessID , astProcCTB_p );
		aubReturn = TRUE;
	}
	return aubReturn;
}

/**********************************************************************************************************************
*	Function Name				: wvdPEIsExecuting
*	Date						:
*	Description					: check whether process is on execution
*	Return Value				: TRUE if process is on execution
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	:
*	Called by					:
*	Author						:
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*
*
**********************************************************************************************************************/
Type_uByte	wubPEIsExecuteing( ProcCTB_ts *astProcCTB_p )
{
	Type_uByte	aubReturn = FALSE;

	if ( astProcCTB_p->ubNumOfActiveBuf != NO_EXECUTE_PROCESS ) {
		aubReturn = TRUE;
	}

	return aubReturn;
}

/**********************************************************************************************************************
*	Function Name				: wucPEExecute
*	Date						:
*	Description					: Process execution
*	Return Value				: depends on Subroutine's end code
*									Subroutine end code				Return value
*										PE_SEC0					Process Control Table's RETURN0
*										PE_SEC1					Process Control Table's RETURN1
*										PE_SEC_CONTINUE			PERC_STAGE_CONTINUE
*									Process is not exectued:
*										PERC_NO_EXECUTE_PROC
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	:
*	Called by					:
*	Author						:
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.	Date		Revised by			Modification
*
*
**********************************************************************************************************************/
Type_uByte	wubPEExecute( ProcCTB_ts *astProcCTB_p )
{
	Type_uByte	aubReturn = PERC_NO_EXECUTE_PROC;

	if ( astProcCTB_p->ubReqOfExecution == TRUE ) {
		aubReturn = wubPEExecuteOnNow( astProcCTB_p );
	}

	return	aubReturn;
}

/**********************************************************************************************************************
*	Function Name				: wucPEExecuteOnNow
*	Date						: 2000.09.12
*	Description					: Process Execution
*	Return Value				: 
*									Subroutine End Code			Return Value
*									PE_SEC0					Process Control Table's RETURN0
*									PE_SEC1					Process Control Table's RETURN1
*									PE_SEC_CONTINUE			PE_SEC_CONTINUE
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	:
*	Called by					:
*	Author						:
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*
*
**********************************************************************************************************************/

Type_uByte	wubPEExecuteOnNow( ProcCTB_ts *astProcCTB_p )
{
	ProcBuf_ts				*astProcBuf_p;
	ElementOfProcTbl_ts		const*astElementOfProcTbl_p;
	Type_uByte				aubReturn = PERC_NO_EXECUTE_PROC;
	Type_uHWord				auhSubrutineID;
	Type_uByte				aubProcEndCode;
	Type_uByte				aubWork;
	Type_uByte				aubLoopCheck;
	Type_uByte				aubContinueCheck = TRUE;
	astProcCTB_p->ubReqOfExecution = FALSE;
	if ( astProcCTB_p->ubNumOfActiveBuf != NO_EXECUTE_PROCESS ) {

		astProcBuf_p = (astProcCTB_p->ProcBuf_p) + (astProcCTB_p->ubNumOfActiveBuf);

		while ( aubContinueCheck == TRUE ) {
			/** GET Process Table's Element Pointer **/
			aubLoopCheck = TRUE;
			while( aubLoopCheck == TRUE) {

				astElementOfProcTbl_p = *(( astProcCTB_p->ProcTblSAT_p ) + ( astProcBuf_p->uhProcessID ));
				astElementOfProcTbl_p += astProcBuf_p->ubExecStageNum;

				auhSubrutineID = astElementOfProcTbl_p->uhIndex;

				if	( ( auhSubrutineID & PE_HAVE_CHILD ) != 0 ) {
					/** Have Child Process			**/
					/** Process Buffer down one layer **/

					astProcCTB_p->ubNumOfActiveBuf += 1;
					astProcBuf_p++;
					astProcBuf_p->uhProcessID = ( auhSubrutineID & NOT_PE_HAVE_CHILD_MASK );		/** Process ID **/
					astProcBuf_p->ubExecStageNum = STAGE0;											/** Executing Stage No. **/
				}
				else {
					/**No Child Process **/
					aubLoopCheck = FALSE;
				}
			}

			/** Subroutine Execute **/
			aubReturn = (*(*((astProcCTB_p->SubSAT_p) + auhSubrutineID )))();

			/** Handle Subroutine End Code **/

			if ( aubReturn == PE_SEC_CONTINUE ) {
				aubReturn = PERC_STAGE_CONTINUE;
				aubContinueCheck = FALSE;
			}
			else {

				if ( aubReturn == PE_SEC0 ) {
					aubProcEndCode = astElementOfProcTbl_p->ubNextStageBy0;
					aubReturn = astElementOfProcTbl_p->ubReturnValueBy0;
				}
				else {
					aubProcEndCode = astElementOfProcTbl_p->ubNextStageBy1;
					aubReturn = astElementOfProcTbl_p->ubReturnValueBy1;
				}

				aubLoopCheck = TRUE;
				while ( aubLoopCheck == TRUE ) {

					aubWork = ( aubProcEndCode & ((Type_uByte)NOT_PE_CONTINUE_MASK) );
					if ( ( aubWork == PE_PEC0 ) || ( aubWork == PE_PEC1 ) ) {
						
						/** Process ends **/
						
						if ( astProcCTB_p->ubNumOfActiveBuf == 0 ) {
							
							/** Upper most process ends **/
							
							astProcCTB_p->ubNumOfActiveBuf = NO_EXECUTE_PROCESS;
							aubLoopCheck = FALSE;
							aubContinueCheck = FALSE;
						}
						else {
							
							/** Child process ends **/
							
							astProcCTB_p->ubNumOfActiveBuf -= 1;
							astProcBuf_p--;
							astElementOfProcTbl_p = *(( astProcCTB_p->ProcTblSAT_p ) + ( astProcBuf_p->uhProcessID ));
							astElementOfProcTbl_p += astProcBuf_p->ubExecStageNum;

							if ( aubWork == PE_PEC0 ) {

								aubProcEndCode = astElementOfProcTbl_p->ubNextStageBy0;
								aubReturn = astElementOfProcTbl_p->ubReturnValueBy0;
							}
							else {

								aubProcEndCode = astElementOfProcTbl_p->ubNextStageBy1;
								aubReturn = astElementOfProcTbl_p->ubReturnValueBy1;
							}
						}
					}
					else {

						/** Process Continue **/
						
						astProcBuf_p->ubExecStageNum = aubWork;
						aubLoopCheck = FALSE;

						aubContinueCheck = TRUE;
						if ( ( aubProcEndCode & PE_CONTINUE ) == 0 ) {
							aubContinueCheck = FALSE;
						}
					}
				}
			}
		}
	}
	return aubReturn;
}

/**********************************************************************************************************************
*	Function Name				: wucSPESetUp
*	Date						: 2000.09.12
*	Description					: Process Management Structure Setup (simple version)
*	Return Value				: 0
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	:
*	Called by					:
*	Author						:
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*
*
**********************************************************************************************************************/
Type_uByte	wubSPESetUp( SProcCTB_ts 	*astProcCTB_p ,
						 ProcBuf_ts 	*astProcBuf_p ,
						 const ElementOfSProcTbl_ts *const*astProcTblSAT_p ,
    	                 Type_uByte (*const*astSubSAT_p)(void) )
{
	astProcCTB_p->ubReqOfExecution = FALSE;					/** Execution Requirement **/
	astProcCTB_p->ubNumOfActiveBuf = NO_EXECUTE_PROCESS;	/** Executing Process Buffer's layer **/
	astProcCTB_p->ProcBuf_p = astProcBuf_p;					/** Pointer to Process Buffer **/
	astProcCTB_p->ProcTblSAT_p = astProcTblSAT_p;			/** Pointer to Process Table Address Table **/
	astProcCTB_p->SubSAT_p = astSubSAT_p;					/** Pointer to Subroutine Address Table **/

	return 0;
}


/**********************************************************************************************************************
*	Function Name				: wvdSPESetReq
*	Date						:
*	Description					: Set Process Execution Requirement only (simple version)
*	Return Value				:
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	:
*	Called by					:
*	Author						:
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*
*
**********************************************************************************************************************/
void	wvdSPESetReq( SProcCTB_ts *astProcCTB_p )
{
	if ( astProcCTB_p->ubNumOfActiveBuf != NO_EXECUTE_PROCESS ) {
		astProcCTB_p->ubReqOfExecution = TRUE;
	}
}

/**********************************************************************************************************************
*	Function Name				: wvdSPEResetReq
*	Date						: 2000.09.12
*	Description					: Process Execution Requirement clear only (simple version)
*	Return Value				:
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	:
*	Called by					:
*	Author						:
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*
*
**********************************************************************************************************************/
void	wvdSPEResetReq( SProcCTB_ts *astProcCTB_p )
{
	astProcCTB_p->ubReqOfExecution = FALSE;
}

/**********************************************************************************************************************
*	Function Name				: wvdSPESetProcess
*	Date						: 2000.09.12
*	Description					: Process Start(simple version)
*	Return Value				:
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	:
*	Called by					:
*	Author						:
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*
*
**********************************************************************************************************************/
void	wvdSPESetProcess( Type_uHWord auhProcID , SProcCTB_ts *astProcCTB_p )
{
	ProcBuf_ts			*astProcBuf_p;

	astProcCTB_p->ubReqOfExecution = TRUE;					/** Execution Requirement **/
	astProcCTB_p->ubNumOfActiveBuf = 0;						/** Executing Process Buffer's layer **/

	astProcBuf_p = astProcCTB_p->ProcBuf_p;
	astProcBuf_p->uhProcessID = auhProcID;
	astProcBuf_p->ubExecStageNum = STAGE0;
}

/**********************************************************************************************************************
*	Function Name				: wvdSPEResetProcess
*	Date						: 2000.09.12
*	Description					: Force exit Process (simple version)
*	Return Value				:
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	:
*	Called by					:
*	Author						:
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*
*
**********************************************************************************************************************/
void	wvdSPEResetProcess( SProcCTB_ts *astProcCTB_p )
{

	astProcCTB_p->ubReqOfExecution = FALSE;						/** Execution Requirement **/
	astProcCTB_p->ubNumOfActiveBuf = NO_EXECUTE_PROCESS;		/** Executing Process Buffer's layer **/
}

/**********************************************************************************************************************
*	Function Name				: wucSPERestart
*	Date						: 2000.09.12
*
*	Description					: Initialize executing process(simple version)
*	Return Value				: TRUE (executed OK)
*								  FALSE (not executed)
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	:
*	Called by					:
*	Author						:
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*
*
**********************************************************************************************************************/
Type_uByte wubSPERestart( SProcCTB_ts *astProcCTB_p )
{
	Type_uByte aubReturn = FALSE;

	if ( astProcCTB_p->ubNumOfActiveBuf != NO_EXECUTE_PROCESS ) {	/*Modify by wangzhe for Pclint	2005-6-27 20:36*/

		wvdSPESetProcess( (astProcCTB_p->ProcBuf_p)->uhProcessID , astProcCTB_p );
		aubReturn = TRUE;
	}
	return aubReturn;
}
/**********************************************************************************************************************
*	Function Name				: wucSPEIsExecuting
*	Date						: 2000.09.12
*
*	Description					: Check whether process is executing(simple version)
*
*	Return Value				: TRUE if executing
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	:
*	Called by					:
*	Author						:
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*
*
**********************************************************************************************************************/
Type_uByte	wubSPEIsExecuteing( SProcCTB_ts *astProcCTB_p )
{
	Type_uByte	aubReturn = FALSE;

	if ( astProcCTB_p->ubNumOfActiveBuf != NO_EXECUTE_PROCESS ) {
		aubReturn = TRUE;
	}

	return aubReturn;
}

/**********************************************************************************************************************
*	Function Name				: wucSPEExecute
*	Date						: 2000.09.12
*
*	Description					: Process Execute(simple version)
*
*	Return Value				: 
*									
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	:
*	Called by					:
*	Author						:
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*
*
**********************************************************************************************************************/
Type_uByte	wubSPEExecute( SProcCTB_ts *astProcCTB_p )
{
	Type_uByte	aubReturn = PERC_NO_EXECUTE_PROC;

	if ( astProcCTB_p->ubReqOfExecution == TRUE ) {

		aubReturn = wubSPEExecuteOnNow( astProcCTB_p );
	}

	return	aubReturn;
}

/**********************************************************************************************************************
*	Function Name				: wucSPEExecuteOnNow
*	Date						:
*
*	Description					: Process Execution(simple version)
*								  Execute process no matter there is Execute Requirement or not.
*	Return Value				:
*									[Subroutine End Code]	[Return Value]
*									PE_SEC0					Process Control Table's RETURN0
*									PE_SEC1					Process Control Table's RETURN1
*									PE_SEC_CONTINUE			PE_SEC_CONTINUE
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	:
*	Called by					:
*	Author						:
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.	Date		Revised by			Modification
*
*
**********************************************************************************************************************/
Type_uByte	wubSPEExecuteOnNow( SProcCTB_ts *astProcCTB_p )
{
	ProcBuf_ts				*astProcBuf_p;
	ElementOfSProcTbl_ts	const*astElementOfProcTbl_p;
	Type_uByte				aubReturn = PERC_NO_EXECUTE_PROC;
	Type_uHWord				auhSubrutineID;
	Type_uByte				aubProcEndCode;
	Type_uByte				aubWork;
	Type_uByte				aubLoopCheck;
	Type_uByte				aubContinueCheck = TRUE;

	astProcCTB_p->ubReqOfExecution = FALSE;

	if ( astProcCTB_p->ubNumOfActiveBuf != NO_EXECUTE_PROCESS ) {

		astProcBuf_p = (astProcCTB_p->ProcBuf_p) + (astProcCTB_p->ubNumOfActiveBuf);

		while ( aubContinueCheck == TRUE ) {

			/** GET Process Table's Element Pointer **/
			aubLoopCheck = TRUE;
			while( aubLoopCheck == TRUE) {

				astElementOfProcTbl_p = *(( astProcCTB_p->ProcTblSAT_p ) + ( astProcBuf_p->uhProcessID ));
				astElementOfProcTbl_p += astProcBuf_p->ubExecStageNum;

				auhSubrutineID = astElementOfProcTbl_p->uhIndex;

				if	( ( auhSubrutineID & PE_HAVE_CHILD ) != 0 ) {
					/** Have Child Process	**/
					/** Process Buffer 1 layer down **/

					astProcCTB_p->ubNumOfActiveBuf += 1;
					astProcBuf_p++;
					astProcBuf_p->uhProcessID = ( auhSubrutineID & NOT_PE_HAVE_CHILD_MASK );	/**	Process ID **/
					astProcBuf_p->ubExecStageNum = STAGE0;										/** Executing Stage No. **/
				}
				else {
					/** No Child Process **/
					aubLoopCheck = FALSE;
				}
			}

			/** Execute Subroutine **/
			/** At this time: astElementOfProcTbl_p, aucSubrutineID, hold control table & Subroutine ID **/
			
			aubReturn = (*(*((astProcCTB_p->SubSAT_p) + auhSubrutineID )))();

			/** handling Subroutine End Code **/

			if ( aubReturn == PE_SEC_CONTINUE ) {
				aubReturn = PERC_STAGE_CONTINUE;
				aubContinueCheck = FALSE;
			}
			else {

				aubLoopCheck = TRUE;
				while ( aubLoopCheck == TRUE ) {

					aubProcEndCode = astElementOfProcTbl_p->ubNextStage;
					aubReturn = astElementOfProcTbl_p->ubReturnValue;

					aubWork = ( aubProcEndCode & ((Type_uByte)NOT_PE_CONTINUE_MASK) );
					if ( ( aubWork == PE_PEC0 ) || ( aubWork == PE_PEC1 ) ) {

						/** Process end **/
						if ( astProcCTB_p->ubNumOfActiveBuf == 0 ) {
							/** Upper process ends **/
							astProcCTB_p->ubNumOfActiveBuf = NO_EXECUTE_PROCESS;
							aubLoopCheck = FALSE;
							aubContinueCheck = FALSE;
						}
						else {
							/** Child Process ends **/
							astProcCTB_p->ubNumOfActiveBuf -= 1;
							astProcBuf_p--;
							astElementOfProcTbl_p = *(( astProcCTB_p->ProcTblSAT_p ) + ( astProcBuf_p->uhProcessID ));
							astElementOfProcTbl_p += astProcBuf_p->ubExecStageNum;
						}
					}
					else {
						/** Process contine **/
						astProcBuf_p->ubExecStageNum = aubWork;
						aubLoopCheck = FALSE;

						aubContinueCheck = TRUE;
						if ( ( aubProcEndCode & PE_CONTINUE ) == 0 ) {
							aubContinueCheck = FALSE;
						}
					}
				}
			}
		}
	}
	return aubReturn;
}



