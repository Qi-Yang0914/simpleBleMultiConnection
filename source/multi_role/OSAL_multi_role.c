/**************************************************************************************************
    Filename:       OSAL_simpleBLECentral.c
    Revised:        $Date: 2020-03-03 15:46:41 -0800 (Thu, 03 Mar 2011) $
    Revision:       $Revision: 12 $

    Description:    OSAL task initalization for Simple BLE Central app.

**************************************************************************************************/

/**************************************************************************************************
                                              INCLUDES
 **************************************************************************************************/
#include "OSAL.h"
#include "OSAL_Tasks.h"

/* LL */
#include "ll.h"

/* HCI */
#include "hci_tl.h"

#if defined ( OSAL_CBTIMER_NUM_TASKS )
    #include "osal_cbTimer.h"
#endif

/* L2CAP */
#include "l2cap.h"

/* gap */
#include "gap.h"
#include "gapgattserver.h"
#include "gapbondmgr.h"

/* GATT */
#include "gatt.h"

#include "gattservapp.h"

/* Profiles */
#include "multi.h"

/* Application */
#include "multi_role.h"
#include "LC_UI_led_buzzer.h"
#include "LC_Uart.h"
#include "LC_Key.h"

#ifdef PHY_SLB_OTA_ENABLE
    #include "slb_app.h"
#endif

/*********************************************************************
    GLOBAL VARIABLES
*/

// The order in this table must be identical to the task initialization calls below in osalInitTask.
__ATTR_SECTION_SRAM__ const pTaskEventHandlerFn tasksArr[] =
{
    LL_ProcessEvent,
    HCI_ProcessEvent,
    #if defined ( OSAL_CBTIMER_NUM_TASKS )
    OSAL_CBTIMER_PROCESS_EVENT( osal_CbTimerProcessEvent ),           // task 3
    #endif

    L2CAP_ProcessEvent,
    SM_ProcessEvent,
    GAP_ProcessEvent,
    GATT_ProcessEvent,

    // GAPBondMgr_ProcessEvent,
    GATTServApp_ProcessEvent,
    GAPMultiRole_ProcessEvent,
    multiRoleApp_ProcessEvent,
	LC_UI_Led_Buzzer_ProcessEvent,
	LC_Uart_ProcessEvent,
	LC_Key_ProcessEvent,
    #ifdef PHY_SLB_OTA_ENABLE
    SLB_OTA_ProcessEvent,
    #endif
};

__ATTR_SECTION_SRAM__ const uint8 tasksCnt = sizeof( tasksArr ) / sizeof( tasksArr[0] );
uint16* tasksEvents;

/*********************************************************************
    FUNCTIONS
 *********************************************************************/

/*********************************************************************
    @fn      osalInitTasks

    @brief   This function invokes the initialization function for each task.

    @param   void

    @return  none
*/
void osalInitTasks( void )
{
    uint8 taskID = 0;
    tasksEvents = (uint16*)osal_mem_alloc( sizeof( uint16 ) * tasksCnt);
    osal_memset( tasksEvents, 0, (sizeof( uint16 ) * tasksCnt));
    /* LL Task */
    LL_Init( taskID++ );
    /* HCI Task */
    HCI_Init( taskID++ );
    #if defined ( OSAL_CBTIMER_NUM_TASKS )
    /* Callback Timer Tasks */
    osal_CbTimerInit( taskID );
    taskID += OSAL_CBTIMER_NUM_TASKS;
    #endif
    /* L2CAP Task */
    L2CAP_Init( taskID++ );
    /* SM Task */
    SM_Init( taskID++ );
    /* GAP Task */
    GAP_Init( taskID++ );
    /* GATT Task */
    GATT_Init( taskID++ );
    /* Profiles */
    // GAPBondMgr_Init( taskID++ );
    GATTServApp_Init( taskID++ );
    GAPMultiRole_Init( taskID++ );
    /* Application */
    multiRoleApp_Init( taskID++ );
	LC_UI_Led_Buzzer_Task_Init(taskID++);
	LC_Uart_Task_Init(taskID++);
	LC_Key_Task_Init(taskID++);
    #ifdef PHY_SLB_OTA_ENABLE
    SLB_OTA_Init( taskID );
    #endif
}

/*********************************************************************
*********************************************************************/
