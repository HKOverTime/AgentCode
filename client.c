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
    int i = 0;
    unsigned char c,d;


Printf("Hello Debug env\n");
//Printf("Hello nrecv = %d\n", nrecv);

    Pro_Check_The_VmInfo_On_Host(id);
//    Pro_Up_File(id, "from.txt","to.txt");
    Pro_Down_File(id, "from.txt","to.txt");
    while (1){MIC_SLEEP(100);}  
/*    
    API_Serial_Recv( id ,  &c , 1);
    while(i < 255){
        
        c = c +1;
        API_Serial_Send( id , &c ,1 );
Printf("right %d\n", c);
        API_Serial_Recv( id , &d , 1);
        if (d != c){
Printf("d =  %d  c = %d.\n",d,c);
        }
        i ++;
    } */
}
