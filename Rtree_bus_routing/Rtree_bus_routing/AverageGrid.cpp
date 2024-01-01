#include "AverageGrid.h"
#include "routing.h"
AverageGrid::AverageGrid(pcb pcb_instance, bending_node start_bending_node, string layer_id, vector<bending_node>& start_end_bn_vector, vector<component>& component_vector, map<string, int>& bus_id_index_map) {
    this->layer_id = layer_id;
    bus_id = start_bending_node.bus_id;
    left_num = (int)((start_bending_node.bending_node_coordinates.x - start_bending_node.width / 2.0 - pcb_instance.left_bottom_vertex.x) / start_bending_node.width);
    right_num = (int)((pcb_instance.right_top_vertex.x - start_bending_node.bending_node_coordinates.x - start_bending_node.width / 2.0) / start_bending_node.width);
    top_num = (int)((pcb_instance.right_top_vertex.y - start_bending_node.bending_node_coordinates.y - start_bending_node.height / 2.0) / start_bending_node.height);
    bottom_num = (int)((start_bending_node.bending_node_coordinates.y - start_bending_node.height / 2.0 - pcb_instance.left_bottom_vertex.y) / start_bending_node.height);
    start_vertex.x = start_bending_node.bending_node_coordinates.x - start_bending_node.width / 2.0 - left_num * start_bending_node.width;
    start_vertex.y = start_bending_node.bending_node_coordinates.y - start_bending_node.height / 2.0 - bottom_num * start_bending_node.height;
    end_vertex.x = start_bending_node.bending_node_coordinates.x + start_bending_node.width / 2.0 + right_num * start_bending_node.width;
    end_vertex.y = start_bending_node.bending_node_coordinates.y + start_bending_node.height / 2.0 + top_num * start_bending_node.height;
    row_num = top_num + bottom_num + 1;
    col_num = left_num + right_num + 1;
    cout << "left right bottom top num : ";
    cout << left_num << " " << right_num << " " << bottom_num << " " << top_num << endl;
    cout << start_vertex.x << " " << start_vertex.y << " " << end_vertex.x << " " << end_vertex.y << endl;
    cell_array = new cell * [row_num];
    for (int k = 0; k < row_num; k++)
        cell_array[k] = new cell[col_num];
    this->start_end_bn_vector = start_end_bn_vector;
    this->component_vector = component_vector;
    this->bus_id_index_map = bus_id_index_map;
    cell_width = start_bending_node.width;
}

