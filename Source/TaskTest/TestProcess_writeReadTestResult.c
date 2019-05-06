/*
 * TestProcess_writeReadTestResult.c
 *
 *  Created on: 2018��10��8��
 *      Author: zheng
 */


#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include "TestProcess_writeReadTestResult.h"
#include "IWUP_cmd_group/CmdGroup_qcTest.h"
#include "DataType.h"
#include "BF_Util.h"

#define TIMEOUT_TEST_PROCESS_WRITEREAD_TESTRESULT   1200
#define TIMEOUT_TEST_PROCESS_ERASE_TESTRESULT   4000

RET_TEST_WRITEREAD_TESTRESULT TestProcess_writeTestResult(char result)
{
    RET_TEST_WRITEREAD_TESTRESULT ret;
    //�������ò��Խ��ָ��----------------------------------
    uint8_t para[2];
    para[0] = WRITEREAD_TESTRESULT_TYPE_WRITE;
    para[1] = result;
    //�ӳ�100ms
    Task_sleep((100*1000)/Clock_tickPeriod);
    SendCmd_qcTest_writeReadTestResult(para, 2);

    //�ȴ����շ�����Ϣ
    uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_WRITEREAD_TESTRESULT, TIMEOUT_TEST_PROCESS_WRITEREAD_TESTRESULT);
    if (events & EVENT_TESTPROCESS_WRITEREAD_TESTRESULT)
    {
        ret = RET_TEST_WRITEREAD_TESTRESULT_SUCCESS;
        TRACE_CODE("���ò��Խ���ɹ�.\n");
    }
    else
    {
        ret = RET_TEST_WRITEREAD_TESTRESULT_ERROR;
        TRACE_DEBUG("���ò��Խ��%dms��ʱ��\n",TIMEOUT_TEST_PROCESS_WRITEREAD_TESTRESULT);
    }
    return ret;
}

RET_TEST_WRITEREAD_TESTRESULT TestProcess_readTestResult(char* result)
{
    RET_TEST_WRITEREAD_TESTRESULT ret;
    //���ͻ�ȡ���Խ��ָ��----------------------------------
    BF_taskSleepMs(200);
    uint8_t para[2];
    para[0] = WRITEREAD_TESTRESULT_TYPE_READ;
    para[1] = 0x00;
    SendCmd_qcTest_writeReadTestResult(para, 2);

    //�ȴ����շ�����Ϣ
    uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_WRITEREAD_TESTRESULT, TIMEOUT_TEST_PROCESS_WRITEREAD_TESTRESULT);
    if (events & EVENT_TESTPROCESS_WRITEREAD_TESTRESULT)
    {
        ret = RET_TEST_WRITEREAD_TESTRESULT_SUCCESS;
        TRACE_CODE("��ȡ���Խ���ɹ�.\n");
        *result = testResult;
    }
    else
    {
        ret = RET_TEST_WRITEREAD_TESTRESULT_ERROR;
        TRACE_DEBUG("��ȡ���Խ��%dms��ʱ��\n",TIMEOUT_TEST_PROCESS_WRITEREAD_TESTRESULT);
    }
    return ret;
}

RET_TEST_WRITEREAD_TESTRESULT TestProcess_clearTestResult()
{
    RET_TEST_WRITEREAD_TESTRESULT ret;
    //���ͻ�ȡ���Խ��ָ��----------------------------------
    BF_taskSleepMs(200);
    uint8_t para[2];
    para[0] = WRITEREAD_TESTRESULT_TYPE_CLEAR;
    para[1] = 0x00;
    SendCmd_qcTest_writeReadTestResult(para, 2);

    //�ȴ����շ�����Ϣ
    uint32_t events = TestEvent_pend(EVENT_TESTPROCESS_WRITEREAD_TESTRESULT, TIMEOUT_TEST_PROCESS_ERASE_TESTRESULT);
    if (events & EVENT_TESTPROCESS_WRITEREAD_TESTRESULT)
    {
        ret = RET_TEST_WRITEREAD_TESTRESULT_SUCCESS;
        TRACE_CODE("�������Խ���ɹ�.\n");
        BF_taskSleepMs(200);
    }
    else
    {
        ret = RET_TEST_WRITEREAD_TESTRESULT_ERROR;
        TRACE_DEBUG("�������Խ��%dms��ʱ��\n",TIMEOUT_TEST_PROCESS_WRITEREAD_TESTRESULT);
    }
    return ret;
}

