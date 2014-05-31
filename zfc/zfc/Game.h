#pragma once
#include<iostream>
#include<vector>
#include<list>
#include<queue>
#include<string>
#include<set>
#include<sstream>
#include<ctime>
#include<cstdlib>

#include "sendCmd.h"

using namespace std;


/**********************************************************************************
* ��Ϸ��  
**********************************************************************************/
class Game
{
private:
	//============================================================================
	// ��ɫ�ͻ�������
	//============================================================================
	enum Roles
	{
		PoliceClient, ThiefClient
	};

	//============================================================================
	// ��������
	//============================================================================
	enum MapType
	{
		//δ̽��  ƽ��   �ϰ��� 
		Unknown, Empty, Block
	};

	//============================================================================
	// �ƶ�����
	//============================================================================
	enum Move
	{
		East, South, West, North, Keep
	};

	//============================================================================
	// λ�ý������
	//============================================================================
	struct Node
	{
		int x;		//��ǰλ��
		int y;
		Move move;	//�ƶ�����
		bool visible;

		//������������
		int scanToPreX;		//ɨ��ʱ��ŵķ�λ������λ���ϰ����ϣ�
		int scanToPreY;
		int scanToX;		//ɨ��ʱ׼ȷ�ķ�λ(��ŷ�λ��Χ�Ŀհ�����)
		int scanToY;
		bool directHoriz;	//��ǰ�Ƿ���ɨ��ˮƽ��

		bool isHorizScan;	//�����ϣ�����������������ɨ��
		int sober;

	};

	//============================================================================
	// �����뷽��ļнǽ��
	//============================================================================
	struct AngleNode
	{
		Move direct;
		int index;
		double value;
		AngleNode(int _index, Move _direct, double _value):index(_index),direct(_direct),value(_value){}
	};

	//============================================================================
	// �����뷽��ļнǽ��ıȽϺ���
	//============================================================================
	struct AngleNodeCmp
	{
		bool operator()(const AngleNode& a, const AngleNode& b)
		{
			return a.value < b.value;
		}
	};

private:
	//============================================================================
	// ����
	//============================================================================
	Roles role;						//��ɫ����
	vector<vector<MapType> > map;	//��ͼ����ά���飩
	int step;						//��ǰ�ǵڼ���

	int mapWidth;					//��ͼ��
	int mapHeight;					//��ͼ��

	int policeRange;				//������Ұ��С
	int thiefRange;					//С͵��Ұ��С

	bool hasEnemy;					//�Ƿ��е���

	vector<Node> polices;			//���� k��λ�ô洢id=k�ľ������ݣ�λ��XY����һ��Ӧ������ƶ��ȣ�
	int lockedPolice;				//׷���ľ���Id������Χ��

	int soloPolice;					//ĳ�����⾯��Id (һ������������ľ��츺������Ŀ�ʼ��������ɨ�裬��������ˢ��ɨ��)
	Move soloDirect;				//��������
	vector<int> helixDists;			//��¼�����ľ��� ��1,1,2,2,3,3,4,4,5,5...��
	int helixDistIndex;				//�����������������

	vector<Node> thiefs;			//С͵
	int lockedThief;				//��׷����С͵Id


	vector<vector<int> > scanHorLine;		//ˮƽɨ����
	vector<vector<int> > scanVerLine;		//��ֱɨ����
	vector<int> scanHorIndex;				//ˮƽɨ������
	vector<int> scanVerIndex;				//��ֱɨ������


public:
	void setRole(string str);		//���ÿͻ��˵Ľ�ɫ����
	void receiveINI(char* str);		//����INIָ������д���
	void receiveINF(char* str);		//����INFָ������д���
	void sendMOV();					//����MOVָ��

private:
	//============================================================================
	// �����ַ���
	//============================================================================
	void splitIdXY(string str, vector<Node> &outList);				//���� id1,x1,y1;id2,x2,y2;id3,x3,y3;...�����ַ���
	void splitXY(string str, int &outx, int &outy);					//���� x,y... �ַ���
	void splitXYs(string str, vector<vector<MapType> > &outTable);	//���� x1,y1;x2,y2;x3,y3;... �ַ���

	//============================================================================
	// ��������
	//============================================================================
	void analyzeINI(string s);		//����INI����
	void analyzeINF(string s);		//����INF����

	//============================================================================
	// ��������
	//============================================================================
	string buildMOV();				//����role���ͣ���ȡpolices/thiefs�����ݹ��� MOVָ��

	//============================================================================
	// �����ƶ�����
	//============================================================================
	void computeMove();				//������һ���ƶ�
	void policeMove();				//���㾯����һ��
	void thiefMove();				//����С͵��һ��

	//============================================================================
	// ������ƶ�
	//============================================================================
	void policeScan();				//����ɨ�裨ˢ�ӷ���
	void helixScan();				//����ɨ�裨����������ֻ��һ������ʹ��
	void policeScan2();
	void policePursue();			//����׷��

	bool isValidXY(int x, int y);
	bool isValidMove(int x, int y, Move move);
	//============================================================================
	// С͵���ƶ�
	//============================================================================
	void thiefScan();				//С͵ɨ��
	void thiefEscape();				//С͵����

	//============================================================================
	// �����ƶ��ĸ�������
	//============================================================================
	void findValidPositionAroundXY(int x, int y, int& outx, int& outy, bool isThief=false);						//����(x,y)������Χ�������Чλ�ã���ֵ��tox,toy
	bool findValidNeighborByDirect(int x, int y, Move direct, int& outx, int& outy, bool isRecursion=false);	//����(x,y)��ָ�������һ���ɴ�λ�ã��������ڣ���˳ʱ�뻻����Ѱ��			
	double calcRadian(int x, int y, int centerx, int centery, Move move);										//���������нǣ�����(center->xy) �� ����(move)�ļн�
	void calcScanLine();																						//����ɨ���ߺ����������븺������ɨ��ľ����



//////////////////////////////////////////////////////////////////////////////////////////////////
// A*�㷨
//////////////////////////////////////////////////////////////////////////////////////////////////
private:
	//============================================================================
	// Astar�������
	//============================================================================
	class ANode
	{
	public:
		int x;			//����
		int y;
		int step;		//���߲���
		int remain;		//ʣ�����
		int total;		//�ܴ���=step+remain
		ANode* parent;	

		ANode(int _x, int _y, int _step, int _remain, ANode* _parent=NULL):x(_x),y(_y),step(_step),remain(_remain),total(step+remain),parent(_parent){}
	};

	//============================================================================
	// Astar�㷨���ıȽ���
	//============================================================================
	struct ANodeLess
	{
		bool operator() (const ANode* a, const ANode* b) const
		{
			return a->total < b->total;
		}
	};

	//============================================================================
	// Astar�㷨���õ������������������ظ������Ŀ�����
	//============================================================================
	multiset<ANode*, ANodeLess> openList;
	multiset<ANode*, ANodeLess> closeList;
	vector<vector<int> > marks;								//0=δ���� 1=openlist 2=closelist
	vector<vector<multiset<ANode*>::const_iterator > > stores;	//�洢ÿ��λ�ö�Ӧ��ANode������

	//============================================================================
	// Ѱ·�㷨A*
	//============================================================================
	void astar(int x1, int y1, int x2, int y2, Move& outMove, bool isThief=false);		//Astar�㷨

};