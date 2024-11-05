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
#if(DEBUG_INFO == 0)
//	BOOST Pins
#define		GPIO_PWM_BUZZER		P0
//	LED Pins
#define		GPIO_LED_CTL		P3
#define		GPIO_LED_1			P34
#define		GPIO_LED_2			P33
//	Key Pins
#define		GPIO_KEY_PWR		P26

//	Hall Pins
#define		GPIO_HALL_EN		P15
#define		GPIO_HALL_1			P7
#define		GPIO_HALL_2			P18
#define		GPIO_HALL_3			P20

//	Charge Pins
#define		GPIO_DET_5V			P1
#define		GPIO_DET_CHG		P2
#define		GPIO_DET_BAT_EN		P14
#define		GPIO_DET_BAT		P11
#define		GPIO_DET_BAT_AD		ADC_CH1N_P11
//	Sensor Pins
#define		GPIO_DRIVE_CLK		P31
#define		GPIO_DRIVE_DAT		P32

#else
//	BOOST Pins
#define		GPIO_PWM_BUZZER		P0
//	LED Pins
#define		GPIO_LED_CTL		P3
#define		GPIO_LED_1			P34
#define		GPIO_LED_2			P33
//	Key Pins
#define		GPIO_KEY_PWR		P26

//	Hall Pins
#define		GPIO_HALL_EN		P15
#define		GPIO_HALL_1			P7
#define		GPIO_HALL_2			P18
#define		GPIO_HALL_3			P20

//	Charge Pins
#define		GPIO_DET_5V			P1
#define		GPIO_DET_CHG		P2
#define		GPIO_DET_BAT_EN		P14
#define		GPIO_DET_BAT		P11
#define		GPIO_DET_BAT_AD		ADC_CH1N_P11
//	Sensor Pins
#define		GPIO_DRIVE_CLK		P31
#define		GPIO_DRIVE_DAT		P32

#endif

/*------------------------------------------------------------------*/
/*						MACROS										*/
/*------------------------------------------------------------------*/
#define		BIT_LED_BUFFER_DP				(7)

#define		BIT_COUNT_FLAG_CYCLE			(6)
#define		BIT_COUNT_FLAG_START_SOTP		(4)
#define		BIT_COUNT_FLAG_TIME				(2)
#define		BIT_COUNT_FLAG_CNT				(0)

#define		BIT_CHOOSE_SWITCH_MODE			(4)
#define		BIT_CHOOSE_TIME_CNT				(0)

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
	uint8		app_write_data[20];
	uint8		app_notify_data[20];
	uint8		app_write_len;
	uint8		app_notify_len;
}lc_app_set_t;

typedef struct
{
	uint32			dev_timeout_poweroff_cnt;		//	power off time
	uint8			dev_ble_mac[6];
	uint8			dev_timer_poweroff_flag;		//	power off timer enable flag			:1	enable,		0	disable
	uint8			dev_poweron_switch_flag;		//	power on switch exist or not		:1	no switch,	0	
	uint8			dev_power_flag;					//	device working flag					:1	working,	0	power off
	uint8			dev_ble_con_state;				//	BLE	connection state				:1	connected,	0	disconnected
	uint8			dev_charging_flag;				//	1	charging,	0	no charging
}lc_dev_sys_param;


/*------------------------------------------------------------------*/
/* 					 external variables							 	*/
/*------------------------------------------------------------------*/
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
