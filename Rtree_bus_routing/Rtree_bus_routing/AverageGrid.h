#ifndef AVERAGEGRID_H
#define AVERAGEGRID_H
#include<map>
#include "data.h"
class AverageGrid {
public:
	AverageGrid(pcb pcb_instance, bending_node start_bending_node, string layer_id, vector<bending_node>& start_end_bn_vector, vector<component> &component_vector, map<string, int> &bus_id_index_map);
	void construct_cell(bending_node& start_bending_node, bending_node& end_bending_node, vector<bus>& bus_vector);
	void construct_single_cell(int row_index, int col_index, vector<bus>& bus_vector);

public:
	int left_num;
	int right_num;
	int top_num;
	int bottom_num;
	int row_num;
	int col_num;
	vertex start_vertex;
	vertex end_vertex;
	cell** cell_array;
	int end_row_index;
	int end_col_index;
	string layer_id;
	vector<bending_node> start_end_bn_vector;
	string bus_id;
	vector<component> component_vector;
	map<string, int> bus_id_index_map;
	double cell_width;
};

#endif

