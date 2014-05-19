#include "stdafx.h"
#include "udp.h"
#include "udp_api.h"
#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

SOCKADDR_IN g_ServerSockAddr; //��������SOCK��ַ
SOCKET g_SockClient = 0; //�ͻ���SOCKET
HANDLE g_ThreadHandle;  //�߳̾��

//��ȡ��ǰӦ�ó���Ŀ¼
int UDP_GetAppDirectory(char path[MAX_PATH]);

/************************************************************************/
/* ��ʼ��SOCKET                                                                     */
/************************************************************************/
int UDP_InitSocket(char* pServerIpAddr, unsigned short usServerPort,
                   unsigned short usClientPort)
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested=MAKEWORD(1,1);
    err=WSAStartup(wVersionRequested,&wsaData);
    if(err)
    {
        UDP_LOG("Winsock����ĳ�ʼ��");
        return -1;
    }

    if(LOBYTE(wsaData.wVersion)!=1||HIBYTE(wsaData.wVersion!=1))
    {
        WSACleanup();
        return -1;
    }

    //����SOCKET
    SOCKET sockClient=socket(AF_INET,SOCK_DGRAM,0);
    if (SOCKET_ERROR == sockClient)
    {
        UDP_LOG("Create Client Socket Failed.");
        return -1;
    }
    g_SockClient = sockClient;

    //��SOKECT
    SOCKADDR_IN addrClient;
    addrClient.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrClient.sin_family=AF_INET;
    addrClient.sin_port=htons(usClientPort);
    if (SOCKET_ERROR == bind(g_SockClient,(SOCKADDR*)&addrClient,sizeof(SOCKADDR)))
    {
        UDP_LOG("Bind Client Socket Failed.");
        return -1;
    }
    
    //��ʼ����������ַ��Ϣ
    g_ServerSockAddr.sin_addr.S_un.S_addr = inet_addr(pServerIpAddr);
    g_ServerSockAddr.sin_family=AF_INET;
    g_ServerSockAddr.sin_port=htons(usServerPort);

    return 0;
}

/************************************************************************/
/* �ر�SOCKET                                                                     */
/************************************************************************/
int UDP_CloseSocket()
{
    closesocket(g_SockClient);
    WSACleanup();
    return 0;
}

/************************************************************************/
/* ������Ϣ���ͻ���                                                                     */
/************************************************************************/
int UDP_SendMsgToServer(const char* pMsg)
{
    int iMsgLength = 0;
    int iRet = 0;

    if (NULL == pMsg)
    {
        UDP_LOG("������ϢΪ��");
        return -1;
    }
    
    iMsgLength = (int)strlen(pMsg) + 1;
    //UDP_LOG("UDP_SendMsgToServer: length= %d,string:%s\n", iRet, pMsg);
    iRet = sendto(g_SockClient,pMsg,iMsgLength,0,(SOCKADDR*)&g_ServerSockAddr,sizeof(SOCKADDR));


    if (SOCKET_ERROR == iRet)
    {
        UDP_LOG("UDP_SendMsgToServer ������Ϣʧ�ܡ�");
        return -1;
    }

    return 0;
}

/************************************************************************/
/* �̹߳��Ӻ���                                                                     */
/************************************************************************/
DWORD WINAPI ThreadFunc(LPVOID lp)
{
    SOCKADDR_IN addrfrom;//������ն˵�ַ��Ϣ�� 
    int len = sizeof(SOCKADDR);
    char recvBuf[MSG_MAX] = {0};
    int retval;

    while(1)
    {
        retval = recvfrom(g_SockClient,recvBuf,sizeof(recvBuf),0,
            (SOCKADDR*)&addrfrom,&len);
        if(SOCKET_ERROR == retval)
        {
            UDP_LOG("recvfrom ������Ϣʧ��");
            break;//���ɹ����ǽ��˳���
        }
        
        //to do �����ϲ�ӿ�
        if(lp) (*(PFN_RECV)lp)(recvBuf);

        //��ӡ              
        UDP_LOG("Client ThreadFunc:%s,length=%d\n",recvBuf,retval);
    }

    return 0;
}

/************************************************************************/
/* ��ȡ��ǰӦ�ó����·��                                               */
/************************************************************************/
int UDP_GetAppDirectory(char path[MAX_PATH])
{
    char *pTmp = NULL;

    if (path == NULL)
    {
        return -1;
    }

    GetModuleFileNameA(NULL, path, MAX_PATH);
    
    pTmp = strrchr(path, '\\');
    pTmp++;
    while (*pTmp != '\0')
    {
        *pTmp = '\0';
        pTmp++;
    }
    return 0;
}
/************************************************************************/
/* ��ʼ���ͻ�����Դ                                                                     */
/************************************************************************/
int InitClient(char* pServerIpAddr, unsigned short usServerPort,
               unsigned short usClientPort, PFN_RECV pFn)
{
    if (-1 == UDP_InitSocket(pServerIpAddr, usServerPort, usClientPort))
    {   
        UDP_LOG("UDP_InitSocket ��ʼ��Socketʧ��");
        return -1;
    }  
    
    //�������ն��̣߳� �������socket��Ϊ�ṹ����������̺߳����С� 
    g_ThreadHandle = CreateThread(NULL,0,ThreadFunc,(void*)pFn,0,NULL);
    if (NULL == g_ThreadHandle)
    {
        UDP_LOG("CreateThread �����߳�ʧ��");
        return -1;
    }

    return 0;
}


/************************************************************************/
/* �����Դ                                                                     */
/************************************************************************/
void waitThreadEnd()
{
    DWORD dwExitCode = 0;

    while(1)
    {
        Sleep(5000);
        GetExitCodeThread(g_ThreadHandle,&dwExitCode);
        if (dwExitCode != STILL_ACTIVE )
        {
            break;
        }
    }

    CloseHandle(g_ThreadHandle);//�ǵùر��̡߳�
    UDP_CloseSocket();
}