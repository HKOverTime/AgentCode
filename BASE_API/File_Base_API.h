#ifndef _FILE_BASE_API_H_
#define _FILE_BASE_API_H_

#include "../generic.h"

#define MAX_FILE_POOL_NUM          100
#define MAX_FILE_NAME_LEN          512
#define MAX_READ_LEN              1000

#define FILE_BUSY_STATE              1
#define FILE_NOT_BUSY_STATE          0
#define GET_FILE_POOL_NUM_ERROR     -1

#define FILE_OPEN_MODE            "wb+"

#define FILE_OPEN_ERROR             -1
#define FILE_WRITE_OK                1
#define FILE_READ_ERROR             -1
#define FILE_READ_OVERLEN           -2

typedef struct file_map{
    int busy;
    char filename[MAX_FILE_NAME_LEN];
    FILE *pfile;
}F_map;

int API_Init_File_ENV();
int API_File_Open(char *name);
    // return the poolnum
int API_File_Close(int fnum);
int API_File_Write(int fnum,char *buf,int write_size);
int API_File_Read (int fnum,char *buf,int read_size);
#endif
