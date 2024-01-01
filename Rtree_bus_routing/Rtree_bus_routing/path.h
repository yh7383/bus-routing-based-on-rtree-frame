#ifndef PATH
#define PATH
#include "Item.h"

struct _path {
	string layer_id;
	int aperture_width;
	vector<_vertex> vertex;
};

class Pathes {
public:
	vector<_path> ps;
public:
	void set_pathes(vector<node_yes>& nodes) {
		ps.clear();
		for (int j = 0; j < (int)nodes.size(); j++) {
			node_yes node = nodes[j];
			_path p;
			p.layer_id = node.data[0];
			p.aperture_width = str_2_int(node.data[1]);
			for (int i = 2; i < (int)node.data.size(); i += 2)
				p.vertex.push_back(_vertex(str_2_double(node.data[i]), str_2_double(node.data[i + 1])));
			ps.push_back(p);
		}
	}

	vector<_path> get_pathes() { return ps; }

	//center表示器件中心点，degree表示旋转角度， lb,rt为左下角和右上角坐标
	void GetBoundingRect(_vertex& lb, _vertex& rt, _vertex& center, double degree) {

		double xi, yi;
		double ss = sin(degree * 3.1415926 / 180);
		double cs = cos(degree * 3.1415926 / 180);

		lb.x = lb.y = 100000;
		rt.x = rt.y = -100000;


		for (int i = 0; i < (int)ps.size(); i++)
		{
			for (int j = 0; j < (int)ps[i].vertex.size(); j++)
			{
				xi = ps[i].vertex[j].x;
				yi = ps[i].vertex[j].y;

				double xpin = xi * cs - yi * ss + center.x;
				yi = yi * cs + xi * ss + center.y;
				xi = xpin;

				lb.x = __min(lb.x, xi);
				lb.y = __min(lb.y, yi);
				rt.x = __max(rt.x, xi);
				rt.y = __max(rt.y, yi);
			}
		}
	};
};

#endif
