#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#define REMOTEPORT 8888
#define REMOTEIP "fe80::f8f:a714:31f:7d20"
int main(int argc,char *argv[])
{
	int mysocket;
	struct sockaddr_in6 addr;
	int addr_len;
	//socket
	if((mysocket=socket(AF_INET6,SOCK_DGRAM,0))<0)
	{
		perror("error:");
		return(1);
	}
 	printf("[Starting] UDP Server started.\n");
 	
	addr_len=sizeof(struct sockaddr_in6);
	bzero(&addr,sizeof(addr));
	addr.sin6_family=AF_INET6;
	addr.sin6_port=htons(REMOTEPORT);
	inet_pton(AF_INET6,REMOTEIP,&addr.sin6_addr);
 	
 	//msg写入文件操作 
	char filename[] = "send.txt";
	FILE* fp = fopen(filename, "w");
	printf("[Wrting]   Please Enter the data to be written to the file:\n");
	char data[512];
	scanf("%[^\n]%*c", data);  
	fprintf(fp, "%s", data);
	fclose(fp);
	printf("[Wrting]   Data has been written to the file.\n");
 	
 	//读取文件操作 
	fp = fopen(filename, "r");
 	char sendbuf[512];
	bzero(sendbuf, 512);
	fread(sendbuf, sizeof(char), 512, fp);
	
	int dwSent = sendto(mysocket, sendbuf, sizeof(sendbuf), 0,(struct sockaddr *)&addr,addr_len);
	printf("[Seding]   Send msg:  %s\n", sendbuf);
	
	char recvbuf[512];
	bzero(recvbuf, 512);
	
	//接收数据 
	sockaddr_in6 addrSever;
	int ret = recvfrom(mysocket, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&addrSever, (socklen_t*)&addr_len);	
	printf("[Recving]  Recv msg from server: %s\n", recvbuf);

	printf("[Success]  Msg transfer complete.\n");
	printf("[Closing]  Closing the server.\n");

}
