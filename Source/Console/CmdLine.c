/*
 * CmdLine.c
 *
 *  Created on: 2017年8月9日
 *      Author: blue.zhengxinkang
 */

//#include <CmdGroup_general.h>
//#include <CmdGroup_action.h>
//#include <CmdGroup_keyManage.h>
//#include <Driver_portWake.h>
//#include <NotifyGroup_general.h>
#include <string.h>
#include <stdlib.h>

#include "Console.h"
#include "CmdLine.h"
//*****************************************************************************
// 1、include
//*****************************************************************************
#include <stdio.h>
#include "Config/ConfigProtocol.h"
//#include <UartFrame.h>
#include "Common\Trace.h"
//#include "IWUP_config.h"
#include "StringExchange.h"
#include "TaskTest.h"
#include "Test_process.h"
#include "Driver_adc.h"
#include "Driver_gpio.h"
#include "IWUP_config.h"
#include "IWUP_cmd_group/CmdGroup_qcTest.h"
#include "IWUP_notify_group/NotifyGroup_qcTest.h"
#include "TestProcess_powerMode.h"
#include "BoardAction.h"
//#include "Ble_adv.h"
//#include "Ble_advDataConfig.h"
//#include "StateMachine_ble.h"
//#include "ProtocolBle.h"
//#include "board_uart.h"
//#include "HalMcuRtc.h"
//#include "BuzzPort.h"
//#include "StepMotor.h"
//#include "HardWatchDog.h"
//#include "McuFlash.h"
//#include "Random/Random.h"
//#include "SegsApi/LockSecureInterface.h"
//#include "MemoryInfo_dncp.h"

// 命令行版本定义，命令有变更时，需要相应更新本版本号
const CmdLineVersion g_kCmdLineVersion =
{
        1,      // 主版本号
        0,      // 次版本号
        0,      // 修订版本号
        0       // 编译版本号
};

//*****************************************************************************
// 2、命令处理函数声明列表
//*****************************************************************************
//static int Cmd_bleEventTest(int argc, char *argv[]);
//static int Cmd_bleAdvDataTest(int argc, char *argv[]);
//static int Cmd_bleAdvTest(int argc, char *argv[]);
//static int Cmd_wakeTxTest(int argc, char *argv[]);
#if defined     IWUP_ROLE_WIRELESS_MODULE
static int Cmd_iwupCmdTest(int argc, char *argv[]);
#elif defined   IWUP_ROLE_LOCK_BOARD
static int Cmd_iwupNotifyTest(int argc, char *argv[]);
#endif
static int Cmd_gpioTest(int argc, char *argv[]);
static int Cmd_adcTest(int argc, char *argv[]);
static int Cmd_testTaskTest(int argc, char *argv[]);
static int Cmd_setPinLowPower(int argc, char *argv[]);
static int Cmd_boardAction(int argc, char *argv[]);

static int Cmd_help(int argc, char *argv[]);
static int Cmd_version(int argc, char *argv[]);
static int Cmd_welcome(int argc, char *argv[]);
//int Cmd_showparam(int argc, char *argv[]);
static int Cmd_reset(int argc, char *argv[]);
static int Cmd_demo(int argc, char *argv[]);
static int Cmd_TraceSetLevel(int argc, char *argv[]);
//static int Cmd_TraceGetLevel(int argc, char *argv[]);


/**
 * @brief 命令行命令表，保存命令关键字与其处理函数之间的对应关系。
 * @details 每条命令对应一个结点，结点包括：命令关键字、处理函数、简短描述文本。
 */
