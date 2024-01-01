#include<iostream>
#include<string>
#include<vector>
#ifndef DATA_H
#define DATA_H
using namespace std;
const int ERROR = -1;
const int EXCHANGELAYER = -2;
const int TOWARDSLEFT = 0;
const int TOWARDSRIGHT = 1;
const int UPWARD = 2;
const int DOWNWARD = 3;
const int OUTCOMPONENT = 5;
const int HORIZONAL = 6;
const int VERTICAL = 7;
const double PENALTY_PARA = 9000.0;
const int clearance_rate = 1;
const double EQUAL_JUDGE = 0.01;
const string PADSTACK_ID_1 = "'VIA8-BGA'";
const double MIN_BN_DIS = 40.0;
const double MIN_SPACING = 7.0;
const double AngleLength = 240.0;
const double FlatLength = 100.0;
const double MinAngleLength = 80.0;
const double Alpha = 2.0;

struct vertex {
    double x;
    double y;
};
struct pcb {
    vertex left_bottom_vertex;
    vertex right_top_vertex;
};
struct via {
    vector<string> padstack_id;
    string via_type;
    string net_id;
    vertex via_location;
};
struct layer {
    string layer_id;
    double min_clearance;
    double wire_width;
};
struct wire {
    string net_id;
    string layer_id;
    vector<vertex> path;
    double wire_width;
};
struct cell {
    vertex left_bottom_vertex;
    vertex right_top_vertex;
    int row_mark;
    int col_mark;
    double congestion_rate;
    bool hasSet = false;
    bool inOpenset = false;
    bool inCloseSet = false;
    double total_cost;
    int grade;
    cell* parent = NULL;
    double distance_boundary[4];
    bool neighbor_select = false;
    int cell_path_index;
};
struct component {
    int component_id;
    vertex left_bottom_vertex;
    vertex right_top_vertex;
};
struct pin {
    vertex location_coordinates;
    int component_id;
    int location;
    string net_id;
    int cluster_index = -1;
};
struct directional_point {
    vertex location_coordinates;
    int back_direction;
};
struct net {
    string net_id;
    vector<pin> pin_vector;
    vector<wire> wire_vector;
    int layer_num = 1;
    bool is_clustered = false;
    string layer_id; //后加的
    directional_point now_position;
};
struct bending_node {
    vertex bending_node_coordinates;
    double width;
    double height;
    string layer_id;
    string bus_id;
    bool isStartBendingNode = false;
    bool isEndBendingNode = false;
    int component_id;
    int location = OUTCOMPONENT;
    bool enable = true;
};
struct routing_area {
    vertex left_bottom_vertex;
    vertex right_top_vertex;
    int routing_orientation;
    string layer_id;
};
struct exchange_layer {
    bool start_access = true;
    bool end_access = true;
    bool original_start_access = true;
    bool original_end_access = true;
    bool exchange_start_access = true;
    bool exchange_end_access = true;
    double start_original_dis_boun[4];
    double start_exchange_dis_boun[4];
    double end_original_dis_boun[4];
    double end_exchange_dis_boun[4];
};
struct bus {
    string bus_id;
    string layer_id;
    vector<net> net_vector;
    vector<routing_area> routing_area_vector;
    vector<bending_node> bending_node_vector;
    vector<pin> start_pin_vector;
    vector<pin> end_pin_vector;
    bool isOppositeDir;
    string routing_layer_id;
    bool ex_layer = false;
    exchange_layer exchange_lay_ins;
    bool start_adjust = false;
    bool end_adjust = false;
    bool hasRouted = false;
    bool min_index_min_location;
};
struct door_segment {
    vertex start_vertex;
    vertex end_vertex;
    int direction;
    int obstacle_index;
    bool is_obstacle_seg = false;
    bool is_edge_seg = false;
    bool is_start_obstacle = true;
    bool is_door_seg = true;
};
struct room {
    door_segment left_edge;
    door_segment right_edge;
    door_segment top_edge;
    door_segment bottom_edge;
    bool left_seg_set = false;
    bool right_seg_set = false;
    bool top_seg_set = false;
    bool bottom_seg_set = false;
};
struct edge_obatacle {
    vector<pair<vertex, vertex>> left_edge_vector;
    vector<pair<vertex, vertex>> right_edge_vector;
    vector<pair<vertex, vertex>> top_edge_vector;
    vector<pair<vertex, vertex>> bottom_edge_vector;
};
struct obstacle {
    vertex left_bottom_vertex;
    vertex right_top_vertex;
    bool isComponent = false;
    bool left_boundary = false;
    bool right_boundary = false;
    bool top_boundary = false;
    bool bottom_boundary = false;
    edge_obatacle edge_seg;
};
struct room_path {
    int room_index;
    int exit_location;
};
struct coarse_boundary_path {
    vector<vector<room_path>> to_top_path;
    vector<vector<room_path>> to_bottom_path;
    vector<vector<room_path>> to_left_path;
    vector<vector<room_path>> to_right_path;
};
struct search_tree              //搜索room的tree结构体
{
    int room_index;
    vector<pair<int, door_segment>> edge_vector;    //网格边界，保存边界段的位置和door_segment
    vector<pair<pair<int, door_segment>, search_tree>> door_vector;     //room中door的段，保存door的segment数据及位置，以及通向下个room的搜索树
    bool hasSearched = false;
    bool isEndRoom = false;
};
struct coarse_end_index {
    int start_row_index;
    int end_row_index;
    int start_col_index;
    int end_col_index;
};
struct routing_info {
    layer layer_instance;
    bending_node start_bending_node;
    bending_node end_bending_node;
    exchange_layer exchange_lay_ins;
};
struct coarse_cell {
    vertex left_bottom_vertex;
    vertex right_top_vertex;
    int row_index;
    int col_index;
    int grade;
    double total_cost;
    coarse_cell* parent;
    int entrance_location;
    bool inOpenSet = false;
    bool inCloseSet = false;
    bool top_access = false;
    bool bottom_access = false;
    bool left_access = false;
    bool right_access = false;
    bool has_set_top = false;
    bool has_set_bottom = false;
    bool has_set_left = false;
    bool has_set_right = false;
    bool isCommonCell = true;
    vector<bending_node> to_top_bn_vector;
    vector<bending_node> to_bottom_bn_vector;
    vector<bending_node> to_left_bn_vector;
    vector<bending_node> to_right_bn_vector;
    bool hasSetObstacle = false;
    vector<obstacle> obstacle_vector;
    edge_obatacle obstacle_seg;
    edge_obatacle access_seg;
    vector<door_segment> door_seg_vector;
    bending_node start_bending_node;
    vector<room> room_vector;
    bool isEndCoarse = false;
    vector<int> end_bn_room_index;
    vector<vector<room_path>> to_end_bn_path;
    vector<bending_node> to_end_bn_vector;
    string layer_id;
    bool isStartCoarse = false;
    bool hasCalculateBounder = false;
};
struct common_cell {
    vertex left_bottom_vertex;
    vertex right_top_vertex;
    common_cell* parent_cell;
    bool isStartCell = false;
    bool isEndCell = false;
};
struct dr_info {
    double bus_width;
    double wire_width;
    double min_clearance;
};

#endif

