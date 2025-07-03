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
lc_dev_sys_param	LC_Dev_System_Param	=	
{
	.dev_UUID = {0x31, 0x30, 0x30, 0x30, 0x38, 0x30, 0x30, 0x30},
	.dev_UUID_Buffer = {{0x31, 0x30, 0x30, 0x30, 0x38, 0x30, 0x30, 0x33},},
	.dev_infrared_outpu_time = 7,
	.dev_app_output_time = 7,
};
void* Press_I2C	= NULL;
const uint8 DEFAULT_ADMIN_KEY[4] = {0xAA, 0xBB, 0xCC, 0xDD};
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
 *	@fn			Byte_to_TwoAcs
 *	@brief		transfer Nibble of one Byte to two Bytes
 *	@param[in]	des:	output string
 *	@param[in]	scr:	need to be transfered data
 *	@param[in]	des_len:	length of output data
 *	@return		NONE
 */
void Byte_to_TwoAcs(uint8 *des, uint8 *scr, uint8 des_len)
{
	uint8 i = 0;
	uint8 cur_byte = 0;
	uint8 half_byte = 0;
	for(i =0; i < des_len; i++)
	{
		cur_byte = *(scr + i/2);
		half_byte = (i%2)? (cur_byte & 0x0f):((cur_byte >> 4) & 0x0f);
		*(des + i) = (half_byte < 0x0A)?(half_byte + '0') : (half_byte - 0x0a + 'A');
	}
}
uint8 checksum(uint8 *data, uint16 len)
{
	uint16 sum = 0;

	for(uint16 i = 0; i < len; i++)
	{
		sum += data[i];
	}

	return((uint8)sum);
}
/*!
 *	@fn			check_key_UUID
 *	@brief		check UUID is exist
 *	@param[in]	target_uuid: targe needed to check
 *	@param[in]	total_uuid:	all UUID
 *	@param[in]	check_num:	check number
 *	@return		PPlus_SUCCESS:find the target
 *				PPlus_ERR_NOT_FOUND:not find
 */
uint8 check_key_UUID(uint8 *target_uuid, uint8 *total_uuid, uint8 check_num)
{
	uint8 ret = PPlus_SUCCESS;
	uint8 i;
	if(check_num > UUID_MAX_NUM)
	{
		ret = PPlus_ERR_INVALID_ADDR;
		return ret;
	}
	for(i = 0;i < check_num; i++)
	{
		if(osal_memcmp(total_uuid + i, target_uuid, UUID_LENGTH))
		{
			ret = PPlus_SUCCESS;
			return ret;
		}
	}
	ret = PPlus_ERR_NOT_FOUND;
	return ret;
}
/*!
 *	@fn			find_key_UUID
 *	@brief		Look through the UUID entries to find an address.
 *	@param[in]	target_uuid: targe needed to check
 *	@param[in]	total_uuid:	all UUID
 *	@return		index  (0 - (UUID_MAX_NUM-1),
 *				UUID_MAX_NUM if no UUID
 */
uint8 find_key_UUID(uint8 *target_uuid, uint8 *totoal_uuid)
{
	for(uint8 idx = 0; idx < UUID_MAX_NUM; idx++)
	{
		if(osal_memcmp(totoal_uuid+idx, target_uuid, UUID_LENGTH))
		{
			return(idx);
		}
	}
	return(UUID_MAX_NUM);
}
/*!
 *	@fn			LC_IIC_Master_WriteBytes
 *	@brief		write a serial data by i2c interface 
 *	@param[in]	pi2c: 
 *	@param[in]	slave_addr:	slave deivce ID
 *	@param[in]	reg: slave start address
 *	@param[in]	data: write data
 *	@param[in]	size: write data length
 *	@return		
 */
int LC_IIC_Master_WriteBytes(void* pi2c,uint8 slave_addr, uint8 reg, uint8* data, uint8 size)
{
	HAL_ENTER_CRITICAL_SECTION();
	hal_i2c_tx_start(pi2c);
	hal_i2c_addr_update(pi2c, slave_addr);
	hal_i2c_send(pi2c, &reg,1);
	hal_i2c_send(pi2c, data, size);
	HAL_EXIT_CRITICAL_SECTION();
	return	hal_i2c_wait_tx_completed(pi2c);
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
extern	void	__ATTR_SECTION_SRAM__	__attribute__((used))	LC_RF_433M_Send(uint8 evt);
void LC_Timer_Start(void)
{
	hal_timer_init(LC_RF_433M_Send);
	hal_timer_set(AP_TIMER_ID_5, 100);
	LOG("Start timer:\n");
}
void LC_Timer_Stop(void)
{
	hal_timer_stop(AP_TIMER_ID_5);
	LOG("Stop timer\n");
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

/**
 * @brief 
 * 
 */
void BSP_Pin_Init(void)
{
	hal_pwrmgr_register(MOD_USR8, NULL, NULL);
	hal_pwrmgr_lock(MOD_USR8);
	
	uint8 fs_buffer[6];

	osal_snv_read(SNV_FS_ADMIN_KEY, 6, fs_buffer);
	if(fs_buffer[0] == 0x55 && fs_buffer[1] == 0xAA)
	{
		osal_memcpy(LC_Dev_System_Param.dev_cur_admin_key, fs_buffer+2, 4);
	}
	else
	{
		osal_memset(LC_Dev_System_Param.dev_cur_admin_key, 0xff, 4);
	}

	hal_gpio_pin_init(GPIO_LED_RED, OEN);
	hal_gpio_pin_init(GPIO_LED_GREEN, OEN);
	hal_gpio_pin_init(GPIO_LED_BLUE, OEN);

	LED_WRITE_STATUS(GPIO_LED_RED, LED_ON);
	LED_WRITE_STATUS(GPIO_LED_GREEN, LED_OFF);
	LED_WRITE_STATUS(GPIO_LED_BLUE, LED_OFF);

	hal_gpio_pin_init(GPIO_AUDIO_OTP, OEN);
	OTP_SEND_HIGH();

	hal_gpio_pin_init(GPIO_OUT_1, OEN);
	OUTPUT_STATUS(0);

	hal_gpio_pin_init(GPIO_INFRARED, IE);
	hal_gpio_pull_set(GPIO_INFRARED, PULL_DOWN);
	hal_gpioin_register(GPIO_INFRARED, LC_Key_Pin_IntHandler, NULL);

	hal_i2c_pin_init(I2C_0, GPIO_IIC_SDA, GPIO_IIC_SCL);
	Press_I2C = hal_i2c_init(I2C_0, I2C_CLOCK_100K);
	if(Press_I2C == NULL)
	{
		LOG("i2c mater init failed\n");
	}
	else
	{
		LOG("i2c master init successed\n");
	}
	for(uint8 i = 0; i < UUID_MAX_NUM*2; i++)
	{
		hal_i2c_read(Press_I2C, IIC_AT24C02_ID, i*4, LC_Dev_System_Param.dev_UUID_Buffer[0]+i*4, 4);
	}
	// hal_i2c_read(Press_I2C, IIC_AT24C02_ID, 0, LC_Dev_System_Param.dev_UUID_Buffer[0], 128);
	// hal_i2c_read(Press_I2C, IIC_AT24C02_ID, 128, LC_Dev_System_Param.dev_UUID_Buffer[16], 128);
	LOG_DUMP_BYTE(LC_Dev_System_Param.dev_UUID_Buffer[0], 256);
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

