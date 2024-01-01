#include "routing.h"
#include<algorithm>
using namespace std;
const int RoutingNum = 26;

//

//call back function
bool RtreeSearchCallback(ValueType num)
{
    //overlap_num.push_back(num);//记录重叠矩形id
    return true; // keep going
};

//initiate pcb

void Routing::init_pcb(string path) {

    dsn_instance.set_path(path);
    _vertex left_bottom_vertex, right_top_vertex;
    dsn_instance.GetPcbBoundingRect(left_bottom_vertex, right_top_vertex);
    cout << left_bottom_vertex.x << " " << left_bottom_vertex.y << "  " << right_top_vertex.x << " " << right_top_vertex.y << endl;
    pcb_instance = { {left_bottom_vertex.x, left_bottom_vertex.y}, {right_top_vertex.x, right_top_vertex.y} };
    pcb_width = right_top_vertex.x - left_bottom_vertex.x;
    pcb_height = right_top_vertex.y - left_bottom_vertex.y;
    vector<_rect> comp_rect_vector = dsn_instance.T.find_component();
    for (_component component_instance : dsn_instance.comps.cs) {
        for (_place place_instance : component_instance.places.ps) {
            component ct;
            ct.component_id = dsn_instance.comps.name_to_id[place_instance.place_id];
            ct.left_bottom_vertex = { comp_rect_vector[ct.component_id].left_bottom_vertex.x, comp_rect_vector[ct.component_id].left_bottom_vertex.y };
            ct.right_top_vertex = { comp_rect_vector[ct.component_id].right_top_vertex.x, comp_rect_vector[ct.component_id].right_top_vertex.y };
            component_vector.push_back(ct);
            cout << ct.component_id << " " << ct.left_bottom_vertex.x << " " << ct.left_bottom_vertex.y << "  " << ct.right_top_vertex.x << " " << ct.right_top_vertex.y << endl;
        }
    }
    vector<_bus> net_pin_vector = dsn_instance.T.get_couple_pin({ 0.0, 0.0 }, comp_rect_vector, dsn_instance.wiring);
    for (unsigned int i = 0; i < net_pin_vector.size(); i++) {
        vector<pin> pin_vector;
        pin_vector.push_back({ {net_pin_vector[i].vertex[0].x, net_pin_vector[i].vertex[0].y}, net_pin_vector[i].vertex[0].obstacle_index, net_pin_vector[i].vertex[0].location, net_pin_vector[i].net_id });
        pin_vector.push_back({ {net_pin_vector[i].vertex[1].x, net_pin_vector[i].vertex[1].y}, net_pin_vector[i].vertex[1].obstacle_index, net_pin_vector[i].vertex[1].location, net_pin_vector[i].net_id });
        vector<net> net_vector;
        net_vector.push_back({ net_pin_vector[i].net_id, pin_vector });
    }
    for (unsigned int i = 0; i < net_pin_vector.size(); i++) {
        vector<pin> pin_vector;
        pin_vector.push_back({ {net_pin_vector[i].vertex[0].x, net_pin_vector[i].vertex[0].y}, net_pin_vector[i].vertex[0].obstacle_index, net_pin_vector[i].vertex[0].location, net_pin_vector[i].net_id });
        pin_vector.push_back({ {net_pin_vector[i].vertex[1].x, net_pin_vector[i].vertex[1].y}, net_pin_vector[i].vertex[1].obstacle_index, net_pin_vector[i].vertex[1].location, net_pin_vector[i].net_id });
        net n({ net_pin_vector[i].net_id, pin_vector });
        n.layer_id = net_pin_vector[i].layer_id;
        net_vector.push_back(n);
    }
    wire_gap = dsn_instance.clarance;
}

