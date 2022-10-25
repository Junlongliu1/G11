/****************************************************************************
 *   FileName    : compile_switch.h
 *   Description : compile switch
 ****************************************************************************/
#ifndef  _COMPILE_SWITCH_H_
#define  _COMPILE_SWITCH_H_

#define COMPILE_SWITCH_8INCH				(0u)	/* 8.0 inch */
#define COMPILE_SWITCH_10INCH				(1u)	/* 10.25 inch */

#define COMPILE_SWITCH_DISP_SIZ COMPILE_SWITCH_10INCH

/*************************Sample Start**********************************************
#if (COMPILE_SWITCH_DISP_SIZE == COMPILE_SWITCH_8INCH)
	//8inch Source
#else
	//10inch Source
#endif

#if (COMPILE_SWITCH_DISP_SIZE == COMPILE_SWITCH_10INCH)
	//10inch Source
#else
	//8inch Source
#endif
*************************Sample End**********************************************/

#endif /* _COMPILE_SWITCH_H_ */
