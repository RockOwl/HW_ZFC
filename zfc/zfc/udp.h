#ifndef UDP_H 
#define UDP_H

#include <WinSock2.h>
#include <Windows.h>

#define MSG_MAX 8192

//udp��ʼ��
int UDP_InitSocket(char* pServerIpAddr, unsigned short usServerPort,
                   unsigned short usClientPort);

//���sock�����Դ
int UDP_CloseSocket();

//�̴߳�����
DWORD WINAPI ThreadFunc(LPVOID lp);

//��¼��־����
//extern void log(const char *fmt, ...);

//�����Դ
void ClearRes();

#ifndef UDP_LOG
#define UDP_LOG printf
#endif

#endif