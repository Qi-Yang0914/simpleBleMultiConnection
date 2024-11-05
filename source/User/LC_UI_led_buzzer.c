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
 
#include "LC_UI_Led_Buzzer.h"
#include "LC_Event_Handler.h"
/*------------------------------------------------------------------*/
/* 					 	local variables			 					*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/* 					 	public variables		 					*/
/*------------------------------------------------------------------*/
uint8	LC_Ui_Led_Buzzer_TaskID;

/*------------------------------------------------------------------*/
/* 					 	local functions			 					*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/* 					 	public functions		 					*/
/*------------------------------------------------------------------*/


/*!
 *	@fn			LC_UI_Led_Buzzer_Task_Init 
 *	@brief		Initialize function for the UI_LED_BUZZER Task. 
 *	@param[in]	task_id			:the ID assigned by OSAL,
 *								used to send message and set timer.
 *	@retrurn	none.
 */
void LC_UI_Led_Buzzer_Task_Init(uint8 task_id)
{
	LC_Ui_Led_Buzzer_TaskID	=	task_id;
}
/*!
 *	@fn			LC_UI_Led_Buzzer_ProcessEvent
 *	@brief		UI_LED_BUZZER Task event processor.This function
 *				is called to processs all events for the task.Events
 *				include timers,messages and any other user defined events.
 *	@param[in]	task_id			:The OSAL assigned task ID.
 *	@param[in]	events			:events to process.This is a bit map and can
 *									contain more than one event.
 */
uint16	LC_UI_Led_Buzzer_ProcessEvent(uint8 task_id, uint16 events)
{
	VOID task_id;	// OSAL required parameter that isn't used in this function
	if(events & SYS_EVENT_MSG)
	{
		uint8	*pMsg;
		if((pMsg = osal_msg_receive(LC_Ui_Led_Buzzer_TaskID)) != NULL)
		{
			LC_Common_ProcessOSALMsg((osal_event_hdr_t *)pMsg);
            // Release the OSAL message
			VOID osal_msg_deallocate(pMsg);
		}
		return(events ^ SYS_EVENT_MSG);
	}

	if(events & UI_EVENT_LEVEL1)
	{
		if(LC_Dev_System_Param.dev_power_flag == SYSTEM_WORKING)
		{
			osal_start_timerEx(LC_Ui_Led_Buzzer_TaskID, UI_EVENT_LEVEL1, 1*1000);
		}
		else if(LC_Dev_System_Param.dev_power_flag == SYSTEM_POWEROFF)
		{
			LC_Dev_Poweroff();
		}
		return(events ^ UI_EVENT_LEVEL1);
	}

	if(events & DEAL_APP_DATA_EVT)
	{
		LOG("APP Send Data\n");
		return(events ^ DEAL_APP_DATA_EVT);
	}

    // Discard unknown events
    return 0;
}
/** @}*/