//*****************************************************************************
// 3、命令行命令表
//*****************************************************************************
const CmdLineEntry g_kConsoleCmdTable[] =
{
//    { "lockTest",               Cmd_lockTest,               "\t\t: lockTest MessageGet/InitStatusSet(EMPTY/DEPLOYED/SERVICE)/lockIdSet(%02x*8)/lockSuperkeySet/lockTypeSet/lockModelSet/lockSn/lockManufSet" },
//    { "randomTest",             Cmd_randomTest,             "\t\t: randomTest" },
//    { "mcuFlashTest",           Cmd_mcuFlashTest,           "\t\t: mcuFlashTest init/write/read" },
//    { "hardWatchDogTest",       Cmd_hardWatchDogTest,       "\t: hardWatchDogTest init/start/stop" },
//    { "stepMotorTest",          Cmd_stepMotorTest,          "\t\t: stepMotorTest forward(stepsOut period)/reverse(stepsOut period)" },
//    { "buzzPortTest",           Cmd_buzzPortTest,           "\t\t: buzzPortTest" },
//    { "mcuRtcGet",              Cmd_mcuRtcGet,              "\t\t: Cmd_mcuRtcGet" },
//    { "mcuRtcSet",              Cmd_mcuRtcSet,              "\t\t: Cmd_mcuRtcSet year mo da ho mi se" },
//    { "SingleWireSend",         Cmd_SingleWireSend,         "\t\t: Cmd_SingleWireSend xx xx xx" },
//    { "UartSend",               Cmd_UartSend,               "\t\t: UartSend xx xx xx" },
//    { "UartSendNum",            Cmd_UartSendNum,            "\t\t: UartSendNum xx" },
//    { "bleEvent",           Cmd_bleEventTest,       "\t\t: bleEvent triggerStart/addAdmin." },
//    { "bleAdvData",         Cmd_bleAdvDataTest,     "\t\t: bleAdvData advRefresh/advPrint num." },
//    { "bleAdv",             Cmd_bleAdvTest,         "\t\t\t: bleAdv enable/disable/restart/disconnect/isConnected/bleSendData/resetBle." },
//    { "wakeTxTest",         Cmd_wakeTxTest,         "\t\t: wakeTxTest high/low." },
#if defined     IWUP_ROLE_WIRELESS_MODULE
    { "iwupCmdTest",        Cmd_iwupCmdTest,        "\t\t: iwupCmdTest SendCmd_general_getDeviceInfo/... para" },
#elif defined   IWUP_ROLE_LOCK_BOARD
    { "iwupNotifyTest",     Cmd_iwupNotifyTest,     "\t\t: iwupNotifyTest SendNotify_Echo/SendNotify_general_button/..." },
#endif
    { "gpioTest",               Cmd_gpioTest,               "\t\t: gpioTest" },
    { "adcTest",                Cmd_adcTest,                "\t\t\t: adcTest" },
    { "testTaskTest",           Cmd_testTaskTest,           "\t\t: testTaskTest EVENT_START_TEST" },
    { "setPinLowPower",         Cmd_setPinLowPower,         "\t\t: setPinLowPower" },
    { "boardAction",            Cmd_boardAction,            "\t\t: boardAction" },

    { "help",       Cmd_help,       "\t\t\t: Display list of commands. Short format: h or ?." },
    { "?",          Cmd_help,       0 },
    { "h",          Cmd_help,       0 },
    { "version",    Cmd_version,    "\t\t\t: Display version infomation." },
    { "welcome",    Cmd_welcome,    "\t\t\t: Display welcome message." },
    { "showparam",  Cmd_showparam,  "\t\t: Display para list of a cmd." },
    { "reset",      Cmd_reset,      "\t\t\t: Reset system." },
    { "demo",       Cmd_demo,       "\t\t\t: Demo for cmd implementation and param parse." },
    { "TraceGetLevel",          Cmd_TraceGetLevel,          "\t\t: TraceGetLevel" },
    { "TraceSetLevel",          Cmd_TraceSetLevel,          "\t\t: TraceSetLevel DISABLE/FATAL/ERROR/WARNING/INFO/DEBUG/MARK/CODE" },
    { 0, 0, 0 }
};

void CmdLine_Init(void)
{
    Console_Init();
}

