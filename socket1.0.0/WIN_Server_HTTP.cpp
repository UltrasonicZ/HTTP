//// unsigned.cpp : 定义控制台应用程序的入口点。
////
//
//#include "stdafx.h"
//#include "zhelpers.h"
//
//int main(void)
//{
//	void *context = zmq_ctx_new();
//	void *sink = zmq_socket(context, ZMQ_ROUTER);
//
//	zmq_bind(sink, "inproc://example");
//
//	void *anonymous = zmq_socket(context, ZMQ_REQ);
//	zmq_connect(anonymous, "inproc://exmaple");
//	s_send(anonymous, "ROUTER uses a generated UUID");
//	s_dump(sink);
//
//	void *identified = zmq_socket(context, ZMQ_REQ);
//	zmq_setsockopt(identified, ZMQ_IDENTITY, "PEER2",5);
//	zmq_connect(identified, "inproc://example");
//	s_send(identified, "ROUTER socket uses REQ's socket identity");
//	s_dump(sink);
//
//	zmq_close(sink);
//	zmq_close(anonymous);
//	zmq_close(identified);
//	zmq_ctx_destroy(context);
//
//	return 0;
//}

#include "stdafx.h"
#include <stdio.h>  
#include <winsock2.h>  

#pragma comment(lib,"ws2_32.lib")  

int main(int argc, char* argv[])  
{  
	//初始化WSA  
	WORD sockVersion = MAKEWORD(2,2);  
	WSADATA wsaData;  
	if(WSAStartup(sockVersion, &wsaData)!=0)  
	{  
		return 0;  
	}  

	//创建套接字  
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
	if(slisten == INVALID_SOCKET)  
	{  
		printf("socket error !");  
		return 0;  
	}  

	//绑定IP和端口  
	sockaddr_in sin;  
	sin.sin_family = AF_INET;  
	sin.sin_port = htons(8888);  
	sin.sin_addr.S_un.S_addr = INADDR_ANY;   
	if(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)  
	{  
		printf("bind error !");  
	}  

	//开始监听  
	if(listen(slisten, 5) == SOCKET_ERROR)  
	{  
		printf("listen error !");  
		return 0;  
	}  

	char buf[0x400];
	sprintf(buf, "%s",
			"HTTP/1.0 200 OK\r\n"
	        "Content-Length: 20\r\n"
	        "Content-Type:text/html\r\n"
	        "\r\n"
	        "你好，TCP客户端！");
	//循环接收数据  
	SOCKET sClient;  
	sockaddr_in remoteAddr;  
	int nAddrlen = sizeof(remoteAddr);  
	char revData[255];   
	while (true)  
	{  
		printf("等待连接...\n");  
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);  
		if(sClient == INVALID_SOCKET)  
		{  
			printf("accept error !");  
			continue;  
		}  
		printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));  

		//接收数据  
		int ret = recv(sClient, revData, 255, 0);         
		if(ret > 0)  
		{  
			revData[ret] = 0x00;  
			printf(revData);  
		}  
	/*	n = read(clnt_fd, inbuf, sizeof(inbuf));
		write(fileno(stdout), inbuf, n);
		write(clnt_fd, buf, (int)strlen(buf));*/
		//发送数据  
		const char * sendData = "你好，TCP客户端！\n";  
		send(sClient, buf, strlen(buf), 0);  
		closesocket(sClient);  
	}  

	closesocket(slisten);  
	WSACleanup();  
	return 0;  
} 