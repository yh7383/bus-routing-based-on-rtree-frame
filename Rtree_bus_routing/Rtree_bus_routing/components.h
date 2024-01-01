#ifndef COMPONENTS
#define COMPONENTS
#include "Item.h"
#include "place.h"

struct _component {
	string image_id;
	//vector<_place> place;
	Places places;
};

class Components {
public:
	vector<_component> cs;
	map<string, int> name_to_id;
public:
	void set_component(vector<node_yes> nodes) {
		cs.clear();
		//Places ps;
		int pl = 0;
		for (int i = 0; i < (int)nodes.size(); i++) {
			_component c;
			c.image_id = nodes[i].data[0];
			c.places.set_places(nodes[i].child["place"]);
			//c.place = ps.get_places();
			cs.push_back(c);

			for (int j = 0; j < (int)c.places.ps.size(); j++)
			{
				name_to_id.insert(make_pair(c.places.ps[j].place_id, pl++));
			}
		}
	}
	vector<_component> get_component() { return cs; }
	void Append(_place& plc) {
		_component c;
		c.image_id = "";
		c.places.ps.push_back(plc);
		cs.push_back(c);
		name_to_id.insert(make_pair(plc.place_id, cs.size() - 1));
	};

};

#endif
