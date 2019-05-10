/*
 * CmdLine.c
 *
 *  Created on: 2017��8��9��
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
// 1��include
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
#include "Driver_hc595.h"
#include "Driver_hc165.h"
#include "Driver_ADCmpc3901.h"
#include "Hal_expandOutput.h"
#include "Hal_expandInput.h"
#include "Driver_voice.h"
#include "Lock_atcion.h"
#include "BF_Util.h"
#include "UTC_clock.h"
#include "w25qxx.h"
#include "Hal_flash.h"
#include "Driver_internalFlash.h"
#include "Driver_spi.h"
#include "Hal_electricCurrent.h"
#include "Hal_oled.h"
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

// �����а汾���壬�����б��ʱ����Ҫ��Ӧ���±��汾��
const CmdLineVersion g_kCmdLineVersion =
{
        1,      // ���汾��
        0,      // �ΰ汾��
        0,      // �޶��汾��
        0       // ����汾��
};

//*****************************************************************************
// 2��������������б�
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
static int Cmd_595test(int argc, char *argv[]);
static int Cmd_165test(int argc, char *argv[]);
static int Cmd_adcMpc3901Test(int argc, char *argv[]);
static int Cmd_expandOutTest(int argc, char *argv[]);
static int Cmd_expandInTest(int argc, char *argv[]);
static int Cmd_voiceTest(int argc, char *argv[]);

static int Cmd_help(int argc, char *argv[]);
static int Cmd_version(int argc, char *argv[]);
static int Cmd_welcome(int argc, char *argv[]);
//int Cmd_showparam(int argc, char *argv[]);
static int Cmd_reset(int argc, char *argv[]);
static int Cmd_demo(int argc, char *argv[]);
static int Cmd_TraceSetLevel(int argc, char *argv[]);
static int Cmd_timeTest(int argc, char *argv[]);
static int Cmd_flashTest(int argc, char *argv[]);
static int Cmd_currentOffsetSet(int argc, char *argv[]);
static int Cmd_abnormalTest(int argc, char *argv[]);
//static int Cmd_TraceGetLevel(int argc, char *argv[]);


/**
 * @brief �������������������ؼ������䴦����֮��Ķ�Ӧ��ϵ��
 * @details ÿ�������Ӧһ����㣬������������ؼ��֡�����������������ı���
 */
//*****************************************************************************
// 3�������������
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
    { "595test",                Cmd_595test,                "\t\t\t: 595test" },
    { "165test",                Cmd_165test,                "\t\t\t: 165test" },
    { "adcMpc3901Test",         Cmd_adcMpc3901Test,         "\t\t: adcMpc3901Test" },
    { "expandOutTest",          Cmd_expandOutTest,          "\t\t: expandOutTest serial value" },
    { "expandInTest",           Cmd_expandInTest,           "\t\t: expandInTest serial" },
    { "voiceTest",              Cmd_voiceTest,              "\t\t: voiceTest data" },
    { "timeTest",               Cmd_timeTest,               "\t\t: timeTest" },
    { "flashTest",              Cmd_flashTest,              "\t\t: flashTest" },
    { "currentOffsetSet",       Cmd_currentOffsetSet,       "\t: currentOffsetSet" },
    { "abnormalTest",           Cmd_abnormalTest,           "\t: abnormalTest" },

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

static uint8_t printTimeIndex=0;
static void print_timeoutHandler();
#define TIMER_PRINT_PERIOD      5

void CmdLine_Init(void)
{
    Console_Init();
    BF_UtilRegeistTimerCallback(print_timeoutHandler, &printTimeIndex, TIMER_PRINT_PERIOD);
}

//*****************************************************************************
// 4���������
//*****************************************************************************

//----------------------------------------------------CmdLine abnormalTest-----------------
static uint32_t round = 0;
static uint64_t addr = 0;
static uint64_t addr_start = 0;

