#include <winsock2.h>
#pragma comment(lib,"Ws2_32.lib" )
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 512		 
#define PORT 6000

void Init() {
	int socket_return = 0; 
	WSADATA wsadata;
	socket_return = WSAStartup(MAKEWORD(2,2),&wsadata);
	if(socket_return){
	    printf("[ERROR]    Socket library reference error!\n");
	    exit(0); 
	}
}

SOCKET CreateClient(const char* ip){
	SOCKET c_client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (c_client == INVALID_SOCKET)
	{
		printf("[Error]    Description Failed to create the socket!\n");
		WSACleanup();
	}
	
	// IP 
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(ip);
	
	if (connect(c_client, (sockaddr *)&addr, sizeof(addr)) == INVALID_SOCKET)
	{
		printf("[Error]    Server connection failure!\n");
		WSACleanup();
	}
	return c_client;
}


int main(){
	
	char send_buf[SIZE];
	char recv_buf[SIZE];
	
	Init(); 
	char ip[] = "131.26.128.1";
	SOCKET c_client = CreateClient(ip);
	
	printf("[Status]   TCP Server started. \n");

	while (true) {
		//发送数据 
		memset(&send_buf, 0, sizeof(send_buf));	
		printf("[Writing]  Please enter the client message to send:\n");
		scanf("%s",send_buf);
		send(c_client, send_buf, BUFSIZ, 0);
		if(strcmp(send_buf, "quit") == 0) break;

		//接收数据 
		memset(&recv_buf, 0, sizeof(recv_buf));	
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

	closesocket(c_client);
	WSACleanup();
	
	return 0;
}
