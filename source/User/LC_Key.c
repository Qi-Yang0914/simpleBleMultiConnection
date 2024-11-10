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
/*------------------------------------------------------------------*/
/*						head files include 							*/
/*------------------------------------------------------------------*/
#include "LC_Key.h"
#include "LC_UI_led_buzzer.h"
#include "LC_Event_Handler.h"
/*------------------------------------------------------------------*/
/* 					 	local variables			 					*/
/*------------------------------------------------------------------*/
static uint8 key_err = 0;
/*------------------------------------------------------------------*/
/* 					 	public variables		 					*/
/*------------------------------------------------------------------*/
uint8 LC_Key_TaskID;
lc_key_struct_data LC_Key_Param = {
    .key_down_sys_tick = 0,
    .key_down_flag = 0,
    .key_repeated_num = 0,

};
/*------------------------------------------------------------------*/
/* 					 	local functions			 					*/
/*------------------------------------------------------------------*/
static void LC_KeyScanf(void)
{
    uint8 lc_keynewvalue = 0;
    static uint8 lc_keyoldvalue = 0;

    if(hal_gpio_read(GPIO_KEY_PWR) == 0)
    {
        lc_keynewvalue = 1;
    }

    if (lc_keynewvalue)
    {
        if ((lc_keyoldvalue == 0) || (lc_keyoldvalue != lc_keynewvalue))
        {
            lc_keyoldvalue = lc_keynewvalue;
            LC_Key_Param.key_down_flag = lc_keynewvalue;
            osal_start_timerEx(LC_Key_TaskID, KEY_EVENT_LEVEL1, 10);
            osal_stop_timerEx(LC_Key_TaskID, KEY_STOPSCANF_EVT);
        }
		if(key_err == 0)
		{
			key_err	=	1;
		}
    }
    else
    {
        if (lc_keyoldvalue != 0)
        {
            lc_keyoldvalue = 0;
            LC_Key_Param.key_down_flag = 0;
            osal_start_timerEx(LC_Key_TaskID, KEY_EVENT_LEVEL1, 10);
            osal_start_timerEx(LC_Key_TaskID, KEY_STOPSCANF_EVT, 500);
        }
		else
		{
			if(key_err == 0)
			{
				LOG("key err\n");
				key_err	=	1;
				osal_start_timerEx(LC_Key_TaskID, KEY_STOPSCANF_EVT, 500);
			}
		}
    }
}

/*------------------------------------------------------------------*/
/* 					 	public functions		 					*/
/*------------------------------------------------------------------*/
/*!
 *	@fn			LC_Key_Task_Init 
 *	@brief		Initialize function for the KEY Task. 
 *	@param[in]	task_id		: 	the ID assigned by OSAL,
 *								used to send message and set timer.
 *	@return	none.
 */
void LC_Key_Task_Init(uint8 task_id)
{
    LC_Key_TaskID = task_id;
}
/*!
 *	@fn			LC_Key_ProcessEvent
 *	@brief		KEY Task event processor.This function
 *				is called to processs all events for the task.Events
 *				include timers,messages and any other user defined events.
 *	@param[in]	task_id			:The OSAL assigned task ID.
 *	@param[in]	events			:events to process.This is a bit map and can
 *									contain more than one event.
 */
