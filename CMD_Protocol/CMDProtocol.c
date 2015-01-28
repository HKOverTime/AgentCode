#include "CMDProtocol.h"


int Pro_Env_Init(){
}

#define MAX_ADD_FILE_SIZE            1000
#define MIC_FOR_RECV_BUF_LEN         1023
//#define MAX_FILE_NAME_LEN            255
#define PRO_UP_FILE_ERROR             -1
#define PRO_UP_FILE_OK                 1

#define GENERAL_CMD                    2
    #define UP_FILE_CMD                    1
        #define FILE_PATH_ERROR      -1
        #define FILE_CAN_CREATE       5
        #define FILE_ADD_ERROR       -1
        #define FILE_ADD_OK           6
        #define TO_FILE_PATH_NAME     1
        #define ADD_FILE_DATA         2
        #define TO_FILE_ADD_END       3
        #define CHECK_FILE_SIZE       4
////File Serial_operat
#define S_FILE_TOUCH_ERROR      -1
#define S_MAX_ADD_FILE_SIZE   1000
#define S_FILE_ADD_ERROR        -1
#define S_FILE_ADD_OK           -2
#define S_FILE_CHECK_SIZE_OK     1
#define S_FILE_CLOSE_OK          1
#define S_ADD_FILE_ERROR        -1
int file_serial_touch(SERIAL_HANDLE_ID VM_ID, char *filepath){
    char buffer[MIC_FOR_CONN_BUF_LEN] ;
    char recvbuf[MIC_FOR_CONN_BUF_LEN] ;
    int nsend = -1,nrecv=-1;
    int to_file_path_len   = strlen(filepath);   
    buffer[0] = GENERAL_CMD;
    buffer[1] = UP_FILE_CMD;
    buffer[2] = TO_FILE_PATH_NAME;
    buffer[3] = to_file_path_len;
    strncpy((&(buffer[4])),filepath,to_file_path_len);
    nsend = API_Serial_Send(VM_ID,buffer,to_file_path_len + 4);
    
    nrecv = API_Serial_Recv(VM_ID,recvbuf,MIC_FOR_CONN_BUF_LEN);
    if(nrecv != 4 
        || recvbuf[0] != GENERAL_CMD 
        || recvbuf[1] != UP_FILE_CMD
        || recvbuf[2] == FILE_PATH_ERROR ){
        return S_FILE_TOUCH_ERROR ;
    }
Printf("The file pool id is %d\n",recvbuf[3]);
    return recvbuf[3];
}

int file_serial_send_file_id(SERIAL_HANDLE_ID id,int file_id){
    char buffer[MIC_FOR_CONN_BUF_LEN] ;
    int nsend = -1;
Printf("The file id is %d\n", file_id);
    buffer[0] = GENERAL_CMD;
    buffer[1] = UP_FILE_CMD;
    buffer[2] = FILE_CAN_CREATE;
    buffer[3] = (char)file_id;
    nsend = API_Serial_Send(id,buffer,4);
    
    return file_id;
}

int file_serial_add_date(SERIAL_HANDLE_ID VM_ID,int file_id,
      char *filedata,int size ){

    char buffer[MIC_FOR_CONN_BUF_LEN] ;
    char recvbuf[MIC_FOR_CONN_BUF_LEN] ;
    int nsend = -1,nrecv=-1;
    if (size > MAX_ADD_FILE_SIZE){
        return S_ADD_FILE_ERROR;
    }

Printf("ADD file data now\n");
    buffer[0] = GENERAL_CMD;
    buffer[1] = UP_FILE_CMD;
    buffer[2] = ADD_FILE_DATA;
    buffer[3] = (char)file_id;
    buffer[4] = size;
    memcpy( (&(buffer[5])), filedata,size);
        
    nsend = API_Serial_Send(VM_ID,buffer,buffer[4] + 5);
    
    nrecv = API_Serial_Recv(VM_ID,recvbuf,MIC_FOR_CONN_BUF_LEN);
    if(nrecv != 3 
        || recvbuf[0] != GENERAL_CMD 
        || recvbuf[1] != UP_FILE_CMD
        || recvbuf[2] == FILE_ADD_ERROR ){
        return S_ADD_FILE_ERROR ;
    }
Printf("%d -->  ADD data END. [size=%d]\n", file_id,size);
}

int file_serial_send_Add_File_OK(SERIAL_HANDLE_ID id){
    char buffer[MIC_FOR_CONN_BUF_LEN] ;
    int nsend = -1;
    buffer[0] = GENERAL_CMD;
    buffer[1] = UP_FILE_CMD;
    buffer[2] = FILE_ADD_OK;
    nsend = API_Serial_Send(id,buffer,3);
    
    return 1;
}

int file_serial_check_size(SERIAL_HANDLE_ID VM_ID,int file_id,
      int file_size){
    return S_FILE_CHECK_SIZE_OK;
}