static int Cmd_abnormalTest(int argc, char *argv[])
{
    if(round == 0)
    {
        uint32_t num;
        sscanf(argv[1],"%d", &num);
        addr_start = FLASH_ADDR_TEXT_START + (num-1)*0x1000*10;
        BF_UtilWaitHandle(printTimeIndex);
        TRACE_DEBUG("��ȡ��¼��\n");
    }
    else
    {
        TRACE_DEBUG("��ȡ��¼�����ڲ��������Ժ����ԡ�\n");
    }
    return;
}

static void print_timeoutHandler()
{
    uint32_t buf[52];
    addr = addr_start +0x100*round;
    TRACE_DEBUG("��ַΪ��%x\n", addr);
//    W25QXX_Read(buf, addr, 208);
    extFlash_read((size_t)addr, 208, (uint8_t*)buf);
    uint32_t avg=buf[50];
    uint32_t second = buf[51];
    UTCTimeStruct s_time;
    UTC_convertUTCTime(&s_time, second);
    TRACE_INFO("%04d-%02d-%02d %02d:%02d:(%02d): ",
                s_time.year,s_time.month+1,s_time.day+1,
                s_time.hour,s_time.minutes,s_time.seconds, buf[50]);
    uint32_t avg_f = avg/1000;
    uint32_t avg_l = (avg%1000)/100;

    if(avg == 0xffffffff)
        goto FINISH;
    else if(avg < 1000)
        TRACE_INFO("ƽ��������%d��\tuA\n",avg);
    else
        TRACE_INFO("ƽ��������%d.%d��\tmA\n",avg_f,avg_l);

    for(uint8_t j=0; j<1; j++)
    {
        addr = addr_start +0x100*round;
        for (uint8_t k = 0; k < 50; k++)
        {
            TRACE_DEBUG("%d\t", buf[k]);
            if (k % 25 == 24)
                TRACE_DEBUG("\n");
        }
        TRACE_DEBUG("\n");
        round++;
    }

    if(round < 160)
    {
        BF_UtilWaitHandle(printTimeIndex);
    }
    else
    {
FINISH:
        TRACE_INFO("finish��\n");
        round = 0;
    }
}

//----------------------------------------------------CmdLine currentOffsetSet-----------------
static int Cmd_currentOffsetSet(int argc, char *argv[])
{
    int32_t offset = 0;
    int32_t thisCurrent = avgCurrentCount(0, false, 1);
    int32_t oldOffset = Hal_electricCurrent_offsetGet();
    offset = thisCurrent-3+oldOffset;
    TRACE_DEBUG("����У׼��%d\n",offset);
    Hal_flash_writeCurrentOffset(offset);
    Hal_electricCurrent_offsetRead();
    return 0;
}

//----------------------------------------------------CmdLine flashTest-----------------
static int Cmd_flashTest(int argc, char *argv[])
{
    uint8_t buf[256];
    if (!memcmp("read", argv[1], strlen("read")))
    {
        uint64_t addr;
        sscanf(argv[2],"%x", &addr);
//        W25QXX_Read(buf, addr, sizeof(buf));
        extFlash_read(addr, sizeof(buf), buf);
        for (uint8_t i = 0; i < 16; i++)
        {
            TRACE_DEBUG_HEXGROUP(buf + i * 16, 16, ' ');
        }
    }
    else if (!memcmp("eraseChip", argv[1], strlen("eraseChip")))
    {
        TRACE_DEBUG("����оƬ-��ʼ��\n");
//        W25QXX_Erase_Chip();
        extFlash_eraseChip();
        TRACE_DEBUG("����оƬ-������\n",);
    }
    else if (!memcmp("interFlash", argv[1], strlen("interFlash")))
    {
        flashParam.time = 123;
        flashParamParamWrite();

        flashParam.time = 0;
        flashParam.currentOffset = 0;

        flashParamParamRead();
        TRACE_DEBUG("Ԥ������Ϊ123 xxx,ʵ������Ϊ%d %d \n", flashParam.time, flashParam.currentOffset);

        flashParam.currentOffset = 456;
        flashParamParamWrite();
        flashParamParamRead();
        TRACE_DEBUG("Ԥ������Ϊ123 456,ʵ������Ϊ%d %d \n", flashParam.time, flashParam.currentOffset);
    }
    else if (!memcmp("ReadId", argv[1], strlen("readId")))
    {
        uint32_t id = extFlash_readId();
        TRACE_DEBUG("Flash id:%x \n", id);
    }

//    for(uint8_t i=0; i<255; i++)
//    {
//        buf[i] = i;
//    }
//    W25QXX_Write_NoCheck(buf, 0x00000000, sizeof(buf));
//    memset(buf, 0x00, sizeof(buf));
//    W25QXX_Read(buf, 0x00000000, sizeof(buf));
//    for(uint8_t i=0; i<16; i++)
//    {
//        TRACE_DEBUG_HEXGROUP(buf+i*16, 16, ' ');
//    }

    return 0;
}

