#include "dsnPcbDataBase.h"
//#include "glog/logging.h"

dsnPcbDataBase::dsnPcbDataBase()
{

}

dsnPcbDataBase::dsnPcbDataBase(std::string fileName) : m_fileName(fileName)
{
	T.Open(fileName);
	std::cerr << "Build  Pcb database..." << std::endl;
	if (!Init())
	{
		std::cerr << "ERROR: Building  Pcb database failed." << std::endl;
		assert(false);
	}
}

void dsnPcbDataBase::set_path(string fileName) {
	m_fileName = fileName;
	T.Open(fileName);
	std::cerr << "Build  Pcb database..." << std::endl;
	if (!Init())
	{
		std::cerr << "ERROR: Building  Pcb database failed." << std::endl;
		assert(false);
	}
}

dsnPcbDataBase::~dsnPcbDataBase()
{
}

double dsnPcbDataBase::GetPadstackRadius(node_yes& pad)
{
	double r = 0;
	//LOG(INFO) << "pad.child[shape].size() = " << pad.child["shape"].size() << endl;
	//google::FlushLogFiles(google::INFO);
	for (int i = 0; i < (int)pad.child["shape"].size(); i++)
	{
		if (pad.child["shape"][i].child["circle"].size() > 0)
			r = __max(r, str_2_double(pad.child["shape"][i].child["circle"][0].data[1]) / 2);
		if (pad.child["shape"][i].child["rect"].size() > 0)
			r = __max(r, abs(str_2_double(pad.child["shape"][i].child["rect"][0].data[1])));
		if (pad.child["shape"][i].child["polygon"].size() > 0)
			r = __max(r, r);
	}
	return r;
}

bool dsnPcbDataBase::Init()
{
	T.get_pcb_yes();
	clarance = str_2_double(T.find("clearance").data[0]);
	wire_width = str_2_double(T.find("width").data[0]);

	//读取padstack和image信息
 	node_yes lib = T.find("library");
	pads.set_padstacks(lib.child["padstack"]);
	/*T.pcb_yes = T.find("PCB");*/
	//cout << "padstack size:" << T.pcb_yes.child["PCB"][0].child["library"][0].child["padstack"].size() << " " << endl;
	cout << "padstack.size = " << pads.ps.size() << endl;
	//LOG(INFO) << "padstack.size = " << pads.ps.size() << endl;
	images.set_images(T.find_all("image", lib));
	cout << "image.size = " << images.is.size() << endl;
	//LOG(INFO) << "image.size = " << images.is.size() << endl;

	node_yes node;
	//读取net信息
	nets.set_nets(T.find_all("net", node));
	//info.netsFind.resize(info.nets.size(), false);
	cout << "nets.size = " << nets.ns.size() << endl;
	//LOG(INFO) << "nets.size = " << nets.ns.size() << endl;
	//google::FlushLogFiles(google::INFO);
	//读取层信息
	vector<node_yes> _layer = T.find_all("layer", node);
	for (int i = 0; i < (int)_layer.size(); i++)
	{
		layer_name_to_id.insert(make_pair(_layer[i].data[0], i));
		//layer_id_to_name.insert(make_pair(i,_layer[i].data[0]));
		strLayerName.push_back(_layer[i].data[0]);
	}
	nLayers = __min(nLayers, _layer.size());	//设置为dsn中层数与，实际指定层数的较小值
	cout << "layer.size = " << _layer.size() << endl;
	//LOG(INFO) << "layer.size = " << nLayers << endl;

	//读取器件信息
	comps.set_component(T.find_all("component", node));
	cout << "components.size = " << comps.cs.size() << endl;
	//LOG(INFO) << "components.size = " << comps.cs.size() << endl;

	wiring = T.read_wiring(T.find("wiring"));

	return true;
}

double dsnPcbDataBase::GetPadstackRadius(string strPad)
{
	//if (pads.name_to_id.find(strPad) == pads.name_to_id.end())	return 0;
	int i = pads.name_to_id.at(strPad);
	return pads.ps[i].shapes[0].GetRadius();
}

double dsnPcbDataBase::GetPinRadius(int compID)
{
	return pads.GetRadius(images.get_image(comps.cs[compID].image_id).pins.ps[0].padstack_id);
	return pinRadius;
}

double dsnPcbDataBase::GetWireWidth(int compID)
{
	return wire_width;
}

double dsnPcbDataBase::GetDegree(int compID)
{
	return comps.cs[compID].places.ps[0].rotation;
}

double dsnPcbDataBase::GetClarance(int compID)
{
	return clarance;
}

_vertex dsnPcbDataBase::GetCenter(int compID)
{
	return _vertex(comps.cs[compID].places.ps[0].x, comps.cs[compID].places.ps[0].y);
}

_vertex dsnPcbDataBase::GetLBBound(int compID)
{
	return _vertex();
}

_vertex dsnPcbDataBase::GetRTBound(int compID)
{
	return _vertex();
}

void dsnPcbDataBase::put_wiring_to_pcb(_wiring& mywire)
{
	T.put_wiring_to_pcb(mywire);
}

void dsnPcbDataBase::ClearWirings()
{
	T.ClearWirings();
}

int dsnPcbDataBase::GetCompIndex(string str)
{
	string s = str.substr(0, str.find("-"));
	return 0;
}


// 获取板子上默认线宽
double dsnPcbDataBase::GetBrdWireWidth()
{
	return wire_width * 2;
}


// 获取板子上的默认间隙
double dsnPcbDataBase::GetBrdClarance()
{
	return clarance * 2;
}


// 获取PCB的边界信息
void dsnPcbDataBase::GetPcbBoundingRect(_vertex& lb, _vertex& rt)
{
	node_yes node;
	vector<node_yes> bound = T.find_all("boundary", node);
	if (bound.size() > 0)
	{
		lb.x = lb.y = -100000;
		rt.x = rt.y = 100000;
	}

	for (int i = 0; i < bound.size(); i++)
	{
		if (bound[i].child["path"].size() > 0)
		{
			double x1, y1, x2, y2;
			vector<string>& s = bound[i].child["path"][0].data;
			x1 = x2 = str_2_double(s[2]);
			y1 = y2 = str_2_double(s[3]);
			for (int i = 4; i < s.size(); i += 2)
			{
				double si = str_2_double(s[i]);
				double si2 = str_2_double(s[i + 1]);
				x1 = __min(x1, si);
				x2 = __max(x2, si);
				y1 = __min(y1, si2);
				y2 = __max(y2, si2);
			}
			lb.x = __max(lb.x, x1);
			lb.y = __max(lb.y, y1);
			rt.x = __min(rt.x, x2);
			rt.y = __min(rt.y, y2);
		}
		if (bound[i].child["rect"].size() > 0)
		{
			vector<string>& s = bound[i].child["rect"][0].data;
			lb.x = __max(lb.x, str_2_double(s[1]));
			lb.y = __max(lb.y, str_2_double(s[2]));
			rt.x = __min(rt.x, str_2_double(s[3]));
			rt.y = __min(rt.y, str_2_double(s[4]));
		}
	}
}