//*****************************************************************************
// 4、命令处理函数
//*****************************************************************************
//----------------------------------------------------CmdLine setPinLowPower-----------------
static int Cmd_boardAction(int argc, char *argv[])
{
    uint8_t hexData[30];
    if(0 != strlen(argv[2])%2)
    {
        TRACE_ERROR("Para len error! para len not Double number!\n");
        return 0;
    }
    StringToHexGroup((char*)argv[2], strlen(argv[2]), (char*)hexData);

    if(!memcmp("setModeNormal", argv[1], strlen("setModeNormal")))
    {
        BoardAction_setModeNormal();
        TRACE_DEBUG("正常模式。\n");
    }
    else if(!memcmp("buttonSetting", argv[1], strlen("buttonSetting")))
    {
        BoardAction_setting(300);
        TRACE_DEBUG("执行设置-%dms。\n",300);
    }
    else if(!memcmp("buttonClear", argv[1], strlen("buttonClear")))
    {
        TRACE_DEBUG("开始清空-%dms。\n",6000);
        BoardAction_clear(6000);
        TRACE_DEBUG("清空完成。\n");
    }
    return 0;
}
//----------------------------------------------------CmdLine setPinLowPower-----------------
static int Cmd_setPinLowPower(int argc, char *argv[])
{
    TRACE_DEBUG("Cmd_setPinLowPower.\n");
    SetPin_lowPower();
    return 0;
}

//----------------------------------------------------CmdLine iwupTest-----------------
#if defined     IWUP_ROLE_WIRELESS_MODULE
static int Cmd_iwupCmdTest(int argc, char *argv[])
{
    uint8_t hexData[30];
    if(0 != strlen(argv[2])%2)
    {
        TRACE_ERROR("Para len error! para len not Double number!\n");
        return 0;
    }
    StringToHexGroup((char*)argv[2], strlen(argv[2]), (char*)hexData);
//CMDGROUP_QCTEST ##################
    if(!memcmp("SendCmd_qcTest_echo", argv[1], strlen("SendCmd_qcTest_echo")))
    {
        SendCmd_qcTest_echo( hexData, strlen(argv[2])/2 );
    }
    else if(!memcmp("SendCmd_qcTest_selfTest", argv[1], strlen("SendCmd_qcTest_selfTest")))
    {
        SendCmd_qcTest_selfTest( hexData, strlen(argv[2])/2 );
    }
    else if(!memcmp("SendCmd_qcTest_action", argv[1], strlen("SendCmd_qcTest_action")))
    {
        SendCmd_qcTest_action( hexData, strlen(argv[2])/2 );
    }
    else if(!memcmp("SendCmd_qcTest_getDeviceInfo", argv[1], strlen("SendCmd_qcTest_getDeviceInfo")))
    {
        SendCmd_qcTest_getDeviceInfo();
    }
    else if(!memcmp("SendCmd_qcTest_getDeviceStatus", argv[1], strlen("SendCmd_qcTest_getDeviceStatus")))
    {
        SendCmd_qcTest_getDeviceStatus();;
    }
    else if(!memcmp("SendCmd_qcTest_setDevicePower", argv[1], strlen("SendCmd_qcTest_setDevicePower")))
    {
        uint8_t para = 0x00;
        SendCmd_qcTest_setDevicePower( &para, 1 );
    }
    else if(!memcmp("SendCmd_qcTest_setDeviceInfo", argv[1], strlen("SendCmd_qcTest_setDeviceInfo")))
    {
        SendCmd_qcTest_setDeviceInfo( hexData, strlen(argv[2])/2 );
    }
    return 0;
}
#elif defined   IWUP_ROLE_LOCK_BOARD
static int Cmd_iwupNotifyTest(int argc, char *argv[])
{
    uint8_t hexData[30];
    if(0 != strlen(argv[2])%2)
    {
        TRACE_ERROR("Para len error! para len not Double number!\n");
        return 0;
    }
    StringToHexGroup((char*)argv[2], strlen(argv[2]), (char*)hexData);
//NOTIFYGROUP_QCTEST ##################
    if(!memcmp("SendNotify_qcTest_card", argv[1], strlen("SendNotify_qcTest_card")))
    {
        SendNotify_qcTest_card( hexData, strlen(argv[2])/2 );
    }
    else if(!memcmp("SendNotify_qcTest_keyBoard", argv[1], strlen("SendNotify_qcTest_keyBoard")))
    {
        SendNotify_qcTest_keyBoard( hexData, strlen(argv[2])/2 );
    }
}
#endif

