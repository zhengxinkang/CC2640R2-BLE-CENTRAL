/*
 * CmdLine.h
 *
 *  Created on: 2017年8月9日
 *      Author: blue.zhengxinkang
 */

#ifndef SOURCE_CONSOLE_CMDLINE_H_
#define SOURCE_CONSOLE_CMDLINE_H_


extern void CmdLine_Init(void);
int Cmd_showparam(int argc, char *argv[]);
int Cmd_TraceGetLevel(int argc, char *argv[]);


#endif /* SOURCE_CONSOLE_CMDLINE_H_ */
