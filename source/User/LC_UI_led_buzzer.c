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
#include "LC_Uart.h"
#include "multiRoleProfile.h"
/*------------------------------------------------------------------*/
/* 					 	local variables			 					*/
/*------------------------------------------------------------------*/
static	uint8	LC_RF433_Key_Press_Flag	=	0;
static	uint8	deal_rf433_key = 0;
/*------------------------------------------------------------------*/
/* 					 	public variables		 					*/
/*------------------------------------------------------------------*/
uint8	LC_Ui_Led_Buzzer_TaskID;
volatile uint32 LC_IR_Analysis_100ns_Cnt = 0;
volatile uint32 LC_IR_Analysis_KeyValue = 0;

/*------------------------------------------------------------------*/
/* 					 	local functions			 					*/
/*------------------------------------------------------------------*/
static	void	LC_RF433M_Deal_Key(void)
{
	static uint32 per_data;
	static uint8 data_cnt;
	LOG("RF 433M CMD 0x%8x\n", LC_433m_Data.key_data);
	data_cnt++;
	if(data_cnt >= 3)
	{
		if(per_data == 0)
		{
			per_data = (LC_433m_Data.key_data >> 4);
		}
		else
		{
			if(per_data == (LC_433m_Data.key_data >> 4))
			{
				if(data_cnt > 6)
				{
					LOG("learn finish\n");
				}
			}
			else
			{
				if(data_cnt > 5)
				{
					LOG("learn again\n");
					data_cnt = 0;
					per_data = 0;
				}
			}
		}
	}

	// uint16	LC_IR_Keyboard_Num		=	0; 
	// uint16	LC_IR_Keyboard_UserNum	=	0;
	
	// LC_IR_Keyboard_Num		=	(uint16)((LC_433m_Data.key_data) & 0x000000ff);
	// LC_IR_Keyboard_UserNum	=	(uint16)((LC_433m_Data.key_data >> 8) & 0x0000ffff);
	// LOG("ir analysis key 0x%08x 0x%08x\n",LC_IR_Keyboard_UserNum, LC_IR_Keyboard_Num);
	// if(LC_IR_Keyboard_UserNum == IR_KeyBoard_Type_24Keys)
	// {
	// 	if(deal_rf433_key == 0)
	// 	{
	// 		deal_rf433_key	=	1;
	// 		osal_start_timerEx(LC_Ui_Led_Buzzer_TaskID, RF_433M_DEAL_EVT, 300);
	// 	}
	// 	else
	// 	{
	// 		return;
	// 	}
	// 	LOG("ir analysis key 0x%08x 0x%08x\n",LC_IR_Keyboard_UserNum, LC_IR_Keyboard_Num);
	// }
	
}
/**
 * @brief	Decode 433M serial
 * 
 * @param data 
 * @param high_or_low 
 */