//----------------------------------------------------Cmd_gpioTest-----------------
static int Cmd_gpioTest(int argc, char *argv[])
{
    TRACE_DEBUG("GPIO测试.\n");
    if(!memcmp("GPIO_GET", argv[1], strlen("GPIO_GET")))
    {
        uint8_t portValue = 0;
        if(!memcmp("PORT_MOTOR_FORWARD", argv[2], strlen("PORT_MOTOR_FORWARD")))
        {
            portValue=Driver_gpioGet(PORT_MOTOR_FORWARD);
            TRACE_DEBUG("PORT_MOTOR_FORWARD 电平状态是：%d.\n", portValue);
        }
        else if(!memcmp("PORT_MOTOR_REVERSAL", argv[2], strlen("PORT_MOTOR_REVERSAL")))
        {
            portValue=Driver_gpioGet(PORT_MOTOR_REVERSAL);
            TRACE_DEBUG("PORT_MOTOR_REVERSAL 电平状态是：%d.\n", portValue);
        }
    }
    else if(!memcmp("GPIO_SET", argv[1], strlen("GPIO_SET")))
    {
        PortValue portValue = (PortValue)StringToUint32(argv[3]);
        if(!memcmp("PORT_ANTI_LOCK", argv[2], strlen("PORT_ANTI_LOCK")))
        {
            Driver_gpioSet(PORT_ANTI_LOCK, portValue);
            TRACE_DEBUG("PORT_ANTI_LOCK 电平状态设置为：%d.\n", portValue);
        }
        else if(!memcmp("PORT_PREVENT_DISMANTLE", argv[2], strlen("PORT_PREVENT_DISMANTLE")))
        {
            Driver_gpioSet(PORT_PREVENT_DISMANTLE,portValue);
            TRACE_DEBUG("PORT_PREVENT_DISMANTLE 电平状态设置为：%d.\n", portValue);
        }
    }
    return 0;
}

//----------------------------------------------------Cmd_adcTest-----------------
static int Cmd_adcTest(int argc, char *argv[])
{
    TRACE_DEBUG("ADC测试.\n");
    Driver_adcGet();
    return 0;
}

//----------------------------------------------------Cmd_testTaskTest-----------------
static int Cmd_testTaskTest(int argc, char *argv[])
{
    if(!memcmp("EVENT_START_TEST", argv[1], strlen("EVENT_START_TEST")))
    {
        uint8_t macAddr[6];
        TRACE_DEBUG("send EVENT_START_TEST.\n");
        StringToHexGroup(argv[2], strlen(argv[2]), (char*)macAddr);
        Test_process_setMacAddr(macAddr);
        if(!IsBusy_testProcess())
            TestEvent_post(EVENT_START_TEST);
    }
    return 0;
}

