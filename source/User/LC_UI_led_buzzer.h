/**
*	@file	LC_UI_led_buzzer.h
*	@date	09/16/2020
*	@version	1.0.0
*
*/

/*!
 * 	@defgroup	LC_UI_led_buzzer
 *	@brief
 *	@{*/

#ifndef	LC_UI_LED_BUZZER_H_
#define	LC_UI_LED_BUZZER_H_
/*------------------------------------------------------------------*/
/*						C++ guard macro								*/
/*------------------------------------------------------------------*/
#ifdef	__cplusplus
extern	"C"	{
#endif
/*------------------------------------------------------------------*/
/*						head files include 							*/
/*------------------------------------------------------------------*/
#include "LC_Common.h"
/*------------------------------------------------------------------*/
/*						Pins definitions							*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*						MACROS										*/
/*------------------------------------------------------------------*/
//	IR Keyboard User Number Type
#define		IR_KeyBoard_Type_24Keys		0x0384
/*------------------------------------------------------------------*/
/*						UI Task Events definitions					*/
/*------------------------------------------------------------------*/
#define		UI_EVENT_LEVEL1		0x0001
#define		DEAL_APP_DATA_EVT	0x0002
#define     RF_433M_CHECK_EVT   0x0004
#define     RF_433M_DEAL_EVT    0x0008
/*------------------------------------------------------------------*/
/*						Data structures								*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*						external variables							*/
/*------------------------------------------------------------------*/
extern uint8	LC_Ui_Led_Buzzer_TaskID;
extern  volatile uint32 LC_IR_Analysis_100ns_Cnt;
extern  volatile uint32 LC_IR_Analysis_KeyValue;
/*------------------------------------------------------------------*/
/*						User function prototypes					*/
/*------------------------------------------------------------------*/
void 	LC_UI_Led_Buzzer_Task_Init			(uint8 task_id);
uint16	LC_UI_Led_Buzzer_ProcessEvent		(uint8 task_id, uint16 events);


#ifdef	__cplusplus
}
#endif

#endif	/*	LC_UI_LED_BUZZER_H_	*/
/**	@}*/
