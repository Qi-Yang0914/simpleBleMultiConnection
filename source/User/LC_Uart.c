/**
 *	@file		LC_Uart.h
 *	@author		YQ
 *	@date		04/10/2021
 *	@version	1.0.0
 *
 */

/*!
 * 	@defgroup	LC_Uart
 *	@brief
 *	@{*/

/*------------------------------------------------------------------*/
/*						head files include 							*/
/*------------------------------------------------------------------*/
#include	"LC_Uart.h"
#include	"LC_UI_led_buzzer.h"
#include "LC_Event_Handler.h"
/*------------------------------------------------------------------*/
/* 					 	public variables		 					*/
/*------------------------------------------------------------------*/
uint8       LC_Uart_TaskID;
BUP_ctx_t   mBUP_Ctx;
/*------------------------------------------------------------------*/
/* 					 	local variables		 					    */
/*------------------------------------------------------------------*/
static uint8 UartData[20];
static uint8 UartDataLen;
/*------------------------------------------------------------------*/
/* 					 	local functions                             */
/*------------------------------------------------------------------*/
// static  void    on_BUP_Evt(BUP_Evt_t* pev)
// {
//     switch(pev->ev){

//     }
// }

//timer for uart send delay slot
static  void    rx_start_timer(uint16_t timeout)
{
	// LOG("uart start Timer\n");
	osal_start_timerEx(LC_Uart_TaskID, UART_EVT_UARTRX_TIMER, timeout);
}
/*------------------------------------------------------------------*/
/* 					 	public functions		 					*/
/*------------------------------------------------------------------*/
void    uartrx_timeout_timer_start(void)
{
    osal_start_timerEx(LC_Uart_TaskID, UART_EVT_TO_TIMER, 10);
}
void    uartrx_timeout_tiemr_stop(void)
{
    osal_stop_timerEx(LC_Uart_TaskID, UART_EVT_TO_TIMER);
}
void	LC_UART_TX_Send(uint8 *data, uint16 len)
{
	osal_memcpy(mBUP_Ctx.tx_buf, data, len);
	mBUP_Ctx.tx_size	=	len;
	mBUP_Ctx.tx_state	=	BUP_TX_ST_SENDING;
	osal_set_event(LC_Uart_TaskID, UART_EVT_UART_TX_COMPLETE);
}
/**
 * @brief 
 * 
 * @param cb 
 * @return int 
 */
int    LC_Uart_Init(void)
{
    BUP_ctx_t*   pctx   =   &mBUP_Ctx;
    uart_Cfg_t  CDX_Cfg =   {
        .tx_pin         =   UART_TX_PIN,
        .rx_pin         =   UART_RX_PIN,
        .rts_pin        =   GPIO_DUMMY,
        .cts_pin        =   GPIO_DUMMY,
        .baudrate       =   Uart_Baudrate,
        .use_fifo       =   TRUE,
        .hw_fwctrl      =   FALSE,
        .use_tx_buf     =   TRUE,
        .parity         =   FALSE,
        .evt_handler    =   LC_USART_Handler_Evt,
    };

    hal_uart_init(CDX_Cfg, UART1);
    hal_uart_set_tx_buf(UART1, pctx->hal_uart_tx_buf, UART_TX_BUF_SIZE);

    // hal_pwrmgr_register(MOD_USR1, NULL, NULL);
	osal_memset(&mBUP_Ctx, 0, sizeof(mBUP_Ctx));
    return  PPlus_SUCCESS;
}

int     LC_Uart_RXData(void)
{
    BUP_ctx_t*  pctx    =   &mBUP_Ctx;

    osal_memcpy(pctx->rx_buf + pctx->rx_size, pctx->hal_uart_rx_buf, pctx->hal_uart_rx_size);
    if(pctx->rx_offset != 0)
	{
        return  PPlus_ERR_BLE_BUSY;
    }

    pctx->rx_size +=    pctx->hal_uart_rx_size;
    pctx->hal_uart_rx_size  =   0;
    switch(pctx->rx_state)
	{
        case BUP_RX_ST_IDLE:
            pctx->rx_state  =   BUP_RX_ST_DELAY_SLOT;
            rx_start_timer(1);
            break;
        case BUP_RX_ST_DELAY_SLOT:
        case BUP_RX_ST_SENDING:
        default:
            return  PPlus_ERR_INVALID_STATE;
    }
    return  PPlus_SUCCESS;
    
}

