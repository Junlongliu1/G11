/****************************************************************************
 *   FileName    : temp_compile_switch.h
 *   Description : 0.5work-1work compile switch
 ****************************************************************************/
#ifndef  _TEMP_COMPILE_SWITCH_H_
#define  _TEMP_COMPILE_SWITCH_H_

#define TEMP_COMPILE_SWITCH_05WORK				(0u)	/* 0.5work */
#define TEMP_COMPILE_SWITCH_10WORK				(1u)	/* 1.0work */

#define TEMP_COMPILE_SWITCH_05WORK_10WORK TEMP_COMPILE_SWITCH_10WORK


/*************************Sample Start**********************************************
#if (TEMP_COMPILE_SWITCH_05WORK_10WORK == TEMP_COMPILE_SWITCH_05WORK)
	//0.5work Source
#else
	//1work Source
#endif
*************************Sample End**********************************************/


#endif /* _TEMP_COMPILE_SWITCH_H_ */
