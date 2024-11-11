/**
*	@file	LC_Common.h
*	@date	09/17/2020
*	@version	1.0.0
*
*/

/*!
 * 	@defgroup	LC_Common
 *	@brief
 *	@{*/
/*------------------------------------------------------------------*/
/*						head files include 							*/
/*------------------------------------------------------------------*/
#include "LC_Common.h"
/*------------------------------------------------------------------*/
/* 					 	local variables			 					*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/* 					 	public variables		 					*/
/*------------------------------------------------------------------*/
lc_433m_rec_t	LC_433m_Data	=
{
	.data_tail	=	0,
	.data_head	=	0,
	.high_low	=	{0,},
	.key_press_flag	=	0,
	.time_span	=	{0},
	.key_data	=	0,
	.get_key_data	=	0,
};
lc_app_set_t		LC_App_Set_Param;
lc_dev_sys_param	LC_Dev_System_Param	=	
{
	.dev_psk_flag = TRUE,
	.dev_psk = {'1', '2', '3', '4', '5', '6'},
};
/*------------------------------------------------------------------*/
/* 					 	local functions			 					*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/* 					 	public functions		 					*/
/*------------------------------------------------------------------*/
/*!
 *	@fn			clock_time_exceed_func
 *	@brief		
 */
uint32 clock_time_exceed_func(uint32 ref, uint32 span_ms)
{
#if 0
	u32 deltTick ,T0 ;
	T0 = hal_read_current_time();
	deltTick =TIME_DELTA(T0,ref);
	if(deltTick>span_ms){
		return 1 ;
	}else {
		return 0 ;
	}
#else 
	uint32 deltTick  = 0 ;
	deltTick = hal_ms_intv(ref) ;
	if(deltTick>span_ms){
		return 1 ;
	}else {
		return 0 ;
	}	
#endif
}
/*!
 *	@fn			halfbyte_into_str
 *	@brief		MAC address transform into device name.
 *	@param[in]	byte:	half byte of MAC.
 *	@return		ASCII of halfbyte.
 */
uint8	halfbyte_into_str(uint8 byte)
{
	int8 temp = 0;
	if(byte < 0x0a)
		temp = byte + '0';
	else
		temp = byte - 0x0a + 'a';

	return temp;
}
/*!
 *	@fn			LC_Common_ProcessOSALMsg
 *	@brief		Process an incoming task message,nothing.
 *	@param[in]	pMsg	:message to process
 *	@return		none.
 */
void LC_Common_ProcessOSALMsg(osal_event_hdr_t *pMsg)
{
	switch(pMsg->event)
	{
		default:
			// do nothing
		break;
	}
}

extern	void	__ATTR_SECTION_SRAM__  __attribute__((used))	LC_RGB_Valeu_Deal(uint8 evt);
void LC_Timer_Start(void)
{
	hal_timer_init(LC_RGB_Valeu_Deal);
	hal_timer_set(AP_TIMER_ID_5, 100);
	LOG("Start timer:\n");
}
void LC_Timer_Stop(void)
{
	hal_timer_stop(AP_TIMER_ID_5);
	// LOG("Stop timer\n");
}
// /*!
//  *	@fn			LC_Switch_Poweron
//  *	@brief		press switch to power on.
//  *	@param[in]	cur_state	:
//  *	@param[in]	power_start_tick	:set time for long press to poweron,
//  *									power_start_tick*25ms
//  *	@return		none.
//  */
// static	void LC_Switch_Poweron(uint8 cur_state, uint8 power_start_tick)
// {
// 	if(LC_Dev_System_Param.dev_poweron_switch_flag)
// 	{
// 		LC_Dev_System_Param.dev_power_flag		=	SYSTEM_WORKING;
// 		return;
// 	}
// 	uint8	poweron_start_num	=	power_start_tick;

// 	if(!cur_state)
// 	{
// 		while(poweron_start_num)
// 		{
// 			if(hal_gpio_read(GPIO_KEY_PWR) == 0)
// 			{
// 				poweron_start_num--;
// 				WaitMs(100);
// 				hal_watchdog_feed();
// 				LOG("press first %d\n", poweron_start_num);
// 			}
// 			else
// 			{
// 				LOG("release \n");
// 				poweron_start_num	=	power_start_tick;
// 				LC_Dev_System_Param.dev_power_flag		=	SYSTEM_POWEROFF;
// 				LC_Dev_Poweroff();
// 				return ;
// 			}
// 		}
// 		LC_Dev_System_Param.dev_power_flag		=	SYSTEM_WORKING;
// 	}
// }
extern	void	__ATTR_SECTION_SRAM__  __attribute__((used))	LC_Key_Pin_IntHandler(GPIO_Pin_e pin, IO_Wakeup_Pol_e type);
extern	void	__ATTR_SECTION_SRAM__  __attribute__((used))	LC_Gpio_IR_IntHandler	(GPIO_Pin_e pin, IO_Wakeup_Pol_e type);

/**
 * @brief 
 * 
 */
void BSP_Pin_Init(void)
{
	hal_pwrmgr_register(MOD_USR8, NULL, NULL);
	hal_pwrmgr_lock(MOD_USR8);
	hal_gpio_pin_init(GPIO_RF_433M, IE);
	hal_gpio_pull_set(GPIO_RF_433M, STRONG_PULL_UP);
	hal_gpioin_register(GPIO_RF_433M, LC_Gpio_IR_IntHandler, LC_Gpio_IR_IntHandler);

	hal_gpio_pin_init(GPIO_KEY_PWR, IE);
	hal_gpio_pull_set(GPIO_KEY_PWR, STRONG_PULL_UP);
	hal_gpioin_register(GPIO_KEY_PWR, NULL, LC_Key_Pin_IntHandler);
}

/*!
 *	@fn			LC_Dev_Poweroff
 *	@brief		the process of power off,need to disable adv and all events.
 *	@param[in]	none.
 *	@return		none.
 */
void LC_Dev_Poweroff(void)
{
	// LOG("POWER OFF[%d]\n", LC_Dev_System_Param.dev_power_flag);
	// hal_gpio_pin_init(GPIO_LED_1, IE);
	// hal_gpio_pull_set(GPIO_LED_1, FLOATING);
	// hal_gpio_pin_init(GPIO_LED_2, IE);
	// hal_gpio_pull_set(GPIO_LED_2, FLOATING);

	// hal_gpio_pin_init(GPIO_LED_CTL, IE);
	// hal_gpio_pull_set(GPIO_LED_CTL, STRONG_PULL_UP);

	// pwroff_cfg_t	User_Set_Wakeup[2];
	// User_Set_Wakeup[0].pin	=	GPIO_KEY_PWR;
	// User_Set_Wakeup[0].type	=	NEGEDGE;

	// User_Set_Wakeup[1].pin	=	GPIO_DET_5V;
	// User_Set_Wakeup[1].type	=	POSEDGE;
	// hal_pwrmgr_unlock(MOD_USR8);

	// AP_WDT->CRR	=	0x76;	//	feed watch dog
	// while(hal_gpio_read(GPIO_KEY_PWR) == 0){
	// 	WaitUs(10*1000);
	// 	AP_WDT->CRR	=	0x76;	//	feed watch dog
	// }
	// hal_pwrmgr_poweroff(&User_Set_Wakeup[0], 2);
}

/** @}*/