int file_serial_close(SERIAL_HANDLE_ID VM_ID,int file_id){
    char buffer[MIC_FOR_CONN_BUF_LEN] ;
    int nsend = -1;
    buffer[0] = GENERAL_CMD;
    buffer[1] = UP_FILE_CMD;
    buffer[2] = TO_FILE_ADD_END;
    buffer[3] = (char)file_id;
    nsend = API_Serial_Send(VM_ID,buffer,3);
    return S_FILE_CLOSE_OK;
}

int Pro_Up_File(SERIAL_HANDLE_ID VM_ID, char *From_File_Path_On_Host,
    char *To_File_Path_On_Vm){

    int from_file_path_len,to_file_path_len;
    int file_ID = -1;
    from_file_path_len = strlen(From_File_Path_On_Host);
    to_file_path_len   = strlen(To_File_Path_On_Vm);   
    
    if(from_file_path_len > MAX_FILE_NAME_LEN || 
        to_file_path_len > MAX_FILE_NAME_LEN )
        return PRO_UP_FILE_ERROR;
    
    // send file name
    file_ID = file_serial_touch( VM_ID, To_File_Path_On_Vm);
    if( file_ID == S_FILE_TOUCH_ERROR ){
        return PRO_UP_FILE_ERROR ;
    }
    ///   file can create
    ///   send file data now
    char helloWorld[100] = "Hello World!\n";
    int str_len = strlen(helloWorld);
    file_serial_add_date ( VM_ID , file_ID ,
         helloWorld , str_len );
    ///   check file size and close it 
    file_serial_check_size ( VM_ID , file_ID , 100);
    file_serial_close ( VM_ID , file_ID );
}

int UP_FILE_CMD_DO(SERIAL_HANDLE_ID id,char *recvbuf){
    int TheFileNumber = 1;
int i = 3;
    switch(recvbuf[0]){
        case TO_FILE_PATH_NAME:
Printf("This is a TO_FILE_PATH_NAME\n");
Printf("The To_FILE_NAME_LEN is  %d.\n",(recvbuf[1]));
Printf("The To_FILE_NAME is %s.\n",&(recvbuf[2]));
            TheFileNumber = API_File_Open(&(recvbuf[2]));
            file_serial_send_file_id(id, TheFileNumber);
            return TheFileNumber;
            break;
        case ADD_FILE_DATA:
Printf("This is a ADD_FILE_DATA\n");
Printf("The ADD_FILE_NUMBER is  %d.\n",(recvbuf[1]));
Printf("The ADD_FILE_SIZE is  %d.\n",(recvbuf[2]));
Printf("The FILE_DATA is %s.\n",&(recvbuf[3]));
Printf("The recv[%d] is %d.\n",i,recvbuf[i]);
            API_File_Write(recvbuf[1],&(recvbuf[3]),recvbuf[2]);
            file_serial_send_Add_File_OK(id);
            return TheFileNumber;
            break;
        case CHECK_FILE_SIZE:
Printf("CHECK THE FILE \n");
            return TheFileNumber;
            break;
        case TO_FILE_ADD_END:
Printf("CLOSE THE FILE \n");
            API_File_Close(recvbuf[1]);
            return TheFileNumber;
            break;
    }
    return -1;
}

int Pro_Waite_CMD_AND_DO_IT(SERIAL_HANDLE_ID VM_ID) {
    char recvbuf[ MIC_FOR_RECV_BUF_LEN ] ;
    int nrecv = -1;
    nrecv = API_Serial_Recv(VM_ID,recvbuf,MIC_FOR_RECV_BUF_LEN);
    while(nrecv >= 0){
        if (nrecv == 0) {
Printf("NO RECV CMD\n");
MIC_USLEEP(100);
             continue ;
        }
        switch(recvbuf[0]){
        case GENERAL_CMD:
            switch(recvbuf[1]){
                case UP_FILE_CMD:
Printf("This is a UP_FILE_CMD\n");
                    UP_FILE_CMD_DO(VM_ID,&(recvbuf[2]));
                    break;
            }
            break;
        }
        nrecv = API_Serial_Recv(VM_ID,recvbuf,MIC_FOR_RECV_BUF_LEN);
    }
}