uint16 LC_Key_ProcessEvent(uint8 task_id, uint16 events)
{
    VOID task_id; // OSAL required parameter that isn't used in this function
    if (events & SYS_EVENT_MSG)
    {
        uint8 *pMsg;
        if ((pMsg = osal_msg_receive(LC_Key_TaskID)) != NULL)
        {
            LC_Common_ProcessOSALMsg((osal_event_hdr_t *)pMsg);
            // Release the OSAL message
            VOID osal_msg_deallocate(pMsg);
        }
        return (events ^ SYS_EVENT_MSG);
    }
    if (events & KEY_EVENT_LEVEL1)
    {
        static	uint8	LC_last_button_pressed = 0;
        static	uint8	LC_last_button_numbale = 0;

        static	uint32	LC_last_button_press_time = 0;
        static	uint32	LC_last_button_release_time = 0;
        static	uint32	LC_key_time_temp	= 0;

		static	uint8	Key_Long_Press_3s_Enable	=	0;		//key pressed 3s once
		static	uint8	Key_Press_Once_Enable		=	0;		//key pressed once flag
		static	uint8 	Key_Long_Press_5s_Enable	=	0;
        LC_key_time_temp = hal_systick() | 1;

		if(LC_Key_Param.key_down_flag)
		{
			if((LC_last_button_numbale) && clock_time_exceed_func(LC_last_button_press_time,3000))
			{
				LC_Key_Param.key_repeated_num	=	0;
				if(Key_Long_Press_3s_Enable == 0)
				{
					Key_Long_Press_3s_Enable	=	1;
				}
				if(clock_time_exceed_func(LC_last_button_press_time,5000))
				{
					if(Key_Long_Press_5s_Enable == 0)
					{
						Key_Long_Press_5s_Enable = 1;
					}
				}
			}
		}
		else
		{
			if(Key_Long_Press_3s_Enable == 1)
			{
				Key_Long_Press_3s_Enable	=	0;
				Key_Press_Once_Enable		=	1;
				LOG("Key_Long_Release:\n");
			}
		}

        if (LC_Key_Param.key_down_flag)
        {
            if (!LC_last_button_pressed && clock_time_exceed_func(LC_last_button_release_time, 20))
            {
                LC_last_button_pressed = 1;
                LC_last_button_press_time = LC_key_time_temp;
                LC_last_button_numbale = LC_Key_Param.key_down_flag;
				if(Key_Press_Once_Enable == 0)
				{
					Key_Press_Once_Enable = 1;
					LOG("key press %d\n", LC_last_button_numbale);
				}
            }
        }
        else
        {
            if (LC_last_button_pressed && clock_time_exceed_func(LC_last_button_press_time, 20))
            {
                LC_last_button_release_time = LC_key_time_temp;
                LC_last_button_pressed = 0;
				if(Key_Press_Once_Enable == 1)
				{
					Key_Press_Once_Enable = 0;
					LOG("key release %d\n", LC_last_button_numbale);
				}
            }
        }
        if (LC_Key_Param.key_repeated_num && LC_Key_Param.key_down_sys_tick && clock_time_exceed_func(LC_Key_Param.key_down_sys_tick, 350))
        {
            LOG("Key total Kick num: %d\n", LC_Key_Param.key_repeated_num);
            LC_Key_Param.key_down_sys_tick = 0;
            LC_Key_Param.key_repeated_num = 0;
        }
        if (LC_last_button_numbale && !LC_Key_Param.key_down_flag && clock_time_exceed_func(LC_last_button_press_time, 50))
        {
            // LC_Key_Param.key_repeated_num++ ;
            LC_Key_Param.key_down_sys_tick = LC_key_time_temp;
            LOG("key time num: %d, key is%d\n", LC_Key_Param.key_repeated_num, LC_last_button_numbale);
            LC_last_button_numbale = 0;
        }
        if (LC_Key_Param.key_down_flag || LC_Key_Param.key_repeated_num)
        {
            osal_start_timerEx(LC_Key_TaskID, KEY_EVENT_LEVEL1, 20);
        }
        return (events ^ KEY_EVENT_LEVEL1);
    }

    if (events & KEY_SCANF_EVT)
    {
        osal_start_timerEx(LC_Key_TaskID, KEY_SCANF_EVT, 40);
        LC_KeyScanf();
        return (events ^ KEY_SCANF_EVT);
    }

    if (events & KEY_STOPSCANF_EVT)
    {
        LOG("stop scanf \n");
		key_err	=	0;
        hal_gpioin_register(GPIO_KEY_PWR, NULL, LC_Key_Pin_IntHandler);
        osal_stop_timerEx(LC_Key_TaskID, KEY_SCANF_EVT);
        return (events ^ KEY_STOPSCANF_EVT);
    }

    // Discard unknown events
    return 0;
}

/** @}*/
