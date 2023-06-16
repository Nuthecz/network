#include <winsock2.h>
#pragma comment(lib,"Ws2_32.lib" )
#include <Windows.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 1024

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
    
    //creat socket 
    SOCKET s_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);;
    if (s_socket == INVALID_SOCKET)
    {
        printf("[Error]    creat socket error");
        exit(1);
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    
    printf("[Starting] UDP Server started. \n");
    bind(s_socket, (sockaddr*)&server_addr, sizeof(sockaddr));
    char recvbuf[512];
    
    while(true){
        memset(recvbuf, 0, 512);
        sockaddr_in client_addr;
        memset(&client_addr, 0, sizeof(sockaddr_in));
        int nClientAddrLen = sizeof(sockaddr);
        
        //recvive msg
        int ret = recvfrom(s_socket, recvbuf, 512, 0, (sockaddr*)&client_addr, &nClientAddrLen);
        printf("[Recving]  Recv from IP:[%s] Port:[%d]\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("[Recving]  Recv msg: %s \n", recvbuf);
        char filename[] = "recv.txt";
        FILE* fp = fopen(filename, "w");
        fprintf(fp, "%s", recvbuf);
        fclose(fp);
        
        //send msg 
        char sendbuf[] = "Have received msg";
        sendto(s_socket, sendbuf, strlen(sendbuf), 0, (sockaddr*)&client_addr, sizeof(client_addr));
        printf("[Seding]   Have send msg back to client.\n");
        printf("[Success]  Msg transfer complete.\n");
        printf("[Closing]  Disconnecting from the server.\n");
        printf("\n");
        
    }
    

    
    
    closesocket(s_socket);
    WSACleanup();
    return 0;
}
