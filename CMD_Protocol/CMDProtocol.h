#ifndef _CMD_PROTOCOL_H_
#define _CMD_PROTOCOL_H_

#include "../generic.h"

#define LINUX_OS_TYPE                   1
#define WINDOWS_OS_TYPE                 2
#define MAX_BUFFER_LEN              65535

#define VIRTUAL_REFLECT_CONNECT         1
#define PRO_CONNECT_OK                  1
#define PRO_CONNECT_ERROR              -1



#define PRO_ENV_INIT_OK                 1
#define PRO_ENV_INIT_ERROR             -1
#define PRO_ENV_UP_FILE_RUN_NOW         1
#define PRO_ENV_UP_FILE_ERROR          -1 
#define PRO_ANALYSIS_ERROR             -1
#define PRO_ANALYSIS_OK_BUT_DO_ERROR   -2
#define PRO_ANALYSIS_OK_AND_DO_OK       1

#define READ_OK                         1

int Pro_Env_Init();
int Pro_Up_File(SERIAL_HANDLE_ID VM_ID, char *From_File_Path_On_Host,
    char *To_File_Path_On_Vm);
int Pro_Waite_CMD_AND_DO_IT(SERIAL_HANDLE_ID VM_ID);

int Pro_Connect_And_Send_VmInfo_From_Agent(SERIAL_HANDLE_ID VM_ID,
    int buflen, char *ip, int OS_TYPE, char *VM_name);
int Pro_Check_The_VmInfo_On_Host(SERIAL_HANDLE_ID VM_ID);
//int Pro_Analysis_And_Do_It(char *CMD_BUFFER);




#endif
