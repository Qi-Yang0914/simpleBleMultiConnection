/**
*	@file	LC_Common.h
*	@date	10/20/2020
*	@version	1.0.1
*
*/

/*!
 * 	@defgroup	LC_Common
 *	@brief
 *	@{*/
#ifndef		LC_COMMON_H_
#define		LC_COMMON_H_
/*------------------------------------------------------------------*/
/*						C++ guard macro								*/
/*------------------------------------------------------------------*/
#ifdef	__cplusplus
	 extern  "C" {
#endif
/*------------------------------------------------------------------*/
/* 				 		head files include 							*/
/*------------------------------------------------------------------*/

#include "att.h"
#include "bcomdef.h"
#include "gapbondmgr.h"
#include "gapgattserver.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "global_config.h"
#include "hci.h"
#include "hci_tl.h"
#include "linkdb.h"
#include "ll.h"
#include "ll_def.h"
#include "ll_hw_drv.h"
#include "OSAL.h"
#include "OSAL_PwrMgr.h"
#include "pwrmgr.h"
#include "rf_phy_driver.h"
#include "osal_snv.h"
#include "flash.h"
#include "gpio.h"
#include "i2c.h"
#include "watchdog.h"
#include "LC_Event_Handler.h"
#include "multiRoleProfile.h"
#include "multi.h"
/*------------------------------------------------------------------*/
/*						Pins definitions							*/
/*------------------------------------------------------------------*/
//	RF 433M
#define		GPIO_RF_433M_RX	P7
#define		GPIO_RF_433M_TX	P7
//	Key Pins
#define		GPIO_KEY_PWR	P14
//	Uart
#define		GPIO_UART_TX	GPIO_DUMMY
#define		GPIO_UART_RX	GPIO_DUMMY

#define		GPIO_AUDIO_OTP	P34
#define		GPIO_LED_RED	P2
#define		GPIO_LED_GREEN	P3
#define		GPIO_LED_BLUE	P7
#define		GPIO_IIC_SCL	P11
#define		GPIO_IIC_SDA	P20
#define		GPIO_OUT_1		P14
#define		GPIO_IN_1		P15
#define		GPIO_INFRARED	P18


/*------------------------------------------------------------------*/
/*						MACROS										*/
/*------------------------------------------------------------------*/
#define RF_START_CODE_L_MIN      95		//	start minimum 4ms
#define RF_DATA_H_ONE_MAX        17		//	1 H maximum	1.4ms
#define RF_DATA_H_ONE_MIN        9		//	1 H minimum 0.6ms
#define RF_DATA_H_ZERO_MAX       8		//	0 H maximum 0.4ms
#define RF_DATA_H_ZERO_MIN       2		//	0 H minimum 0.15ms
#define RF_DATA_COUNT            24		//	24bits

#define		ERR_FUNCODE		0x01
#define		ERR_REG_ADD		0x02
#define		ERR_DATA		0x03
#define		ERR_PROCESS		0x04
#define		ERR_CRC			0x05

#define		IIC_AT24C02_ID			(0x50)

//	FS_ID
#define		SNV_FS_ADMIN_KEY		(0xA0)
#define		SNV_FS_433M_KEY			(0xA1)

#define		SET_BIT_X(a, b)					(a |= BIT(b))
#define		RESET_BIT_X(a, b)				(a &= ~BIT(b))
#define		GET_BIT_X(a, b)					(a & BIT(b))

#define		LED_COMMON_ANODE				(1)
#define		LED_COMMON_ACTHODE				(2)

#ifndef LED_POLARITY
#define	LED_POLARITY	LED_COMMON_ACTHODE
#endif

#if( LED_POLARITY == LED_COMMON_ANODE)
#define		LED_ON							(1)
#define		LED_OFF							(0)
#elif (LED_POLARITY == LED_COMMON_ACTHODE)
#define		LED_ON							(0)
#define		LED_OFF							(1)
#endif

#define		RF_SEND_LOW()		hal_gpio_write(GPIO_RF_433M_TX, 0)
#define		RF_SEND_HIGH()		hal_gpio_write(GPIO_RF_433M_TX, 1)
#define		OTP_SEND_LOW()		hal_gpio_write(GPIO_AUDIO_OTP, 0)
#define		OTP_SEND_HIGH()		hal_gpio_write(GPIO_AUDIO_OTP, 1)
#define		LED_WRITE_STATUS(pin, status)	hal_gpio_write(pin, status)
#define		OUTPUT_STATUS(status)			hal_gpio_write(GPIO_OUT_1, status)

#define		UUID_MAX_NUM		(32)
#define		UUID_LENGTH			(8)
/*------------------------------------------------------------------*/
/*						UI Task Events definitions					*/
/*------------------------------------------------------------------*/


/*------------------------------------------------------------------*/
/* 					 	Data structures							 	*/
/*------------------------------------------------------------------*/
typedef uint8_t				u8;
typedef uint16_t			u16;
typedef uint32_t			u32;
typedef signed   char		int8_t;		//!< Signed 8 bit integer
typedef unsigned char		uint8_t;		//!< Unsigned 8 bit integer
typedef signed   short		int16_t;		//!< Signed 16 bit integer
typedef unsigned short		uint16_t;		//!< Unsigned 16 bit integer
typedef signed   int		int32_t;		//!< Signed 32 bit integer
typedef unsigned int		uint32_t;		//!< Unsigned 32 bit integer
typedef signed   char		int8;			//!< Signed 8 bit integer
typedef unsigned char		uint8;			//!< Unsigned 8 bit integer
typedef signed   short		int16;			//!< Signed 16 bit integer
typedef unsigned short		uint16;		//!< Unsigned 16 bit integer
typedef signed   long		int32;			//!< Signed 32 bit integer
typedef unsigned long		uint32;		//!< Unsigned 32 bit integer

typedef		enum
{
	LC_DEV_BLE_DISCONNECTION	=	0,
	LC_DEV_BLE_CONNECTION,
}lc_dev_ble_state;

typedef		enum
{
	State_Off	=	0,
	State_On	=	1,
}lc_state_bool;

typedef		enum
{
	SYSTEM_STANDBY	=	0,	SYSTEM_POWEROFF	=	0,
	SYSTEM_WORKING	=	1,
	SYSTEM_SUSPEND	=	2,
	SYSTEM_CHARGINE	=	3,
}lc_sys_run_t;

typedef	enum
{
	TIME_EVT_NONE = 0,
	TIME_EVT_LEARN = 1,
	TIME_EVT_SEND = 2,
}time_evt_e;

typedef enum
{
	RF_IDEL = 0,
	RF_START_REC,
	RF_STOP_REC,
	RF_START_SEND,
	RF_STOP_SEND = 4,
}rf_action_e;

typedef	struct
{
	uint8	data_tail;
	uint8	data_head;
	uint8	high_low[200];
	uint8	key_press_flag;
	uint16	time_span[200];
	uint32	key_data;
	uint32	get_key_data;
}lc_433m_rec_t;

typedef	struct
{
	uint8		app_write_data[20];
	uint8		app_notify_data[20];
	uint8		app_write_len;
	uint8		app_notify_len;
	uint8		app_connHandle;
}lc_app_set_t;

typedef struct
{
	uint8	ble_con_st;
	uint8	ble_con_handle;
	uint8	authenticated_flag;
}dev_con_t;

typedef struct
{
	uint8 app_write_data[40];
	uint16 app_write_connHandle;
	uint8 app_write_len;
	uint8 app_conn_status;
}role_slave_t;

typedef struct 
{
	uint8 remote_notify[20];
	uint16 remote_connHandle;
	uint8 remote_notify_len;
	uint8 remote_conn_status;
}role_master_t;

typedef struct
{
	// dev_con_t		dev_con_param[MAX_NUM_LL_CONN];
	role_slave_t Role_Slave;
	role_master_t Role_Master;
	uint8	dev_ble_mac[6];
	uint8	dev_UUID[8];
	uint8	dev_UUID_Buffer[UUID_MAX_NUM][UUID_LENGTH];
	uint8	dev_cur_admin_key[4];
	uint8	dev_rand_challenge[4];
	uint8	dev_audio_send_flag;
	uint8	dev_audio_send_tick;
	uint8	dev_audio_channel;
	uint8	dev_channel_bit;
	uint8	dev_infrared_outpu_time;//Delay_Lock
	uint8	dev_app_output_time;
}lc_dev_sys_param;


/*------------------------------------------------------------------*/
/* 					 external variables							 	*/
/*------------------------------------------------------------------*/
extern	lc_dev_sys_param	LC_Dev_System_Param;
extern	void* Press_I2C;
extern	const uint8 DEFAULT_ADMIN_KEY[4];
/*------------------------------------------------------------------*/
/* 					 User function prototypes					 	*/
/*------------------------------------------------------------------*/
uint32	clock_time_exceed_func			(uint32 ref,		uint32 span_ms	);
uint8	halfbyte_into_str(uint8 byte);
/*!
 *	@fn			Byte_to_TwoAcs
 *	@brief		transfer Nibble of one Byte to two Bytes
 *	@param[in]	des:	output string
 *	@param[in]	scr:	need to be transfered data
 *	@param[in]	des_len:	length of source data
 *	@return		NONE
 */
void Byte_to_TwoAcs(uint8 *des, uint8 *scr, uint8 des_len);
/*!
 *	@fn			check_key_UUID
 *	@brief		check UUID is exist
 *	@param[in]	target_uuid: targe needed to check
 *	@param[in]	total_uuid:	all UUID
 *	@param[in]	check_num:	check number
 *	@return		PPlus_SUCCESS:find the target
 *				PPlus_ERR_NOT_FOUND:not find
 */
uint8 check_key_UUID(uint8 *target_uuid, uint8 *total_uuid, uint8 check_num);
/*!
 *	@fn			find_key_UUID
 *	@brief		Look through the UUID entries to find an address.
 *	@param[in]	target_uuid: targe needed to check
 *	@param[in]	total_uuid:	all UUID
 *	@return		index  (0 - (UUID_MAX_NUM-1),
 *				UUID_MAX_NUM if no UUID
 */
uint8 find_key_UUID(uint8 *target_uuid, uint8 *totoal_uuid);
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
int LC_IIC_Master_WriteBytes(void* pi2c,uint8 slave_addr, uint8 reg, uint8* data, uint8 size);
uint8 checksum(uint8 *data, uint16 len);
void	LC_Common_ProcessOSALMsg	 	(osal_event_hdr_t *pMsg				);
void	LC_Timer_Start					(void);
void	LC_Timer_Stop					(void);

void BSP_Pin_Init(void);
void LC_Dev_Poweroff(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LC_COMMON_H_ */
/** @}*/
