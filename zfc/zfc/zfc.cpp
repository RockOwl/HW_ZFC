// zfc.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "zfc.h"
#include "recvCmd.h"
#include "sendCmd.h"

//#include <windows.h>

string teamName="";
string serverIp="";
ushort serverPort=0;
string myRole=""; //"POL" OR "THI"
ushort localPort=0;
Game game;


//void Wchar_tToString(std::string& szDst, _TCHAR* wchar)
//{
//	wchar_t * wText = wchar;
//	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,wText,-1,NULL,0,NULL,FALSE);// WideCharToMultiByte������
//	char *psText;  // psTextΪchar*����ʱ���飬��Ϊ��ֵ��std::string���м����
//	psText = new char[dwNum];
//	WideCharToMultiByte (CP_OEMCP,NULL,wText,-1,psText,dwNum,NULL,FALSE);// WideCharToMultiByte���ٴ�����
//	szDst = psText;// std::string��ֵ
//	delete []psText;// psText�����
//}


int checkPara(int argc, _TCHAR* argv[])
{
	//string teamName2, serverIp2, serverPort2, myRole2, localPort2;

    if(argc!=6)
    {
        log("���󣺲�����������(%d)��", argc);
        log("�����ʽ�� zfc ���� ������IP �������˿� ��ɫ��ע��POL��THI�� ���ؽ��ն˿ڣ�ע�����ֽ�ɫʹ�ò�ͬ�˿ڣ�");
        goto checkParaErr;
    }

	//Wchar_tToString(teamName2, argv[1]);
	//Wchar_tToString(serverIp2, argv[2]);
	//Wchar_tToString(serverPort2, argv[3]);
	//Wchar_tToString(myRole2, argv[4]);
	//Wchar_tToString(localPort2, argv[5]);

	//teamName=teamName2;
	//serverIp=serverIp2;
	//serverPort = stoi(serverPort2);
	//myRole = myRole2;
	//localPort = stoi(localPort2.c_str());


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

