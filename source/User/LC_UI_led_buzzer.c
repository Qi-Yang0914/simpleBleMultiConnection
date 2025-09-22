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
#include "LC_AES128_ECB_CBC.h"
#include "multi_role.h"
/*------------------------------------------------------------------*/
/* 					 	local variables			 					*/
/*------------------------------------------------------------------*/
static uint8 i2c_add;
static uint8 i2c_write_cnt;
/*------------------------------------------------------------------*/
/* 					 	public variables		 					*/
/*------------------------------------------------------------------*/
uint8	LC_Ui_Led_Buzzer_TaskID;

/*------------------------------------------------------------------*/
/* 					 	local functions			 					*/
/*------------------------------------------------------------------*/
static void Write_24C02_Serial(uint8 address, uint8 cnt)
{
	i2c_add = address;
	i2c_write_cnt = cnt;
	osal_start_timerEx(LC_Ui_Led_Buzzer_TaskID, IIC_WRITE_EVT, 10);
}
/*------------------------------------------------------------------*/
/* 					 	public functions		 					*/
/*------------------------------------------------------------------*/
uint8 online_send_one_data(uint8 channel)
{
	if(LC_Dev_System_Param.dev_audio_send_flag == 0)
	{
		LC_Dev_System_Param.dev_audio_send_flag = 1;
		LC_Dev_System_Param.dev_audio_channel = channel;
		LC_Dev_System_Param.dev_audio_send_tick = 0;
		LC_Dev_System_Param.dev_channel_bit = 0;
		OTP_SEND_LOW();
		WaitMs(5);
		LC_Timer_Start();
		return PPlus_SUCCESS;
	}
	else
	{
		return PPlus_ERR_BUSY;
	}
}
void Output_Set_Time(uint8 second)
{
	LED_WRITE_STATUS(GPIO_LED_RED, LED_OFF);
	LED_WRITE_STATUS(GPIO_LED_GREEN, LED_ON);
	LED_WRITE_STATUS(GPIO_LED_BLUE, LED_OFF);
	OUTPUT_STATUS(1);
	osal_start_timerEx(LC_Ui_Led_Buzzer_TaskID, OUTPUT_TIMEOUT_EVT, second*1000);
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

	if(events & REMOTE_NOTI_EVT)
	{
		uint8 data[5];
		AES128_ECB_decrypt(LC_Dev_System_Param.Role_Master.remote_notify+3, AES128_MiKey, LC_Dev_System_Param.Role_Master.remote_notify+3);
		LOG("og data\n");
		LOG_DUMP_BYTE(LC_Dev_System_Param.Role_Master.remote_notify, 20);
		if(find_key_UUID(LC_Dev_System_Param.Role_Master.remote_notify+7, LC_Dev_System_Param.dev_UUID_Buffer[0]) < UUID_MAX_NUM)
		{
			LOG("check pass\n");
			data[0] = 0xBB;
			data[1] = 0x02;
			data[2] = 0x81;
			data[3] = 0x00;
			data[4] = checksum(data+1, 3);
			online_send_one_data(1);
			Output_Set_Time(LC_Dev_System_Param.dev_infrared_outpu_time);
		}
		else
		{
			data[0] = 0xBB;
			data[1] = 0x02;
			data[2] = 0x81;
			data[3] = 0x04;
			data[4] = checksum(data+1, 3);
			online_send_one_data(2);
		}
		Master_Write_Slave(LC_Dev_System_Param.Role_Master.remote_connHandle, data, 5);
		return(events ^ REMOTE_NOTI_EVT);
	}

	if(events & APP_DATA_EVT)
	{
		uint8 *app_data;
		uint8 noti_len;
		app_data = LC_Dev_System_Param.Role_Slave.app_write_data;
		LOG("APP data\n");
		LOG_DUMP_BYTE(app_data, LC_Dev_System_Param.Role_Slave.app_write_len);
		if(app_data[0] == 0xCC)
		{
			app_data[0] = 0xAA;
			switch(app_data[2])
			{
				case 0x02:
					app_data[1] = 7;
					app_data[2] = 0x82;
					osal_memcpy(app_data + 3, LC_Dev_System_Param.dev_ble_mac, 6);
					app_data[9] = checksum(app_data+1, 8);
					noti_len = 10;
				break;
				case 0x03:
					app_data[1] = 9;
					app_data[2] = 0x83;
					osal_memcpy(app_data+3, LC_Dev_System_Param.dev_UUID, 8);
					app_data[11] = checksum(app_data+1, 10);
					noti_len = 12;
				break;

				case 0x04:
					if((osal_memcmp(DEFAULT_ADMIN_KEY, app_data+3, 4)) || (osal_memcmp(LC_Dev_System_Param.dev_cur_admin_key, app_data+3, 4)))
					{
						AES128_ECB_decrypt(app_data+7, AES128_MiKey, app_data+7);
						LOG("decrypt data\n");
						LOG_DUMP_BYTE(app_data+7, 16);
						if(find_key_UUID(app_data+11, LC_Dev_System_Param.dev_UUID_Buffer[0]) < UUID_MAX_NUM)
						{
							online_send_one_data(1);
							Output_Set_Time(app_data[24]);
							app_data[3] = PPlus_SUCCESS;
						}
						else
						{
							app_data[3] = ERR_DATA;
						}
					}
					else
					{
						app_data[3] = ERR_FUNCODE;
					}
					app_data[1] = 2;
					app_data[2] = 0x84;
					app_data[4] = checksum(app_data+1, 3);
					noti_len = 5;
				break;

				case 0x05:
					if((osal_memcmp(DEFAULT_ADMIN_KEY, app_data+3, 4)) || (osal_memcmp(LC_Dev_System_Param.dev_cur_admin_key, app_data+3, 4)))
					{
						osal_memcpy(LC_Dev_System_Param.dev_cur_admin_key, app_data+7, 4);
						app_data[5] = 0x55;
						app_data[6] = 0xAA;
						osal_snv_write(SNV_FS_ADMIN_KEY, 6, app_data+5);
						app_data[3] = PPlus_SUCCESS;
					}
					else
					{
						app_data[3] = ERR_DATA;
					}
					app_data[1] = 2;
					app_data[2] = 0x85;
					app_data[4] = checksum(app_data+1, 3);
					noti_len = 5;
				break;

				case 0x06:
					if((app_data[3] > 30) || (app_data[3] < 3))
					{
						app_data[3] = ERR_DATA;
					}
					else
					{
						LC_Dev_System_Param.dev_infrared_outpu_time = app_data[3];
						app_data[3] = PPlus_SUCCESS;
					}
					app_data[1] = 2;
					app_data[2] = 0x86;
					app_data[4] = checksum(app_data+1, 3);
					noti_len = 5;
				break;

				case 0x07:
					app_data[1] = 3;
					app_data[2] = 0x87;
					app_data[3] = hal_gpio_read(GPIO_IN_1);
					app_data[4] = hal_gpio_read(GPIO_INFRARED);
					app_data[5] = checksum(app_data+1, 4);
					noti_len = 6;
				break;

				case 0x08:
					// if((app_data[3] > 3) || (app_data[3] == 0))
					// {
					// 	app_data[3] = ERR_DATA;
					// }
					// else
					{
						online_send_one_data(app_data[3]);
						app_data[3] = PPlus_SUCCESS;
					}
					app_data[1] = 2;
					app_data[2] = 0x88;
					app_data[4] = checksum(app_data+1, 3);
					noti_len = 5;
				break;

				case 0x09:
					if(app_data[3] < (UUID_MAX_NUM - 1)*UUID_LENGTH)
					{
						osal_memcpy(LC_Dev_System_Param.dev_UUID_Buffer[app_data[3]/8], app_data+4, 8);
						Write_24C02_Serial(app_data[3], 2);
						// int ret = LC_IIC_Master_WriteBytes(Press_I2C, IIC_AT24C02_ID, app_data[3], app_data+4, 4);
						// WaitMs(5);
						// LC_IIC_Master_WriteBytes(Press_I2C, IIC_AT24C02_ID, app_data[3]+4, app_data+8, 4);
						// LOG("write i2c %d\n", ret);
						app_data[3] = PPlus_SUCCESS;
					}
					else
					{
						app_data[3] = ERR_FUNCODE;
					}
					app_data[1] = 2;
					app_data[2] = 0x89;
					app_data[4] = checksum(app_data+1, 3);
					noti_len = 5;
				break;

				case 0x0A:
					if(app_data[3] < (UUID_MAX_NUM - 1)*UUID_LENGTH)
					{
						osal_memset(LC_Dev_System_Param.dev_UUID_Buffer[app_data[3]/8], 0xff, 8*app_data[4]);
						Write_24C02_Serial(app_data[3], app_data[4]*2);
						// int ret = LC_IIC_Master_WriteBytes(Press_I2C, IIC_AT24C02_ID, app_data[3], LC_Dev_System_Param.dev_UUID_Buffer[app_data[3]/8], 8*app_data[4]);
						// LOG("write i2c %d\n", ret);
						app_data[3] = PPlus_SUCCESS;
					}
					else
					{
						app_data[3] = ERR_FUNCODE;
					}
					app_data[1] = 2;
					app_data[2] = 0x8A;
					app_data[4] = checksum(app_data+1, 3);
					noti_len = 5;
				break;

				default:
					app_data[1] = 2;
					app_data[2] |= 0x80;
					app_data[3] = ERR_FUNCODE;
					app_data[4] = checksum(app_data+1, 3);
					noti_len = 5;
				break;
			}
		}
		else
		{
			app_data[0] = 0xAA;
			app_data[1] = 2;
			app_data[2] |= 0x80;
			app_data[3] = ERR_FUNCODE;
			app_data[4] = checksum(app_data+1, 3);
			noti_len = 5;
		}
		MultiProfile_Notify(LC_Dev_System_Param.Role_Slave.app_write_connHandle, MULTIPROFILE_CHAR2, noti_len, app_data);
		return(events ^ APP_DATA_EVT);
	}

	if(events & OUTPUT_TIMEOUT_EVT)
	{
		if(LC_Dev_System_Param.Role_Master.remote_conn_status + LC_Dev_System_Param.Role_Slave.app_conn_status)
		{
			LED_WRITE_STATUS(GPIO_LED_RED, LED_OFF);
			LED_WRITE_STATUS(GPIO_LED_GREEN, LED_OFF);
			LED_WRITE_STATUS(GPIO_LED_BLUE, LED_ON);
		}
		else
		{
			LED_WRITE_STATUS(GPIO_LED_RED, LED_ON);
			LED_WRITE_STATUS(GPIO_LED_GREEN, LED_OFF);
			LED_WRITE_STATUS(GPIO_LED_BLUE, LED_OFF);
		}
		OUTPUT_STATUS(0);
		return(events ^ OUTPUT_TIMEOUT_EVT);
	}

	if(events & RF_STOP_SEND_EVT)
	{
		if(LC_Dev_System_Param.dev_audio_send_flag == 2)
		{
			LC_Dev_System_Param.dev_channel_bit = 0;
			LC_Dev_System_Param.dev_audio_send_tick = 0;
			LC_Dev_System_Param.dev_audio_send_flag = 0;
		}
		else if(LC_Dev_System_Param.dev_audio_send_flag == 1)
		{
			LC_Timer_Start();
		}
		return(events ^ RF_STOP_SEND_EVT);
	}

	if(events & IIC_WRITE_EVT)
	{
		static uint8 index;

		int ret = LC_IIC_Master_WriteBytes(Press_I2C, IIC_AT24C02_ID, i2c_add+index*4, LC_Dev_System_Param.dev_UUID_Buffer[0]+i2c_add+index*4, 4);
		if(ret == PPlus_SUCCESS)
		{
			index++;
			LOG("write i2c index %d\n", index);
			if(index == i2c_write_cnt)
			{
				index = 0;
				i2c_add = 0;
				i2c_write_cnt = 0;
				return(events ^ IIC_WRITE_EVT);
			}
		}
		osal_start_timerEx(LC_Ui_Led_Buzzer_TaskID, IIC_WRITE_EVT, 50);
		return(events ^ IIC_WRITE_EVT);
	}

	if(events & OUTPUT_INT_CHK_EVT)
	{
		if(hal_gpio_read(GPIO_IN_1) == 0)
		{
			online_send_one_data(1);
			Output_Set_Time(LC_Dev_System_Param.dev_infrared_outpu_time);
		}
		return(events ^ OUTPUT_INT_CHK_EVT);
	}

	if(events & INFRARED_INT_EVT)
	{
		online_send_one_data(1);
		return(events ^ INFRARED_INT_EVT);
	}
    // Discard unknown events
    return 0;
}
/** @}*/

