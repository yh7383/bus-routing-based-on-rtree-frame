#ifndef OUTLINE
#define OUTLINE
#include "Item.h"
#include "shape.h"

struct _outline {
	_shape shape;
};

class Outlines {
public:
	vector<_outline> os;
public:
	void set_outlines(vector<node_yes>& nodes) {
		os.clear();
		for (int j = 0; j < (int)nodes.size(); j++) {
			node_yes node = nodes[j];
			_outline o;
			o.shape.type = node.child.begin()->first;
			o.shape.layer_id = node.child.begin()->second[0].data[0];
			for (int k = 1; k < (int)node.child.begin()->second[0].data.size(); k++)
				o.shape.data.push_back(str_2_double(node.child.begin()->second[0].data[k]));
			os.push_back(o);
		}
	}
	vector<_outline>& get_outlines() { return os; }

	void GetBoundingRect(_vertex& lb, _vertex& rt, _vertex& center, double degree) {
		lb.x = lb.y = -100000;
		rt.x = rt.y = 100000;
		for (int i = 0; i < (int)os.size(); i++)
		{
			_vertex lb1(100000, 100000), rt1(-100000, -100000);
			os[i].shape.GetBoundingRect(lb1, rt1, center, degree);
			lb.x = __max(lb.x, lb1.x);;
			lb.y = __max(lb.y, lb1.y);
			rt.x = __min(rt.x, rt1.x);
			rt.y = __min(rt.y, rt1.y);
		}
	}
};

#endif