double Routing::check_single_congestion(vertex obstacle_left_bottom_vertex, vertex obstacle_right_top_vertex, vertex left_bottom_vertex, vertex right_top_vertex, double temp_distance_boundary[]) {
    double cell_width = right_top_vertex.x - left_bottom_vertex.x;
    double cell_height = right_top_vertex.y - left_bottom_vertex.y;
    double congestion_rate = 0.0;
    double min_x = min(obstacle_left_bottom_vertex.x, left_bottom_vertex.x);
    double max_x = max(obstacle_right_top_vertex.x, right_top_vertex.x);
    double min_y = min(obstacle_left_bottom_vertex.y, left_bottom_vertex.y);
    double max_y = max(obstacle_right_top_vertex.y, right_top_vertex.y);
    double cross_width = cell_width + (obstacle_right_top_vertex.x - obstacle_left_bottom_vertex.x) - (max_x - min_x);
    double cross_height = cell_width + (obstacle_right_top_vertex.y - obstacle_left_bottom_vertex.y) - (max_y - min_y);
    if (cross_width > 0.0 && cross_height > 0.0) {
        congestion_rate = cross_width * cross_height / (cell_width * cell_height);
    }
    temp_distance_boundary[0] = 0.0;
    temp_distance_boundary[1] = 0.0;
    temp_distance_boundary[2] = 0.0;
    temp_distance_boundary[3] = 0.0;
    if (abs(congestion_rate) < 0.001) {
        temp_distance_boundary[0] = cell_height;
        temp_distance_boundary[1] = cell_width;
        temp_distance_boundary[2] = cell_height;
        temp_distance_boundary[3] = cell_width;
    }
    else if (abs(congestion_rate - 1.0) < 0.001) {
        temp_distance_boundary[0] = 0.0;
        temp_distance_boundary[1] = 0.0;
        temp_distance_boundary[2] = 0.0;
        temp_distance_boundary[3] = 0.0;
    }
    else {
        if (obstacle_left_bottom_vertex.x > left_bottom_vertex.x && obstacle_left_bottom_vertex.x < right_top_vertex.x) {
            temp_distance_boundary[3] = obstacle_left_bottom_vertex.x - left_bottom_vertex.x;
        }
        if (obstacle_right_top_vertex.x > left_bottom_vertex.x && obstacle_right_top_vertex.x < right_top_vertex.x) {
            temp_distance_boundary[1] = right_top_vertex.x - obstacle_right_top_vertex.x;
        }
        if (obstacle_left_bottom_vertex.y > left_bottom_vertex.y && obstacle_left_bottom_vertex.y < right_top_vertex.y) {
            temp_distance_boundary[0] = obstacle_left_bottom_vertex.y - left_bottom_vertex.y;
        }
        if (obstacle_right_top_vertex.y > left_bottom_vertex.y && obstacle_right_top_vertex.y < right_top_vertex.y) {
            temp_distance_boundary[2] = right_top_vertex.y - obstacle_right_top_vertex.y;
        }
    }
    return congestion_rate;
}