static	void	LC_RF433_Analysis(uint16 data, uint8 high_or_low)
{

	static	uint32	l_433_data_count=	0;
	static	uint8	l_data_433_step	=	0;
	if(LC_RF433_Key_Press_Flag == 1)
	{
		if(!high_or_low)
		{
			if(data>=180)
			{
				LC_RF433_Key_Press_Flag	=	0;	//	rf key release
				// LOG("start\n");
			}
		}
		return;
	}

	switch(l_data_433_step)
	{
		case	0:
			if(high_or_low == 0 && data >= RF_START_CODE_L_MIN)
			{
				l_data_433_step	=	1;
				LC_433m_Data.key_data	=	0;
				l_433_data_count	=	0;
				// LOG("0\n");
			}
			else
			{
				l_data_433_step	=	0;
			}
		break;

		case	1:
			if(high_or_low == 1)
			{
				if(data >= RF_DATA_H_ONE_MIN && data <= RF_DATA_H_ONE_MAX)
				{
					LC_433m_Data.key_data	=	(LC_433m_Data.key_data << 1) |	0x01;
				}
				else if(data >= RF_DATA_H_ZERO_MIN && data <= RF_DATA_H_ZERO_MAX)
				{
					LC_433m_Data.key_data	=	(LC_433m_Data.key_data << 1) & (~0x01);
				}
				else
				{
					l_433_data_count	=	0;
					l_data_433_step		=	0;
				}
				// LOG("433 bit %d %x\n", l_433_data_count, LC_433m_Data.key_data);

				if(++l_433_data_count >= RF_DATA_COUNT)
				{
					l_433_data_count	=	0;
					l_data_433_step		=	0;
					if(LC_RF433_Key_Press_Flag == 0)
					{
						LC_RF433M_Deal_Key();
					}
				}
			}
		break;

		default:
		break;
	}
}
static	void	LC_RF433_Check_Serial(void)
{
	for(uint8 i = 0;i <= 50;i++)
	{
		if(LC_433m_Data.data_tail == LC_433m_Data.data_head)	break;
		LC_433m_Data.data_tail++;
		LC_433m_Data.data_tail %= 200;
		LC_RF433_Analysis(LC_433m_Data.time_span[LC_433m_Data.data_tail], LC_433m_Data.high_low[LC_433m_Data.data_tail]);
	}
}
/*------------------------------------------------------------------*/
/* 					 	public functions		 					*/
/*------------------------------------------------------------------*/
void RF_Start_Receive(void)
{
	hal_gpio_pin_init(GPIO_RF_433M, IE);
	hal_gpio_pull_set(GPIO_RF_433M, STRONG_PULL_UP);
	hal_gpioin_register(GPIO_RF_433M, LC_Gpio_IR_IntHandler, LC_Gpio_IR_IntHandler);

	LC_Timer_Start(TIME_EVT_LEARN);
	osal_start_timerEx(LC_Ui_Led_Buzzer_TaskID, RF_433M_CHECK_EVT, 100);
}

void RF_Stop_Receive(void)
{
	hal_gpio_pull_set(GPIO_RF_433M, FLOATING);
	hal_gpioin_register(GPIO_RF_433M, NULL, NULL);
	hal_gpioin_disable(GPIO_RF_433M);
	LC_Timer_Stop();
}

void RF_Start_Send(uint32 cmd)
{
	LC_Dev_System_Param.dev_rf_cmd = cmd;
	LC_Dev_System_Param.dev_rf_cmd_bit_index = 0;
	LC_Dev_System_Param.dev_rf_send_times = 50;
	LC_Dev_System_Param.dev_rf_send_index = 0;
	hal_gpioin_disable(GPIO_RF_433M);
	hal_gpio_pin_init(GPIO_RF_433M, OEN);
	LC_Timer_Start(TIME_EVT_SEND);

}

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
	BSP_Pin_Init();
	RF_Start_Receive();
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
		return(events ^ UI_EVENT_LEVEL1);
	}

	if(events & SNV_FS_DEAL_EVT)
	{
		if(LC_Dev_System_Param.dev_psk_flag == 2)
		{
			uint8 fs_buffer[8] = {0x99, 0x98,};
			osal_memcpy(fs_buffer + 2, LC_Dev_System_Param.dev_psk, 6);
			osal_snv_write(SNV_FS_ID_PSK, 8, fs_buffer);
			LC_Dev_System_Param.dev_psk_flag = 1;
			GAPMultiRole_TerminateConnection(0xffff);
		}
		return(events ^ SNV_FS_DEAL_EVT);
	}

	if(events & RF_433M_CHECK_EVT)
	{
		LC_RF433_Check_Serial();
		osal_start_timerEx(LC_Ui_Led_Buzzer_TaskID, RF_433M_CHECK_EVT, 50);
		return(events ^ RF_433M_CHECK_EVT);
	}

	if(events & RF_433M_DEAL_EVT)
	{
		if(deal_rf433_key == 1)
		{
			deal_rf433_key = 0;
		}
		return(events ^ RF_433M_DEAL_EVT);
	}


    // Discard unknown events
    return 0;
}
/** @}*/

