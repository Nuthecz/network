#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#define SIZE 512
#define LOCALPORT 8888
int main()
{
    int s_server;
    struct sockaddr_in6 addr;
    
    if((s_server=socket(PF_INET6, SOCK_STREAM, 0))<0)
    {
        perror("error:");
        return(1);
    }
    
    int addr_len = sizeof(struct sockaddr_in6);
    bzero(&addr, sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port=htons(LOCALPORT);
    addr.sin6_addr=in6addr_any;
    
    printf("[Status]   wait client connect...\n");
    char send_buf[SIZE];
    char recv_buf[SIZE];
    char buf [1025];
    if(bind(s_server,(struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        return(1);
    }
    
    if (listen(s_server, 100) == -1 ) {
        printf("[Error]    Failed to set listening status!\n");
    }
    
    sockaddr_in6 client_addr;
    socklen_t len = sizeof(struct sockaddr);
    int s_accept = accept(s_server, (sockaddr*)&client_addr, &len);
    printf("[Recving]  Recv from IP:[%s] Port:[%d]\n", inet_ntop(AF_INET6, &client_addr.sin6_addr, buf, sizeof(buf)), ntohs(client_addr.sin6_port));
    
    while(true){
        //接收数据 
        bzero(recv_buf, 512);    
        recv(s_accept, recv_buf, BUFSIZ, 0);
        if(strcmp(recv_buf, "quit") == 0) break;
        printf("[Showing]  Client information:\n");
        printf("%s\n",recv_buf);
        
        //写入文件 
        char filename[] = "client.txt";
        FILE* fp = fopen(filename, "a+");
        fprintf(fp, "%s\r\n", recv_buf);
        fclose(fp);
        
        //发送数据 
        bzero(send_buf, 512);
        printf("[Writing]  Please enter the server message to send:\n");
        scanf("%s",send_buf);
        send(s_accept, send_buf, BUFSIZ, 0);
        if(strcmp(send_buf, "quit") == 0) break;
        
    }

    return 0;
}