////----------------------------------------------------CmdLine randomTest-----------------
//static int Cmd_randomTest(int argc, char *argv[])
//{
//    TRACE_DEBUG("Random num:%02x", Random_getUnint8());
//    return 0;
//}
//
////----------------------------------------------------CmdLine mcuFlashTest-----------------
//int Cmd_mcuFlashTest(int argc, char *argv[])
//{
//    uint32_t addr = 0;
//    uint32_t len = 0;
//    uint8_t data[252];
//    for(uint8_t i=0;i<255;i++)
//    {
//        data[i] = i;
//    }
//    sscanf(argv[2],"%d", &addr);
//    sscanf(argv[3],"%d", &len);
//
//    if(!memcmp("test", argv[1], strlen("test")))
//    {
//        McuFlash_init();
//    }
//    else if(!memcmp("write", argv[1], strlen("write")))
//    {
//        for(uint8_t serial=0;serial<32;serial++)
//        {
//            for(uint8_t i=0;i<252;i++)
//            {
//                data[i] = i+serial;
//            }
//            McuFlash_write(addr, len, data);
//        }
//    }
//    else if(!memcmp("read", argv[1], strlen("read")))
//    {
//        McuFlash_read(addr, len, data);
//        TRACE_DEBUG_HEXGROUP(data, len, ' ');
//    }
//
//    return 0;
//}
////----------------------------------------------------CmdLine hardWatchDogTest-----------------
//int Cmd_hardWatchDogTest(int argc, char *argv[])
//{
//    if(!memcmp("init", argv[1], strlen("init")))
//    {
//        Hwd_init();
//    }
//    else if(!memcmp("start", argv[1], strlen("start")))
//    {
//        Hwd_startFeed();
//    }
//    else if(!memcmp("stop", argv[1], strlen("stop")))
//    {
//        Hwd_stopFeed();
//    }
//
//    return 0;
//}
////----------------------------------------------------CmdLine buzzPortTest-----------------
//int Cmd_stepMotorTest(int argc, char *argv[])
//{
//    uint32_t stepOut = 0;
//    uint32_t period = 0;
//    sscanf(argv[2],"%d", &stepOut);
//    sscanf(argv[3],"%d", &period);
//    if(!memcmp("forward", argv[1], strlen("forward")))
//    {
//        StepMotor_unlock(stepOut, period);
//    }
//    else if(!memcmp("reverse", argv[1], strlen("reverse")))
//    {
//        StepMotor_lock(stepOut, period);
//    }
//
//    return 0;
//}
//
////----------------------------------------------------CmdLine buzzPortTest-----------------
//int Cmd_buzzPortTest(int argc, char *argv[])
//{
//    if(!memcmp(argv[1], "init", strlen("init")))
//    {
//        uint32_t frequency;
//        uint32_t dutyPercent;
//        sscanf(argv[2],"%d", &frequency);
//        sscanf(argv[3],"%d", &dutyPercent);
//        BuzzPort_init(frequency, dutyPercent);
//    }
//    else if(!memcmp(argv[1], "change", strlen("change")))
//    {
//        uint32_t frequency;
//        uint32_t dutyPercent;
//        sscanf(argv[2],"%d", &frequency);
//        sscanf(argv[3],"%d", &dutyPercent);
//        BuzzPort_change(frequency, dutyPercent);
//    }
//    else if(!memcmp(argv[1], "on", strlen("on")))
//    {
//        BuzzPort_on();
//    }
//    else if(!memcmp(argv[1], "off", strlen("off")))
//    {
//        BuzzPort_off();
//    }
//    return 0;
//}
//
////----------------------------------------------------CmdLine mcuRtcTest-----------------
//int Cmd_mcuRtcGet(int argc, char *argv[])
//{
//    McuRtcTime s_time;
//    HalMcuRtc_get(&s_time);
//
//    if( rtc_valid_tm(&s_time))
//        TRACE_DEBUG("time is  Illegal !\n");
//    TRACE_DEBUG("<<<getTime: %04d-%02d-%02d  %02d:%02d:%02d.\n",\
//          s_time.year,   s_time.month,  s_time.day, s_time.hour, s_time.minutes,  s_time.seconds);
//    return 0;
//}
//
//int Cmd_mcuRtcSet(int argc, char *argv[])
//{
//    McuRtcTime s_time;
//    uint32_t year, mon, day, hour, min, sec;
//    sscanf(argv[1],"%d", &year);
//    sscanf(argv[2],"%d", &mon);
//    sscanf(argv[3],"%d", &day);
//
//    sscanf(argv[4],"%d", &hour);
//    sscanf(argv[5],"%d", &min);
//    sscanf(argv[6],"%d", &sec);
//
//    s_time.year     = year;
//    s_time.month    = mon;
//    s_time.day      = day;
//    s_time.hour     = hour;
//    s_time.minutes  = min;
//    s_time.seconds  = sec;
//    if( rtc_valid_tm(&s_time))
//        TRACE_DEBUG("time is  Illegal !\n");
//    TRACE_DEBUG(">>>setTime: %04d-%02d-%02d  %02d:%02d:%02d.\n",\
//          s_time.year,   s_time.month,  s_time.day, s_time.hour, s_time.minutes,  s_time.seconds);
//    HalMcuRtc_set(&s_time);
//    return 0;
//}
////----------------------------------------------------CmdLine SingleWire-----------------
//int Cmd_SingleWireSend(int argc, char *argv[])
//{
//    uint8_t sendBuf[20];
//    uint8_t len = argc - 1;
//    if(len > 20)
//    {
//        TRACE_DEBUG("Len too long, only sen 20 bytes.\n");
//        len = 20;
//    }
//    for(uint8_t i = 1;i <= len;i++)
//    {
//        sscanf(argv[i],"%x", sendBuf+i-1);
//    }
//    TRACE_DEBUG("SingleWireSend:");
//    TRACE_DEBUG_HEXGROUP(sendBuf, len, ' ');
//    UartFrame_send(sendBuf, len);
//    return 0;
//}
//
//int Cmd_UartSend(int argc, char *argv[])
//{
//    uint8_t sendBuf[20];
//    uint8_t len = argc - 1;
//    if(len > 20)
//    {
//        TRACE_DEBUG("Len too long, only sen 20 bytes.\n");
//        len = 20;
//    }
//    for(uint8_t i = 1;i <= len;i++)
//    {
//        sscanf(argv[i],"%x", sendBuf+i-1);
//    }
//    TRACE_DEBUG("UartSend:");
//    TRACE_DEBUG_HEXGROUP(sendBuf, len, ' ');
//    UART_WriteTransport(sendBuf, len);
//    return 0;
//}
//
//static     uint8_t sendBuf[240] = {1};
//int Cmd_UartSendNum(int argc, char *argv[])
//{
//    uint32_t len;
//
//    sscanf(argv[1],"%d", &len);
//    for(uint8_t i=0; i<len; i++)
//        sendBuf[i] = i;
//
//    TRACE_DEBUG("UartSend %d data:\n", len);
////    TRACE_DEBUG_HEXGROUP(sendBuf, len, ' ');
//    UART_WriteTransport(sendBuf, len);
//    return 0;
//}
//----------------------------------------------------CmdLine Tools----------------------
// 显示帮助，简单显示命令列表
int Cmd_help(int argc, char *argv[])
{
    CmdLineEntry *cmdEntry;

    ConsoleOut("Central\n");
    ConsoleOut("Available commands\n");
    ConsoleOut("------------------\n");

    cmdEntry = (CmdLineEntry *) &g_kConsoleCmdTable[0];

    // 遍历整个命令表，打印出有提示信息的命令
    while (cmdEntry->cmdKeyword)
    {
        // 延时一下，等待控制台缓冲区空出足够的空间
        //System_Delay(1);

        if (cmdEntry->cmdHelp)
            ConsoleOut("%s%s\n", cmdEntry->cmdKeyword, cmdEntry->cmdHelp);
        cmdEntry++;
    }
    return (0);
}