double Routing::check_cell_congestion(vertex left_bottom_vertex, vertex right_top_vertex, string layer_id, double distance_boundary[], vector<component>& component_vector, vector<bus>& bus_vector, vector<bending_node> &start_end_bn_vector, string bus_id, map<string, int>& bus_id_index_map) {
    double total_congestion_rate = 0.0;
    double temp_distance_boundary[4];
    bool fit = false;
    /*if (layer_id == "ART05" && judge_equal(left_bottom_vertex.x, 3961.71) && judge_equal(left_bottom_vertex.y, 4775.56)) {
        fit = true;
    }*/
    for (int i = 0; i < 4; i++) {
        if(i == 1 || i == 3)
            distance_boundary[i] = right_top_vertex.x - left_bottom_vertex.x;
        else
            distance_boundary[i] = right_top_vertex.y - left_bottom_vertex.y;
    }
    for (unsigned int i = 0; i < component_vector.size(); i++) {
        //double temp_rate = total_congestion_rate;
        total_congestion_rate += check_single_congestion(component_vector[i].left_bottom_vertex, component_vector[i].right_top_vertex, left_bottom_vertex, right_top_vertex, temp_distance_boundary);
        /*if (fit && total_congestion_rate - temp_rate > EQUAL_JUDGE)
            cout << "component: " << i << endl;*/
        for (int m = 0; m < 4; m++) {
            if (temp_distance_boundary[m] < distance_boundary[m]) {
                distance_boundary[m] = temp_distance_boundary[m];
            }
        }
    }
    for (bus bus_instance : bus_vector) {
        int i = 0;
        for (routing_area routing_area_instance : bus_instance.routing_area_vector) {
            if (routing_area_instance.layer_id == layer_id) {
                //double temp_rate = total_congestion_rate;
                total_congestion_rate += check_single_congestion(routing_area_instance.left_bottom_vertex, routing_area_instance.right_top_vertex, left_bottom_vertex, right_top_vertex, temp_distance_boundary);
                /*if (fit && total_congestion_rate - temp_rate > EQUAL_JUDGE)
                    cout << bus_instance.bus_id << " routing area : " << i << endl;*/
                for (int m = 0; m < 4; m++) {
                    if (temp_distance_boundary[m] < distance_boundary[m]) {
                        distance_boundary[m] = temp_distance_boundary[m];
                    }
                }
            }
            i++;
        }
    }
    for (bending_node temp_bn : start_end_bn_vector) {
        if (temp_bn.layer_id == layer_id && temp_bn.enable && bus_id_index_map[temp_bn.bus_id] > bus_id_index_map[bus_id]) {// temp_bn.bus_id != bus_id
            //double temp_rate = total_congestion_rate;
            total_congestion_rate += check_single_congestion({ temp_bn.bending_node_coordinates.x - temp_bn.width / 2.0, temp_bn.bending_node_coordinates.y - temp_bn.height / 2.0 }, { temp_bn.bending_node_coordinates.x + temp_bn.width / 2.0, temp_bn.bending_node_coordinates.y + temp_bn.height / 2.0 }, left_bottom_vertex, right_top_vertex, temp_distance_boundary);
            /*if (fit && total_congestion_rate - temp_rate > EQUAL_JUDGE)
                cout << temp_bn.bus_id << " is start bn: " << temp_bn.isStartBendingNode << endl;*/
            for (int m = 0; m < 4; m++) {
                if (temp_distance_boundary[m] < distance_boundary[m]) {
                    distance_boundary[m] = temp_distance_boundary[m];
                }
            }
        }
    }
    
    return total_congestion_rate;
}

void Routing::init_net_vector(int layernum)//只考虑两个pin的情况
{
    for (int i = 0; i < layer_net_vector[layernum].net_vector.size(); ++i) {
        if (layer_net_vector[layernum].net_vector[i].pin_vector[0].component_id > layer_net_vector[layernum].net_vector[i].pin_vector[1].component_id) {//component_id小的在成对pin的前面
            swap(layer_net_vector[layernum].net_vector[i].pin_vector[0], layer_net_vector[layernum].net_vector[i].pin_vector[1]);
        }
        layer_net_vector[layernum].net_vector[i].now_position.location_coordinates = layer_net_vector[layernum].net_vector[i].pin_vector[0].location_coordinates;
        layer_net_vector[layernum].net_vector[i].now_position.back_direction = get_point_back_direction(layer_net_vector[layernum].net_vector[i].now_position.location_coordinates);
    }
}

void Routing::init_obstacle_vector()
{

}

void Routing::init_layer_net_vector()
{
    for (int i = 0; i < net_vector.size(); ++i) {
        bool flag = true;
        for (int j = 0; j < layer_net_vector.size(); ++j) {
            if (layer_net_vector[j].layer_id == net_vector[i].layer_id) {
                flag = false;
                layer_net_vector[j].net_vector.push_back(net_vector[i]);
            }
        }
        if (flag) {
            layer_net temp_layer_net(net_vector[i].layer_id, net_vector[i]);
            layer_net_vector.push_back(temp_layer_net);
        }
    }
}

