#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#define LOCALPORT 8888
int main()
{
    int mysocket;
    struct sockaddr_in6 addr;
    int addr_len;
 
    if((mysocket=socket(AF_INET6,SOCK_DGRAM,0))<0)
    {
        perror("error:");
        return(1);
    }
    
    char buf[1024 + 1];
    addr_len=sizeof(struct sockaddr_in6);
    bzero(&addr,sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port=htons(LOCALPORT);
    addr.sin6_addr=in6addr_any;
    
    printf("[Starting] UDP Server started. \n");
    
    
    if(bind(mysocket,(struct sockaddr *)&addr,sizeof(addr))<0)
    {
        perror("connect");
        return(1);
    }
    char recvbuf[512];
    while(true){
        bzero(recvbuf, 512);
        sockaddr_in6 client_addr;
        bzero(&client_addr, sizeof(sockaddr_in6));
        
        //接受数据 
        int ret = recvfrom(mysocket, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&client_addr, (socklen_t*)&addr_len);
        
        printf("[Recving]  Recv from IP:[%s] Port:[%d]\n", inet_ntop(AF_INET6, &client_addr.sin6_addr, buf, sizeof(buf)), ntohs(client_addr.sin6_port));
        printf("[Recving]  Recv msg: %s \n", recvbuf);
        char filename[] = "recv.txt";
        FILE* fp = fopen(filename, "w");
        fprintf(fp, "%s", recvbuf);
        fclose(fp);
        
        //发送数据 
        char sendbuf[] = "Have received msg";
        sendto(mysocket, sendbuf, strlen(sendbuf), 0, (sockaddr*)&client_addr, addr_len);
        printf("[Seding]   Have send msg back to client.\n");    
        printf("[Success]  Msg transfer complete.\n");
        printf("[Closing]  Disconnecting from the server.\n");
        printf("\n");
    
    }

    return 0;
}