int Cmd_version(int argc, char *argv[])
{
    ConsoleOut("Version: %d.%d.%d.%d\n",
            g_kCmdLineVersion.major,
            g_kCmdLineVersion.minor,
            g_kCmdLineVersion.revision,
            g_kCmdLineVersion.build
            );
    return(0);
}

int Cmd_welcome(int argc, char *argv[])
{
    Console_Welcome();
    return(0);
}


// 显示参数
int Cmd_showparam(int argc, char *argv[])
{
    int i = 0;

    ConsoleOut("The params is:\n");
    for (i = 0; i < argc; i++)
    {
        ConsoleOut("    Param argv[%d]: %s\n", i, argv[i]);
    }

    return(0);
}


int Cmd_reset(int argc, char *argv[])
{
    return (5);
}

// 命令处理函数示例
int Cmd_demo(int argc, char *argv[])
{
    if (0 == strcmp(argv[1], "subcmd1"))
    {
        if (0 == strcmp(argv[2], "param"))
        {
            // 调用相关功能函数
            ConsoleOut("Exc: subcmd1 param");
        }
    }
    else if (0 == strcmp(argv[1], "subcmd2"))
    {
        ConsoleOut("Exc: subcmd2");
    }
    else if (0 == argv[1] || 0 == strcmp(argv[1], "help") ||
             0 == strcmp(argv[1], "?"))
    {
        ConsoleOut("====== Sub commands ======\n");
        ConsoleOut(" mycmd subcmd1 param : Sub command description\n");
        ConsoleOut(" mycmd subcmd2       : Sub command description\n");
    }
    else
    {
        ConsoleOut("Invalid param: %s\n", argv[1]);
    }

    return (0);
}

