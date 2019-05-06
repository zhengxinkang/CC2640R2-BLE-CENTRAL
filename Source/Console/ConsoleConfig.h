/*
 * ConsoleConfig.h
 *
 *  Created on: 2017��8��9��
 *      Author: blue.zhengxinkang
 */

#ifndef SOURCE_CONSOLE_CONSOLECONFIG_H_
#define SOURCE_CONSOLE_CONSOLECONFIG_H_


// �������Ŀ�����оƬ��Դ���ʵ���������ĺ궨�塣
// �����δŪ����غ����˼���뱣��Ĭ�ϡ�

/**
 * @brief ����������ʾ���ִ���
 * @details ����ʾ������ӡ��ÿ����ͷ������ʾ�û��������
 */
#define CONSOLE_PROMPT_STRING               "<Lock>"


/**
 * @brief �����Ƿ���Ҫ�ڳ�ʼ��ʱ��ӡ��ӭ��Ϣ��
 * @details 1 Ϊ��Ҫ��ӡ��0 Ϊ����Ҫ��ӡ��
 *  �����Ҫ��ʾ��ӭ��Ϣ������Ҫ�����涨�����Ļ�ӭ��Ϣ��
 *  �����ӡ�Ŀɺ��Ծ�����Ϣ�Ķ��塣
 */
#define CONSOLE_NEED_WELCOME_MSG            1


#ifdef CONSOLE_NEED_WELCOME_MSG

/**
 * @brief ���延ӭ��Ϣ�еĲ�Ʒ��Ϣ��
 */
#define CONSOLE_WELCOME_PRODUCT             "Embedded"

/**
 * @brief ���延ӭ��Ϣ�еİ�Ȩ��Ϣ��
 * @details �粻��Ҫ����ע�͸úꡣ
 */
#define CONSOLE_WELCOME_COPYRIGHT           "Copyright(C) Stag TI-RTOS 2017-2018"

/**
 * @brief ���延ӭ��Ϣ�е�������Ϣ��
 * @details �粻��Ҫ����ע�͸úꡣ
 */
#define CONSOLE_WELCOME_OTHER               "EmConsole for cc2640r2 port"

/**
 * @brief �����Ƿ��ڻ�ӭ��Ϣ�д�ӡ�汾��Ϣ��
 * @details 1 Ϊ��Ҫ��ӡ��0 Ϊ����Ҫ��ӡ��
 *  �����еİ汾�Ž��� CmdLine ʵ���ж��塣
 */
#define CONSOLE_WELCOME_NEED_VERSION        1

#endif  // CONSOLE_NEED_WELCOME_MSG

/**
 * @brief ���������л����С��
 * @details ��ֵ������һ�������ı�����֧�ֵ�����ַ���Ŀ��
 */
#define CONSOLE_LINE_BUF_SIZE               64

/**
 * @brief ����������֧�ֵ���ʷ����������
 * @details ��ֵ���������¼�ͷ������������Ŀ��������ڵ���1��
 * @note ���ֻ֧��256����ʷ����
 */
#define CONSOLE_CMD_HISTORY_NUM             2

/**
 * @brief �����������ջ����С��
 * @details ��ֵ������һ�������ı�����֧�ֵ�����ַ���Ŀ��
 * @note ���ջ��彫��Consoleģ��ʵ�֣�������ConsoleDriver��
 */
#define CONSOLE_RX_BUF_SIZE                 255

/**
 * @brief �����������ͻ����С��
 * @details ���ͻ���Խ��һ���Է��͵��ַ�Խ�ࡣ
 *  ��ʵʱ����ͨ�ţ�����ʱ���ϴ�ķ��ͻ���������һ���Դ�ӡ�϶���ַ���
 *  <p> �ڴ�ӡ��������ʱ���� help �����г�����֧�ֵ��������ʾ�����ݿ��ܲ�������
 *  ���沿�־������ضϣ���ͨ������Ϊ���ͻ��������õù�С��ԭ�򣬽���İ취�У�
 *  - �����ͻ��壬���ѱ������õô�һ�㡣ͨ���������豸��RAM��Դ��
 *  - �ڴ�ӡ���ı���ʱ�����Ϊ������ӡ��䣬��ӡ�������ʱ��
 *
 * @note ��ֵ�Ƿ񱻲��ã�ȡ���ھ����ConsoleDriver��ʵ�֡�
 *  �����ṩ������ConsoleDriver��Ĭ�Ͻ����ñ��������ֵ��
 */
#define CONSOLE_TX_BUF_SIZE                 256//���hexgroup��ʱ��,��Ҫ�õ��ܴ�Ŀռ�

/**
 * @brief �������̨����֧�ֵ�������������
 * @details ��ֵ������Ӧ����������ܽ��յĲ���������
 */
#define CONSOLE_MAX_ARGUMENTS               16


#endif /* SOURCE_CONSOLE_CONSOLECONFIG_H_ */
