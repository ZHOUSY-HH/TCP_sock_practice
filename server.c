#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include <conio.h>

#define WSVERS MAKEWORD(2,0)
#define BUFFLEN 2000

void quit(int n, SOCKET sock)
{
	printf("error%d,%d",n,GetLastError());
	closesocket(sock);                             // 关闭监听套接字
    WSACleanup();                                  // 卸载winsock library
    printf("按回车键继续...\n");
	getchar();										// 等待任意按键
	getchar();
	exit(1);
}

int main()
{
	char buf[BUFFLEN+1];
	char buf1[BUFFLEN+1];
	struct sockaddr_in fsin;          //客户端的地址;
	SOCKET msock, ssock;              //主套接字和次套接字;
	WSADATA wsadata;
	const char* service = "50500";    //端口号;
	struct sockaddr_in sin;
	int alen;
	char *pts;
	time_t now;
	int cc;
	
	WSAStartup(WSVERS, &wsadata);
	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;                                   //因特网地址簇; 
	sin.sin_addr.s_addr = INADDR_ANY;                            //任何IP地址;
	sin.sin_port = htons((u_short)atoi(service));               //设置端口;
	bind(msock, (struct sockaddr*)&sin, sizeof(sin));           //绑定IP地址和端口号;
	
	listen(msock, 5);       //开始监听，队列为5，有返回值，返回0表示成功，-1表示失败;
	
	while(!_kbhit())
	{
		alen = sizeof(struct sockaddr);      //取地址结构长度;
		ssock = accept(msock, (struct sockaddr*)&fsin, &alen);      //如果在连接请求队列中有连接请求，则接受连接请求并建立连接，返回该连接的套接字，否则，本语句被阻塞直到队列非空。fsin包含客户端IP地址和端口号
		//备注：新建立的套接字准备发送send()和接收数据recv()。
		//addrlen(也就是第三个参数),一个值结果参数，调用函数必须初始化为包含addr所指向结构大小的数值，函数返回时包含对等地址(一般为服务器地址)的实际数值；
		
		cc = recv(ssock, buf, BUFFLEN, 0);      //接受数据; 
		if(cc==SOCKET_ERROR||cc==0)
			quit(cc, msock);
		printf("收到消息：%s\n",buf);
		
		(void) time(&now);
		pts = ctime(&now);      //返回的是静态空间;
		printf("收到时间：%s",pts);
		
		cc = send(ssock, pts, strlen(pts), 0);  //发送数据(时间); 
		if(cc==SOCKET_ERROR||cc==0)
			quit(cc, msock);
		cc = send(ssock, buf, strlen(buf)+1, 0);  //发送数据(客户端数据);
		if(cc==SOCKET_ERROR||cc==0)
			quit(cc, msock);
		//printf("收到时间：%s\n\n",pts);
	    closesocket(ssock);
	}
	closesocket(msock);                                 // 关闭监听套接字
    WSACleanup();                                       // 卸载winsock library
    printf("按回车键继续...");
    getchar();										     // 等待任意按键
	getchar();
	return 0;
}
