#pragma once
#ifndef PCB_DATABASE_H
#define PCB_DATABASE_H


#include <string>
#include <assert.h>
#include <math.h>
#include "tree.h"
#include "components.h"
#include "image.h"
#include "net.h"
#include "outline.h"
#include "padstack.h"
#include "path.h"
#include "pin.h"
#include "place.h"
#include "wiring.h"

class dsnPcbDataBase
{
public:
	dsnPcbDataBase();
	dsnPcbDataBase(std::string fileName);
	~dsnPcbDataBase();

	bool Init();
	void set_path(string path);

public:
	string m_fileName;

	// Index map
	std::vector<string> strLayerName;
	//std::vector<int, std::string> layer_id_to_name;	           //层名称 <layer id, layer name>
	std::map<std::string, int> layer_name_to_id;            //<layer name, layer id>

	//std::map<int, std::string>	net_id_to_name;		//<net id, net name>
	//std::map<std::string, int>  net_name_to_id;        //<net name, net id>

	//std::map<std::string, int> instance_name_to_id;   //<instance name, instance int>

	//std::map<std::string, int> component_name_to_id;  //<component name, component int>
	//std::map<std::string, int> image_name_to_id;	  //<imgae name, image id>

	// Objects
	Components comps;	//所有器件
	Nets nets;	//所有net
	Padstacks pads;	//所有padstack
	Images images;	//所有image
	_wiring wiring;
	//std::vector<_netclass> netclasses;


	// For differential pair lookup
	std::map<std::string, std::pair<int, int> > name_to_diff_pair_net_map;  // <net name, <netId1, netId2>>

	int nLayers;

	double clarance;	//默认间隙
	double wire_width;	//默认线宽
	double pinRadius;	//默认pin半径

	double GetPadstackRadius(node_yes& pad);
	double GetPadstackRadius(string strPad);	//获取padstack的半径
	double GetPinRadius(int compID = -1 /*-1表示PCB板上的信息*/);	//获取器件compID的默认pin半径
	double GetWireWidth(int compID = -1/*-1表示PCB板上的信息*/);	//获取器件compID的默认线宽
	double GetDegree(int compID = -1/*-1表示PCB板上的信息*/);	//获取器件compID的旋转角度
	double GetClarance(int compID = -1/*-1表示PCB板上的信息*/);		//获取器件compID的默认 间隙
	_vertex GetCenter(int compID = -1/*-1表示PCB板上的信息*/);		//获取器件compID的 中心坐标

	_vertex GetLBBound(int compID);		//获取器件compID的默认 左下角坐标(全局坐标)
	_vertex GetRTBound(int compID);		//获取器件compID的默认 右上角坐标(全局坐标)


	Tree T;

	void put_wiring_to_pcb(_wiring& mywire);
	void write_new(string path) { T.write_new(path); };
	void ClearWirings();
	int GetCompIndex(string str);

public:


	// 获取板子上默认线宽
	double GetBrdWireWidth();
	// 获取板子上的默认间隙
	double GetBrdClarance();
	// 获取PCB的边界信息
	void GetPcbBoundingRect(_vertex& outline_lb, _vertex& outline_rt);
};

#endif