//----------------------------------------------------CmdLine timeTest-----------------
static int Cmd_timeTest(int argc, char *argv[])
{
    if (!memcmp("set", argv[1], strlen("set")))
    {
        TRACE_DEBUG("����ʱ�䣺\n");
        UTCTimeStruct s_time;
        uint32_t year, mon, day, hour, min, sec;
        sscanf(argv[2],"%d", &year);
        sscanf(argv[3],"%d", &mon);
        sscanf(argv[4],"%d", &day);

        sscanf(argv[5],"%d", &hour);
        sscanf(argv[6],"%d", &min);
        sscanf(argv[7],"%d", &sec);

        s_time.year = year;
        s_time.month = mon-1;
        s_time.day = day-1;
        s_time.hour = hour;
        s_time.minutes = min;
        s_time.seconds = sec;
//        s_time.tm_wday = week;
//        if( rtc_valid_tm(&s_time))
//            TRACE_DEBUG("input time is  Illegal.\n");

        TRACE_DEBUG(">>>setTime: %04d-%02d-%02d \t %02d:%02d:%02d \n",\
              s_time.year,   s_time.month+1,  s_time.day+1, s_time.hour, s_time.minutes,  s_time.seconds);
        UTCTime second = UTC_convertUTCSecs( &s_time );
        Hal_flash_writeTime(second);
        UTC_setClock(second);
        Hal_oled_timeChange(s_time);
    }
    else if(!memcmp("get", argv[1], strlen("get")))
    {
        UTCTime second = Hal_flash_readTime();
        UTCTimeStruct s_time;
        UTC_convertUTCTime( &s_time, second );
        TRACE_DEBUG(">>>getTime: %04d-%02d-%02d \t %02d:%02d:%02d\n",\
              s_time.year,   s_time.month+1,  s_time.day+1, s_time.hour, s_time.minutes,  s_time.seconds);
    }
    else if(!memcmp("show", argv[1], strlen("show")))
    {
        UTCTime thisTime;
        TRACE_DEBUG("Cmd_timeTest!\n");
        thisTime = UTC_getClock();
        UTCTimeStruct thisTimeStruct;
        UTC_convertUTCTime(&thisTimeStruct, thisTime);
        TRACE_DEBUG("�ڴ�ʱ��-%d��%d��%d�� %dʱ%d��%d��\n",
                    thisTimeStruct.year,thisTimeStruct.month+1,thisTimeStruct.day+1,
                    thisTimeStruct.hour,thisTimeStruct.minutes,thisTimeStruct.seconds);
    }
    return 0;
}

//----------------------------------------------------CmdLine voiceTest-----------------
static int Cmd_voiceTest(int argc, char *argv[])
{
    uint32_t serial = StringToHex(argv[1],2);
//    Send_threelines(serial);
    TRACE_DEBUG("voice = %x \n", serial);

    return 0;
}

//----------------------------------------------------CmdLine expandInTest-----------------
static int Cmd_expandInTest(int argc, char *argv[])
{
    PortValue portValue = PORT_VALUE_LOW;

    uint32_t serial = StringToUint32(argv[1]);
    portValue = Hal_expandInput_get(serial);
    TRACE_DEBUG("Serial = %d,value = %d.\n",serial, portValue);

    return 0;
}

