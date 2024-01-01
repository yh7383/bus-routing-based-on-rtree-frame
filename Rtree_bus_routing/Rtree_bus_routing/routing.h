#include<map>
#include<vector>
#include "data.h"
#include "dsnPcbDataBase.h"
#include "AverageGrid.h"
#include "RTree.h"

typedef int ValueType;

//定义线段范围
struct Line {

    Line() {};

    Line(double a_minX, double a_minY, double a_maxX, double a_maxY)
    {
        min[0] = a_minX;
        min[1] = a_minY;

        max[0] = a_maxX;
        max[1] = a_maxY;
    }

    //水平的边，坐标顺序为左到右，垂直的边，坐标顺序为上到下
    double min[2];
    double max[2];

    bool takeUP;//是否被占用
};

//定义矩形范围 
struct Rect
{
    Rect() {}

    /*min[0]=left_bottom_vertex.x
    min[1]=left_bottom_vertex.y
    max[0]=right_top_vertex.x
    max[1]=right_top_vertex.y*/
    Rect(double a_minX, double a_minY, double a_maxX, double a_maxY)
    {
        left_bottom[0] = a_minX;
        left_bottom[1] = a_minY;

        right_top[0] = a_maxX;
        right_top[1] = a_maxY;
    }

    double left_bottom[2];
    double right_top[2];

    //不可使用的线段
    vector<Line> up_lines;//上边界的线段
    vector<Line> down_lines;//下边界的线段
    vector<Line> right_lines;//右边界的线段
    vector<Line> left_lines;//左边界的线段
};

struct layer_net
{
    string layer_id;
    vector<net> net_vector;
    layer_net(string temp_layer_id, net temp_net) 
    {
        layer_id = temp_layer_id;
        net_vector.push_back(temp_net);
    }
};

struct net_routing_process
{
    string net_id;
    vertex start_point;
    vertex now_point;
    vertex end_point;
    vector<Line> compplete_lines;
    net_routing_process(net temp_net)
    {
        net_id = temp_net.net_id;
        start_point = temp_net.pin_vector[0].location_coordinates;
        now_point = temp_net.pin_vector[0].location_coordinates;
        end_point = temp_net.pin_vector[1].location_coordinates;
    }
};

class Routing {
public:
    //pcb information
    pcb pcb_instance;
    double pcb_width;
    double pcb_height;
    double wire_gap;//wire间距
    int rtree_num;
    dsnPcbDataBase dsn_instance;

    //RTree 
    typedef RTree<ValueType, double, 2, float> MyTree;//定义Rtree 
    MyTree rtree;
    vector<ValueType> overlap_num;

    //my information
    vector<layer_net> layer_net_vector;//层-线网
    vector<net> net_vector;//存储网线信息
    vector<component> component_vector;//存储元器件信息
    vector<obstacle> obstacle_vector;//存储障碍物信息


public:
    //initiate pcb
    void init_pcb(string path);
    void init_obstacle_vector();
    void init_net_vector(int layernum);
    void init_layer_net_vector();
    static double check_single_congestion(vertex obstacle_left_bottom_vertex, vertex obstacle_right_top_vertex, vertex left_bottom_vertex, vertex right_top_vertex, double temp_distance_boundary[]);
    static double check_cell_congestion(vertex left_bottom_vertex, vertex right_top_vertex, string layer_id, double distance_boundary[], vector<component>& component_vector, vector<bus>& bus_vector, vector<bending_node>& start_end_bn_vector, string bus_id, map<string, int>& bus_id_index_map);
    
    //RTree operation
    void init_RTree();

    //RTree routing
    int get_point_back_direction(vertex point);
    void adjust_point(vertex &point1, vertex &point2);
    double standardize_double(double number);
    void routing_singal_layer(int layer_num);
    void Rtree_bus_routing();
    Rect comfirm_rect(directional_point point);

    //display_information
    void display_net();
    void display_component();
    void rec_link();

};


