#include <winsock2.h>
#pragma comment(lib,"Ws2_32.lib" )
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PORT 6000        

//init
void start_socket(){
    int socket_return = 0; 
    WSADATA wsadata;
    socket_return = WSAStartup(MAKEWORD(2,2),&wsadata);
    if(socket_return){
        printf("require version fail!");
        exit(0); 
    }
} 


int main(){
    start_socket();
    
    // creat socket
    SOCKET c_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (c_socket == INVALID_SOCKET)
    {
        printf("[Error]    creat socket error");
        exit(1);
    }
    
    sockaddr_in server_addr = { 0 };
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.S_un.S_addr = inet_addr("131.26.128.1");
    
    printf("[Starting] UDP Server started.\n");
    
    
    //write to file
    char filename[] = "send.txt";
    FILE* fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("[Error]    write the file false\n");
        exit(1);
    }
    printf("[Wrting]   Please Enter the data to be written to the file:\n");
    char data[512];
    scanf("%[^\n]%*c", data);  
    fprintf(fp, "%s", data);
    fclose(fp);
    printf("[Wrting]   Data has been written to the file.\n");
    
    
    //send msg
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("[Error]    Read the file false\n");
        exit(1);
    }
    char sendbuf[512];
    memset(sendbuf, 0, 512);
    fread(sendbuf, sizeof(char), 512, fp);
    int dwSent = sendto(c_socket, sendbuf, 512, 0, (sockaddr*)&server_addr, sizeof(server_addr));
    if (dwSent == 0)
    {
        printf("[Error]    Send msg to the server error\n");
        exit(1);
    }
    printf("[Seding]   Send msg:  %s\n", sendbuf);

    char recvBuf[512];
    memset(recvBuf, 0, 512);
    
    //recvive msg
    sockaddr_in addrSever = { 0 };
    int nServerAddrLen = sizeof(sockaddr_in);
    
    int dwRecv = recvfrom(c_socket, recvBuf, 512, 0, (sockaddr*)&addrSever, &nServerAddrLen);
    printf("[Recving]  Recv msg from server: %s\n", recvBuf);
    
    printf("[Success]  Msg transfer complete.\n");
    printf("[Closing]  Closing the server.\n");
    
    closesocket(c_socket);
    WSACleanup();
    return 0;
}