//----------------------------------------------------CmdLine expandOutTest-----------------
static int Cmd_expandOutTest(int argc, char *argv[])
{
    PortValue portValue = PORT_VALUE_LOW;

    uint32_t serial = StringToUint32(argv[1]);
    uint32_t value = StringToUint32(argv[2]);

    if(0 == value)
    {
        portValue = PORT_VALUE_LOW;
    }
    else
    {
        portValue = PORT_VALUE_HIGH;
    }

    Hal_expandOutput(serial, portValue);
    return 0;
}

//----------------------------------------------------CmdLine adcMpc3901Test-----------------
static int Cmd_adcMpc3901Test(int argc, char *argv[])
{
//    TRACE_DEBUG("%d.\n", Get_AD_adcIC());
    return 0;
}

//----------------------------------------------------CmdLine 165test-----------------
static int Cmd_165test(int argc, char *argv[])
{
    uint32_t data = Driver_hc165_get();
    TRACE_DEBUG("��������-%06x��\n",data);
    return 0;
}

//----------------------------------------------------CmdLine 595test-----------------
static int Cmd_595test(int argc, char *argv[])
{
    uint32_t outData = StringToHex(argv[1], strlen(argv[1]));
    TRACE_DEBUG("�������-%x��\n",outData);

    Driver_hc595_sendData(outData);

    return 0;
}

//----------------------------------------------------CmdLine boardAction-----------------
static int Cmd_boardAction(int argc, char *argv[])
{
//    uint8_t hexData[30];
//    if(0 != strlen(argv[2])%2)
//    {
//        TRACE_ERROR("Para len error! para len not Double number!\n");
//        return 0;
//    }
//    StringToHexGroup((char*)argv[2], strlen(argv[2]), (char*)hexData);
//
//    if(!memcmp("setModeNormal", argv[1], strlen("setModeNormal")))
//    {
//        BoardAction_setModeNormal();
//        TRACE_DEBUG("����ģʽ��\n");
//    }
//    else if(!memcmp("buttonSetting", argv[1], strlen("buttonSetting")))
//    {
//        BoardAction_setting(300);
//        TRACE_DEBUG("ִ������-%dms��\n",300);
//    }
//    else if(!memcmp("buttonClear", argv[1], strlen("buttonClear")))
//    {
//        TRACE_DEBUG("��ʼ���-%dms��\n",6000);
//        BoardAction_clear(6000);
//        TRACE_DEBUG("�����ɡ�\n");
//    }
//    else if(!memcmp("cardAction", argv[1], strlen("cardAction")))
//    {
//        TRACE_DEBUG("ִ��ˢ����\n");
//        Lock_action_cardActive(1000);
//    }
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
//    uint8_t hexData[30];
//    if(0 != strlen(argv[2])%2)
//    {
//        TRACE_ERROR("Para len error! para len not Double number!\n");
//        return 0;
//    }
//    StringToHexGroup((char*)argv[2], strlen(argv[2]), (char*)hexData);
////CMDGROUP_QCTEST ##################
//    if(!memcmp("SendCmd_qcTest_echo", argv[1], strlen("SendCmd_qcTest_echo")))
//    {
//        SendCmd_qcTest_echo( hexData, strlen(argv[2])/2 );
//        TRACE_DEBUG("SendCmd_qcTest_echo\n");
//    }
//    else if(!memcmp("SendCmd_qcTest_selfTest", argv[1], strlen("SendCmd_qcTest_selfTest")))
//    {
//        SendCmd_qcTest_selfTest( hexData, strlen(argv[2])/2 );
//    }
//    else if(!memcmp("SendCmd_qcTest_action", argv[1], strlen("SendCmd_qcTest_action")))
//    {
//        SendCmd_qcTest_action( hexData, strlen(argv[2])/2 );
//    }
//    else if(!memcmp("SendCmd_qcTest_getDeviceInfo", argv[1], strlen("SendCmd_qcTest_getDeviceInfo")))
//    {
//        SendCmd_qcTest_getDeviceInfo();
//    }
//    else if(!memcmp("SendCmd_qcTest_getDeviceStatus", argv[1], strlen("SendCmd_qcTest_getDeviceStatus")))
//    {
//        SendCmd_qcTest_getDeviceStatus();;
//    }
//    else if(!memcmp("SendCmd_qcTest_setDevicePower", argv[1], strlen("SendCmd_qcTest_setDevicePower")))
//    {
//        uint8_t para = 0x00;
//        SendCmd_qcTest_setDevicePower( &para, 1 );
//    }
//    else if(!memcmp("SendCmd_qcTest_setDeviceInfo", argv[1], strlen("SendCmd_qcTest_setDeviceInfo")))
//    {
//        SendCmd_qcTest_setDeviceInfo( hexData, strlen(argv[2])/2 );
//    }
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
//    TRACE_DEBUG("GPIO����.\n");
//    if(!memcmp("GPIO_GET", argv[1], strlen("GPIO_GET")))
//    {
//        uint8_t portValue = 0;
//        if(!memcmp("PORT_MOTOR_FORWARD", argv[2], strlen("PORT_MOTOR_FORWARD")))
//        {
//            portValue=Driver_gpioGet(PORT_MOTOR_FORWARD);
//            TRACE_DEBUG("PORT_MOTOR_FORWARD ��ƽ״̬�ǣ�%d.\n", portValue);
//        }
//        else if(!memcmp("PORT_MOTOR_REVERSAL", argv[2], strlen("PORT_MOTOR_REVERSAL")))
//        {
//            portValue=Driver_gpioGet(PORT_MOTOR_REVERSAL);
//            TRACE_DEBUG("PORT_MOTOR_REVERSAL ��ƽ״̬�ǣ�%d.\n", portValue);
//        }
//    }
//    else if(!memcmp("GPIO_SET", argv[1], strlen("GPIO_SET")))
//    {
//        PortValue portValue = (PortValue)StringToUint32(argv[3]);
//        if(!memcmp("PORT_ANTI_LOCK", argv[2], strlen("PORT_ANTI_LOCK")))
//        {
//            Driver_gpioSet(PORT_ANTI_LOCK, portValue);
//            TRACE_DEBUG("PORT_ANTI_LOCK ��ƽ״̬����Ϊ��%d.\n", portValue);
//        }
//        else if(!memcmp("PORT_PREVENT_DISMANTLE", argv[2], strlen("PORT_PREVENT_DISMANTLE")))
//        {
//            Driver_gpioSet(PORT_PREVENT_DISMANTLE,portValue);
//            TRACE_DEBUG("PORT_PREVENT_DISMANTLE ��ƽ״̬����Ϊ��%d.\n", portValue);
//        }
//    }
    return 0;
}

