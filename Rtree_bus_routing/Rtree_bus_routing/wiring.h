#ifndef WIRING
#define WIRING
#include "Item.h"
#include "path.h"
#include "Tree.h"

struct _via {
	vector<string> padstack_id;
	_vertex vertex;
	string net_id;
	string clearance_class;
};
struct _wire {
	_path polyline_path;
	string net_id;
	string clearance_class;
	string type;
};
struct _wiring {
	vector<_wire> wire;
	vector<_via> via;
};


#endif
