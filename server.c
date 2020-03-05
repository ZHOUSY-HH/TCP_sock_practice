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
	closesocket(sock);                             // �رռ����׽���
    WSACleanup();                                  // ж��winsock library
    printf("���س�������...\n");
	getchar();										// �ȴ����ⰴ��
	getchar();
	exit(1);
}

int main()
{
	char buf[BUFFLEN+1];
	char buf1[BUFFLEN+1];
	struct sockaddr_in fsin;          //�ͻ��˵ĵ�ַ;
	SOCKET msock, ssock;              //���׽��ֺʹ��׽���;
	WSADATA wsadata;
	const char* service = "50500";    //�˿ں�;
	struct sockaddr_in sin;
	int alen;
	char *pts;
	time_t now;
	int cc;
	
	WSAStartup(WSVERS, &wsadata);
	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;                                   //��������ַ��; 
	sin.sin_addr.s_addr = INADDR_ANY;                            //�κ�IP��ַ;
	sin.sin_port = htons((u_short)atoi(service));               //���ö˿�;
	bind(msock, (struct sockaddr*)&sin, sizeof(sin));           //��IP��ַ�Ͷ˿ں�;
	
	listen(msock, 5);       //��ʼ����������Ϊ5���з���ֵ������0��ʾ�ɹ���-1��ʾʧ��;
	
	while(!_kbhit())
	{
		alen = sizeof(struct sockaddr);      //ȡ��ַ�ṹ����;
		ssock = accept(msock, (struct sockaddr*)&fsin, &alen);      //����������������������������������������󲢽������ӣ����ظ����ӵ��׽��֣����򣬱���䱻����ֱ�����зǿա�fsin�����ͻ���IP��ַ�Ͷ˿ں�
		//��ע���½������׽���׼������send()�ͽ�������recv()��
		//addrlen(Ҳ���ǵ���������),һ��ֵ������������ú��������ʼ��Ϊ����addr��ָ��ṹ��С����ֵ����������ʱ�����Եȵ�ַ(һ��Ϊ��������ַ)��ʵ����ֵ��
		
		cc = recv(ssock, buf, BUFFLEN, 0);      //��������; 
		if(cc==SOCKET_ERROR||cc==0)
			quit(cc, msock);
		printf("�յ���Ϣ��%s\n",buf);
		
		(void) time(&now);
		pts = ctime(&now);      //���ص��Ǿ�̬�ռ�;
		printf("�յ�ʱ�䣺%s",pts);
		
		cc = send(ssock, pts, strlen(pts), 0);  //��������(ʱ��); 
		if(cc==SOCKET_ERROR||cc==0)
			quit(cc, msock);
		cc = send(ssock, buf, strlen(buf)+1, 0);  //��������(�ͻ�������);
		if(cc==SOCKET_ERROR||cc==0)
			quit(cc, msock);
		//printf("�յ�ʱ�䣺%s\n\n",pts);
	    closesocket(ssock);
	}
	closesocket(msock);                                 // �رռ����׽���
    WSACleanup();                                       // ж��winsock library
    printf("���س�������...");
    getchar();										     // �ȴ����ⰴ��
	getchar();
	return 0;
}
