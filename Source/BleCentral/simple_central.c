/******************************************************************************
 Release Name: simplelink_cc2640r2_sdk_01_50_00_58
 Release Date: 2017-10-17 18:09:51
 *****************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <string.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/display/Display.h>

#if defined( USE_FPGA ) || defined( DEBUG_SW_TRACE )
#include <driverlib/ioc.h>
#endif // USE_FPGA | DEBUG_SW_TRACE

#include "bcomdef.h"

#include <icall.h>
#include "util.h"
/* This Header file contains all BLE API and icall structure definition */
#include "icall_ble_api.h"

#include "central.h"
#include "simple_gatt_profile.h"

#include "board_key.h"
#include "board.h"

#include "simple_central.h"

#include "ble_user_config.h"
#include "TaskConfig.h"
#include "MyEventConfig.h"
#include "BF_Util.h"
#include "Trace.h"
#include "TaskTest.h"
#include "Hal_oled.h"
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define SBC_STATE_CHANGE_EVT                  0x0001
#define SBC_KEY_CHANGE_EVT                    0x0002
#define SBC_RSSI_READ_EVT                     0x0004
#define SBC_PAIRING_STATE_EVT                 0x0008
#define SBC_PASSCODE_NEEDED_EVT               0x0010
#define SBP_TIMER_TIMEUP_EVT                  0x0020
//// Simple Central Task Events
//#define SBC_ICALL_EVT                         ICALL_MSG_EVENT_ID // Event_Id_31
//#define SBC_QUEUE_EVT                         UTIL_QUEUE_EVENT_ID // Event_Id_30
//#define SBC_START_DISCOVERY_EVT               Event_Id_00
//
//#define SBC_ALL_EVENTS                        (SBC_ICALL_EVT           | \
//                                               SBC_QUEUE_EVT           | \
//                                               SBC_START_DISCOVERY_EVT)

// Maximum number of scan responses
#define DEFAULT_MAX_SCAN_RES                  8

// Scan duration in ms
#define DEFAULT_SCAN_DURATION                 300

// Discovery mode (limited, general, all)
#define DEFAULT_DISCOVERY_MODE                DEVDISC_MODE_ALL

// TRUE to use active scan
#define DEFAULT_DISCOVERY_ACTIVE_SCAN         TRUE

// TRUE to use white list during discovery
#define DEFAULT_DISCOVERY_WHITE_LIST          FALSE

// TRUE to use high scan duty cycle when creating link
#define DEFAULT_LINK_HIGH_DUTY_CYCLE          FALSE

// TRUE to use white list when creating link
#define DEFAULT_LINK_WHITE_LIST               FALSE

// Default RSSI polling period in ms
#define DEFAULT_RSSI_PERIOD                   1000

// After the connection is formed, the central will accept connection parameter
// update requests from the peripheral
#define DEFAULT_ENABLE_UPDATE_REQUEST         GAPCENTRALROLE_PARAM_UPDATE_REQ_AUTO_ACCEPT

// Minimum connection interval (units of 1.25ms) if automatic parameter update
// request is enabled
#define DEFAULT_UPDATE_MIN_CONN_INTERVAL      40//400

// Maximum connection interval (units of 1.25ms) if automatic parameter update
// request is enabled
#define DEFAULT_UPDATE_MAX_CONN_INTERVAL      40//800

// Slave latency to use if automatic parameter update request is enabled
#define DEFAULT_UPDATE_SLAVE_LATENCY          0

// Supervision timeout value (units of 10ms) if automatic parameter update
// request is enabled
#define DEFAULT_UPDATE_CONN_TIMEOUT           200//600

// Default GAP pairing mode
#define DEFAULT_PAIRING_MODE                  GAPBOND_PAIRING_MODE_WAIT_FOR_REQ

// Default MITM mode (TRUE to require passcode or OOB when pairing)
#define DEFAULT_MITM_MODE                     FALSE

// Default bonding mode, TRUE to bond
#define DEFAULT_BONDING_MODE                  TRUE

// Default GAP bonding I/O capabilities
#define DEFAULT_IO_CAPABILITIES               GAPBOND_IO_CAP_DISPLAY_ONLY

// Default service discovery timer delay in ms
#define DEFAULT_SVC_DISCOVERY_DELAY           1000

// TRUE to filter discovery results on desired service UUID
#define DEFAULT_DEV_DISC_BY_SVC_UUID          TRUE

// Length of bd addr as a string
#define B_ADDR_STR_LEN                        15

// Type of Display to open
#if !defined(Display_DISABLE_ALL)
  #if defined(BOARD_DISPLAY_USE_LCD) && (BOARD_DISPLAY_USE_LCD!=0)
    #define SBC_DISPLAY_TYPE Display_Type_LCD
  #elif defined (BOARD_DISPLAY_USE_UART) && (BOARD_DISPLAY_USE_UART!=0)
    #define SBC_DISPLAY_TYPE Display_Type_UART
  #else // !BOARD_DISPLAY_USE_LCD && !BOARD_DISPLAY_USE_UART
    #define SBC_DISPLAY_TYPE 0 // Option not supported
  #endif // BOARD_DISPLAY_USE_LCD && BOARD_DISPLAY_USE_UART
#else // Display_DISABLE_ALL
  #define SBC_DISPLAY_TYPE 0 // No Display
#endif // Display_DISABLE_ALL

// Task configuration
#define SBC_TASK_PRIORITY                     2

#ifndef SBC_TASK_STACK_SIZE
#define SBC_TASK_STACK_SIZE                   2200
#endif

// Application states
enum
{
  BLE_STATE_IDLE,
  BLE_STATE_CONNECTING,
  BLE_STATE_CONNECTED,
  BLE_STATE_DISCONNECTING
};

// Discovery states
enum
{
  BLE_DISC_STATE_IDLE,                // Idle
  BLE_DISC_STATE_MTU,                 // Exchange ATT MTU size
  BLE_DISC_STATE_SVC,                 // Service discovery
  BLE_DISC_STATE_CHAR                 // Characteristic discovery
};

// Key states for connections
typedef enum {
  GATT_RW,                 // Perform GATT Read/Write
  RSSI,                    // Toggle RSSI updates
  CONN_UPDATE,             // Send Connection Parameter Update
  GET_CONN_INFO,           // Display Current Connection Information
  DISCONNECT               // Disconnect
} keyPressConnOpt_t;

/*********************************************************************
 * TYPEDEFS
 */

// App event passed from profiles.
//typedef struct
//{
//  appEvtHdr_t hdr; // event header
//  uint8_t *pData;  // event data
//} sbcEvt_t;

// RSSI read data structure
typedef struct
{
  uint16_t period;      // how often to read RSSI
  uint16_t connHandle;  // connection handle
  Clock_Struct *pClock; // pointer to clock struct
} readRssi_t;

/*********************************************************************
 * GLOBAL VARIABLES
 */

// Display Interface
Display_Handle dispHandle = NULL;

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

// Entity ID globally used to check for source and/or destination of messages
static ICall_EntityID selfEntity;

// Event globally used to post local events and pend on system and
// local events.
static ICall_SyncHandle syncEvent;

// Clock object used to signal timeout
static Clock_Struct startDiscClock;

// Queue object used for app messages
//static Queue_Struct appMsg;
//static Queue_Handle appMsgQueue;

// Task configuration
Task_Struct sbcTask;
Char sbcTaskStack[SBC_TASK_STACK_SIZE];

// GAP GATT Attributes
static const uint8_t attDeviceName[GAP_DEVICE_NAME_LEN] = "Simple Central";

// Number of scan results and scan result index
static uint8_t scanRes = 0;
static int8_t scanIdx = -1;

// Scan result list
static gapDevRec_t devList[DEFAULT_MAX_SCAN_RES];

// Scanning state
static bool scanningStarted = FALSE;

// Connection handle of current connection
static uint16_t connHandle = GAP_CONNHANDLE_INIT;

// Application state
static uint8_t state = BLE_STATE_IDLE;

// Discovery state
static uint8_t discState = BLE_DISC_STATE_IDLE;

// Discovered service start and end handle
static uint16_t svcStartHdl = 0;
static uint16_t svcEndHdl = 0;

// Discovered characteristic handle
static uint16_t charHdl = 0;

// Value to write
//static uint8_t charVal = 0;

// Value read/write toggle
static bool doWrite = FALSE;

// GATT read/write procedure state
static bool procedureInProgress = FALSE;

// Maximum PDU size (default = 27 octets)
static uint16 maxPduSize;

// Array of RSSI read structures
static readRssi_t readRssi[MAX_NUM_BLE_CONNS];

// Key option state.
static keyPressConnOpt_t keyPressConnOpt = DISCONNECT;

static uint32_t countSend = 0;
/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void SimpleBLECentral_init(void);
static void SimpleBLECentral_taskFxn(UArg a0, UArg a1);

static void SimpleBLECentral_processGATTMsg(gattMsgEvent_t *pMsg);
static void SimpleBLECentral_handleKeys(uint8_t shift, uint8_t keys);
static void SimpleBLECentral_processStackMsg(ICall_Hdr *pMsg);
static void SimpleBLECentral_processAppMsg(sbcEvt_t *pMsg);
static void SimpleBLECentral_processRoleEvent(gapCentralRoleEvent_t *pEvent);
static void SimpleBLECentral_processGATTDiscEvent(gattMsgEvent_t *pMsg);
static void SimpleBLECentral_startDiscovery(void);
static bool SimpleBLECentral_findSvcUuid(uint16_t uuid, uint8_t *pData,
                                         uint8_t dataLen);
static void SimpleBLECentral_addDeviceInfo(uint8_t *pAddr, uint8_t addrType);
static void SimpleBLECentral_processPairState(uint8_t state, uint8_t status);
static void SimpleBLECentral_processPasscode(uint16_t connectionHandle,
                                             uint8_t uiOutputs);

static void SimpleBLECentral_processCmdCompleteEvt(hciEvt_CmdComplete_t *pMsg);
static bStatus_t SimpleBLECentral_StartRssi(uint16_t connHandle, uint16_t period);
static bStatus_t SimpleBLECentral_CancelRssi(uint16_t connHandle);
static readRssi_t *SimpleBLECentral_RssiAlloc(uint16_t connHandle);
static readRssi_t *SimpleBLECentral_RssiFind(uint16_t connHandle);
static void SimpleBLECentral_RssiFree(uint16_t connHandle);

static uint8_t SimpleBLECentral_eventCB(gapCentralRoleEvent_t *pEvent);
static void SimpleBLECentral_passcodeCB(uint8_t *deviceAddr, uint16_t connHandle,
                                        uint8_t uiInputs, uint8_t uiOutputs);
static void SimpleBLECentral_pairStateCB(uint16_t connHandle, uint8_t state,
                                         uint8_t status);

void SimpleBLECentral_startDiscHandler(UArg a0);
void SimpleBLECentral_keyChangeHandler(uint8 keys);
void SimpleBLECentral_readRssiHandler(UArg a0);

//static uint8_t SimpleBLECentral_enqueueMsg(uint8_t event, uint8_t status, uint8_t *pData);
#define SimpleBLECentral_enqueueMsg  BF_UtilQueue_enqueueMsg

void simpleCentral_findDevice();
void simpleCentral_select();
void simpleCentral_action();

#ifdef FPGA_AUTO_CONNECT
static void SimpleBLECentral_startGapDiscovery(void);
static void SimpleBLECentral_connectToFirstDevice(void);
#endif // FPGA_AUTO_CONNECT

/*********************************************************************
 * EXTERN FUNCTIONS
 */
extern void AssertHandler(uint8 assertCause, uint8 assertSubcause);

/*********************************************************************
 * PROFILE CALLBACKS
 */

