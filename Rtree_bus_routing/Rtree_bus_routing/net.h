#ifndef NET
#define NET
#include "Item.h"
#include "pin.h"

struct _net {
	string net_id;
	vector<string> pins;
};

class Nets {
public:
	vector<_net> ns;
	map<string, int> name_to_id;
public:
	void set_nets(vector<node_yes> nodes) {
		//结果使用方法（得到一个place_id-pin_id）：n.pins[下标]
		ns.clear();
		for (int i = 0; i < (int)nodes.size(); i++) {
			_net n;
			n.net_id = nodes[i].data[0];
			auto iter = nodes[i].child.find("pins");
			if (iter == nodes[i].child.end()) continue;
			for (int j = 0; j < (int)iter->second[0].data.size(); j++) {
				n.pins.push_back(iter->second[0].data[j]);
			}
			ns.push_back(n);

			name_to_id.insert(make_pair(n.net_id, i));
		}
	}
	vector<_net> get_nets() { return ns; }
};

#endif