//RTree operation============================================================================

void Routing::init_RTree()
{
    rtree.RemoveAll();
    rtree_num = 0;
    for (int i = 0; i < component_vector.size(); ++i, ++rtree_num) {
        Rect temp_rect(component_vector[i].left_bottom_vertex.x,
                       component_vector[i].left_bottom_vertex.y,
                       component_vector[i].right_top_vertex.x,
                       component_vector[i].right_top_vertex.y);
        rtree.Insert(temp_rect.left_bottom, temp_rect.right_top, i);
    }
}

//bus routing===============================================================================

//使double number等于wire_gap的整数倍
double Routing::standardize_double(double number)
{
    return (int(number / wire_gap)) * wire_gap;
}

//调整两个点，以满足矩阵两个点的要求
void Routing::adjust_point(vertex &point1, vertex &point2)
{
    if (point1.x > point2.x) swap(point1, point2);
    else if (point1.x == point2.x && point1.y > point2.y) swap(point1, point2);
}

//获得某个点背靠的方向
int Routing::get_point_back_direction(vertex point)
{
    int x[4] = { -1,0,1,0 };
    int y[4] = { 0,-1,0,1 };
    for (int i = 0; i < 4; ++i) {//0：左边。1：下边。2：右边。3：上边
        double min[2], max[2];
        min[0] = max[0] = point.x + x[i] * 0.001;
        min[1] = max[1] = point.y + y[i] * 0.001;
        if (rtree.Search(min, max, RtreeSearchCallback) == 0) return (i + 2) % 4;
    }
    return -1;
}


Rect Routing::comfirm_rect(directional_point point)
{
    directional_point new_point = point;
    //确定向左、向下、向右、向上各可延伸多少
    double extent[4];
    int type = point.back_direction;
    int x[4] = { -1,0,1,0 };
    int y[4] = { 0,-1,0,1 };
    //有限扩展紧贴边反方向形成的矩形
    extent[type] = 0;
    for (int j = 1; j < 4; ++j) {//0：左走，1：下走，2：右走，3：上走
        int i;//i为正确试探方向
        if (j == 1) i = (type + 2) % 4;//背靠的反方向
        else if (j == 2) i = (type + 1) % 4;//背靠逆时针第一个方向
        else i = (type + 3) % 4;//背靠顺时针第一个方向

        double min_lenth = wire_gap, max_lenth = standardize_double(max(pcb_height, pcb_width));
        while (min_lenth < max_lenth || min_lenth < wire_gap) {
            double mid_lenth = standardize_double((min_lenth + max_lenth) / 2);
            vertex point1_coordinates, point2_coordinates;
            point1_coordinates = new_point.location_coordinates;
            point2_coordinates.x = point.location_coordinates.x + x[i] * mid_lenth;
            point2_coordinates.y = point.location_coordinates.y + y[i] * mid_lenth;
            adjust_point(point1_coordinates, point2_coordinates);
            double min[2], max[2];
            min[0] = point1_coordinates.x, min[1] = point1_coordinates.y;
            max[0] = point2_coordinates.x, max[1] = point2_coordinates.y;
            int flag = rtree.Search(min, max, RtreeSearchCallback);
            if (flag == 1) min_lenth = mid_lenth + wire_gap;
            else max_lenth = mid_lenth - wire_gap;
        }

        if (i == 0) extent[i] = min(min_lenth, standardize_double(point.location_coordinates.x - pcb_instance.left_bottom_vertex.x));
        else if (i == 1) extent[i] = min(min_lenth, standardize_double(point.location_coordinates.y - pcb_instance.left_bottom_vertex.y));
        else if (i == 2) extent[i] = min(min_lenth, standardize_double(pcb_instance.right_top_vertex.x - point.location_coordinates.x));
        else extent[i] = min(min_lenth, standardize_double(pcb_instance.right_top_vertex.y - point.location_coordinates.y));
        if (j == 1) {//更新新点
            if (i == 0) new_point.location_coordinates.x -= extent[i];
            else if (i == 1) new_point.location_coordinates.y -= extent[i];
            else if (i == 2) new_point.location_coordinates.x += extent[i];
            else new_point.location_coordinates.y += extent[i];
        }
    }

    return Rect(point.location_coordinates.x - extent[0], point.location_coordinates.y - extent[1], point.location_coordinates.x + extent[2], point.location_coordinates.y + extent[3]);
}