// Central GAPRole Callbacks
static gapCentralRoleCB_t SimpleBLECentral_roleCB =
{
  SimpleBLECentral_eventCB     // GAPRole Event Callback
};

// Bond Manager Callbacks
static gapBondCBs_t SimpleBLECentral_bondCB =
{
  (pfnPasscodeCB_t)SimpleBLECentral_passcodeCB, // Passcode callback
  SimpleBLECentral_pairStateCB                  // Pairing / Bonding state Callback
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

#ifdef FPGA_AUTO_CONNECT
/*********************************************************************
 * @fn      SimpleBLECentral_startGapDiscovery
 *
 * @brief   Start discovering devices
 *
 * @param   none
 *
 * @return  none
 */
static void SimpleBLECentral_startGapDiscovery(void)
{
  // Start discovery
  if ((state != BLE_STATE_CONNECTED) && (!scanningStarted))
  {
    scanningStarted = TRUE;
    scanRes = 0;

    Display_print0(dispHandle, 2, 0, "Discovering...");
    Display_clearLines(dispHandle, 3, 4);

    GAPCentralRole_StartDiscovery(DEFAULT_DISCOVERY_MODE,
                                  DEFAULT_DISCOVERY_ACTIVE_SCAN,
                                  DEFAULT_DISCOVERY_WHITE_LIST);
  }
}

/*********************************************************************
 * @fn      SimpleBLECentral_connectToFirstDevice
 *
 * @brief   Connect to first device in list of discovered devices
 *
 * @param   none
 *
 * @return  none
 */
static void SimpleBLECentral_connectToFirstDevice(void)
{
  uint8_t addrType;
  uint8_t *peerAddr;

  scanIdx = 0;

  if (state == BLE_STATE_IDLE)
  {
    // connect to current device in scan result
    peerAddr = devList[scanIdx].addr;
    addrType = devList[scanIdx].addrType;

    state = BLE_STATE_CONNECTING;

    GAPCentralRole_EstablishLink(DEFAULT_LINK_HIGH_DUTY_CYCLE,
                                 DEFAULT_LINK_WHITE_LIST,
                                 addrType, peerAddr);

    Display_print0(dispHandle, 2, 0, "Connecting");
    Display_print0(dispHandle, 3, 0, Util_convertBdAddr2Str(peerAddr));
    Display_clearLine(dispHandle, 4);
  }
}
#endif // FPGA_AUTO_CONNECT

/*********************************************************************
 * @fn      SimpleBLEPeripheral_createTask
 *
 * @brief   Task creation function for the Simple Peripheral.
 *
 * @param   none
 *
 * @return  none
 */
void SimpleBLECentral_createTask(void)
{
  Task_Params taskParams;

  // Configure task
  Task_Params_init(&taskParams);
  taskParams.stack = sbcTaskStack;
  taskParams.stackSize = SBC_TASK_STACK_SIZE;
  taskParams.priority = SBC_TASK_PRIORITY;

  Task_construct(&sbcTask, SimpleBLECentral_taskFxn, &taskParams, NULL);
}

/*********************************************************************
 * @fn      SimpleBLECentral_Init
 *
 * @brief   Initialization function for the Simple Central App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notification).
 *
 * @param   none
 *
 * @return  none
 */
static void SimpleBLECentral_init(void)
{
  uint8_t i;

  // ******************************************************************
  // N0 STACK API CALLS CAN OCCUR BEFORE THIS CALL TO ICall_registerApp
  // ******************************************************************
  // Register the current thread as an ICall dispatcher application
  // so that the application can send and receive messages.
  ICall_registerApp(&selfEntity, &syncEvent);

  BF_UtilQueue_Init(syncEvent);
  BF_UtilEvtCallbackInit(SBP_TIMER_TIMEUP_EVT);

#if defined( USE_FPGA )
  // configure RF Core SMI Data Link
  IOCPortConfigureSet(IOID_12, IOC_PORT_RFC_GPO0, IOC_STD_OUTPUT);
  IOCPortConfigureSet(IOID_11, IOC_PORT_RFC_GPI0, IOC_STD_INPUT);

  // configure RF Core SMI Command Link
  IOCPortConfigureSet(IOID_10, IOC_IOCFG0_PORT_ID_RFC_SMI_CL_OUT, IOC_STD_OUTPUT);
  IOCPortConfigureSet(IOID_9, IOC_IOCFG0_PORT_ID_RFC_SMI_CL_IN, IOC_STD_INPUT);

  // configure RF Core tracer IO
  IOCPortConfigureSet(IOID_8, IOC_PORT_RFC_TRC, IOC_STD_OUTPUT);
#else // !USE_FPGA
  #if defined( DEBUG_SW_TRACE )
    // configure RF Core tracer IO
    IOCPortConfigureSet(IOID_8, IOC_PORT_RFC_TRC, IOC_STD_OUTPUT | IOC_CURRENT_4MA | IOC_SLEW_ENABLE);
  #endif // DEBUG_SW_TRACE
#endif // USE_FPGA

  // Create an RTOS queue for message from profile to be sent to app.
//  appMsgQueue = Util_constructQueue(&appMsg);

  // Setup discovery delay as a one-shot timer
  Util_constructClock(&startDiscClock, SimpleBLECentral_startDiscHandler,
                      DEFAULT_SVC_DISCOVERY_DELAY, 0, false, 0);

  Board_initKeys(SimpleBLECentral_keyChangeHandler);

//  dispHandle = Display_open(SBC_DISPLAY_TYPE, NULL);

  // Initialize internal data
  for (i = 0; i < MAX_NUM_BLE_CONNS; i++)
  {
    readRssi[i].connHandle = GAP_CONNHANDLE_ALL;
    readRssi[i].pClock = NULL;
  }

  // Setup the Central GAPRole Profile. For more information see the GAP section
  // in the User's Guide:
  // http://software-dl.ti.com/lprf/sdg-latest/html/
  {
    uint8_t scanRes = DEFAULT_MAX_SCAN_RES;

    GAPCentralRole_SetParameter(GAPCENTRALROLE_MAX_SCAN_RES, sizeof(uint8_t),
                                &scanRes);
  }

  // Set GAP Parameters to set the discovery duration
  // For more information, see the GAP section of the User's Guide:
  // http://software-dl.ti.com/lprf/sdg-latest/html/
  GAP_SetParamValue(TGAP_GEN_DISC_SCAN, DEFAULT_SCAN_DURATION);
  GAP_SetParamValue(TGAP_LIM_DISC_SCAN, DEFAULT_SCAN_DURATION);
  GGS_SetParameter(GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN,
                   (void *)attDeviceName);

  // Setup the GAP Bond Manager. For more information see the GAP Bond Manager
  // section in the User's Guide:
  // http://software-dl.ti.com/lprf/sdg-latest/html/
  {
    // Don't send a pairing request after connecting; the device waits for the
    // application to start pairing
    uint8_t pairMode = DEFAULT_PAIRING_MODE;
    // Do not use authenticated pairing
    uint8_t mitm = DEFAULT_MITM_MODE;
    // This is a display only device
    uint8_t ioCap = DEFAULT_IO_CAPABILITIES;
    // Create a bond during the pairing process
    uint8_t bonding = DEFAULT_BONDING_MODE;

    GAPBondMgr_SetParameter(GAPBOND_PAIRING_MODE, sizeof(uint8_t), &pairMode);
    GAPBondMgr_SetParameter(GAPBOND_MITM_PROTECTION, sizeof(uint8_t), &mitm);
    GAPBondMgr_SetParameter(GAPBOND_IO_CAPABILITIES, sizeof(uint8_t), &ioCap);
    GAPBondMgr_SetParameter(GAPBOND_BONDING_ENABLED, sizeof(uint8_t), &bonding);
  }

  // Initialize GATT Client
  VOID GATT_InitClient();

  // Register to receive incoming ATT Indications/Notifications
  GATT_RegisterForInd(selfEntity);

  // Initialize GATT attributes
  GGS_AddService(GATT_ALL_SERVICES);         // GAP
  GATTServApp_AddService(GATT_ALL_SERVICES); // GATT attributes

  // Start the Device
  VOID GAPCentralRole_StartDevice(&SimpleBLECentral_roleCB);

  // Register with bond manager after starting device
  GAPBondMgr_Register(&SimpleBLECentral_bondCB);

  // Register with GAP for HCI/Host messages (for RSSI)
  GAP_RegisterForMsgs(selfEntity);

  // Register for GATT local events and ATT Responses pending for transmission
  GATT_RegisterForMsgs(selfEntity);

  //Set default values for Data Length Extension
  {
    //Set initial values to maximum, RX is set to max. by default(251 octets, 2120us)
    #define APP_SUGGESTED_PDU_SIZE 251 //default is 27 octets(TX)
    #define APP_SUGGESTED_TX_TIME 2120 //default is 328us(TX)

    //This API is documented in hci.h
    //See the LE Data Length Extension section in the BLE-Stack User's Guide for information on using this command:
    //http://software-dl.ti.com/lprf/sdg-latest/html/cc2640/index.html
    //HCI_LE_WriteSuggestedDefaultDataLenCmd(APP_SUGGESTED_PDU_SIZE, APP_SUGGESTED_TX_TIME);
  }

  Display_print0(dispHandle, 0, 0, "BLE Central");
}

/*********************************************************************
 * @fn      SimpleBLECentral_taskFxn
 *
 * @brief   Application task entry point for the Simple Central.
 *
 * @param   none
 *
 * @return  events not processed
 */
static void SimpleBLECentral_taskFxn(UArg a0, UArg a1)
{
  // Initialize application
  SimpleBLECentral_init();

  Module_initInTask();

  // Application main loop
  for (;;)
  {
    uint32_t events;

    events = Event_pend(syncEvent, Event_Id_NONE, SBC_ALL_EVENTS,
                        ICALL_TIMEOUT_FOREVER);

    if (events)
    {
      ICall_EntityID dest;
      ICall_ServiceEnum src;
      ICall_HciExtEvt *pMsg = NULL;

      if (ICall_fetchServiceMsg(&src, &dest,
                                (void **)&pMsg) == ICALL_ERRNO_SUCCESS)
      {
        if ((src == ICALL_SERVICE_CLASS_BLE) && (dest == selfEntity))
        {
          // Process inter-task message
          SimpleBLECentral_processStackMsg((ICall_Hdr *)pMsg);
        }

        if (pMsg)
        {
          ICall_freeMsg(pMsg);
        }
      }

      // If RTOS queue is not empty, process app message
      if (events & SBC_QUEUE_EVT)
      {
//        while (!Queue_empty(appMsgQueue))
        while (!BF_UtilQueueEmpty())
        {
//          sbcEvt_t *pMsg = (sbcEvt_t *)Util_dequeueMsg(appMsgQueue);
          sbpEvt_t *pMsg = (sbpEvt_t *) BF_UtilDeQueue();
          if (pMsg)
          {
            // Process message
            SimpleBLECentral_processAppMsg(pMsg);

            // Free the space from the message
            ICall_free(pMsg);
          }
        }
      }

      if (events & SBC_START_DISCOVERY_EVT)
      {
        SimpleBLECentral_startDiscovery();
      }

      MyEventConfig_handle(events);
    }
  }
}

/*********************************************************************
 * @fn      SimpleBLECentral_processStackMsg
 *
 * @brief   Process an incoming task message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void SimpleBLECentral_processStackMsg(ICall_Hdr *pMsg)
{
  switch (pMsg->event)
  {
    case GAP_MSG_EVENT:
      SimpleBLECentral_processRoleEvent((gapCentralRoleEvent_t *)pMsg);
      break;

    case GATT_MSG_EVENT:
      SimpleBLECentral_processGATTMsg((gattMsgEvent_t *)pMsg);
      break;

    case HCI_GAP_EVENT_EVENT:
      {
        // Process HCI message
        switch(pMsg->status)
        {
          case HCI_COMMAND_COMPLETE_EVENT_CODE:
            SimpleBLECentral_processCmdCompleteEvt((hciEvt_CmdComplete_t *)pMsg);
            break;

          case HCI_BLE_HARDWARE_ERROR_EVENT_CODE:
            AssertHandler(HAL_ASSERT_CAUSE_HARDWARE_ERROR,0);
            break;

          default:
            break;
        }
      }
      break;

    default:
      break;
  }
}

/*********************************************************************
 * @fn      SimpleBLECentral_processAppMsg
 *
 * @brief   Central application event processing function.
 *
 * @param   pMsg - pointer to event structure
 *
 * @return  none
 */
static void SimpleBLECentral_processAppMsg(sbcEvt_t *pMsg)
{
  switch (pMsg->hdr.event)
  {
    case SBC_STATE_CHANGE_EVT:
      SimpleBLECentral_processStackMsg((ICall_Hdr *)pMsg->pData);

      // Free the stack message
      ICall_freeMsg(pMsg->pData);
      break;

    case SBC_KEY_CHANGE_EVT:
      SimpleBLECentral_handleKeys(0, pMsg->hdr.state);
      break;

    case SBC_RSSI_READ_EVT:
      {
        readRssi_t *pRssi = (readRssi_t *)pMsg->pData;

        // If link is up and RSSI reads active
        if (pRssi->connHandle != GAP_CONNHANDLE_ALL &&
            linkDB_Up(pRssi->connHandle))
        {
          // Restart timer
          Util_restartClock(pRssi->pClock, pRssi->period);

          // Read RSSI
          VOID HCI_ReadRssiCmd(pRssi->connHandle);
        }
      }
      break;

    // Pairing event
    case SBC_PAIRING_STATE_EVT:
      {
        SimpleBLECentral_processPairState(pMsg->hdr.state, *pMsg->pData);

        ICall_free(pMsg->pData);
        break;
      }

    // Passcode event
    case SBC_PASSCODE_NEEDED_EVT:
      {
        SimpleBLECentral_processPasscode(connHandle, *pMsg->pData);

        ICall_free(pMsg->pData);
        break;
      }

    case SBP_TIMER_TIMEUP_EVT:
      {
        BF_UtilProcessEvt(pMsg->hdr.state);
        break;
      }

    default:
      // Do nothing.
      break;
  }
}


static gapCentralRoleEvent_t peripheralByRssi;
static uint8_t times_findPeripheralByRssi = 0;
static uint8_t times_findPeripheralByRssiError = 0;
static uint8_t peripheralMinRssi = 0xff;
static uint8_t peripheralTimesSearchSatisfy = 0;
static uint8_t peripheralTimesSearchNotSatisfy = 0;
static bool bleBindingStatus = false;
uint8_t searchDeviceAddr[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void simpleCentral_clearBindingStatus()
{
    bleBindingStatus = false;
}

bool simpleCentral_getBindingStatus()
{
    return bleBindingStatus;
}

/*********************************************************************
 * @fn      SimpleBLECentral_processRoleEvent
 *
 * @brief   Central role event processing function.
 *
 * @param   pEvent - pointer to event structure
 *
 * @return  none
 */
static void SimpleBLECentral_processRoleEvent(gapCentralRoleEvent_t *pEvent)
{
  switch (pEvent->gap.opcode)
  {
    case GAP_DEVICE_INIT_DONE_EVENT:
      {
        maxPduSize = pEvent->initDone.dataPktLen;

        Display_print0(dispHandle, 1, 0, Util_convertBdAddr2Str(pEvent->initDone.devAddr));
        Display_print0(dispHandle, 2, 0, "Initialized");

        // Prompt user to begin scanning.
        Display_print0(dispHandle, 5, 0, "Discover ->");

#ifdef FPGA_AUTO_CONNECT
        SimpleBLECentral_startGapDiscovery();
#endif // FPGA_AUTO_CONNECT
      }
      break;

    case GAP_DEVICE_INFO_EVENT:
      {
        // if filtering device discovery results based on service UUID
        if (DEFAULT_DEV_DISC_BY_SVC_UUID == TRUE)
        {
          if (SimpleBLECentral_findSvcUuid(SIMPLEPROFILE_SERV_UUID,
                                           pEvent->deviceInfo.pEvtData,
                                           pEvent->deviceInfo.dataLen))
          {
            TRACE_CODE("�����豸: %s, rssi:-%d dB\n", \
                        (uint8 *)Util_convertBdAddr2Str(pEvent->deviceInfo.addr),\
                        (uint8)(-( pEvent->deviceInfo.rssi )));

            if( true == BF_UtilCompairHex(pEvent->deviceInfo.addr, searchDeviceAddr, 6) )
            {
                //rssiֵ����Ҫ��
                if(peripheralMinRssi > (uint8)(-( pEvent->deviceInfo.rssi )))
                {
                    if(0 == times_findPeripheralByRssi)
                    {
                        times_findPeripheralByRssi++;
                        memcpy(&peripheralByRssi, pEvent, sizeof(peripheralByRssi));
                        TRACE_CODE("��1�η�������: %s, rssi:-%d dB\n", \
                                    (uint8 *)Util_convertBdAddr2Str(pEvent->deviceInfo.addr),\
                                    (uint8)(-( pEvent->deviceInfo.rssi )));
                    }
                    else
                    {
                        //��λ�ȡ����ͬ���豸��������
                        if( 0 == memcmp((uint8 *)Util_convertBdAddr2Str(pEvent->deviceInfo.addr), \
                                        (uint8 *)Util_convertBdAddr2Str(peripheralByRssi.deviceInfo.addr), \
                                 strlen((char *)Util_convertBdAddr2Str(peripheralByRssi.deviceInfo.addr))) )
                        {
                            times_findPeripheralByRssi++;
                            TRACE_CODE("��%d�η�������: %s, rssi:-%d dB\n", \
                                        times_findPeripheralByRssi,\
                                        (uint8 *)Util_convertBdAddr2Str(pEvent->deviceInfo.addr),\
                                        (uint8)(-( pEvent->deviceInfo.rssi )));
                        }
                        //һ���µ��豸��������
                        else
                        {
                            times_findPeripheralByRssi=1;
                            TRACE_CODE("�������󣬵�%d�η�������: %s, rssi:-%d dB\n", \
                                        times_findPeripheralByRssi,\
                                        (uint8 *)Util_convertBdAddr2Str(pEvent->deviceInfo.addr),\
                                        (uint8)(-( pEvent->deviceInfo.rssi )));
                            TRACE_CODE("save-%s\nnew -%s\n",(uint8 *)Util_convertBdAddr2Str(peripheralByRssi.deviceInfo.addr),\
                                        (uint8 *)Util_convertBdAddr2Str(pEvent->deviceInfo.addr));
                            memcpy(&peripheralByRssi, pEvent, sizeof(peripheralByRssi));
                        }
                    }
                }
                else//rssiֵ������Ҫ��
                {
                    TRACE_ERROR("mac��ַƥ��ɹ���������RSSIֵ������Ҫ��Ҫ��<%d,���˴�ɨ��Ϊ%d��\n", peripheralMinRssi, (uint8)(-( pEvent->deviceInfo.rssi )) );
                    times_findPeripheralByRssiError++;
                    if(times_findPeripheralByRssiError >= peripheralTimesSearchNotSatisfy)
                    {
                        TRACE_ERROR("mac��ַƥ��ɹ���������RSSIֵ������Ҫ��Ҫ��<%d,���˴�ɨ��Ϊ%d,�ﵽ%d�Σ����ش���\n",\
                                    peripheralMinRssi, (uint8)(-( pEvent->deviceInfo.rssi )) ,peripheralTimesSearchNotSatisfy);
                        TestEvent_post(EVENT_TESTPROCESS_BLE_DISCOVER_FAIL_RSSI);
                    }
                }

                if(times_findPeripheralByRssi >= peripheralTimesSearchSatisfy)
                {
                    SimpleBLECentral_addDeviceInfo(pEvent->deviceInfo.addr, pEvent->deviceInfo.addrType);
                    bleBindingStatus = true;
                    TRACE_CODE("�󶨳ɹ���\n");
                }
            }
          }
        }
      }
      break;

    case GAP_DEVICE_DISCOVERY_EVENT:
      {
        // discovery complete
        scanningStarted = FALSE;

        // if not filtering device discovery results based on service UUID
        if (DEFAULT_DEV_DISC_BY_SVC_UUID == FALSE)
        {
          // Copy results
          scanRes = pEvent->discCmpl.numDevs;
          memcpy(devList, pEvent->discCmpl.pDevList, (sizeof(gapDevRec_t) * scanRes));
        }

        Display_print1(dispHandle, 2, 0, "Devices Found %d", scanRes);
        TRACE_CODE("Devices Found %d\n", scanRes);
        if(scanRes == 0)
        {
            TestEvent_post(EVENT_TESTPROCESS_BLE_DISCOVER_FAIL);
        }
        else
        {
            TestEvent_post(EVENT_TESTPROCESS_BLE_DISCOVER);
        }
//        simpleCentral_findDevice(scanRes);

        for(uint8_t i=0; i<scanRes; i++)
        {
            Display_print0(dispHandle, 20+i, 0, Util_convertBdAddr2Str(devList[i].addr));
        }

        if (scanRes > 0)
        {
#ifndef FPGA_AUTO_CONNECT
          Display_print0(dispHandle, 3, 0, "<- To Select");
        }

        // Initialize scan index.
//        scanIdx = -1;
        TRACE_CODE("1----------------scanIdx = -1.\n");

        // Prompt user that re-performing scanning at this state is possible.
        Display_print0(dispHandle, 5, 0, "Discover ->");

#else // FPGA_AUTO_CONNECT
          SimpleBLECentral_connectToFirstDevice();
        }
#endif // FPGA_AUTO_CONNECT
      }
      break;

    case GAP_LINK_ESTABLISHED_EVENT:
      {
        if (pEvent->gap.hdr.status == SUCCESS)
        {
          state = BLE_STATE_CONNECTED;
          connHandle = pEvent->linkCmpl.connectionHandle;
          procedureInProgress = TRUE;

          // If service discovery not performed initiate service discovery
          if (charHdl == 0)
          {
            Util_startClock(&startDiscClock);
          }

          Display_print0(dispHandle, 2, 0, "Connected");
          Display_print0(dispHandle, 3, 0, Util_convertBdAddr2Str(pEvent->linkCmpl.devAddr));

          // Display the initial options for a Right key press.
          SimpleBLECentral_handleKeys(0, KEY_LEFT);
        }
        else
        {
          state = BLE_STATE_IDLE;
          connHandle = GAP_CONNHANDLE_INIT;
          discState = BLE_DISC_STATE_IDLE;

          Display_print0(dispHandle, 2, 0, "Connect Failed");
          Display_print1(dispHandle, 3, 0, "Reason: %d", pEvent->gap.hdr.status);
        }
      }
      break;

    case GAP_LINK_TERMINATED_EVENT:
      {
        state = BLE_STATE_IDLE;
        connHandle = GAP_CONNHANDLE_INIT;
        discState = BLE_DISC_STATE_IDLE;
        charHdl = 0;
        procedureInProgress = FALSE;
        keyPressConnOpt = DISCONNECT;
        scanIdx = -1;
        TRACE_CODE("2----------------scanIdx = -1.\n");
        TRACE_CODE("Disconnected success.\n");
        TestEvent_post(EVENT_TESTPROCESS_BLE_DISCONNECT);
        // Cancel RSSI reads
        SimpleBLECentral_CancelRssi(pEvent->linkTerminate.connectionHandle);

        Display_print0(dispHandle, 2, 0, "Disconnected");
        Display_print1(dispHandle, 3, 0, "Reason: %d", pEvent->linkTerminate.reason);
        Display_clearLine(dispHandle, 4);
        Display_clearLine(dispHandle, 6);

        // Prompt user to begin scanning.
        Display_print0(dispHandle, 5, 0, "Discover ->");
      }
      break;

    case GAP_LINK_PARAM_UPDATE_EVENT:
      {
        Display_print1(dispHandle, 2, 0, "Param Update: %d", pEvent->linkUpdate.status);
      }
      break;

    default:
      break;
  }
}

/*********************************************************************
 * @fn      SimpleBLECentral_handleKeys
 *
 * @brief   Handles all key events for this device.
 *
 * @param   shift - true if in shift/alt.
 * @param   keys - bit field for key events. Valid entries:
 *                 HAL_KEY_SW_2
 *                 HAL_KEY_SW_1
 *
 * @return  none
 */
static void SimpleBLECentral_handleKeys(uint8_t shift, uint8_t keys)
{
//  hciActiveConnInfo_t *pConnInfo; // pointer to hold return connection information
//  (void)shift;  // Intentionally unreferenced parameter
  if (keys & KEY_LEFT)
  {
//      TestEvent_post(EVENT_TESTPROCESS_CONFIRM_SUCCESS);
  }

  if (keys & KEY_RIGHT)
  {
//      TestEvent_post(EVENT_TESTPROCESS_CONFIRM_FAIL);
  }
//
//  if (keys & KEY_LEFT)
//  {
//    // If not connected
//    if (state == BLE_STATE_IDLE)
//    {
//      // If not currently scanning
//      if (!scanningStarted)
//      {
//        // Increment index of current result.
//        scanIdx++;
//
//        // If there are no scanned devices
//        if (scanIdx >= scanRes)
//        {
//          // Prompt the user to begin scanning again.
//          scanIdx = -1;
//          TRACE_DEBUG("3----------------scanIdx = -1.\n");
//          Display_print0(dispHandle, 2, 0, "");
//          Display_print0(dispHandle, 3, 0, "");
//          Display_print0(dispHandle, 5, 0, "Discover ->");
//        }
//        else
//        {
//          // Display the indexed scanned device.
//          Display_print1(dispHandle, 2, 0, "Device %d", (scanIdx + 1));
//          Display_print0(dispHandle, 3, 0, Util_convertBdAddr2Str(devList[scanIdx].addr));
//          Display_print0(dispHandle, 5, 0, "Connect ->");
//          Display_print0(dispHandle, 6, 0, "<- Next Option");
//        }
//      }
//    }
//    else if (state == BLE_STATE_CONNECTED)
//    {
//      keyPressConnOpt = (keyPressConnOpt == DISCONNECT) ? GATT_RW :
//                                          (keyPressConnOpt_t) (keyPressConnOpt + 1);
//
//      //clear excess lines to keep display clean if another option chosen
//      Display_doClearLines(dispHandle, 7, 16);
//
//      switch (keyPressConnOpt)
//      {
//        case GATT_RW:
//          Display_print0(dispHandle, 5, 0, "GATT Read/Write ->");
//          break;
//
//        case RSSI:
//          Display_print0(dispHandle, 5, 0, "Toggle Read RSSI ->");
//          break;
//
//        case CONN_UPDATE:
//          Display_print0(dispHandle, 5, 0, "Connection Update ->");
//          break;
//
//        case GET_CONN_INFO:
//          Display_print0(dispHandle, 5, 0, "Connection Info ->");
//          break;
//
//        case DISCONNECT:
//          Display_print0(dispHandle, 5, 0, "Disconnect ->");
//          break;
//
//        default:
//          break;
//      }
//
//      Display_print0(dispHandle, 6, 0, "<- Next Option");
//    }
//
//    return;
//  }
//
//  if (keys & KEY_RIGHT)
//  {
//    if (state == BLE_STATE_IDLE)
//    {
//      if (scanIdx == -1)
//      {
//        if (!scanningStarted)
//        {
//          scanningStarted = TRUE;
//          scanRes = 0;
//
//          Display_print0(dispHandle, 2, 0, "Discovering...");
//          Display_print0(dispHandle, 3, 0, "");
//          Display_print0(dispHandle, 4, 0, "");
//          Display_print0(dispHandle, 5, 0, "");
//          Display_print0(dispHandle, 6, 0, "");
//
//          GAPCentralRole_StartDiscovery(DEFAULT_DISCOVERY_MODE,
//                                        DEFAULT_DISCOVERY_ACTIVE_SCAN,
//                                        DEFAULT_DISCOVERY_WHITE_LIST);
//        }
//      }
//      // Connect if there is a scan result
//      else
//      {
//        // connect to current device in scan result
//        uint8_t *peerAddr = devList[scanIdx].addr;
//        uint8_t addrType = devList[scanIdx].addrType;
//
//        state = BLE_STATE_CONNECTING;
//
//        GAPCentralRole_EstablishLink(DEFAULT_LINK_HIGH_DUTY_CYCLE,
//                                     DEFAULT_LINK_WHITE_LIST,
//                                     addrType, peerAddr);
//
//        Display_print0(dispHandle, 2, 0, "Connecting");
//        Display_print0(dispHandle, 3, 0, Util_convertBdAddr2Str(peerAddr));
//        Display_clearLine(dispHandle, 4);
//
//        // Forget the scan results.
//        scanRes = 0;
//        scanIdx = -1;
//        TRACE_DEBUG("4----------------scanIdx = -1.\n");
//      }
//    }
//    else if (state == BLE_STATE_CONNECTED)
//    {
//      switch (keyPressConnOpt)
//      {
//        case GATT_RW:
//          if (charHdl != 0 &&
//              procedureInProgress == FALSE)
//          {
//            uint8_t status;
//
//            // Do a read or write as long as no other read or write is in progress
//            if (doWrite)
//            {
//              // Do a write
//              attWriteReq_t req;
//
//              req.pValue = GATT_bm_alloc(connHandle, ATT_WRITE_REQ, 20, NULL);
//              if ( req.pValue != NULL )
//              {
//                req.handle = charHdl;
//                req.len = 20;
//                memcpy(req.pValue, "TestQcStart000000000", 20);
//                countSend++;
//                memcpy(req.pValue+12, &countSend, sizeof(countSend));
//                req.sig = 0;
//                req.cmd = 0;
//
//                status = GATT_WriteCharValue(connHandle, &req, selfEntity);
//                if ( status != SUCCESS )
//                {
//                  GATT_bm_free((gattMsg_t *)&req, ATT_WRITE_REQ);
//                }
//              }
//              else
//              {
//                status = bleMemAllocError;
//              }
//            }
//            else
//            {
//              // Do a read
//              attReadReq_t req;
//
//              req.handle = charHdl;
//              status = GATT_ReadCharValue(connHandle, &req, selfEntity);
//            }
//
//            if (status == SUCCESS)
//            {
//              procedureInProgress = TRUE;
//              doWrite = !doWrite;
//            }
//          }
//          break;
//
//        case RSSI:
//          // Start or cancel RSSI polling
//          if (SimpleBLECentral_RssiFind(connHandle) == NULL)
//          {
//            SimpleBLECentral_StartRssi(connHandle, DEFAULT_RSSI_PERIOD);
//          }
//          else
//          {
//            SimpleBLECentral_CancelRssi(connHandle);
//
//            Display_print0(dispHandle, 4, 0, "RSSI Cancelled");
//          }
//          break;
//
//        case CONN_UPDATE:
//           // Connection update
//          GAPCentralRole_UpdateLink(connHandle,
//                                    DEFAULT_UPDATE_MIN_CONN_INTERVAL,
//                                    DEFAULT_UPDATE_MAX_CONN_INTERVAL,
//                                    DEFAULT_UPDATE_SLAVE_LATENCY,
//                                    DEFAULT_UPDATE_CONN_TIMEOUT);
//          break;
//
//        case GET_CONN_INFO:
//          pConnInfo= ICall_malloc(sizeof(hciActiveConnInfo_t));
//
//          if (pConnInfo != NULL)
//          {
//            // This is hard coded to assume we want connection info for a single
//            // valid connection as is the normal use case for simple central.
//            // A full featured application may chose to use HCI_EXT_GetConnInfoCmd()
//            // to obtain a full list of all active connections and their connId's
//            // to retrive more specific conneciton information if more than one
//            // valid connectin is expected to exist.
//            HCI_EXT_GetActiveConnInfoCmd(0, pConnInfo);
//            Display_print1(dispHandle, 7, 0, "AccessAddress: 0x%x", pConnInfo->accessAddr);
//            Display_print1(dispHandle, 8, 0, "Connection Interval: %d", pConnInfo->connInterval);
//            Display_print3(dispHandle, 9, 0, "HopVal:%d, nxtCh:%d, mSCA:%d", \
//                           pConnInfo->hopValue, pConnInfo->nextChan, \
//                           pConnInfo->mSCA);
//            Display_print5(dispHandle, 10, 0, "ChanMap: \"%x:%x:%x:%x:%x\"",\
//                           pConnInfo->chanMap[4], pConnInfo->chanMap[3],\
//                           pConnInfo->chanMap[2], pConnInfo->chanMap[1],\
//                           pConnInfo->chanMap[0]);
//
//            ICall_free(pConnInfo);
//          }
//          else
//          {
//            Display_print0(dispHandle, 4, 0, "ERROR: Failed to allocate memory for return connection information");
//          }
//          break;
//
//        case DISCONNECT:
//          state = BLE_STATE_DISCONNECTING;
//
//          GAPCentralRole_TerminateLink(connHandle);
//
//          Display_print0(dispHandle, 2, 0, "Disconnecting");
//          Display_print0(dispHandle, 3, 0, "");
//          Display_print0(dispHandle, 4, 0, "");
//          Display_print0(dispHandle, 5, 0, "");
//
//          keyPressConnOpt = GATT_RW;
//          break;
//
//        default:
//          break;
//      }
//    }
//
//    return;
//  }
}

#include "TimerConfig.h"
#include "TestProcess_ble.h"
typedef struct
{
  uint32_t iFlashKeyStatus;
  uint8_t iFlashAESKey[16];
  uint8_t iFlashFactoryAESKey[16];
  uint8_t iFlashHardwareUniqueIDCipher[8];
  uint8_t iFlashHardwareUniqueID[4];
  uint8_t iFlashOwnPassword[4];
  uint8_t iFlashNFCID[4];
  uint16_t iFlashAdvLongInterval;
  uint32_t iFlashBLEBaseTime;
  uint32_t iFlashFactoryTime;
  uint32_t NumReplacePwd[3];
  uint32_t NumShiftingPwd[3];
  uint8_t keyBaseNum[4];
  uint8_t iFlashOpenLockDuration;
  uint32_t utc_times;
  uint32_t latest_begin_time;
  uint32_t wechat_pwd_count;
  uint8_t once_pwd_count;
  uint8_t limit_pwd_count;

  uint8_t lockOperateKeyMark;
  uint8_t lockOperateKey[16];
}flashParam_ts;

static flashParam_ts flashParam;
static uint16_t flashParamIndex=0;
static uint32_t countSendNum = 0;
static uint32_t countRecieveNum = 0;

//Rssi �����0xff�������Rssi
void simpleCentral_searchCondition(uint8_t minRssi, uint8_t timesSearchSatisfy, uint8_t timesSearchNoSatisfy)
{
    peripheralMinRssi = minRssi;
    peripheralTimesSearchSatisfy = timesSearchSatisfy;
    peripheralTimesSearchNotSatisfy = timesSearchNoSatisfy;

    times_findPeripheralByRssi = 0;
    times_findPeripheralByRssiError = 0;
}

void simpleCentral_setSearchDeviceAddr(uint8_t* searchAddr)
{
    searchDeviceAddr[0] = searchAddr[5];
    searchDeviceAddr[1] = searchAddr[4];
    searchDeviceAddr[2] = searchAddr[3];
    searchDeviceAddr[3] = searchAddr[2];
    searchDeviceAddr[4] = searchAddr[1];
    searchDeviceAddr[5] = searchAddr[0];
}

//void simpleCentral_findDevice()
//{
//    TRACE_DEBUG("simpleCentral_findDevice finish\n");
//    simpleCentral_select();
//}

//void simpleCentral_findCharacteristicSuccess()
//{
//    TimerConfig_start(&sendDataClock, TIMER_SENDDATA_PERIOD_DEFAULT);
//}

extern void Timer_sendDataHandle(UArg arg)//active MyEvent_sendDataHandle
{
//    MyEvent_post(MY_SENDDATA_EVT);
}


extern void BleTest_writeData()
{
    uint8_t status;
    // Do a write
    attWriteReq_t req;

    req.pValue = GATT_bm_alloc(connHandle, ATT_WRITE_REQ, 20, NULL);
    if (req.pValue != NULL)
    {
        req.handle = charHdl;
        req.len = 20;
        memcpy(req.pValue, "TestQcStart000000000", 20);
        countSend++;
        memcpy(req.pValue + 12, &countSend, sizeof(countSend));
        req.sig = 0;
        req.cmd = 0;
        TRACE_CODE("Write data. Times = %d.\n", countSend);
        status = GATT_WriteCharValue(connHandle, &req, selfEntity);
        if (status != SUCCESS)
        {
            GATT_bm_free((gattMsg_t *) &req, ATT_WRITE_REQ);
        }
    }
    else
    {
        status = bleMemAllocError;
    }
}

extern void BleTest_readData()
{
//    uint8_t status;
    // Do a read
    attReadReq_t req;
    TRACE_CODE("Read data. Times = %d.\n", countSend);
    req.handle = charHdl;
//    status = GATT_ReadCharValue(connHandle, &req, selfEntity);
    GATT_ReadCharValue(connHandle, &req, selfEntity);
}

extern bool BleTest_finish(bool isSuccess)
{
    bool ret = FALSE;
    TRACE_CODE("Ble data test finish. send %d , read %d.\n", countSendNum, countRecieveNum);
//    if(countSend!=0 && countSend == countSendNum && countSendNum == countRecieveNum)
    if(isSuccess)
    {
        TRACE_CODE("iFlashKeyStatus:\t\t%08x\n",flashParam.iFlashKeyStatus);

        TRACE_CODE("iFlashAESKey:\t\t\t");
        TRACE_CODE_HEXGROUP(flashParam.iFlashAESKey, 16, ' ');

        TRACE_CODE("iFlashFactoryAESKey:\t\t");
        TRACE_CODE_HEXGROUP(flashParam.iFlashFactoryAESKey, 16, ' ');

        TRACE_CODE("iFlashHardwareUniqueIDCipher:\t");
        TRACE_CODE_HEXGROUP(flashParam.iFlashHardwareUniqueIDCipher, 8, ' ');
        for(uint8_t i=0; i<8; i++)
        {
            if(0xff != flashParam.iFlashHardwareUniqueIDCipher[i])
            {
                ret = TRUE;
            }
        }

        TRACE_CODE("iFlashHardwareUniqueID:\t\t");
        TRACE_CODE_HEXGROUP(flashParam.iFlashHardwareUniqueID, 4, ' ');

        TRACE_CODE("iFlashOwnPassword:\t\t");
        TRACE_CODE_HEXGROUP(flashParam.iFlashOwnPassword, 4, ' ');
    }
    GAPCentralRole_TerminateLink(connHandle);
    return ret;
}

extern void MyEvent_sendDataHandle()
{
    if(countSend >= 10&&false == doWrite)
    {
//        TRACE_DEBUG("Ble data test finish. send %d , read %d.\n", countSendNum, countRecieveNum);
//        if(countSend!=0 && countSend == countSendNum && countSendNum == countRecieveNum)
//        {
//            TestProcess_setResult(RET_TEST_BLE_SUCCESS);
//            TRACE_DEBUG("iFlashKeyStatus:\t\t%08x\n",flashParam.iFlashKeyStatus);
//
//            TRACE_DEBUG("iFlashAESKey:\t\t\t");
//            TRACE_DEBUG_HEXGROUP(flashParam.iFlashAESKey, 16, ' ');
//
//            TRACE_DEBUG("iFlashFactoryAESKey:\t\t");
//            TRACE_DEBUG_HEXGROUP(flashParam.iFlashFactoryAESKey, 16, ' ');
//
//            TRACE_DEBUG("iFlashHardwareUniqueIDCipher:\t");
//            TRACE_DEBUG_HEXGROUP(flashParam.iFlashHardwareUniqueIDCipher, 8, ' ');
//
//            TRACE_DEBUG("iFlashHardwareUniqueID:\t\t");
//            TRACE_DEBUG_HEXGROUP(flashParam.iFlashHardwareUniqueID, 4, ' ');
//
//            TRACE_DEBUG("iFlashOwnPassword:\t\t");
//            TRACE_DEBUG_HEXGROUP(flashParam.iFlashOwnPassword, 4, ' ');
//        }
//        GAPCentralRole_TerminateLink(connHandle);
//        return;
    }
    else
    {
//        TimerConfig_start(&sendDataClock, TIMER_SENDDATA_PERIOD_DEFAULT);
        if(false == doWrite)
            doWrite = true;
        else
            doWrite = false;
    }

//    uint8_t status;
    // Do a read or write as long as no other read or write is in progress
    if (doWrite)
    {
//        // Do a write
//        attWriteReq_t req;
//
//        req.pValue = GATT_bm_alloc(connHandle, ATT_WRITE_REQ, 20, NULL);
//        if (req.pValue != NULL)
//        {
//            req.handle = charHdl;
//            req.len = 20;
//            memcpy(req.pValue, "TestQcStart000000000", 20);
//            countSend++;
//            memcpy(req.pValue + 12, &countSend, sizeof(countSend));
//            req.sig = 0;
//            req.cmd = 0;
//            TRACE_DEBUG("Write data. Times = %d.\n", countSend);
//            status = GATT_WriteCharValue(connHandle, &req, selfEntity);
//            if (status != SUCCESS)
//            {
//                GATT_bm_free((gattMsg_t *) &req, ATT_WRITE_REQ);
//            }
//        }
//        else
//        {
//            status = bleMemAllocError;
//        }
    }
    else
    {
//        // Do a read
//        attReadReq_t req;
//        TRACE_DEBUG("Read data. Times = %d.\n", countSend);
//        req.handle = charHdl;
//        status = GATT_ReadCharValue(connHandle, &req, selfEntity);
    }
}

RET_FIND_DEVICE_INLIST BleTest_findDeviceInList(uint8_t* addr)
{
    RET_FIND_DEVICE_INLIST ret;
    // If not currently scanning
    if(scanningStarted)
    {
        ret = RET_FIND_DEVICE_INLIST_ERROR_SCANNING;
    }
    else
    {
        // Increment index of current result.
        scanIdx++;

        // If there are no scanned devices
        if (scanIdx >= scanRes)
        {
            ret = RET_FIND_DEVICE_INLIST_ERROR_NODEVICE;
            // Prompt the user to begin scanning again.
            scanIdx = -1;
            TRACE_CODE("5----------------scanIdx = -1.\n");
            Display_print0(dispHandle, 2, 0, "");
            Display_print0(dispHandle, 3, 0, "");
            Display_print0(dispHandle, 5, 0, "Discover ->");
            TRACE_DEBUG("BleTest_findDeviceInList: û���ҵ��κ��豸!\n");
        }
        else
        {
            // Display the indexed scanned device.
            Display_print1(dispHandle, 2, 0, "Device %d", (scanIdx + 1));
            Display_print0(dispHandle, 3, 0,Util_convertBdAddr2Str(devList[scanIdx].addr));
            Display_print0(dispHandle, 5, 0, "Connect ->");
            Display_print0(dispHandle, 6, 0, "<- Next Option");
            TRACE_CODE("����ɨ�赽���б�%d��ƥ��Ŀ��: [%s]\n", scanRes, (uint8 *)Util_convertBdAddr2Str(peripheralByRssi.deviceInfo.addr));
            for(uint8_t i=0; i<scanRes; i++)
            {
                TRACE_CODE("compare... %s\n",Util_convertBdAddr2Str(devList[i].addr));
                if(true == BF_UtilCompairHex(devList[i].addr, addr, 6))
//                if( 0 == memcmp((uint8 *)Util_convertBdAddr2Str(devList[i].addr), \
//                                (uint8 *)Util_convertBdAddr2Str(peripheralByRssi.deviceInfo.addr), \
//                                strlen((char *)Util_convertBdAddr2Str(peripheralByRssi.deviceInfo.addr))) )
                {
                    ret = RET_FIND_DEVICE_INLIST_OK;
                    TRACE_CODE("ƥ���豸�ɹ� %s.\n",Util_convertBdAddr2Str(devList[i].addr));
                    scanIdx = i;
                    TRACE_CODE("6----------------scanIdx = i = %d.\n", i);
                    break;
//                        simpleCentral_action();
//                        TestEvent_post(EVENT_TESTPROCESS_BLE_DISCOVER_SUCCESS);
                }
                else
                {
                    ret = RET_FIND_DEVICE_INLIST_ERROR_NOFIND;
                    TRACE_CODE("ƥ���豸ʧ�� %s!\n",Util_convertBdAddr2Str(devList[i].addr));
//                        TestProcess_setResult(RET_TEST_BLE_NOFIND);
//                        TestEvent_post(EVENT_TESTPROCESS_BLE_DISCOVER_FAIL);
                }
            }
        }
    }
    return ret;
}

void simpleCentral_select()
{
    // If not connected
    if (state == BLE_STATE_IDLE)
    {
        // If not currently scanning
        if (!scanningStarted)
        {
            // Increment index of current result.
            scanIdx++;

            // If there are no scanned devices
            if (scanIdx >= scanRes)
            {
                // Prompt the user to begin scanning again.
                scanIdx = -1;
                TRACE_CODE("7----------------scanIdx = -1.\n");
                Display_print0(dispHandle, 2, 0, "");
                Display_print0(dispHandle, 3, 0, "");
                Display_print0(dispHandle, 5, 0, "Discover ->");
            }
            else
            {
                // Display the indexed scanned device.
                Display_print1(dispHandle, 2, 0, "Device %d", (scanIdx + 1));
                Display_print0(dispHandle, 3, 0,Util_convertBdAddr2Str(devList[scanIdx].addr));
                Display_print0(dispHandle, 5, 0, "Connect ->");
                Display_print0(dispHandle, 6, 0, "<- Next Option");
                TRACE_CODE("��ҪѰ��: [%s]\n", Util_convertBdAddr2Str(searchDeviceAddr));
                for(uint8_t i=0; i<scanRes; i++)
                {
                    TRACE_CODE("���ζԱ�Ŀ��...\n",Util_convertBdAddr2Str(devList[i].addr));
                    if(true == BF_UtilCompairHex(devList[i].addr, searchDeviceAddr, 6) )
                    {
                        TRACE_CODE("�ҵ�Ŀ�� %s.\n",Util_convertBdAddr2Str(devList[scanIdx].addr));
                        scanIdx = i;
                        TRACE_CODE("8----------------scanIdx = i.\n");
//                        simpleCentral_action();
//                        TestEvent_post(EVENT_TESTPROCESS_BLE_DISCOVER_SUCCESS);
                    }
                    else
                    {
                        TRACE_ERROR("Find device by mac addr fail !.\n");
//                        TestProcess_setResult(RET_TEST_BLE_NOFIND);
//                        TestEvent_post(EVENT_TESTPROCESS_BLE_DISCOVER_FAIL);
                    }
                }

            }
        }
    }
    else if (state == BLE_STATE_CONNECTED)
    {
        keyPressConnOpt =
                (keyPressConnOpt == DISCONNECT) ?
                        GATT_RW : (keyPressConnOpt_t) (keyPressConnOpt + 1);

        //clear excess lines to keep display clean if another option chosen
        Display_doClearLines(dispHandle, 7, 16);

        switch (keyPressConnOpt)
        {
        case GATT_RW:
            Display_print0(dispHandle, 5, 0, "GATT Read/Write ->");
            break;

        case RSSI:
            Display_print0(dispHandle, 5, 0, "Toggle Read RSSI ->");
            break;

        case CONN_UPDATE:
            Display_print0(dispHandle, 5, 0, "Connection Update ->");
            break;

        case GET_CONN_INFO:
            Display_print0(dispHandle, 5, 0, "Connection Info ->");
            break;

        case DISCONNECT:
            Display_print0(dispHandle, 5, 0, "Disconnect ->");
            break;

        default:
            break;
        }

        Display_print0(dispHandle, 6, 0, "<- Next Option");
    }

    return;
}

RET_CONNECT BleTest_connect(bool isFirstConnect)
{
    RET_CONNECT ret = RET_CONNECT_OK;
    if (isFirstConnect)
    {
        TRACE_CODE("��һ������.\n");
        if (scanIdx == -1)
        {
            if (!scanningStarted)
            {
                ret = RET_CONNECT_ERROR_NODEVICE;
            }
            else
            {
                ret = RET_CONNECT_ERROR_SCANNING;
            }
        }
        // Connect if there is a scan result
        else
        {
            ret = RET_CONNECT_OK;
            // connect to current device in scan result
            uint8_t *peerAddr = devList[scanIdx].addr;
            //        uint8_t *peerAddr = peripheralByRssi.deviceInfo.addr;
            uint8_t addrType = devList[scanIdx].addrType;

            state = BLE_STATE_CONNECTING;
            TRACE_CODE("Connect target -%s \n",
                       Util_convertBdAddr2Str(peerAddr));
            bStatus_t status = GAPCentralRole_EstablishLink(
                    DEFAULT_LINK_HIGH_DUTY_CYCLE, DEFAULT_LINK_WHITE_LIST,
                    addrType, peerAddr);

            Display_print0(dispHandle, 2, 0, "Connecting");
            TRACE_CODE("Connect ret %d\n", status);
            TRACE_CODE(Util_convertBdAddr2Str(peerAddr));
            Display_clearLine(dispHandle, 4);

            // Forget the scan results.
            scanRes = 0;
            scanIdx = -1;
            TRACE_CODE("9----------------scanIdx = -1.\n");
        }
    }
    else
    {
        TRACE_CODE("���ǵ�һ������.\n");
        ret = RET_CONNECT_OK;
        state = BLE_STATE_CONNECTING;
        TRACE_CODE("Connect target -%s \n",
                Util_convertBdAddr2Str(peripheralByRssi.deviceInfo.addr));
        bStatus_t status = GAPCentralRole_EstablishLink(
                DEFAULT_LINK_HIGH_DUTY_CYCLE, DEFAULT_LINK_WHITE_LIST,
                peripheralByRssi.deviceInfo.addrType, peripheralByRssi.deviceInfo.addr);

        TRACE_CODE("Connect ret %d\n", status);
        TRACE_CODE(Util_convertBdAddr2Str(peripheralByRssi.deviceInfo.addr));
        Display_clearLine(dispHandle, 4);

        // Forget the scan results.
        scanRes = 0;
        scanIdx = -1;
    }

    return ret;
}

RET_DISCOVER BleTest_discover()
{
    RET_DISCOVER ret = RET_DISCOVER_OK;
    if(state != BLE_STATE_IDLE)
    {
        ret = RET_DISCOVER_ERROR_BLE_STATE;
    }
    else
    {
        if (scanIdx == -1)
        {
            if (!scanningStarted)
            {
                scanningStarted = TRUE;
                scanRes = 0;

                Display_print0(dispHandle, 2, 0, "Discovering...");
                Display_print0(dispHandle, 3, 0, "");
                Display_print0(dispHandle, 4, 0, "");
                Display_print0(dispHandle, 5, 0, "");
                Display_print0(dispHandle, 6, 0, "");

                GAPCentralRole_StartDiscovery(DEFAULT_DISCOVERY_MODE,
                                              DEFAULT_DISCOVERY_ACTIVE_SCAN,
                                              DEFAULT_DISCOVERY_WHITE_LIST);
            }
        }
        // Connect if there is a scan result
        else
        {
            //
            ret = RET_DISCOVER_ERROR_SCAN_RESULT;
            // Forget the scan results.
            scanRes = 0;
            scanIdx = -1;
            TRACE_CODE("10----------------scanIdx = -1.\n");
        }
    }
    return ret;
}

void simpleCentral_action()
{
    hciActiveConnInfo_t *pConnInfo; // pointer to hold return connection information
    if (state == BLE_STATE_IDLE)
    {
        if (scanIdx == -1)
        {
            if (!scanningStarted)
            {
                scanningStarted = TRUE;
                scanRes = 0;

                Display_print0(dispHandle, 2, 0, "Discovering...");
                Display_print0(dispHandle, 3, 0, "");
                Display_print0(dispHandle, 4, 0, "");
                Display_print0(dispHandle, 5, 0, "");
                Display_print0(dispHandle, 6, 0, "");

                GAPCentralRole_StartDiscovery(DEFAULT_DISCOVERY_MODE,
                                              DEFAULT_DISCOVERY_ACTIVE_SCAN,
                                              DEFAULT_DISCOVERY_WHITE_LIST);
            }
        }
        // Connect if there is a scan result
        else
        {
            // connect to current device in scan result
            uint8_t *peerAddr = devList[scanIdx].addr;
            uint8_t addrType = devList[scanIdx].addrType;

            state = BLE_STATE_CONNECTING;

            GAPCentralRole_EstablishLink(DEFAULT_LINK_HIGH_DUTY_CYCLE,
                                         DEFAULT_LINK_WHITE_LIST,
                                         addrType, peerAddr);

            Display_print0(dispHandle, 2, 0, "Connecting");
            Display_print0(dispHandle, 3, 0, Util_convertBdAddr2Str(peerAddr));
            Display_clearLine(dispHandle, 4);

            // Forget the scan results.
            scanRes = 0;
            scanIdx = -1;
            TRACE_CODE("11----------------scanIdx = -1.\n");
        }
    }
    else if (state == BLE_STATE_CONNECTED)
    {
        switch (keyPressConnOpt)
        {
        case GATT_RW:
            if (charHdl != 0 && procedureInProgress == FALSE)
            {
                uint8_t status;

                // Do a read or write as long as no other read or write is in progress
                if (doWrite)
                {
                    // Do a write
                    attWriteReq_t req;

                    req.pValue = GATT_bm_alloc(connHandle, ATT_WRITE_REQ, 20, NULL);
                    if (req.pValue != NULL)
                    {
                        req.handle = charHdl;
                        req.len = 20;
                        memcpy(req.pValue, "TestQcStart000000000", 20);
                        countSend++;
                        memcpy(req.pValue + 12, &countSend, sizeof(countSend));
                        req.sig = 0;
                        req.cmd = 0;

                        status = GATT_WriteCharValue(connHandle, &req, selfEntity);
                        if (status != SUCCESS)
                        {
                            GATT_bm_free((gattMsg_t *) &req, ATT_WRITE_REQ);
                        }
                    }
                    else
                    {
                        status = bleMemAllocError;
                    }
                }
                else
                {
                    // Do a read
                    attReadReq_t req;

                    req.handle = charHdl;
                    status = GATT_ReadCharValue(connHandle, &req, selfEntity);
                }

                if (status == SUCCESS)
                {
                    procedureInProgress = TRUE;
                    doWrite = !doWrite;
                }
            }
            break;

        case RSSI:
            // Start or cancel RSSI polling
            if (SimpleBLECentral_RssiFind(connHandle) == NULL)
            {
                SimpleBLECentral_StartRssi(connHandle, DEFAULT_RSSI_PERIOD);
            }
            else
            {
                SimpleBLECentral_CancelRssi(connHandle);

                Display_print0(dispHandle, 4, 0, "RSSI Cancelled");
            }
            break;

        case CONN_UPDATE:
            // Connection update
            GAPCentralRole_UpdateLink(connHandle,
                                      DEFAULT_UPDATE_MIN_CONN_INTERVAL,
                                      DEFAULT_UPDATE_MAX_CONN_INTERVAL,
                                      DEFAULT_UPDATE_SLAVE_LATENCY,
                                      DEFAULT_UPDATE_CONN_TIMEOUT);
            break;

        case GET_CONN_INFO:
            pConnInfo = ICall_malloc(sizeof(hciActiveConnInfo_t));

            if (pConnInfo != NULL)
            {
                // This is hard coded to assume we want connection info for a single
                // valid connection as is the normal use case for simple central.
                // A full featured application may chose to use HCI_EXT_GetConnInfoCmd()
                // to obtain a full list of all active connections and their connId's
                // to retrive more specific conneciton information if more than one
                // valid connectin is expected to exist.
                HCI_EXT_GetActiveConnInfoCmd(0, pConnInfo);
                Display_print1(dispHandle, 7, 0, "AccessAddress: 0x%x",
                               pConnInfo->accessAddr);
                Display_print1(dispHandle, 8, 0, "Connection Interval: %d",
                               pConnInfo->connInterval);
                Display_print3(dispHandle, 9, 0, "HopVal:%d, nxtCh:%d, mSCA:%d",
                               pConnInfo->hopValue, pConnInfo->nextChan,
                               pConnInfo->mSCA);
                Display_print5(dispHandle, 10, 0, "ChanMap: \"%x:%x:%x:%x:%x\"",
                               pConnInfo->chanMap[4], pConnInfo->chanMap[3],
                               pConnInfo->chanMap[2], pConnInfo->chanMap[1],
                               pConnInfo->chanMap[0]);

                ICall_free(pConnInfo);
            }
            else
            {
                Display_print0(
                        dispHandle,
                        4,
                        0,
                        "ERROR: Failed to allocate memory for return connection information");
            }
            break;

        case DISCONNECT:
            state = BLE_STATE_DISCONNECTING;

            GAPCentralRole_TerminateLink(connHandle);

            Display_print0(dispHandle, 2, 0, "Disconnecting");
            Display_print0(dispHandle, 3, 0, "");
            Display_print0(dispHandle, 4, 0, "");
            Display_print0(dispHandle, 5, 0, "");

            keyPressConnOpt = GATT_RW;
            break;

        default:
            break;
        }
    }

    return;
}

/*********************************************************************
 * @fn      SimpleBLECentral_processGATTMsg
 *
 * @brief   Process GATT messages and events.
 *
 * @return  none
 */
static void SimpleBLECentral_processGATTMsg(gattMsgEvent_t *pMsg)
{
  if (state == BLE_STATE_CONNECTED)
  {
    // See if GATT server was unable to transmit an ATT response
    if (pMsg->hdr.status == blePending)
    {
      // No HCI buffer was available. App can try to retransmit the response
      // on the next connection event. Drop it for now.
      Display_print1(dispHandle, 4, 0, "ATT Rsp dropped %d", pMsg->method);
    }
    else if ((pMsg->method == ATT_READ_RSP)   ||
             ((pMsg->method == ATT_ERROR_RSP) &&
              (pMsg->msg.errorRsp.reqOpcode == ATT_READ_REQ)))
    {
      if (pMsg->method == ATT_ERROR_RSP)
      {
        Display_print1(dispHandle, 4, 0, "Read Error %d", pMsg->msg.errorRsp.errCode);
      }
      else
      {
//        uint32_t countSendNum = 0;
//        uint32_t countRecieveNum = 0;
        memcpy(&countSendNum, pMsg->msg.readRsp.pValue+12, sizeof(countSendNum));
        memcpy(&countRecieveNum, pMsg->msg.readRsp.pValue+16, sizeof(countRecieveNum));
        // After a successful read, display the read value
        Display_print5(dispHandle, 4, 0, "Read rsp: %02x %02x %02x ---send:%d ��:%d", pMsg->msg.readRsp.pValue[0],  pMsg->msg.readRsp.pValue[1],  pMsg->msg.readRsp.pValue[2],  countSendNum,  countRecieveNum);
        TRACE_CODE("Read rsp: ---send:%d recieve:%d\n", countSendNum,  countRecieveNum);
        TRACE_CODE("Read data %02d times:", countRecieveNum);
        TestEvent_post(EVENT_TESTPROCESS_BLE_DATA_TRANS);
        TRACE_CODE_HEXGROUP(pMsg->msg.readRsp.pValue, 20, ' ');

        if(flashParamIndex+12 < sizeof(flashParam_ts))
        {
            memcpy( (uint8_t*)(&flashParam)+flashParamIndex, pMsg->msg.readRsp.pValue, 12 );
            flashParamIndex += 12;
        }
        else if(flashParamIndex < sizeof(flashParam_ts))
        {
            memcpy( (uint8_t*)(&flashParam)+flashParamIndex, pMsg->msg.readRsp.pValue, sizeof(flashParam_ts)-flashParamIndex );
        }
      }
      procedureInProgress = FALSE;
    }
    else if ((pMsg->method == ATT_WRITE_RSP)  ||
             ((pMsg->method == ATT_ERROR_RSP) &&
              (pMsg->msg.errorRsp.reqOpcode == ATT_WRITE_REQ)))
    {
      if (pMsg->method == ATT_ERROR_RSP)
      {
        Display_print1(dispHandle, 4, 0, "Write Error %d", pMsg->msg.errorRsp.errCode);
      }
      else
      {
          TestEvent_post(EVENT_TESTPROCESS_BLE_DATA_TRANS);
        // After a successful write, display the value that was written and
        // increment value
        uint32_t countSendNum = 0;
        uint32_t countRecieveNum = 0;
        memcpy(&countSendNum, pMsg->msg.readRsp.pValue+12, sizeof(countSendNum));
        memcpy(&countRecieveNum, pMsg->msg.readRsp.pValue+16, sizeof(countRecieveNum));
        Display_print5(dispHandle, 4, 0, "Write sent: %02x %02x %02x ---send:%d recieve:%d", pMsg->msg.readRsp.pValue[0],  pMsg->msg.readRsp.pValue[1],  pMsg->msg.readRsp.pValue[2],   countSendNum,  countRecieveNum);
        //        TRACE_DEBUG("Write rsp: ---send:%d recieve:%d\n", countSendNum,  countRecieveNum);
        TRACE_CODE("Write rsp.");
      }

      procedureInProgress = FALSE;

    }
    else if (pMsg->method == ATT_FLOW_CTRL_VIOLATED_EVENT)
    {
      // ATT request-response or indication-confirmation flow control is
      // violated. All subsequent ATT requests or indications will be dropped.
      // The app is informed in case it wants to drop the connection.

      // Display the opcode of the message that caused the violation.
      Display_print1(dispHandle, 4, 0, "FC Violated: %d", pMsg->msg.flowCtrlEvt.opcode);
    }
    else if (pMsg->method == ATT_MTU_UPDATED_EVENT)
    {
      // MTU size updated
      Display_print1(dispHandle, 4, 0, "MTU Size: %d", pMsg->msg.mtuEvt.MTU);
    }
    else if (discState != BLE_DISC_STATE_IDLE)
    {
      SimpleBLECentral_processGATTDiscEvent(pMsg);
    }
  } // else - in case a GATT message came after a connection has dropped, ignore it.

  // Needed only for ATT Protocol messages
  GATT_bm_free(&pMsg->msg, pMsg->method);
}

/*********************************************************************
 * @fn      SimpleBLECentral_processCmdCompleteEvt
 *
 * @brief   Process an incoming OSAL HCI Command Complete Event.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void SimpleBLECentral_processCmdCompleteEvt(hciEvt_CmdComplete_t *pMsg)
{
  switch (pMsg->cmdOpcode)
  {
    case HCI_READ_RSSI:
      {
        int8 rssi = (int8)pMsg->pReturnParam[3];

        Display_print1(dispHandle, 4, 0, "RSSI -dB: %d", (uint32_t)(-rssi));
      }
      break;

    default:
      break;
  }
}

/*********************************************************************
 * @fn      SimpleBLECentral_StartRssi
 *
 * @brief   Start periodic RSSI reads on a link.
 *
 * @param   connHandle - connection handle of link
 * @param   period - RSSI read period in ms
 *
 * @return  SUCCESS: Terminate started
 *          bleIncorrectMode: No link
 *          bleNoResources: No resources
 */
static bStatus_t SimpleBLECentral_StartRssi(uint16_t connHandle, uint16_t period)
{
  readRssi_t *pRssi;

  // Verify link is up
  if (!linkDB_Up(connHandle))
  {
    return bleIncorrectMode;
  }

  // If already allocated
  if ((pRssi = SimpleBLECentral_RssiFind(connHandle)) != NULL)
  {
    // Stop timer
    Util_stopClock(pRssi->pClock);

    pRssi->period = period;
  }
  // Allocate structure
  else if ((pRssi = SimpleBLECentral_RssiAlloc(connHandle)) != NULL)
  {
    pRssi->period = period;
  }
  // Allocate failed
  else
  {
    return bleNoResources;
  }

  // Start timer
  Util_restartClock(pRssi->pClock, period);

  return SUCCESS;
}

/*********************************************************************
 * @fn      SimpleBLECentral_CancelRssi
 *
 * @brief   Cancel periodic RSSI reads on a link.
 *
 * @param   connHandle - connection handle of link
 *
 * @return  SUCCESS: Operation successful
 *          bleIncorrectMode: No link
 */
static bStatus_t SimpleBLECentral_CancelRssi(uint16_t connHandle)
{
  readRssi_t *pRssi;

  if ((pRssi = SimpleBLECentral_RssiFind(connHandle)) != NULL)
  {
    // Stop timer
    Util_stopClock(pRssi->pClock);

    // Free RSSI structure
    SimpleBLECentral_RssiFree(connHandle);

    return SUCCESS;
  }

  // Not found
  return bleIncorrectMode;
}

/*********************************************************************
 * @fn      gapCentralRole_RssiAlloc
 *
 * @brief   Allocate an RSSI structure.
 *
 * @param   connHandle - Connection handle
 *
 * @return  pointer to structure or NULL if allocation failed.
 */
static readRssi_t *SimpleBLECentral_RssiAlloc(uint16_t connHandle)
{
  uint8_t i;

  // Find free RSSI structure
  for (i = 0; i < MAX_NUM_BLE_CONNS; i++)
  {
    if (readRssi[i].connHandle == GAP_CONNHANDLE_ALL)
    {
      readRssi_t *pRssi = &readRssi[i];

      pRssi->pClock = (Clock_Struct *)ICall_malloc(sizeof(Clock_Struct));
      if (pRssi->pClock)
      {
        Util_constructClock(pRssi->pClock, SimpleBLECentral_readRssiHandler,
                            0, 0, false, i);
        pRssi->connHandle = connHandle;

        return pRssi;
      }
    }
  }

  // No free structure found
  return NULL;
}

/*********************************************************************
 * @fn      gapCentralRole_RssiFind
 *
 * @brief   Find an RSSI structure.
 *
 * @param   connHandle - Connection handle
 *
 * @return  pointer to structure or NULL if not found.
 */
static readRssi_t *SimpleBLECentral_RssiFind(uint16_t connHandle)
{
  uint8_t i;

  // Find free RSSI structure
  for (i = 0; i < MAX_NUM_BLE_CONNS; i++)
  {
    if (readRssi[i].connHandle == connHandle)
    {
      return &readRssi[i];
    }
  }

  // Not found
  return NULL;
}

/*********************************************************************
 * @fn      gapCentralRole_RssiFree
 *
 * @brief   Free an RSSI structure.
 *
 * @param   connHandle - Connection handle
 *
 * @return  none
 */
static void SimpleBLECentral_RssiFree(uint16_t connHandle)
{
  uint8_t i;

  // Find RSSI structure
  for (i = 0; i < MAX_NUM_BLE_CONNS; i++)
  {
    if (readRssi[i].connHandle == connHandle)
    {
      readRssi_t *pRssi = &readRssi[i];
      if (pRssi->pClock)
      {
        Clock_destruct(pRssi->pClock);

        // Free clock struct
        ICall_free(pRssi->pClock);
        pRssi->pClock = NULL;
      }

      pRssi->connHandle = GAP_CONNHANDLE_ALL;
      break;
    }
  }
}

/*********************************************************************
 * @fn      SimpleBLECentral_processPairState
 *
 * @brief   Process the new paring state.
 *
 * @return  none
 */
static void SimpleBLECentral_processPairState(uint8_t state, uint8_t status)
{
  if (state == GAPBOND_PAIRING_STATE_STARTED)
  {
    Display_print0(dispHandle, 2, 0, "Pairing started");
  }
  else if (state == GAPBOND_PAIRING_STATE_COMPLETE)
  {
    if (status == SUCCESS)
    {
      Display_print0(dispHandle, 2, 0, "Pairing success");
    }
    else
    {
      Display_print1(dispHandle, 2, 0, "Pairing fail: %d", status);
    }
  }
  else if (state == GAPBOND_PAIRING_STATE_BONDED)
  {
    if (status == SUCCESS)
    {
      Display_print0(dispHandle, 2, 0, "Bonding success");
    }
  }
  else if (state == GAPBOND_PAIRING_STATE_BOND_SAVED)
  {
    if (status == SUCCESS)
    {
      Display_print0(dispHandle, 2, 0, "Bond save success");
    }
    else
    {
      Display_print1(dispHandle, 2, 0, "Bond save failed: %d", status);
    }
  }
}

/*********************************************************************
 * @fn      SimpleBLECentral_processPasscode
 *
 * @brief   Process the Passcode request.
 *
 * @return  none
 */
static void SimpleBLECentral_processPasscode(uint16_t connectionHandle,
                                             uint8_t uiOutputs)
{
  // This app uses a default passcode. A real-life scenario would handle all
  // pairing scenarios and likely generate this randomly.
  uint32_t passcode = B_APP_DEFAULT_PASSCODE;

  // Display passcode to user
  if (uiOutputs != 0)
  {
    Display_print1(dispHandle, 4, 0, "Passcode: %d", passcode);
  }

  // Send passcode response
  GAPBondMgr_PasscodeRsp(connectionHandle, SUCCESS, passcode);
}

/*********************************************************************
 * @fn      SimpleBLECentral_startDiscovery
 *
 * @brief   Start service discovery.
 *
 * @return  none
 */
static void SimpleBLECentral_startDiscovery(void)
{
  attExchangeMTUReq_t req;

  // Initialize cached handles
  svcStartHdl = svcEndHdl = charHdl = 0;

  discState = BLE_DISC_STATE_MTU;

  // Discover GATT Server's Rx MTU size
  req.clientRxMTU = maxPduSize - L2CAP_HDR_SIZE;

  // ATT MTU size should be set to the minimum of the Client Rx MTU
  // and Server Rx MTU values
  VOID GATT_ExchangeMTU(connHandle, &req, selfEntity);
}

/*********************************************************************
 * @fn      SimpleBLECentral_processGATTDiscEvent
 *
 * @brief   Process GATT discovery event
 *
 * @return  none
 */
static void SimpleBLECentral_processGATTDiscEvent(gattMsgEvent_t *pMsg)
{
    if (discState == BLE_DISC_STATE_MTU)
    {
        // MTU size response received, discover simple service
        if (pMsg->method == ATT_EXCHANGE_MTU_RSP)
        {
            uint8_t uuid[ATT_BT_UUID_SIZE] ={ LO_UINT16(SIMPLEPROFILE_SERV_UUID),\
                                              HI_UINT16(SIMPLEPROFILE_SERV_UUID) };
            // Just in case we're using the default MTU size (23 octets)
            Display_print1(dispHandle, 4, 0, "MTU Size: %d", ATT_MTU_SIZE);

            discState = BLE_DISC_STATE_SVC;

            // Discovery simple service
            Display_print2(dispHandle, 10, 0, "Start find server by UUID %02x%02x...\n", uuid[1], uuid[0]);
            TRACE_CODE("Start find server by UUID %02x%02x...", uuid[1], uuid[0]);
            VOID GATT_DiscPrimaryServiceByUUID(connHandle, uuid, ATT_BT_UUID_SIZE, selfEntity);
        }
        else
        {
            TRACE_ERROR("MTU size response received fail !\n");
//            TestProcess_setResult(RET_TEST_BLE_ERROR_MTUSIZE);
        }
    }
    else if (discState == BLE_DISC_STATE_SVC)
    {
        // Service found, store handles
        if (pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP && pMsg->msg.findByTypeValueRsp.numInfo > 0)
        {
            Display_print2(dispHandle, 11,0,"Service found, store handles,svcStartHdl=%04x; svcEndHdl=%04x",svcStartHdl, svcEndHdl);
            TRACE_CODE("Service found, store handles,svcStartHdl=%04x; svcEndHdl=%04x\n",svcStartHdl, svcEndHdl);
            svcStartHdl = ATT_ATTR_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
            svcEndHdl = ATT_GRP_END_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
        }
        else
        {
//        TRACE_ERROR("Service find fail !\n");
//        TestProcess_setResult(RET_TEST_BLE_ERROR_SERVICE);
        }

        // If procedure complete
        if (((pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP)
                && (pMsg->hdr.status == bleProcedureComplete))
                || (pMsg->method == ATT_ERROR_RSP))
        {
            if (svcStartHdl != 0)
            {
                attReadByTypeReq_t req;

                // Discover characteristic
                discState = BLE_DISC_STATE_CHAR;

                req.startHandle = svcStartHdl;
                req.endHandle = svcEndHdl;
                req.type.len = ATT_BT_UUID_SIZE;
                req.type.uuid[0] = LO_UINT16(SIMPLEPROFILE_CHAR1_UUID);
                req.type.uuid[1] = HI_UINT16(SIMPLEPROFILE_CHAR1_UUID);
                Display_print2(
                        dispHandle, 12, 0,
                        "Start Discover characteristic by UUID %02x%02x...",
                        req.type.uuid[1], req.type.uuid[0]);
                TRACE_CODE(
                        "Start Discover characteristic by UUID %02x%02x...\n",
                        req.type.uuid[1], req.type.uuid[0]);
                VOID GATT_ReadUsingCharUUID(connHandle, &req, selfEntity);
            }
        }
        else
        {
//        TRACE_ERROR("Procedure complete fail !\n");
//        TestProcess_setResult(RET_TEST_BLE_ERROR_PROCEDURE_COMPLETE);
        }
    }
    else if (discState == BLE_DISC_STATE_CHAR)
    {
        Display_print0(dispHandle, 13, 0, "Discover characteristic finish.");
        // Characteristic found, store handle
        if ((pMsg->method == ATT_READ_BY_TYPE_RSP)
                && (pMsg->msg.readByTypeRsp.numPairs > 0))
        {
            charHdl = BUILD_UINT16(pMsg->msg.readByTypeRsp.pDataList[0],
                                   pMsg->msg.readByTypeRsp.pDataList[1]);

            Display_print0(dispHandle, 2, 0, "Simple Svc Found");
            TRACE_CODE("Simple Svc Found\n");
            procedureInProgress = FALSE;
            countSend = 0;
            countSendNum = 0;
            countRecieveNum = 0;
            flashParamIndex = 0;

            memset((uint8_t*) &flashParam, 0x00, sizeof(flashParam));
            Display_print1(dispHandle, 13, 0,
                           "Discover characteristic success, charHdl = %04x.",
                           charHdl);
            TRACE_CODE(    "Discover characteristic success, charHdl = %04x.\n",charHdl);
//            simpleCentral_findCharacteristicSuccess();
            TestEvent_post(EVENT_TESTPROCESS_BLE_CONNECT);
        }
        else
        {
            TRACE_ERROR("Characteristic found fail !\n");
//            TestProcess_setResult(RET_TEST_BLE_ERROR_CHARACTERISTIC);
        }

        discState = BLE_DISC_STATE_IDLE;
    }
}

/*********************************************************************
 * @fn      SimpleBLECentral_findSvcUuid
 *
 * @brief   Find a given UUID in an advertiser's service UUID list.
 *
 * @return  TRUE if service UUID found
 */
static bool SimpleBLECentral_findSvcUuid(uint16_t uuid, uint8_t *pData,
                                         uint8_t dataLen)
{
  uint8_t adLen;
  uint8_t adType;
  uint8_t *pEnd;

  pEnd = pData + dataLen - 1;

  // While end of data not reached
  while (pData < pEnd)
  {
    // Get length of next AD item
    adLen = *pData++;
    if (adLen > 0)
    {
      adType = *pData;

      // If AD type is for 16-bit service UUID
      if ((adType == GAP_ADTYPE_16BIT_MORE) ||
          (adType == GAP_ADTYPE_16BIT_COMPLETE))
      {
        pData++;
        adLen--;

        // For each UUID in list
        while (adLen >= 2 && pData < pEnd)
        {
          // Check for match
          if ((pData[0] == LO_UINT16(uuid)) && (pData[1] == HI_UINT16(uuid)))
          {
            // Match found
            return TRUE;
          }

          // Go to next
          pData += 2;
          adLen -= 2;
        }

        // Handle possible erroneous extra byte in UUID list
        if (adLen == 1)
        {
          pData++;
        }
      }
      else
      {
        // Go to next item
        pData += adLen;
      }
    }
  }

  // Match not found
  return FALSE;
}

/*********************************************************************
 * @fn      SimpleBLECentral_addDeviceInfo
 *
 * @brief   Add a device to the device discovery result list
 *
 * @return  none
 */
static void SimpleBLECentral_addDeviceInfo(uint8_t *pAddr, uint8_t addrType)
{
  uint8_t i;

  // If result count not at max
  if (scanRes < DEFAULT_MAX_SCAN_RES)
  {
    // Check if device is already in scan results
    for (i = 0; i < scanRes; i++)
    {
      if (memcmp(pAddr, devList[i].addr , B_ADDR_LEN) == 0)
      {
        return;
      }
    }

    // Add addr to scan result list
    memcpy(devList[scanRes].addr, pAddr, B_ADDR_LEN);
    devList[scanRes].addrType = addrType;

    // Increment scan result count
    scanRes++;
  }
}

/*********************************************************************
 * @fn      SimpleBLECentral_eventCB
 *
 * @brief   Central event callback function.
 *
 * @param   pEvent - pointer to event structure
 *
 * @return  TRUE if safe to deallocate event message, FALSE otherwise.
 */
static uint8_t SimpleBLECentral_eventCB(gapCentralRoleEvent_t *pEvent)
{
  // Forward the role event to the application
  if (SimpleBLECentral_enqueueMsg(SBC_STATE_CHANGE_EVT,
                                  SUCCESS, (uint8_t *)pEvent))
  {
//    // App will process and free the event
    return FALSE;
  }

  // Caller should free the event
  return TRUE;
}

/*********************************************************************
 * @fn      SimpleBLECentral_pairStateCB
 *
 * @brief   Pairing state callback.
 *
 * @return  none
 */
static void SimpleBLECentral_pairStateCB(uint16_t connHandle, uint8_t state,
                                         uint8_t status)
{
  uint8_t *pData;

  // Allocate space for the event data.
  if ((pData = ICall_malloc(sizeof(uint8_t))))
  {
    *pData = status;

    // Queue the event.
    SimpleBLECentral_enqueueMsg(SBC_PAIRING_STATE_EVT, state, pData);
  }
}

/*********************************************************************
 * @fn      SimpleBLECentral_passcodeCB
 *
 * @brief   Passcode callback.
 *
 * @return  none
 */
static void SimpleBLECentral_passcodeCB(uint8_t *deviceAddr, uint16_t connHandle,
                                        uint8_t uiInputs, uint8_t uiOutputs)
{
  uint8_t *pData;

  // Allocate space for the passcode event.
  if ((pData = ICall_malloc(sizeof(uint8_t))))
  {
    *pData = uiOutputs;

    // Enqueue the event.
    SimpleBLECentral_enqueueMsg(SBC_PASSCODE_NEEDED_EVT, 0, pData);
  }
}

/*********************************************************************
 * @fn      SimpleBLECentral_startDiscHandler
 *
 * @brief   Clock handler function
 *
 * @param   a0 - ignored
 *
 * @return  none
 */
void SimpleBLECentral_startDiscHandler(UArg a0)
{
  Event_post(syncEvent, SBC_START_DISCOVERY_EVT);
}

/*********************************************************************
 * @fn      SimpleBLECentral_keyChangeHandler
 *
 * @brief   Key event handler function
 *
 * @param   a0 - ignored
 *
 * @return  none
 */
void SimpleBLECentral_keyChangeHandler(uint8 keys)
{
  SimpleBLECentral_enqueueMsg(SBC_KEY_CHANGE_EVT, keys, NULL);
}

/*********************************************************************
 * @fn      SimpleBLECentral_readRssiHandler
 *
 * @brief   Read RSSI handler function
 *
 * @param   a0 - read RSSI index
 *
 * @return  none
 */
void SimpleBLECentral_readRssiHandler(UArg a0)
{
  SimpleBLECentral_enqueueMsg(SBC_RSSI_READ_EVT, SUCCESS,
                              (uint8_t *)&readRssi[a0]);
}

/*********************************************************************
 * @fn      SimpleBLECentral_enqueueMsg
 *
 * @brief   Creates a message and puts the message in RTOS queue.
 *
 * @param   event - message event.
 * @param   state - message state.
 * @param   pData - message data pointer.
 *
 * @return  TRUE or FALSE
 */

//static uint8_t SimpleBLECentral_enqueueMsg(uint8_t event, uint8_t state,
//                                           uint8_t *pData)
//{
//  sbcEvt_t *pMsg = ICall_malloc(sizeof(sbcEvt_t));
//
//  // Create dynamic pointer to message.
//  if (pMsg)
//  {
//    pMsg->hdr.event = event;
//    pMsg->hdr.state = state;
//    pMsg->pData = pData;
//
//    // Enqueue the message.
//    return Util_enqueueMsg(appMsgQueue, syncEvent, (uint8_t *)pMsg);
//  }
//
//  return FALSE;
//}

/*********************************************************************
*********************************************************************/
/*********************************************************************
*********************************************************************/
extern void MyEvent_post(UArg arg)
{
    // Wake up the application.
    if(NULL != syncEvent)
        Event_post(syncEvent, arg);
}
