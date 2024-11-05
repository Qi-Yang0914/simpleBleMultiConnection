/**
 *	@file		LC_Event_Handler.c
 *	@author		YQ
 *	@date		01/20/2021
 *	@brief		IRQ handler.
 */

/*!
 *	@defgroup	LC_Event_Handler
 *	@brief		DO NOT put Interrupt code in XIP flash!!!
 *	@{*/

/*------------------------------------------------------------------*/
/*						head files include 							*/
/*------------------------------------------------------------------*/
#include	"LC_Event_Handler.h"
#include	"LC_UI_led_buzzer.h"
#include	"LC_Key.h"
#include	"LC_Uart.h"
/*------------------------------------------------------------------*/
/* 					 	local variables			 					*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/* 					 	public variables		 					*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/* 					 	public functions		 					*/
/*------------------------------------------------------------------*/
/**
 *	@fn			LC_RGB_Valeu_Deal
 *	@brief		Callback of timer handler.
 *	@param[in]	evt		:IRQ event.
 *	@return		none.
 */
// void	__ATTR_SECTION_SRAM__  __attribute__((used))	LC_RGB_Valeu_Deal(uint8 evt)
// {
// 	if(evt == HAL_EVT_TIMER_5)
// 	{
// 		;
// 	}
// }
/**
 * @brief 
 * 
 * @param pev 
 */
void	__ATTR_SECTION_SRAM__  __attribute__((used))	LC_USART_Handler_Evt(uart_Evt_t* pev)
{
    BUP_ctx_t*  pctx    =   &mBUP_Ctx;

    switch (pev->type)
    {
        case    UART_EVT_TYPE_RX_DATA:
            if((pctx->hal_uart_rx_size + pev->len) >= UART_RX_BUF_SIZE)
                break;
            uartrx_timeout_tiemr_stop();
            uartrx_timeout_timer_start();
            osal_memcpy(pctx->hal_uart_rx_buf + pctx->hal_uart_rx_size, pev->data, pev->len);
            pctx->hal_uart_rx_size  +=  pev->len;
        break;

        case    UART_EVT_TYPE_RX_DATA_TO:
            if((pctx->hal_uart_rx_size + pev->len) >= UART_RX_BUF_SIZE)
                break;
            uartrx_timeout_tiemr_stop();
            uartrx_timeout_timer_start();
            osal_memcpy(pctx->hal_uart_rx_buf + pctx->hal_uart_rx_size, pev->data, pev->len);
            pctx->hal_uart_rx_size  +=  pev->len;
            
        break;
        case    UART_EVT_TYPE_TX_COMPLETED:
            osal_set_event(LC_Uart_TaskID, UART_EVT_UART_TX_COMPLETE);
        break;
        default:
        break;
    }
}

/**
 *	@fn			LC_IR_Analysis_Data
 *	@brief		analysis NEC,get user code and commond code.
 *	@param[in]	nTimeL		:interval of a negtivate and a posetive edge.	
 *	@return		none.
 */
// void	__ATTR_SECTION_SRAM__  __attribute__((used))	LC_IR_Analysis_Data(uint32	nTimeL)
// {
// 	;
// }
/*!
 *	@fn			LC_Key_Pin_IntHandler
 *	@brief		Callback of key Pin interrupt.
 *	@param[in]	pin		:pin of IR.
 *	@param[in]	type	:type of interrupe.
 *	@return		none.
 */
void	__ATTR_SECTION_SRAM__  __attribute__((used))	LC_Key_Pin_IntHandler(GPIO_Pin_e pin, IO_Wakeup_Pol_e type)
{
    switch (pin)
    {
		default:

			break;
    }
}
/*!
 *	@fn			LC_Gpio_IR_IntHandler
 *	@brief		Callback of IR Pin interrupt.
 *	@param[in]	pin		:pin of IR.
 *	@param[in]	type	:type of interrupe.
 *	@return		none.
 */
// void	__ATTR_SECTION_SRAM__  __attribute__((used))	LC_Gpio_IR_IntHandler(GPIO_Pin_e pin, IO_Wakeup_Pol_e type)
// {
// 	;
// }

/** @}*/