int Pro_Connect_And_Send_VmInfo_From_Agent(SERIAL_HANDLE_ID VM_ID,
    int buflen, char *ip, int OS_TYPE, char *VM_name){

    int iplen,namelen;
    int nsend = -1,nrecv=-1;
    char buffer[MIC_FOR_CONN_BUF_LEN] ;
    char recvbuf[MIC_FOR_CONN_BUF_LEN] ;
    iplen = strlen(ip);
    namelen = strlen(VM_name);
    if(iplen > MIC_VM_IP_LEN || namelen > MIC_VM_NAME_LEN){
        return PRO_CONNECT_ERROR ;
    }
    buffer[0] = VIRTUAL_REFLECT_CONNECT;
    buffer[1] = (MAX_BUFFER_LEN & 0xff00) >> 8;
    buffer[2] = (MAX_BUFFER_LEN & 0x00ff) ;
    buffer[3] = OS_TYPE ;
Printf("MAX_BUF = %d ,buffer[1] = %d, buffer[2] = %d\n",MAX_BUFFER_LEN,buffer[1],buffer[2]);
    
    nsend = API_Serial_Send(VM_ID,buffer,4);
    nrecv = API_Serial_Recv(VM_ID,recvbuf,MIC_FOR_CONN_BUF_LEN);
    if(nrecv != 2 || recvbuf [0] != VIRTUAL_REFLECT_CONNECT 
        || recvbuf[1] != READ_OK ){
        return PRO_CONNECT_ERROR ;
    }
    /// Send VMType OK
    
    buffer[0] = VIRTUAL_REFLECT_CONNECT;
    buffer[1] = 0;
    buffer[2] = iplen;
    strncpy(&(buffer[3]),ip,iplen);
    nsend = API_Serial_Send(VM_ID,buffer,iplen+4);
    nrecv = API_Serial_Recv(VM_ID,recvbuf,MIC_FOR_CONN_BUF_LEN);
    if(nrecv != 2 || recvbuf [0] != VIRTUAL_REFLECT_CONNECT 
        || recvbuf[1] != READ_OK ) {
        return PRO_CONNECT_ERROR ;
    }
    //// Send IP ok

    buffer[0] = VIRTUAL_REFLECT_CONNECT;
    buffer[1] = 0;
    buffer[2] = namelen;
    strncpy(&(buffer[3]),VM_name,namelen);
    nsend = API_Serial_Send(VM_ID,buffer,namelen + 4);
    nrecv = API_Serial_Recv(VM_ID,recvbuf,MIC_FOR_CONN_BUF_LEN);
    if(nrecv != 2 || recvbuf [0] != VIRTUAL_REFLECT_CONNECT 
        || recvbuf[1] != READ_OK ) {
        return PRO_CONNECT_ERROR ;
    }
    
    return PRO_CONNECT_OK;
}

#define PRO_CHECK_THE_VM_HOST_ERROR -1
#define PRO_CHECK_THE_VM_HOST_OK     1

int Pro_Check_The_VmInfo_On_Host(SERIAL_HANDLE_ID VM_ID){
    char buffer[3000];
    char recvbuf[3000];
    int nsend,nrecv;
    int buf_max_len,iplen,vm_namelen;
    char ip[MIC_VM_IP_LEN],vm_name[MIC_VM_NAME_LEN];
    
    // set the READ_OK
    buffer[0] = VIRTUAL_REFLECT_CONNECT;
    buffer[1] = READ_OK;

    nrecv = API_Serial_Recv(VM_ID,recvbuf,MIC_FOR_CONN_BUF_LEN);
    if( nrecv == SERIAL_RECV_ERROR || 
        recvbuf[0] != VIRTUAL_REFLECT_CONNECT ) {
        return PRO_CHECK_THE_VM_HOST_ERROR;
    }
    buf_max_len = (((int)((unsigned char)recvbuf[1])) << 8 ) + 
        ((int)((unsigned char)recvbuf[2]));
    if( recvbuf[3] == LINUX_OS_TYPE ){
        printf("The vm is linux\n");
    }
    else if( recvbuf[3] == WINDOWS_OS_TYPE ){
        printf("The vm is windows\n");
    }
    else {
        printf("Unknow Linux Type .\n");
    }
Printf("The maxbuffer Len is %d\n",buf_max_len);
    nsend = API_Serial_Send(VM_ID,buffer,2);
    // read OS_TYPE and  send READ_OK

    nrecv = API_Serial_Recv(VM_ID,recvbuf,MIC_FOR_CONN_BUF_LEN);
    if( nrecv == SERIAL_RECV_ERROR || 
        recvbuf[0] != VIRTUAL_REFLECT_CONNECT ) {
        return PRO_CHECK_THE_VM_HOST_ERROR;
    }
Printf("ip is %s\n",&(recvbuf[3]));
    nsend = API_Serial_Send(VM_ID,buffer,2);
    // read OS_ip  and send READ_OK

    nrecv = API_Serial_Recv(VM_ID,recvbuf,MIC_FOR_CONN_BUF_LEN);
    if( nrecv == SERIAL_RECV_ERROR || 
        recvbuf[0] != VIRTUAL_REFLECT_CONNECT ) {
        return PRO_CHECK_THE_VM_HOST_ERROR;
    }
Printf("name is %s\n",&(recvbuf[3]));
    nsend = API_Serial_Send(VM_ID,buffer,2);
    return PRO_CHECK_THE_VM_HOST_OK;
}
