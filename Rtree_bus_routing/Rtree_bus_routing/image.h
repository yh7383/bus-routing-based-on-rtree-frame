#ifndef IMAGE
#define IMAGE
#include "Item.h"
#include "outline.h"
#include "pin.h"

struct _image {
	string image_id;
	//vector<_outline> outlines;
	Outlines outlines;
	//vector<_pin> pins;
	Pins pins;
};

class Images {
public:
	vector<_image> is;
	map<string, int> name_to_id;
public:
	void set_images(vector<node_yes> nodes) {
		is.clear();
		//Outlines os;
		//Pins ps;
		for (int j = 0; j < (int)nodes.size(); j++) {
			_image i;
			i.image_id = nodes[j].data[0];
			i.outlines.set_outlines(nodes[j].child["outline"]);
			//i.outlines = os.get_outlines();
			i.pins.set_pins(nodes[j].child["pin"]);
			//i.pins = ps.get_pins();
			is.push_back(i);

			name_to_id.insert(make_pair(i.image_id, j));
		}
	}
	vector<_image>& get_images() { return is; }

	_image& get_image(string str) { return is[name_to_id[str]]; };

	void GetBoundingRect(_vertex& lb, _vertex& rt, _vertex& center, double degree) {
		for (int i = 0; i < (int)is.size(); i++)
		{
			is[i].outlines.GetBoundingRect(lb, rt, center, degree);
		}
	};
};

#endif
