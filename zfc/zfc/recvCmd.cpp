#include "stdafx.h"
#include "zfc.h"
#include "recvCmd.h"


int recvIni(char *str)
{
    //���ճ�ʼ��ָ���
	game.receiveINI(str);

    return 0;
}

int recvInf(char *str)
{
    //����INFָ��   
	game.receiveINF(str);
	game.sendMOV();

    return 0;
}

//���յ�ENDָ��˳�����
int recvEnd(char *str)
{
    log("recv END!");
    exit(0);//�˳�����
}

int recvCmd(char *str)
{
    log("[RECV]: %s",str);
    if(strstr(str,"INF"))
    {
        recvInf(str);
        //���յ�INF֮��Ĵ���
    }
    else if(strstr(str,"INI"))
    {
        recvIni(str);
    }
    else if(strstr(str,"END"))
    {
        recvEnd(str);
    }
    else
    {
        log("\r\n ERROR RECV CMD[%s]!", str);
    }

    return 0;
}

