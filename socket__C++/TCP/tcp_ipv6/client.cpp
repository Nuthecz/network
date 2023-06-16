#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#define SIZE 512
#define REMOTEPORT 8888
#define REMOTEIP "e80::f8f:a714:31f:7d20"
int main(int argc,char *argv[])
{
	int c_client;
	struct sockaddr_in6 addr;
	
	//socket
	if((c_client=socket(AF_INET6,SOCK_STREAM, IPPROTO_TCP))<0)
	{
		perror("error:");
		return(1);
	}
 	printf("[Starting] UDP Server started.\n");
 	
 	int addr_len = sizeof(struct sockaddr_in6);
	bzero(&addr,sizeof(addr));
	addr.sin6_family=AF_INET6;
	addr.sin6_port=htons(REMOTEPORT);
	inet_pton(AF_INET6,REMOTEIP,&addr.sin6_addr);
 	
 	if (connect(c_client, (struct sockaddr *)&addr, sizeof(addr)) == 0)
	{
		printf("[Error]    Server connection failure!\n");
	}
 	char send_buf[SIZE];
	char recv_buf[SIZE];
	
	printf("[Status]   TCP Server started. \n");
	
	while(true){
		//发送数据 
		bzero(send_buf, 512);
		printf("[Writing]  Please enter the client message to send:\n");
		scanf("%s",send_buf);
		send(c_client, send_buf, BUFSIZ, 0);
		if(strcmp(send_buf, "quit") == 0) break;
	
		//接收数据 
		bzero(recv_buf, 512);	
		recv(c_client, recv_buf, BUFSIZ, 0);
		if(strcmp(recv_buf, "quit") == 0) break;
		printf("[Showing]  Server information:\n");
		printf("%s\n",recv_buf);
		
		//写入文件 
		char filename[] = "server.txt";
		FILE* fp = fopen(filename, "a+");
		fprintf(fp, "%s\r\n", recv_buf);
		fclose(fp);
	
	}
	return 0;
}


