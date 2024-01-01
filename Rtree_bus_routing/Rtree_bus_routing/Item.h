#ifndef ITEM
#define ITEM

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
using namespace std;

const int LEFT_VERTICAL = 0;                    //�½�������������ǵױߡ����ߡ���ߺ��ұߣ��Լ���㲼��״̬�������ߺ��Ѳ��ߣ��Լ��߽�ͱ߽���
const int RIGHT_VERTICAL = 1;
const int TOP_HORIZONAL = 2;
const int BOTTOM_HORIZONAL = 3;
const int UNDER_WIRING = 4;
const int WIRED = 5;
const int BOUNDARY = 6;
const int BOUNDARY_MARK = -1;
const int BUS_MAX_WIDTH = 25;

/*ȫ�ֽṹ��
*/
//�����ṹ
struct node_yes {
	vector<string> data;
	map<string, vector<node_yes>> child;
};
//�����ṹ
struct _vertex {
	double x;
	double y;
	bool isBoundary = false;
	int obstacle_index = -1;
	int location = -1;
	_vertex(int i, int j) :x(i), y(j) {};
	_vertex(double xx, double yy) :x(xx), y(yy) {};
	_vertex(double i, double j, bool isBoun, int index, int loc) :x(i), y(j), isBoundary(isBoun), obstacle_index(index), location(loc) {};
	_vertex() {};
};

struct _rect {
	string layer_id;
	_vertex left_bottom_vertex;
	_vertex right_top_vertex;
};
struct _place {
	string place_id;
	double x;
	double y;
	string side;
	int rotation;
};
//struct _component {
//	string image_id;
//	vector<_place> place;
//};
struct Graph_Node
{
	int layer;	//��ʾ�ýڵ����ڵĲ���
	int comp;	//��ʾ�ýڵ����ڵ�comp
	int index;	//��ʾ�ýڵ��������

	Graph_Node() { layer = 0;  comp = 0; index = -1; };
	Graph_Node(int _layer, int _comp, int _index) : comp(_comp), layer(_layer), index(_index) {};

	inline bool bUndef() { return index == -1; };
	inline void SetUndef() { index = -1; };
	inline void SetStart() { index = -2; };
	inline bool bStart() { return index == -2; };
	bool operator == (const Graph_Node& a) { return (comp == a.comp && layer == a.layer && index == a.index); };
	bool operator != (const Graph_Node& a) { return (comp != a.comp || layer != a.layer || index != a.index); };
	bool operator < (const Graph_Node& a) const { return (index < a.index); };
};

struct Graph_Neighbor	//ĳ���ڵ���ڵ�
{
public:
	Graph_Node neighbor;	//�ڵ���Ϣ
	int cost;		//��ʾ�ñߵķ���

	Graph_Neighbor() {};
	Graph_Neighbor(Graph_Node a, int _cost) : neighbor(a), cost(_cost) {};
	//void BackupCost() { backup = cost; };
	//void RetrieveCost() { cost = backup; };
	//void SwapCost() { int t = cost; cost = backup; backup = t; };
private:
	//int backup;		//��ֵ��������㷨ʱʹ��

};
struct _component_boundary {
	_vertex start_vertex;
	_vertex end_vertex;
	int location;
};
struct _bus {
	string net_id;
	string layer_id;
	vector<_vertex> vertex;
};

/*ȫ�ֹ��ߺ���
*/
//��stringתΪint
extern int str_2_int(string s);
//��stringתΪdouble
extern double str_2_double(string s);
//���ַ���ȫ��תΪСд
extern string _2_lower(string s);

#endif