//获取输出级别
int Cmd_TraceGetLevel(int argc, char *argv[])
{
    ConsoleOut("Trace Level is:");
    switch(Trace_GetLevel())
    {
        case TRACE_LEVEL_DISABLE:
        {
            ConsoleOut("DISABLE");
            break;
        }
        case TRACE_LEVEL_FATAL:
        {
            ConsoleOut("FATAL");
            break;
        }
        case TRACE_LEVEL_ERROR:
        {
            ConsoleOut("ERROR");
            break;
        }
        case TRACE_LEVEL_WARNING:
        {
            ConsoleOut("WARNING");
            break;
        }
        case TRACE_LEVEL_INFO:
        {
            ConsoleOut("INFO");
            break;
        }
        case TRACE_LEVEL_DEBUG:
        {
            ConsoleOut("DEBUG");
            break;
        }
        case TRACE_LEVEL_MARK:
        {
            ConsoleOut("MARK");
            break;
        }
        case TRACE_LEVEL_CODE:
        {
            ConsoleOut("CODE");
            break;
        }
        default:
            ConsoleOut("!level in error!!!");
    }
    ConsoleOut("\n");
    return 0;
}

//设置输出级别
int Cmd_TraceSetLevel(int argc, char *argv[])
{
    if(!memcmp("DISABLE", argv[1], sizeof("DISABLE")))
        Trace_SetLevel(TRACE_LEVEL_DISABLE);
    else if(!memcmp("FATAL", argv[1], sizeof("FATAL")))
        Trace_SetLevel(TRACE_LEVEL_FATAL);
    else if(!memcmp("ERROR", argv[1], sizeof("ERROR")))
        Trace_SetLevel(TRACE_LEVEL_ERROR);
    else if(!memcmp("WARNING", argv[1], sizeof("WARNING")))
        Trace_SetLevel(TRACE_LEVEL_WARNING);
    else if(!memcmp("INFO", argv[1], sizeof("INFO")))
        Trace_SetLevel(TRACE_LEVEL_INFO);
    else if(!memcmp("DEBUG", argv[1], sizeof("DEBUG")))
        Trace_SetLevel(TRACE_LEVEL_DEBUG);
    else if(!memcmp("MARK", argv[1], sizeof("MARK")))
        Trace_SetLevel(TRACE_LEVEL_MARK);
    else if(!memcmp("CODE", argv[1], sizeof("CODE")))
        Trace_SetLevel(TRACE_LEVEL_CODE);
    return 0;
}
