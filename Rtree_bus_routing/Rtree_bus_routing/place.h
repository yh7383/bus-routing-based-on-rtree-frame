#ifndef PLACE
#define PLACE
#include "Item.h"

class Places {
public:
	vector<_place> ps;
public:
	void set_places(vector<node_yes>& nodes) {
		ps.clear();
		for (int j = 0; j < (int)nodes.size(); j++) {
			_place p;
			p.place_id = nodes[j].data[0];
			p.x = str_2_double(nodes[j].data[1]);
			p.y = str_2_double(nodes[j].data[2]);
			p.side = nodes[j].data[3];
			p.rotation = str_2_int(nodes[j].data[4]);
			ps.push_back(p);
		}
	}
	vector<_place> get_places() { return ps; }
};

#endif
