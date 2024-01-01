#ifndef PADSTACK
#define PADSTACK
#include "Item.h"
#include "shape.h"

struct _padstack {
	string padstack_id;
	vector<_shape> shapes;
	string attach;

	map<string, _shape> layer;	//特殊用途，不属于定义
};

class Padstacks {
public:
	//数据
	vector<_padstack> ps;
	map<string, int> name_to_id;
public:
	//函数
	void set_padstacks(vector<node_yes>& nodes) {
		ps.clear();
		for (int j = 0; j < (int)nodes.size(); j++) {
			node_yes node = nodes[j];
			_padstack p;
			p.padstack_id = node.data[0];
			for (auto iter = node.child.begin(); iter != node.child.end(); iter++) {
				if (iter->first == "shape") {
					for (int j = 0; j < (int)iter->second.size(); j++) {
						_shape s;
						s.type = iter->second[j].child.begin()->first;
						s.layer_id = iter->second[j].child.begin()->second[0].data[0];
						for (int k = 1; k < (int)iter->second[j].child.begin()->second[0].data.size(); k++)
							s.data.push_back(str_2_double(iter->second[j].child.begin()->second[0].data[k]));
						p.shapes.push_back(s);
					}
				}
				else if (iter->first == "attach")
					p.attach = iter->second[0].data[0];
			}
			ps.push_back(p);
			name_to_id.insert(make_pair(p.padstack_id, j));
		}
	}
	vector<_padstack> get_padstacks() { return ps; }

	double GetRadius(string strName)
	{
		return ps[name_to_id.at(strName)].shapes[0].GetRadius();
	}

	map<string, _padstack> read_padstacks(vector<node_yes>& nodes) {
		//使用id寻找焊盘形状
		//结果使用方法（得到一个_shape）：p[padstack_id].layer[layer_id]
		map<string, _padstack> mps;
		for (int i = 0; i < (int)nodes.size(); i++) {
			node_yes node = nodes[i];
			_padstack p;
			p.padstack_id = node.data[0];
			for (auto iter = node.child.begin(); iter != node.child.end(); iter++) {
				if (iter->first == "shape") {
					for (int j = 0; j < (int)iter->second.size(); j++) {
						_shape s;
						s.type = iter->second[j].child.begin()->first;
						s.layer_id = iter->second[j].child.begin()->second[0].data[0];
						for (int k = 1; k < (int)iter->second[j].child.begin()->second[0].data.size(); k++)
							s.data.push_back(str_2_double(iter->second[j].child.begin()->second[0].data[k]));
						p.layer[s.layer_id] = s;
					}
				}
				else if (iter->first == "attach")
					p.attach = iter->second[0].data[0];
			}
			mps[p.padstack_id] = p;
		}
		return mps;
	}
};

#endif
