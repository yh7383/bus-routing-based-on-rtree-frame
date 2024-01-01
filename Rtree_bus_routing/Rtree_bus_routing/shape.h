#ifndef SHAPE
#define SHAPE
#include "Item.h"

struct _shape {
	string type;
	string layer_id;
	vector<double> data;

	void GetBoundingRect(_vertex& lb, _vertex& rt, _vertex& center, double degree) {

		if (type == "polygon")
		{
			double xi, yi;
			double ss = sin(degree * 3.1415926 / 180);
			double cs = cos(degree * 3.1415926 / 180);

			lb.x = rt.x = data[1] * cs - data[2] * ss + center.x;
			lb.y = rt.y = data[2] * cs + data[1] * ss + center.y;

			for (int i = 3; i < (int)data.size(); i += 2)
			{
				xi = data[i] * cs - data[i + 1] * ss + center.x;
				yi = data[i + 1] * cs + data[i] * ss + center.y;

				lb.x = __min(lb.x, xi);
				lb.y = __min(lb.y, yi);
				rt.x = __max(rt.x, xi);
				rt.y = __max(rt.y, yi);
			}
		}
	};

	double GetRadius()
	{
		if (type == "rect")		return abs(data[0]);
		else if (type == "circle")	return data[0] / 2;
		else return 0;
	}
};

#endif