//----------------------------------------------------Cmd_adcTest-----------------
static int Cmd_adcTest(int argc, char *argv[])
{
    TRACE_DEBUG("ADC����.\n");
    Driver_adcGet();
    return 0;
}

//----------------------------------------------------Cmd_testTaskTest-----------------
static int Cmd_testTaskTest(int argc, char *argv[])
{
//    if(!memcmp("EVENT_START_TEST", argv[1], strlen("EVENT_START_TEST")))
//    {
//        uint8_t macAddr[6];
//        TRACE_DEBUG("send EVENT_START_TEST.\n");
//        StringToHexGroup(argv[2], strlen(argv[2]), (char*)macAddr);
//        Test_process_setMacAddr(macAddr);
//        if(!IsBusy_testProcess())
//            TestEvent_post(EVENT_START_TEST);
//    }
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
// ��ʾ����������ʾ�����б�
int Cmd_help(int argc, char *argv[])
{
    CmdLineEntry *cmdEntry;

    ConsoleOut("Central\n");
    ConsoleOut("Available commands\n");
    ConsoleOut("------------------\n");

    cmdEntry = (CmdLineEntry *) &g_kConsoleCmdTable[0];

    // ���������������ӡ������ʾ��Ϣ������
    while (cmdEntry->cmdKeyword)
    {
        // ��ʱһ�£��ȴ�����̨�������ճ��㹻�Ŀռ�
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


// ��ʾ����
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

// �������ʾ��
int Cmd_demo(int argc, char *argv[])
{
    if (0 == strcmp(argv[1], "subcmd1"))
    {
        if (0 == strcmp(argv[2], "param"))
        {
            // ������ع��ܺ���
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

//��ȡ�������
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

//�����������
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
