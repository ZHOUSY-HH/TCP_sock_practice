#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>

#define BUFLEN 2000						//缓冲区大小 
#define WSVERS MAKEWORD(2,0)            //指明版本2.0。实际上是个无符号短整。
//#pragma comment(lib,"ws2_32.lib")

void inline input(char* temp)
{
	printf("������Ҫ���͵���Ϣ:");
	gets(temp);
}

void quit(int n, SOCKET sock)
{
	printf("error%d,%d",n,GetLastError());
	closesocket(sock);                             // 关闭套接字
    WSACleanup();                                  // 卸载
    printf("���س����˳�...\n");
	getchar();										//读取字符
	getchar();
	exit(1);
}

int main()
{
    const char *host = "127.0.0.2";
    const char *service = "50500";
    struct sockaddr_in sin;
    char buf[BUFLEN+1];
    char buf1[BUFLEN+1];                            //用于接受的缓冲区
    char output[BUFLEN+1];
	SOCKET sock;
	int cc;
	
	input(buf);                                     //输入的消息放到输出缓冲区中
	
	output[0] = '\0';
	
	WSADATA wsadata;
	WSAStartup(2, &wsadata);
	
	/*WORD temp = wsadata.wVersion;                    //查看推荐版本
	printf("%d",temp);*/
	
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);  //三个参数网上资料
	
	memset(&sin, 0, sizeof(sin));                      //从&sin开始的长度为sizeof(sin)的内存清0
	sin.sin_family = AF_INET;                          //因特网地址簇
	sin.sin_addr.s_addr = inet_addr(host);             //设置服务器IP地址(32位)
	sin.sin_port = htons((u_short)atoi(service));      //atoi字符串转化为整数，htons将机器数转化为大端方式
	int ret = connect(sock, (struct sockaddr*)&sin, sizeof(sin));	//连接到服务器，第二个参数指向存放服务器地址的结构，第三个参数为该结构的大小，返回值为0时表示无错误发生，返回SOCKET_ERROR表示出错，应用程序可通过WSAGetLastError()获取相应错误代码。
	
	cc =  send(sock, buf, strlen(buf)+1, 0);           //第二个参数指向缓冲区，第三个参数为缓冲区大小(字节数)，第四个参数一般设置为0，返回值:(>0)接收到的字节数,(=0)对方已关闭,(<0)连接出错
	if(cc==SOCKET_ERROR||cc==0)
		quit(cc,sock);
	
	while(cc>0)
	{
		cc = recv(sock, buf1, BUFLEN, 0);
		if(cc==SOCKET_ERROR)                           //检查错误
			quit(cc,sock);
		if(cc!=0)
			strcat(output,buf1);
	}
	printf("�յ�����Ϣ��\n");
	printf("%s\n",output);
	int temp1 = strlen(output);
	//printf("%c",output[temp1+1]);
	
	printf("���س����˳�...");
	getchar();										//结束
	getchar();
}