void AverageGrid::construct_cell(bending_node& start_bending_node, bending_node& end_bending_node, vector<bus>& bus_vector) {
    int start_col, end_col, start_row, end_row;
    bool start_judge = false, end_judge = false;
    for (int i = 0; i < row_num; i++) {                       //构建均分cell，计算每个cell的位置坐标信息、行列信息和拥塞率，并记下终止pin组的大概行列范围
        for (int m = 0; m < col_num; m++) {
            /*cell_array[i][m].left_bottom_vertex = { m * start_bending_node.width + start_vertex.x, i * start_bending_node.height + start_vertex.y };
            cell_array[i][m].right_top_vertex = { (m + 1) * start_bending_node.width + start_vertex.x, (i + 1) * start_bending_node.height + start_vertex.y };
            cell_array[i][m].row_mark = i;
            cell_array[i][m].col_mark = m;
            double distance_boundary[4];
            cell_array[i][m].congestion_rate = Routing::check_cell_congestion(cell_array[i][m].left_bottom_vertex, cell_array[i][m].right_top_vertex, layer_id , distance_boundary, component_vector, bus_vector, start_end_bn_vector, bus_id, bus_id_index_map);
            for (int k = 0; k < 4; k++) {
                cell_array[i][m].distance_boundary[k] = distance_boundary[k];
            }*/
            if (i == 0 && end_bending_node.bending_node_coordinates.x - end_bending_node.width / 2.0 >= m * start_bending_node.width + start_vertex.x
                && end_bending_node.bending_node_coordinates.x - end_bending_node.width / 2.0 < (m + 1) * start_bending_node.width + start_vertex.x) {
                start_col = m;        
            }
            if (i == 0 && end_bending_node.bending_node_coordinates.x + end_bending_node.width / 2.0 > m * start_bending_node.width + start_vertex.x
                && end_bending_node.bending_node_coordinates.x + end_bending_node.width / 2.0 <= (m + 1) * start_bending_node.width + start_vertex.x) {
                end_col = m;               
            }
            /*if (m == col_num - 1) {
                cout << i << " " << m << " " << cell_array[i][m].left_bottom_vertex.x << " " << cell_array[i][m].left_bottom_vertex.y;
                cout << " " << cell_array[i][m].right_top_vertex.x << " " << cell_array[i][m].right_top_vertex.y << endl;
            }*/
        }
        if (start_judge == false &&  end_bending_node.bending_node_coordinates.y - end_bending_node.height / 2.0 >= i * start_bending_node.height + start_vertex.y
            && end_bending_node.bending_node_coordinates.y - end_bending_node.height / 2.0 < (i + 1) * start_bending_node.height + start_vertex.y) {
            start_row = i;
            start_judge = true;
        }
        if (end_judge == false && end_bending_node.bending_node_coordinates.y + end_bending_node.height / 2.0 > i * start_bending_node.height + start_vertex.y
            && end_bending_node.bending_node_coordinates.y + end_bending_node.height / 2.0 <= (i + 1) * start_bending_node.height + start_vertex.y) {
            end_row = i;
            end_judge = true;
        }
    }
    if (end_bending_node.bending_node_coordinates.y - end_bending_node.height / 2.0 < start_vertex.y) {
        start_row = 0;
    }
    if (end_bending_node.bending_node_coordinates.y + end_bending_node.height / 2.0 > end_vertex.y) {
        end_row = row_num - 1;
    }
    if (end_bending_node.bending_node_coordinates.x - end_bending_node.width / 2.0 < start_vertex.x) {
        start_col = 0;
    }
    if (end_bending_node.bending_node_coordinates.x + end_bending_node.width / 2.0 > end_vertex.x) {
        end_col = col_num - 1;
    }
    cout << "start end col row: " << start_col << " " << end_col << " " << start_row << " " << end_row << endl;

    double max_congestion_rate = 0.0;                                   //从上面确定的终止pin组范围中，挑选与终止节点交叠面积最大的cell作为寻路的终止cell
    vertex end_node_vertex = end_bending_node.bending_node_coordinates;
    for (int i = start_row; i <= end_row; i++) {
        for (int m = start_col; m <= end_col; m++) {
            double temp_distance_boundary[4];
            double temp_congestion_rate = Routing::check_single_congestion({ end_node_vertex.x - end_bending_node.width / 2.0, end_node_vertex.y - end_bending_node.height / 2.0 },
                { end_node_vertex.x + end_bending_node.width / 2.0, end_node_vertex.y + end_bending_node.height / 2.0 }, { m * start_bending_node.width + start_vertex.x, i * start_bending_node.height + start_vertex.y },
                { (m + 1) * start_bending_node.width + start_vertex.x, (i + 1) * start_bending_node.height + start_vertex.y }, temp_distance_boundary);
            if (temp_congestion_rate > max_congestion_rate) {
                max_congestion_rate = temp_congestion_rate;
                end_row_index = i;
                end_col_index = m;
            }
        }
    }
    cout << end_row_index << " " << end_col_index << endl;
    /*for (int m = bottom_num + top_num; m >= 0; m--) {
        for (int k = 0; k <= left_num + right_num; k++) {
            cout << cell_array[m][k].congestion_rate << " ";
        }
        cout << endl;
    }*/
}

void AverageGrid::construct_single_cell(int row_index, int col_index, vector<bus> &bus_vector) {
    int i = row_index, m = col_index;
    cell_array[i][m].left_bottom_vertex = { m * cell_width + start_vertex.x, i * cell_width + start_vertex.y };
    cell_array[i][m].right_top_vertex = { (m + 1) * cell_width + start_vertex.x, (i + 1) *cell_width + start_vertex.y };
    cell_array[i][m].row_mark = i;
    cell_array[i][m].col_mark = m;
    double distance_boundary[4];
    cell_array[i][m].congestion_rate = Routing::check_cell_congestion(cell_array[i][m].left_bottom_vertex, cell_array[i][m].right_top_vertex, layer_id , distance_boundary, component_vector, bus_vector, start_end_bn_vector, bus_id, bus_id_index_map);
    for (int k = 0; k < 4; k++) {
        cell_array[i][m].distance_boundary[k] = distance_boundary[k];
    }
}