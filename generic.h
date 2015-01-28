#ifndef _GENERIC_H_
#define _GENERIC_H_

//#define PTW32_STATIC_LIB

#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<getopt.h>    //strlen
//#include<pthread.h> //for threading , link with lpthread

#include<unistd.h>


#ifdef WIN32
	#include<winsock2.h>
	#include<ws2tcpip.h>
    #include<windows.h>
#else
	#include <netdb.h>
	#include<sys/socket.h>
	#include<arpa/inet.h>
	#include<signal.h>
    #include<sys/types.h>
    #include<sys/stat.h>
    #include<fcntl.h>
    #include<termios.h>
    #include<errno.h>
#endif // head files check

#ifdef WIN32
    #define SERIAL_HANDLE_ID HANDLE
    #define SERIAL_HANDLE_ID_ERROR (HANDLE)(-1)
    #define MIC_USLEEP(x)  usleep(x)
    #define MIC_SLEEP(x)   Sleep(x)
#else 
    #define SERIAL_HANDLE_ID int
    #define SERIAL_HANDLE_ID_ERROR (-1)
    #define FALSE               -1
    #define TRUE                 0
    #define MIC_USLEEP(x)  sleep(x)
    #define MIC_SLEEP(x)   sleep(x * 1000)
    typedef unsigned long DWORD;
#endif // 

// my headers start from here
#include "CMD_Protocol/CMDProtocol.h"
#include "VM_Info.h"
#include "Serial_Base_API.h"
#include "File_Base_API.h"
// end my headers



#ifdef _DEBUG_PRINT_
    #define Printf(fmt, args...) printf( fmt,##args )
#else 
    #define Printf(fmt, args...) ;
#endif

#endif // _GENERIC_H_
