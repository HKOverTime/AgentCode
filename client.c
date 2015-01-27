#include "generic.h"

#define THE_SPEED  921600
#ifdef WIN32
    char DevName[100] = "\\\\.\\pipe\\com_2";
#else
    char DevName[100] = "/dev/ttyS1";
#endif

int main(){
    SERIAL_HANDLE_ID id ;
    id = API_Serial_Open(DevName,THE_SPEED,
            8,1);
    if (id == SERIAL_HANDLE_ID_ERROR){
        printf("Serial Handle open error\n");
        exit(0);
    }
    char recvbuf[1024] ;
    int nrecv = 0,nwrite;


Printf("Hello Debug env\n");
//Printf("Hello nrecv = %d\n", nrecv);

    
    Pro_Check_The_VmInfo_On_Host(id);
    Pro_Up_File(id, "from.txt","to.txt");
    while (1){MIC_SLEEP(100);}
}