int     LC_Uart_TXData(void)
{
    BUP_ctx_t*  pctx    =   &mBUP_Ctx;
    if(pctx->tx_size && pctx->tx_state != BUP_TX_ST_IDLE){
        hal_uart_send_buff(UART1, pctx->tx_buf, pctx->tx_size);
        pctx->tx_size   =   0;
        pctx->tx_state  =   BUP_TX_ST_SENDING;
        
        return  PPlus_SUCCESS;
    }
    LOG("uart send err state\n");
    return PPlus_ERR_INVALID_STATE;
}

int     LC_Uart_TXDataDown(void)
{
    BUP_ctx_t* pctx =   &mBUP_Ctx;
    if(pctx->tx_size){
        LC_Uart_TXData();
        return  PPlus_SUCCESS;
    }
    pctx->tx_state  =   BUP_TX_ST_IDLE;
    return  PPlus_SUCCESS;
}
/**
 * @brief 
 * 
 * @param task_id 
 */
void    LC_Uart_Task_Init(uint8 task_id)
{
    LC_Uart_TaskID  =   task_id;
    LC_Uart_Init();
    LOG("uart task init\n");
}
/**
 * @brief       UI_LED_BUZZER Task event processor.This function
 *				is called to processs all events for the task.Events
 *				include timers,messages and any other user defined events.
 * 
 * @param task_id   :The OSAL assigned task ID.
 * @param events    :events to process.This is a bit map and can
 *									contain more than one event.
 * @return uint16 
 */
uint16	LC_Uart_ProcessEvent(uint8 task_id, uint16 events)
{
	VOID task_id;	// OSAL required parameter that isn't used in this function
	if(events & SYS_EVENT_MSG)
	{
		uint8	*pMsg;
		if((pMsg = osal_msg_receive(LC_Uart_TaskID)) != NULL)
		{
			LC_Common_ProcessOSALMsg((osal_event_hdr_t *)pMsg);
            // Release the OSAL message
			VOID osal_msg_deallocate(pMsg);
		}
		return(events ^ SYS_EVENT_MSG);
	}
    if(events & UART_EVT_TO_TIMER)
	{
        // LOG("RX_TO\n");
        LC_Uart_RXData();
        return (events ^ UART_EVT_TO_TIMER);
    }
    if(events & UART_EVT_UARTRX_TIMER)
	{
        BUP_ctx_t*  pctx    =   &mBUP_Ctx;
        // uint8   size        =   0;
        // bool    start_flag  =   FALSE;
        // uint8   readbuff[20]    =   {0, };
        if(pctx->rx_state != BUP_RX_ST_IDLE && pctx->rx_size)
		{

            if(pctx->rx_state == BUP_RX_ST_DELAY_SLOT)
			{
                // start_flag = TRUE;
                // FLOW_CTRL_BLE_TX_LOCK();
                pctx->rx_state = BUP_RX_ST_SENDING;
            }

            UartDataLen = ((pctx->rx_size - pctx->rx_offset));
            LOG("uart data:\n");
            LOG_DUMP_BYTE(pctx->rx_buf + pctx->rx_offset, UartDataLen);
			osal_memcpy(UartData, pctx->rx_buf+pctx->rx_offset, UartDataLen);
			osal_set_event(LC_Uart_TaskID, UART_EVT_DEAL_CMD);
            // LOG("Success\n");
            pctx->rx_state = BUP_RX_ST_IDLE;
            pctx->rx_offset = 0;
            pctx->rx_size = 0;
            // FLOW_CTRL_BLE_TX_UNLOCK();
            return PPlus_SUCCESS;

        }
		else
		{
            LOG("U2B s=:%x,%x",pctx->rx_state,pctx->rx_size);
            pctx->rx_state = BUP_RX_ST_IDLE;
            pctx->rx_offset = 0;
            pctx->rx_size = 0;
            // FLOW_CTRL_BLE_TX_UNLOCK();

        }
        return (events ^ UART_EVT_UARTRX_TIMER);
    }
    
    if(events & UART_EVT_UART_TX_COMPLETE)
	{
        //data uart send
        // LOG("TX complete\n");
        LC_Uart_TXDataDown();
        return(events ^ UART_EVT_UART_TX_COMPLETE);
    }

	if(events & UART_EVT_DEAL_CMD)
	{
		if((UartData[0] == 0x54) && (UartData[1] == 0x42))
		{
		}
		osal_memset(UartData, 0, UartDataLen);
		UartDataLen = 0;
		return(events ^ UART_EVT_DEAL_CMD);
	}

    // Discard unknown events
    return 0;
}
/** @}*/
