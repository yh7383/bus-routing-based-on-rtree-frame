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
	//std::vector<int, std::string> layer_id_to_name;	           //������ <layer id, layer name>
	std::map<std::string, int> layer_name_to_id;            //<layer name, layer id>

	//std::map<int, std::string>	net_id_to_name;		//<net id, net name>
	//std::map<std::string, int>  net_name_to_id;        //<net name, net id>

	//std::map<std::string, int> instance_name_to_id;   //<instance name, instance int>

	//std::map<std::string, int> component_name_to_id;  //<component name, component int>
	//std::map<std::string, int> image_name_to_id;	  //<imgae name, image id>

	// Objects
	Components comps;	//��������
	Nets nets;	//����net
	Padstacks pads;	//����padstack
	Images images;	//����image
	_wiring wiring;
	//std::vector<_netclass> netclasses;


	// For differential pair lookup
	std::map<std::string, std::pair<int, int> > name_to_diff_pair_net_map;  // <net name, <netId1, netId2>>

	int nLayers;

	double clarance;	//Ĭ�ϼ�϶
	double wire_width;	//Ĭ���߿�
	double pinRadius;	//Ĭ��pin�뾶

	double GetPadstackRadius(node_yes& pad);
	double GetPadstackRadius(string strPad);	//��ȡpadstack�İ뾶
	double GetPinRadius(int compID = -1 /*-1��ʾPCB���ϵ���Ϣ*/);	//��ȡ����compID��Ĭ��pin�뾶
	double GetWireWidth(int compID = -1/*-1��ʾPCB���ϵ���Ϣ*/);	//��ȡ����compID��Ĭ���߿�
	double GetDegree(int compID = -1/*-1��ʾPCB���ϵ���Ϣ*/);	//��ȡ����compID����ת�Ƕ�
	double GetClarance(int compID = -1/*-1��ʾPCB���ϵ���Ϣ*/);		//��ȡ����compID��Ĭ�� ��϶
	_vertex GetCenter(int compID = -1/*-1��ʾPCB���ϵ���Ϣ*/);		//��ȡ����compID�� ��������

	_vertex GetLBBound(int compID);		//��ȡ����compID��Ĭ�� ���½�����(ȫ������)
	_vertex GetRTBound(int compID);		//��ȡ����compID��Ĭ�� ���Ͻ�����(ȫ������)


	Tree T;

	void put_wiring_to_pcb(_wiring& mywire);
	void write_new(string path) { T.write_new(path); };
	void ClearWirings();
	int GetCompIndex(string str);

public:


	// ��ȡ������Ĭ���߿�
	double GetBrdWireWidth();
	// ��ȡ�����ϵ�Ĭ�ϼ�϶
	double GetBrdClarance();
	// ��ȡPCB�ı߽���Ϣ
	void GetPcbBoundingRect(_vertex& outline_lb, _vertex& outline_rt);
};

#endif
