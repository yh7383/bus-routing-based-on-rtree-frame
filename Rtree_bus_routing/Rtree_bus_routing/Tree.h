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
	/*工具函数，主要用于查询树中内容
	*/
	//展示节点中的数据、子节点的索引以及个数
	void show(node_yes& node);
	//在某个节点内搜索（程序内部使用）
	void find_on_node(node_yes& node, string id, vector<node_yes>* p, bool all, int index);
	//在整棵树中搜索，默认返回搜索到的第一个结果，第二个参数可以指定输出第 i 个结果
	node_yes find(string id, int index = 0);
	//默认在整棵树中搜索，返回所有搜索结果，第二个参数可以指定在某棵子树中搜索
	vector<node_yes> find_all(string id, node_yes& node);
	//清除wiring
	void ClearWirings();
public:
	/*写函数，将主树数据写回dsn文件
	*/
	//将_wiring格式数据加入主树中的wiring子树中
	void put_wiring_to_pcb(_wiring& w);
	//在文件中写入指定子节点的数据（程序内部使用）
	void write_one_node(ofstream& out, node_yes& node, int k, vector<string> struct_, string name);
	//将整棵主树写入文件中
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
	/*私有函数，读取dsn文件数据并存储在主树里
	*/
	//读取流中字符串直到遇到指定字符
	bool read_until(istream& in, char c);
	//读取流中空格与转义字符，直到遇到其他字符
	void read_unuseful(istream& in);
	//返回引用字符内的字符串
	string read_quoted_data(istream& in, char& quote);
	//返回一个完整的数据
	string read_data(istream& in, char& quote);
	//返回下一个完整的数据
	string read_next(istream& in, char& quote);
	//根据指定字符切分字符串
	vector<string> split(const string& s, char delimiter);

public:
	//主树
	node_yes pcb_yes;
private:
	/*私有数据
	*/
	//文件流
	ifstream myfile;

	//没用
	map<string, string> pcb_struct;
private:
	/*主函数，读取整棵树
	*/
	node_yes read_yes(istream& in, char& quote);
};

#endif
