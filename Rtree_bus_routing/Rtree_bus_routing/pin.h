#ifndef PIN
#define PIN
#include "Item.h"

struct _pin {
	string padstack_id;
	string place_id;
	string pin_id;
	string clearance_class;
	double x;
	double y;
	int rotate;
};

class Pins {
public:
	vector<_pin> ps;
	map<string, int> name_to_id;
public:
	void set_pins(vector<node_yes>& nodes) {
		ps.clear();
		for (int j = 0; j < (int)nodes.size(); j++) {
			node_yes node = nodes[j];
			if (node.data.size() > 1) {
				_pin p;
				p.padstack_id = node.data[0];
				p.pin_id = node.data[1];
				p.x = str_2_double(node.data[2]);
				p.y = str_2_double(node.data[3]);
				auto r = node.child.find("rotate");
				if (r != node.child.end())
					p.rotate = str_2_int(r->second[0].data[0]);
				ps.push_back(p);

				name_to_id.insert(make_pair(p.pin_id, j));
			}
		}
	}
	vector<_pin> get_pins() { return ps; }

	map<string, map<string, _pin>> read_pins(vector<node_yes>& components, vector<node_yes> images) {
		//通过id来寻找特定引脚
		//结果使用方法（得到一个_pin）：p[place_id][pin_id]
		map<string, vector<string>> pin_id_temp;
		_pin p;
		map<string, map<string, _pin>> mps;
		for (auto iter = components.begin(); iter != components.end(); iter++) {
			for (int i = 0; i < (int)iter->child["place"].size(); i++)
				pin_id_temp[iter->data[0]].push_back(iter->child["place"][i].data[0]);
		}
		for (int i = 0; i < (int)images.size(); i++) {
			for (auto iter = images[i].child["pin"].begin(); iter != images[i].child["pin"].end(); iter++) {
				for (int j = 0; j < (int)pin_id_temp[images[i].data[0]].size(); j++) {
					string place_id_temp = pin_id_temp[images[i].data[0]][j];
					mps[place_id_temp][iter->data[1]].place_id = place_id_temp;
					mps[place_id_temp][iter->data[1]].pin_id = iter->data[1];
					mps[place_id_temp][iter->data[1]].padstack_id = iter->data[0];
					mps[place_id_temp][iter->data[1]].x = str_2_double(iter->data[2]);
					mps[place_id_temp][iter->data[1]].y = str_2_double(iter->data[3]);
					auto r = iter->child.find("rotate");
					if (r != iter->child.end())
						p.rotate = str_2_int(r->second[0].data[0]);
				}
			}
		}
		return mps;
	}
};

#endif
