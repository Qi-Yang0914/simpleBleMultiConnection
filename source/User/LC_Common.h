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
#include "watchdog.h"
#include "LC_Event_Handler.h"
/*------------------------------------------------------------------*/
/*						Pins definitions							*/
/*------------------------------------------------------------------*/
//	RF 433M
#define		GPIO_RF_433M	P34
//	Key Pins
#define		GPIO_KEY_PWR	P0



/*------------------------------------------------------------------*/
/*						MACROS										*/
/*------------------------------------------------------------------*/
#define RF_START_CODE_L_MIN      95		//	start minimum 4ms
#define RF_DATA_H_ONE_MAX        17		//	1 H maximum	1.4ms
#define RF_DATA_H_ONE_MIN        9		//	1 H minimum 0.6ms
#define RF_DATA_H_ZERO_MAX       8		//	0 H maximum 0.4ms
#define RF_DATA_H_ZERO_MIN       2		//	0 H minimum 0.15ms
#define RF_DATA_COUNT            32		//	24bits

//	FS_ID
#define		SNV_FS_ID_PSK			(0xA0)

#define		SET_BIT_X(a, b)					(a |= BIT(b))
#define		RESET_BIT_X(a, b)				(a &= ~BIT(b))
#define		GET_BIT_X(a, b)					(a & BIT(b))

//	PWM Breath configuration
#define		PWM_BREATH_MAX					(999)		//	16,000,000/4/1000	=	4,000Hz

#define		HALL_ENABLE()					hal_gpio_write(GPIO_HALL_EN, 1)
#define		LED_DRIVE_ENABLE()				hal_gpio_write(GPIO_LED_CTL, 0)

#define		LED_MODE_COUNTDOWN_TIME_ON()	hal_gpio_write(GPIO_LED_2, 1)
#define		LED_MODE_COUNTDOWN_TIME_OFF()	hal_gpio_write(GPIO_LED_2, 0)

#define		LED_MODE_BLE_ON()				hal_gpio_write(GPIO_LED_1, 1)
#define		LED_MDOE_BLE_OFF()				hal_gpio_write(GPIO_LED_1, 0)

//	Timer interval
#define		TIMER_1S					1
#define		TIMER_2S					2
#define		TIMER_3S					3
#define		TIMER_6S					6
#define		TIMER_500MS					7

#define		LC_TIMER_INTERVAL			TIMER_1S

#if (LC_TIMER_INTERVAL == TIMER_6S)
#define		LC_DEV_TIMER_DISCON_PWROFF	( 5*10 + 1)
#define		LC_DEV_TIMER_POWEROFF		(10*10 + 1)
#define		LC_DEV_TIMER_SUSPEND		(10*10 + 1)
#elif (LC_TIMER_INTERVAL == TIMER_1S)
#define		LC_DEV_TIMER_DISCON_PWROFF	(5*60 + 1)
#define		LC_DEV_TIMER_CON_PWROFF		(5*60 + 1)
#define		LC_DEV_TIMER_POWEROFF		(10*60 + 1)
#define		LC_DEV_TIMER_SUSPEND		(10*60 + 1)
#define		LC_DEV_BOOST_TIMER_CON		(15U)
#elif (LC_TIMER_INTERVAL == TIMER_500MS)
#define		LC_DEV_TIMER_DISCON_PWROFF	(60 + 1)
#define		LC_DEV_TIMER_POWEROFF		(10*60 + 1)
#define		LC_DEV_TIMER_SUSPEND		(10*60 + 1)
#endif

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
	dev_con_t		dev_con_param[MAX_NUM_LL_CONN];
	uint8			dev_ble_mac[6];
	uint8			dev_psk[6];
	uint8			dev_psk_flag;
}lc_dev_sys_param;


/*------------------------------------------------------------------*/
/* 					 external variables							 	*/
/*------------------------------------------------------------------*/
extern	lc_433m_rec_t	LC_433m_Data;
extern	lc_app_set_t		LC_App_Set_Param;
extern	lc_dev_sys_param	LC_Dev_System_Param;
/*------------------------------------------------------------------*/
/* 					 User function prototypes					 	*/
/*------------------------------------------------------------------*/
uint32	clock_time_exceed_func			(uint32 ref,		uint32 span_ms	);
uint8	halfbyte_into_str(uint8 byte);
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
