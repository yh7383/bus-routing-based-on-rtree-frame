#ifndef TREE
#define TREE
#include "Item.h"
#include "data.h"
#include "wiring.h"

class Tree {
public:
	Tree(string path);
	Tree() {};
	void Open(string path);
	~Tree();
	node_yes get_pcb_yes();
public:
	/*���ߺ�������Ҫ���ڲ�ѯ��������
	*/
	//չʾ�ڵ��е����ݡ��ӽڵ�������Լ�����
	void show(node_yes& node);
	//��ĳ���ڵ��������������ڲ�ʹ�ã�
	void find_on_node(node_yes& node, string id, vector<node_yes>* p, bool all, int index);
	//����������������Ĭ�Ϸ����������ĵ�һ��������ڶ�����������ָ������� i �����
	node_yes find(string id, int index = 0);
	//Ĭ������������������������������������ڶ�����������ָ����ĳ������������
	vector<node_yes> find_all(string id, node_yes& node);
	//���wiring
	void ClearWirings();
public:
	/*д����������������д��dsn�ļ�
	*/
	//��_wiring��ʽ���ݼ��������е�wiring������
	void put_wiring_to_pcb(_wiring& w);
	//���ļ���д��ָ���ӽڵ�����ݣ������ڲ�ʹ�ã�
	void write_one_node(ofstream& out, node_yes& node, int k, vector<string> struct_, string name);
	//����������д���ļ���
	void write_new(string path);
	void write_new_dsn(string path, vector<bus>& bus_vector, int bus_count);
	void write_new_dsn_net(string path, vector<net>& net_vector, int net_count);
	vector<_rect> find_component();
	vector<_vertex> find_boundary();
	bool judge_cross(_vertex a, _vertex b, _vertex c, _vertex d);
	_wire read_wire(node_yes node);
	_wiring read_wiring(node_yes node);
	_path read_path(node_yes node);
	_via read_via(node_yes node);
	vector<_bus> get_couple_pin(_vertex offset_vertex, vector<_rect> component_rect_vector, _wiring w);
private:
	/*˽�к�������ȡdsn�ļ����ݲ��洢��������
	*/
	//��ȡ�����ַ���ֱ������ָ���ַ�
	bool read_until(istream& in, char c);
	//��ȡ���пո���ת���ַ���ֱ�����������ַ�
	void read_unuseful(istream& in);
	//���������ַ��ڵ��ַ���
	string read_quoted_data(istream& in, char& quote);
	//����һ������������
	string read_data(istream& in, char& quote);
	//������һ������������
	string read_next(istream& in, char& quote);
	//����ָ���ַ��з��ַ���
	vector<string> split(const string& s, char delimiter);

public:
	//����
	node_yes pcb_yes;
private:
	/*˽������
	*/
	//�ļ���
	ifstream myfile;

	//û��
	map<string, string> pcb_struct;
private:
	/*����������ȡ������
	*/
	node_yes read_yes(istream& in, char& quote);
};

#endif