void Routing::routing_singal_layer(int layer_num)
{
    int target_sum = layer_net_vector.size(), over_sum = 0, circle = 0;
    while (over_sum < target_sum) {
        for (int i = 0; i < layer_net_vector[layer_num].net_vector.size(); ++i) {
            Rect temp_rect = comfirm_rect(layer_net_vector[layer_num].net_vector[i].now_position);

        }
        break;
    }
}

void Routing::Rtree_bus_routing()
{
    init_layer_net_vector();
    for (int i = 0; i < layer_net_vector.size(); ++i) {
        init_RTree();
        init_net_vector(i);
        routing_singal_layer(i);

        break;
    }
}

//test and check information====================================================================

void Routing::rec_link()
{
    init_layer_net_vector();
    for (int i = 0; i < layer_net_vector[0].net_vector.size(); ++i) {
        wire temp_wire1,temp_wire2;
        temp_wire1.layer_id = layer_net_vector[0].net_vector[i].layer_id;
        temp_wire1.net_id = layer_net_vector[0].net_vector[i].net_id;
        temp_wire1.path.push_back(layer_net_vector[0].net_vector[i].pin_vector[0].location_coordinates);
        temp_wire1.path.push_back({ layer_net_vector[0].net_vector[i].pin_vector[0].location_coordinates.x,layer_net_vector[0].net_vector[i].pin_vector[1].location_coordinates.y });
        temp_wire2.layer_id = layer_net_vector[0].net_vector[i].layer_id;
        temp_wire2.net_id = layer_net_vector[0].net_vector[i].net_id;
        temp_wire2.path.push_back({ layer_net_vector[0].net_vector[i].pin_vector[0].location_coordinates.x,layer_net_vector[0].net_vector[i].pin_vector[1].location_coordinates.y });
        temp_wire2.path.push_back(layer_net_vector[0].net_vector[i].pin_vector[1].location_coordinates);
        layer_net_vector[0].net_vector[i].wire_vector.push_back(temp_wire1);
        layer_net_vector[0].net_vector[i].wire_vector.push_back(temp_wire2);
    }
    dsn_instance.T.write_new_dsn_net("output/test.dsn", layer_net_vector[0].net_vector, layer_net_vector[0].net_vector.size());
}

void Routing::display_net()//显示net 
{
    cout << "display_net" << endl;
    for (int i = 0; i < net_vector.size(); ++i) {
        cout << net_vector[i].net_id << ';' << endl;
        vector<pin> temp_pin_vector=net_vector[i].pin_vector;
        for (int j = 0; j < temp_pin_vector.size(); ++j) {
            cout << temp_pin_vector[j].component_id << ':'
                 << temp_pin_vector[j].location_coordinates.x << ' '
                 << temp_pin_vector[j].location_coordinates.y << endl;
        }
    }

}

void Routing::display_component()
{
    cout << "display_component" << endl;
    for (int i = 0; i < component_vector.size(); ++i) {
        cout << component_vector[i].component_id << ":"
            << "left_bottom "
            << component_vector[i].left_bottom_vertex.x << ' ' << component_vector[i].left_bottom_vertex.y << ":"
            << "right_top "
            << component_vector[i].right_top_vertex.x << ' ' << component_vector[i].right_top_vertex.y << endl;
    }
}
