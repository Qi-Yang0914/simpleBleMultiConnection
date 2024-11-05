/**
*	@file	LC_Key.h
*	@date	09/17/2020
*	@version	1.0.0
*
*/

/*!
 * 	@defgroup	LC_Key
 *	@brief
 *	@{*/

#ifndef LC_KEY_H_
#define LC_KEY_H_

/*------------------------------------------------------------------*/
/*						C++ guard macro								*/
/*------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif
/*------------------------------------------------------------------*/
/*						head files include 							*/
/*------------------------------------------------------------------*/
#include "LC_Common.h"
/*------------------------------------------------------------------*/
/*						Pins definitions							*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*						UI Task Events definitions					*/
/*------------------------------------------------------------------*/
#define		KEY_EVENT_LEVEL1	0x0001
#define		KEY_SCANF_EVT		0x0002
#define		KEY_STOPSCANF_EVT	0x0004
#define		KEY_CHARG_CHECK_EVT	0x0008

/*------------------------------------------------------------------*/
/*						Data structures								*/
/*------------------------------------------------------------------*/
typedef struct
{
	uint32 key_down_sys_tick; //	system time when key down
	uint8 key_down_flag;      //	1:key down	0:key release
	uint8 key_repeated_num;   //	times of key down
} lc_key_struct_data;

/*------------------------------------------------------------------*/
/*						external variables							*/
/*------------------------------------------------------------------*/
extern uint8 LC_Key_TaskID;
extern lc_key_struct_data LC_Key_Param;
/*------------------------------------------------------------------*/
/*						User function prototypes					*/
/*------------------------------------------------------------------*/
void	LC_Key_Task_Init(uint8 task_id);
uint16	LC_Key_ProcessEvent(uint8 task_id, uint16 events);

#ifdef __cplusplus
}
#endif

#endif /* LC_KEY_H_ */
/** @}*/
