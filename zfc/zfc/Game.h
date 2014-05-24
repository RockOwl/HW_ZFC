#include<iostream>
#include<vector>
#include<queue>
#include<list>
#include<string>
#include<sstream>
#include<ctime>
#include<cstdlib>

#include "sendCmd.h"

using namespace std;



/********
astar�������
**********/
class ANode
{
public:
	int x;
	int y;
	int step;
	int remain;
	int total;
	struct ANode* parent;

	ANode(int _x, int _y, int _step, int _remain, ANode* _parent=NULL)
	{
		x = _x;
		y = _y;
		step = _step;
		remain = _remain;
		total = step + remain;
		parent = _parent;
	}

	bool operator<(const ANode &a) const{
		return total > a.total;
	}
};





/*******************************
��Ϸ��
*******************************/
class Game
{
private:
	/**********
	��������
	**********/
	enum MapType
	{
		//δ̽��  ƽ��    �ϰ���  ����    С͵
		Unknown, Empty, Block, Police, Thief
	};
	/**********
	�ƶ�����
	**********/
	enum Move
	{
		East, South, West, North, Keep
	};
	/**********
	�������
	**********/
	struct Node
	{
		int x;
		int y;
		Move move;
	};


	/**********
	�ͻ��˽�ɫ����
	**********/
	enum Roles
	{
		PoliceClient, ThiefClient
	};

private:
	Roles role;						//��ɫ
	vector<vector<int> > map;		//��ͼ����ά���飩
	int step;						//��ǰ�ǵڼ���

	int mapWidth;					//��ͼ��
	int mapHeight;					//��ͼ��

	int policeRange;				//������Ұ��С
	int thiefRange;					//С͵��Ұ��С

	int policeNum;					//��������
	int thiefNum;					//С͵����

	bool hasEnemy;

	vector<Node> polices;			//���� k��λ�ô洢id=k�ľ������ݣ�λ��XY����һ��Ӧ������ƶ��ȣ�
	vector<Node> thiefs;			//С͵ k��λ�ô洢id=k��С͵���ݣ�λ��XY����һ��Ӧ������ƶ��ȣ�

private:
	void splitIdXY(string str, vector<Node> &list);		//���� id1,x1,y1;id2,x2,y2;id3,x3,y3;...�����ַ���
	void splitXY(string str, int &x, int &y);			//���� x,y... �ַ���
	void splitXYs(string str, vector<vector<int> > &table);	//���� x1,y1;x2,y2;x3,y3;... �ַ���

	void analyzeINI(string s);		//����INI����
	void analyzeINF(string s);		//����INF����
	string buildMOV();				//����role���ͣ���ȡpolices/thiefs�����ݹ��� MOVָ��
	void computeMove();				//������һ���ƶ�
	void policeMove();				//���㾯����һ��
	void thiefMove();				//����С͵��һ��

	//Astar�㷨����ظ�������
	int astar(int x1, int y1, int x2, int y2); //Astar�㷨 	return int:0=east, 1=south, 2=west, 3=north, 4=keep
	void astar_InsertList(list<ANode*> &list, ANode* node);
	ANode* astar_RemoveList(list<ANode*> &list, int x, int y, int step, ANode* parent);
	void astar_UpdateList(list<ANode*> &list, int x, int y, int step, ANode* parent);


public:
	void setRole(string str);		//���ÿͻ��˵Ľ�ɫ����
	void receiveINI(char* str);		//����INIָ������д���
	void receiveINF(char* str);		//����INFָ������д���
	void sendMOV();					//����MOVָ��

};