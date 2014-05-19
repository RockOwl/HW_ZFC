// zfc.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "zfc.h"
#include "recvCmd.h"
#include "sendCmd.h"

string teamName="";
string serverIp="";
ushort serverPort=0;
string myRole=""; //"POL" OR "THI"
ushort localPort=0;
Game game;


int checkPara(int argc, _TCHAR* argv[])
{
    if(argc!=6)
    {
        log("���󣺲�����������(%d)��", argc);
        log("�����ʽ�� zfc ���� ������IP �������˿� ��ɫ��ע��POL��THI�� ���ؽ��ն˿ڣ�ע�����ֽ�ɫʹ�ò�ͬ�˿ڣ�");
        goto checkParaErr;
    }

    teamName=argv[1];
    serverIp=argv[2];
    serverPort = atoi(argv[3]);
    myRole = argv[4];
    localPort = atoi(argv[5]);

    log("getPara: teamName[%s] serverIp[%s] serverPort[%u] myRole[%s] localPort[%u]", 
        teamName.c_str(), serverIp.c_str(), serverPort, myRole.c_str(), localPort);
    if(myRole!="POL" && myRole!="THI")
    {
        log("���󣺽�ɫ��������%s������ʹ��POL��THI��",myRole.c_str());
        goto checkParaErr;
    }
	game.setRole(myRole);
    return OK;

checkParaErr:
    return ERROR;
}

int _tmain(int argc, _TCHAR* argv[])
{
    //������
    int rt=checkPara(argc, argv);
    if(rt<0) goto end;

    //��ʼ��UDP���ҽӽ��մ�����recvCmd�������ָ��
    if (ERROR == InitClient((char*)serverIp.c_str(),serverPort,localPort,recvCmd))
    {
        log("InitClient ʧ��");
        return ERROR;
    }

    //�����Լ���ɫ����Ϣ����������POL �� THIָ��
    sendMyInfoToServer();

    waitThreadEnd();//ѭ���ȴ��߳��˳��������ڽ��յ�ENDָ���ֱ����exit(0);�˳�����
end:
    getchar();
    return 0;
}

