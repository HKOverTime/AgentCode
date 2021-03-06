#include "generic.h"

#define THE_SPEED  921600
#ifdef WIN32
    char DevName[100] = "COM2";
    char TestVM_name[100] = "OT's Window Test VM";
#else
    char DevName[100] = "/dev/ttyS1";
    char TestVM_name[100] = "OT's Linux Test VM";
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

unsigned char c = 0 ,d;int i = 0;

Printf("Hello Debug env\n");
//Printf("Hello nrecv = %d\n", nrecv);
    Pro_Connect_And_Send_VmInfo_From_Agent(id,1023,"192.168.1.1",LINUX_OS_TYPE,TestVM_name);
    Pro_Waite_CMD_AND_DO_IT(id);
/*
    API_Serial_Send(id,&c,1);
    while(c< 255){
        API_Serial_Recv(id,&c,1);
        d = c;
printf("--> recv : %d .\n",d);
i++;
printf("%d times.\n",i);
        API_Serial_Send(id,&d,1);
  //      MIC_USLEEP(100);
    }
*/
}
