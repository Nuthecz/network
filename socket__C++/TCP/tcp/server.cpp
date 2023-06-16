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

SOCKET CreateServer(){
	SOCKET s_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s_server == INVALID_SOCKET)
	{
		printf("[ERROR]    Description Failed to create the socket!\n");
		WSACleanup();
	}
	
	// IP 
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (bind(s_server, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		printf("[Error]    Socket binding failed!\n");
		WSACleanup();
	}
	
	if (listen(s_server, SOMAXCONN) < 0) {
		printf("[Error]    Failed to set listening status!\n");
		WSACleanup();
	}

	return s_server;
}


int main(){
	
	char send_buf[SIZE];
	char recv_buf[SIZE];
	
	Init();
	SOCKET s_server = CreateServer();
	printf("[Status]   wait client connect...\n");
	
	sockaddr_in client_addr;
	int addr_len = sizeof(client_addr);

	SOCKET s_accept = accept(s_server, (sockaddr*)&client_addr, (int*)&addr_len);
	if (s_accept == INVALID_SOCKET) {
		printf("[Error]    Connection failed!");
		WSACleanup();
		return 0;
	}
	printf("[Recving]  Recv from IP:[%s] Port:[%d]\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	
	while(true){
		//接收数据 
		memset(&recv_buf, 0, sizeof(recv_buf));	
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
		memset(&send_buf, 0, sizeof(send_buf));	
		printf("[Writing]  Please enter the server message to send:\n");
		scanf("%s",send_buf);
		send(s_accept, send_buf, BUFSIZ, 0);
		if(strcmp(send_buf, "quit") == 0) break;
		
	}

	
	closesocket(s_server);
	closesocket(s_accept);

	WSACleanup();
	
	return 0;
}


