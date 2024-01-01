#include "Tree.h"
#define _USE_MATH_DEFINES
#include<cmath>
#include<math.h>
#include<algorithm>
#define temp 1e-10
//init
struct _component {
	string image_id;
	vector<_place> place;
};
void Tree::Open(string filename)
{
	myfile.open(filename, ifstream::in);
	pcb_struct["pcb"] = "parser/resolution/time_resolution/unit/structure/placement/library/floor_plan/part_library/network/wiring/colors";
	pcb_struct["parser"] = "host_cad/host_version/space_in_quoted_tokens/string_quote/write_resolution/routes_include/constant/via_rotate_first";
	pcb_struct["structure"] = "boundary/control/layer/rule/via/autoroute_settings/snap_angle/grid";
	pcb_struct["autoroute_settings"] = "fanout/eu.mihosoft.freerouting.autoroute/postroute/vias/via_costs/plane_via_costs/start_ripup_costs/start_pass_no/layer_rule/";
	pcb_struct["layer_rule"] = "active/preferred_direction/preferred_direction_trace_costs/against_preferred_direction_trace_costs/";
	pcb_struct["layer"] = "type/property/copper_thickness/dielectric_thickness/";
	pcb_struct["boundary"] = "path/rect";
	pcb_struct["control"] = "off_grid/force_to_terminal_point/pindelay_prop_velocity_factor/via_pin_length/viadelay_prop_velocity_factor/noise_accumulation/noise_calculation/crosstalk_model/bbv_ctr2ctr/roundoff_rotation/average_pair_length/4/";
	pcb_struct["rule"] = "width/clearance/clear/";
	pcb_struct["placement"] = "component/place_control/";
	pcb_struct["place_control"] = "flip_style";
	pcb_struct["component"] = "place/";
	pcb_struct["place"] = "property/";
	pcb_struct["property"] = "height/";
	pcb_struct["library"] = "image/padstack/";
	pcb_struct["image"] = "side/outline/pin/wire_keepout/via_keepout/conductor/";
	pcb_struct["conductor"] = "path/type/";
	pcb_struct["wire_keepout"] = "polygon/";
	pcb_struct["via_keepout"] = "polygon/";
	pcb_struct["padstack"] = "shape/attach/plating/type/hole/";
	pcb_struct["shape"] = "circle/rect/";
	pcb_struct["hole"] = "circle/";
	pcb_struct["part_library"] = "logical_part_mapping/logical_part/";
	pcb_struct["logical_part_mapping"] = "comp/";
	pcb_struct["logical_part"] = "pin/";
	pcb_struct["network"] = "net/class/via/via_rule/";
	pcb_struct["net"] = "pins/supply/";
	pcb_struct["class"] = "circuit/rule/via_rule/clearance_class/";
	pcb_struct["wiring"] = "wire/via/test_points/";
	pcb_struct["wire"] = "path/net/clearance_class/type/";
	pcb_struct["via"] = "net/type/clearance_class/spare/rotate/attr/";
	pcb_struct["colors"] = "color/set_color/";
	pcb_struct["test_points"] = "point/";
	pcb_struct["point"] = "net/";
}

Tree::Tree(string path) {
	Open(path);
}
Tree::~Tree() {
	myfile.close();
}
bool Tree::judge_cross(_vertex a, _vertex b, _vertex c, _vertex d) {
	if (abs((b.x - a.x) * (d.y - c.y) - (d.x - c.x) * (b.y - a.y)) < temp) {
		if ((a.x == c.x) && ((c.y - a.y) * (c.y - b.y) <= temp || (d.y - a.y) * (d.y - b.y) <= temp)) return true;
		else return false;
	}
	else {
		double m = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
		double n = (b.x - a.x) * (d.y - a.y) - (d.x - a.x) * (b.y - a.y);
		double p = (d.x - c.x) * (a.y - c.y) - (a.x - c.x) * (d.y - c.y);
		double q = (d.x - c.x) * (b.y - c.y) - (b.x - c.x) * (d.y - c.y);
		if (m * n <= temp && p * q <= temp) return true;
		else return false;
	}
}
vector<_rect> Tree::find_component() {
	vector<_rect> component_rect_vector;
	vector<_component> component_vector;
	vector<node_yes> component_node_vector = pcb_yes.child["PCB"][0].child["placement"][0].child["component"];
	for (int i = 0; i < component_node_vector.size(); i++) {
		_component component_instance;
		component_instance.image_id = component_node_vector[i].data[0];
		cout << component_instance.image_id << " ";
		for (int m = 0; m < component_node_vector[i].child["place"].size(); m++) {
			_place place_instance;
			place_instance.place_id = component_node_vector[i].child["place"][m].data[0];
			place_instance.x = str_2_double(component_node_vector[i].child["place"][m].data[1]);
			place_instance.y = str_2_double(component_node_vector[i].child["place"][m].data[2]);
			place_instance.side = component_node_vector[i].child["place"][m].data[3];
			place_instance.rotation = str_2_int(component_node_vector[i].child["place"][m].data[4]);
			component_instance.place.push_back(place_instance);
			cout << component_instance.place[m].x << " " << component_instance.place[m].y << " ";
		}
		cout << endl;
		component_vector.push_back(component_instance);
	}
	vector<node_yes> image_node_vector = pcb_yes.child["PCB"][0].child["library"][0].child["image"];
	for (int i = 0; i < image_node_vector.size(); i++) {
		node_yes image_node = image_node_vector[i];
		for (_component component_instance : component_vector) {
			if (component_instance.image_id == image_node.data[0]) {
				double min_x, min_y, max_x, max_y;
				if (image_node.child["outline"][0].child["path"].size() > 0) {
					min_x = str_2_double(image_node.child["outline"][0].child["path"][0].data[2]);
					min_y = str_2_double(image_node.child["outline"][0].child["path"][0].data[3]);
					max_x = str_2_double(image_node.child["outline"][0].child["path"][0].data[2]);
					max_y = str_2_double(image_node.child["outline"][0].child["path"][0].data[3]);
				}
				else if (image_node.child["outline"][0].child["polygon"].size() > 0) {
					min_x = str_2_double(image_node.child["outline"][0].child["polygon"][0].data[2]);
					min_y = str_2_double(image_node.child["outline"][0].child["polygon"][0].data[3]);
					max_x = str_2_double(image_node.child["outline"][0].child["polygon"][0].data[2]);
					max_y = str_2_double(image_node.child["outline"][0].child["polygon"][0].data[3]);
				}
				for (int n = 0; n < image_node.child["outline"].size(); n++) {
					for (int m = 0; m < image_node.child["outline"][n].child["path"].size(); m++) {
						for (int k = 2; k < image_node.child["outline"][n].child["path"][m].data.size(); k += 2) {
							if (str_2_double(image_node.child["outline"][n].child["path"][m].data[k]) < min_x) {
								min_x = str_2_double(image_node.child["outline"][n].child["path"][m].data[k]);
							}
							if (str_2_double(image_node.child["outline"][n].child["path"][m].data[k + 1]) < min_y) {
								min_y = str_2_double(image_node.child["outline"][n].child["path"][m].data[k + 1]);
							}
							if (str_2_double(image_node.child["outline"][n].child["path"][m].data[k]) > max_x) {
								max_x = str_2_double(image_node.child["outline"][n].child["path"][m].data[k]);
							}
							if (str_2_double(image_node.child["outline"][n].child["path"][m].data[k + 1]) > max_y) {
								max_y = str_2_double(image_node.child["outline"][n].child["path"][m].data[k + 1]);
							}
						}
					}
					for (int m = 0; m < image_node.child["outline"][n].child["polygon"].size(); m++) {
						for (int k = 2; k < image_node.child["outline"][n].child["polygon"][m].data.size(); k += 2) {
							if (str_2_double(image_node.child["outline"][n].child["polygon"][m].data[k]) < min_x) {
								min_x = str_2_double(image_node.child["outline"][n].child["polygon"][m].data[k]);
							}
							if (str_2_double(image_node.child["outline"][n].child["polygon"][m].data[k + 1]) < min_y) {
								min_y = str_2_double(image_node.child["outline"][n].child["polygon"][m].data[k + 1]);
							}
							if (str_2_double(image_node.child["outline"][n].child["polygon"][m].data[k]) > max_x) {
								max_x = str_2_double(image_node.child["outline"][n].child["polygon"][m].data[k]);
							}
							if (str_2_double(image_node.child["outline"][n].child["polygon"][m].data[k + 1]) > max_y) {
								max_y = str_2_double(image_node.child["outline"][n].child["polygon"][m].data[k + 1]);
							}
						}
					}
				}
				// cout << component_instance.image_id << " " << min_x << " " << min_y << " " << max_x << " " << max_y << endl;
				for (int m = 0; m < component_instance.place.size(); m++) {
					double vertex_x = component_instance.place[m].x;
					double vertex_y = component_instance.place[m].y;
					int rotation = component_instance.place[m].rotation;
					// cout << rotation << endl;
					_vertex before_left_bottom_vertex, before_right_top_vertex, after_left_bottom_vertex, after_right_top_vertex;
					before_left_bottom_vertex = { min_x, min_y };
					before_right_top_vertex = { max_x, max_y };
					// cout << before_left_bottom_vertex.x << " " << before_left_bottom_vertex.y << " " << before_right_top_vertex.x << " " << before_right_top_vertex.y << endl;
					if (rotation == 0) {
						after_left_bottom_vertex = { vertex_x + before_left_bottom_vertex.x, vertex_y + before_left_bottom_vertex.y };
						after_right_top_vertex = { vertex_x + before_right_top_vertex.x, vertex_y + before_right_top_vertex.y };
					}
					else if (rotation == 90) {
						after_left_bottom_vertex = { vertex_x - before_right_top_vertex.y, vertex_y + before_left_bottom_vertex.x };
						after_right_top_vertex = { vertex_x - before_left_bottom_vertex.y, vertex_y + before_right_top_vertex.x };
					}
					else if (rotation == 180) {
						after_left_bottom_vertex = { vertex_x - before_right_top_vertex.x, vertex_y - before_right_top_vertex.y };
						after_right_top_vertex = { vertex_x - before_left_bottom_vertex.x, vertex_y - before_left_bottom_vertex.y };
					}
					else if (rotation == 270) {
						after_left_bottom_vertex = { vertex_x + before_left_bottom_vertex.y, vertex_y - before_right_top_vertex.x };
						after_right_top_vertex = { vertex_x + before_right_top_vertex.y, vertex_y - before_left_bottom_vertex.x };
					}
					else {
						after_left_bottom_vertex.x = (before_left_bottom_vertex.x - vertex_x) * cos(rotation / 180.0 * M_PI) - (before_left_bottom_vertex.y - vertex_y) * sin(rotation / 180.0 * M_PI) + vertex_x;
						after_left_bottom_vertex.y = (before_left_bottom_vertex.x - vertex_x) * sin(rotation / 180.0 * M_PI) + (before_left_bottom_vertex.y - vertex_y) * cos(rotation / 180.0 * M_PI) + vertex_y;
						after_right_top_vertex.x = (before_right_top_vertex.x - vertex_x) * cos(rotation / 180.0 * M_PI) - (before_right_top_vertex.y - vertex_y) * sin(rotation / 180.0 * M_PI) + vertex_x;
						after_right_top_vertex.y = (before_right_top_vertex.x - vertex_x) * sin(rotation / 180.0 * M_PI) + (before_right_top_vertex.y - vertex_y) * cos(rotation / 180.0 *  M_PI) + vertex_y;
					}
					_rect rect;
					rect.left_bottom_vertex = after_left_bottom_vertex;
					rect.right_top_vertex = after_right_top_vertex;
					component_rect_vector.push_back(rect);
				}
			}
		}
	}
	return component_rect_vector;
}

vector<_bus> Tree::get_couple_pin(_vertex offset_vertex, vector<_rect> component_rect_vector, _wiring w) {
	/*_wiring w = read_wiring(find("wiring"));*/
	vector<_bus> _bus_vector;
	_bus bus_instance;
	vector<_component_boundary> component_boundary_vector;
	for (int i = 0; i < component_rect_vector.size(); i++) {
		component_boundary_vector.push_back({ component_rect_vector[i].left_bottom_vertex, {component_rect_vector[i].left_bottom_vertex.x, component_rect_vector[i].right_top_vertex.y}, LEFT_VERTICAL });
		component_boundary_vector.push_back({ component_rect_vector[i].right_top_vertex, {component_rect_vector[i].right_top_vertex.x, component_rect_vector[i].left_bottom_vertex.y}, RIGHT_VERTICAL });
		component_boundary_vector.push_back({ component_rect_vector[i].right_top_vertex, {component_rect_vector[i].left_bottom_vertex.x, component_rect_vector[i].right_top_vertex.y}, TOP_HORIZONAL });
		component_boundary_vector.push_back({ component_rect_vector[i].left_bottom_vertex, {component_rect_vector[i].right_top_vertex.x, component_rect_vector[i].left_bottom_vertex.y}, BOTTOM_HORIZONAL });
	}
	for (int i = 0; i < w.wire.size(); i++) {
		if (i == 0) {
			bus_instance.net_id = w.wire[0].net_id;
			bus_instance.layer_id = w.wire[0].polyline_path.layer_id;
		}
		else if (w.wire[i].net_id != bus_instance.net_id) {
			_bus_vector.push_back(bus_instance);
			bus_instance.vertex.clear();
			bus_instance.net_id = w.wire[i].net_id;
			bus_instance.layer_id = w.wire[i].polyline_path.layer_id;
		}
		else {
			bus_instance.net_id = w.wire[i].net_id;
			bus_instance.layer_id = w.wire[i].polyline_path.layer_id;
		}		//
		for (int m = 0; m < w.wire[i].polyline_path.vertex.size() - 1; m++) {
			for (int k = 0; k < component_boundary_vector.size(); k++) {
				if (judge_cross(component_boundary_vector[k].start_vertex, component_boundary_vector[k].end_vertex, w.wire[i].polyline_path.vertex[m], w.wire[i].polyline_path.vertex[m + 1])) {
					if (component_boundary_vector[k].location == TOP_HORIZONAL || component_boundary_vector[k].location == BOTTOM_HORIZONAL) {
						// double temp_x = abs(w.wire[i].polyline_path.vertex[m].y - component_boundary_vector[k].start_vertex.y) <= abs(w.wire[i].polyline_path.vertex[m+1].y - component_boundary_vector[k].start_vertex.y) ? w.wire[i].polyline_path.vertex[m].x : w.wire[i].polyline_path.vertex[m + 1].x;						
						double temp_x;
						if (w.wire[i].polyline_path.vertex[m].x == w.wire[i].polyline_path.vertex[m + 1].x) {
							temp_x = w.wire[i].polyline_path.vertex[m].x;
						}
						else {
							double incline_rate = (w.wire[i].polyline_path.vertex[m].y - w.wire[i].polyline_path.vertex[m + 1].y) / (w.wire[i].polyline_path.vertex[m].x - w.wire[i].polyline_path.vertex[m + 1].x);
							double b0 = w.wire[i].polyline_path.vertex[m].y - incline_rate * w.wire[i].polyline_path.vertex[m].x;
							temp_x = (component_boundary_vector[k].start_vertex.y - b0) / incline_rate;
							temp_x = ((int)(100 * temp_x)) / 100.0;
						}						
						bus_instance.vertex.push_back(_vertex(temp_x + offset_vertex.x, component_boundary_vector[k].start_vertex.y + offset_vertex.y, true, k / 4, component_boundary_vector[k].location));
					}
					else {
						// double temp_y = abs(w.wire[i].polyline_path.vertex[m].x - component_boundary_vector[k].start_vertex.x) <= abs(w.wire[i].polyline_path.vertex[m+1].x - component_boundary_vector[k].start_vertex.x) ? w.wire[i].polyline_path.vertex[m].y : w.wire[i].polyline_path.vertex[m + 1].y;
						double incline_rate = (w.wire[i].polyline_path.vertex[m].y - w.wire[i].polyline_path.vertex[m + 1].y) / (w.wire[i].polyline_path.vertex[m].x - w.wire[i].polyline_path.vertex[m + 1].x);
						double b0 = w.wire[i].polyline_path.vertex[m].y - incline_rate * w.wire[i].polyline_path.vertex[m].x;
						double temp_y = component_boundary_vector[k].start_vertex.x * incline_rate + b0;
						temp_y = ((int)(100 * temp_y)) / 100.0;
						bus_instance.vertex.push_back(_vertex(component_boundary_vector[k].start_vertex.x + offset_vertex.x, temp_y + offset_vertex.y, true, k / 4, component_boundary_vector[k].location));
					}
				}
			}
		}
		if (i == w.wire.size() - 1) {
			_bus_vector.push_back(bus_instance);
		}
	}
	cout << "pin size:" << _bus_vector.size() << endl;
	int net_count = 0;
	vector<int> id_vector;
	for (int i = 0; i < _bus_vector.size(); i++) {
		if (_bus_vector[i].vertex.size() == 2) {
			if (_bus_vector[i].vertex[0].obstacle_index == -1 || _bus_vector[i].vertex[1].obstacle_index == -1 || _bus_vector[i].vertex[0].obstacle_index == _bus_vector[i].vertex[1].obstacle_index) {
				_bus_vector.erase(_bus_vector.begin() + i);
				i--;
			}
			else {
				net_count++;
				id_vector.push_back(stoi(_bus_vector[i].net_id.substr(4, 4)));
				cout << net_count << " " << _bus_vector[i].net_id << " " << _bus_vector[i].layer_id << " " << _bus_vector[i].vertex[0].x - offset_vertex.x << " " << _bus_vector[i].vertex[0].y - offset_vertex.y << " " << _bus_vector[i].vertex[1].x - offset_vertex.x << " " << _bus_vector[i].vertex[1].y - offset_vertex.y << endl;
			}
		}
		else if (_bus_vector[i].vertex.size() > 2) {
			int first_index, second_index, flag = 0;
			_vertex first_vertex, second_vertex;
			first_index = _bus_vector[i].vertex[0].obstacle_index;
			first_vertex = _bus_vector[i].vertex[0];
			for (int m = 1; m < _bus_vector[i].vertex.size(); m++) {
				if (_bus_vector[i].vertex[m].obstacle_index != first_index) {
					second_index = _bus_vector[i].vertex[m].obstacle_index;
					second_vertex = _bus_vector[i].vertex[m];
					flag = 1;
					break;
				}
			}
			if (flag) {
				_bus_vector[i].vertex.clear();
				_bus_vector[i].vertex.push_back(first_vertex);
				_bus_vector[i].vertex.push_back(second_vertex);
				net_count++;
				id_vector.push_back(stoi(_bus_vector[i].net_id.substr(4, 4)));
				cout << net_count << " " << _bus_vector[i].net_id << " " << _bus_vector[i].layer_id << " " << _bus_vector[i].vertex[0].x - offset_vertex.x << " " << _bus_vector[i].vertex[0].y - offset_vertex.y << " " << _bus_vector[i].vertex[1].x - offset_vertex.x << " " << _bus_vector[i].vertex[1].y - offset_vertex.y << endl;
			}
			else {
				_bus_vector.erase(_bus_vector.begin() + i);
				i--;
			}

		}
		else {
			_bus_vector.erase(_bus_vector.begin() + i);
			i--;
		}
	}
	cout << "net count:" << net_count << endl;
	sort(id_vector.begin(), id_vector.end(), greater<int>());
	for (auto & num : id_vector)
		cout << num << endl;
	return _bus_vector;
}

bool Tree::read_until(istream& in, char c) {
	char input;
	while (in.get(input))
	{
		if (input == c) return false;
	}
	return true;
}
void Tree::read_unuseful(istream& in) {
	while (true) {
		auto b = in.peek();
		if (b != '\t' && b != '\n' && b != '\r' && b != ' ') break;
		char c;
		in.get(c);
	}
}
string Tree::read_quoted_data(istream& in, char& quote) {
	string s;
	while (true) {
		auto b = in.peek();
		if (b == quote) break;
		char c;
		in.get(c);
		s.push_back(c);
	}
	return s;
}
string Tree::read_data(istream& in, char& quote) {
	string s;
	while (true) {
		auto b = in.peek();
		char c;
		if (b == '\t' || b == '\n' || b == '\r' || b == ' ' || b == '(' || b == ')') break;
		else if (b == quote) {
			in.get(c);
			s.push_back(c);
			s += read_quoted_data(in, quote);
		}
		in.get(c);
		s.push_back(c);
	}
	return s;
}
string Tree::read_next(istream& in, char& quote) {
	string s;
	read_unuseful(in);
	auto b = in.peek();
	char c;
	if (b == EOF) return "EOF";
	if (b == '(' || b == ')') {
		in.get(c);
		s.push_back(c);
	}
	else {
		s = read_data(in, quote);
	}
	return s;
}
vector<string> Tree::split(const string& s, char delimiter)
{
	vector<string> tokens;
	string token;
	istringstream tokenStream(s);
	while (getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}
//core
node_yes Tree::get_pcb_yes() {
	istream& in = myfile;
	char quote = '"';
	pcb_yes = read_yes(in, quote);
	cout << "complish read_yes" << endl;
	//cout << pcb_yes.data[0] << endl;
	return pcb_yes.child.begin()->second[0];
}
node_yes Tree::read_yes(istream& in, char& quote) {
	node_yes node;
	string s;
	while (true) {
		s = read_next(in, quote);
		if (s == ")" || s == "EOF") break;
		if (s[0] == '#') {
			read_until(in, '\n');
			continue;
		}
		if (s == "(") {
			s = read_next(in, quote);
			if (s == "string_quote") {
				node_yes temp_node;
				read_unuseful(in);
				char c;
				in.get(c);
				string st;
				st.push_back(c);
				temp_node.data.push_back(st);
				quote = c;
				read_unuseful(in);
				in.get(c);
				node.child[s].push_back(temp_node);
			}
			else
				node.child[s].push_back(read_yes(in, quote));
		}
		else {
			node.data.push_back(s);
		}
	}
	return node;
}
//function
void Tree::show(node_yes& node) {
	for (int i = 0; i < (int)node.data.size(); i++)
		cout << node.data[i] << "; ";
	cout << endl;
	for (auto iter = node.child.begin(); iter != node.child.end(); iter++)
		cout << iter->first << " - size:" << iter->second.size() << endl;
}
void Tree::find_on_node(node_yes& node, string id, vector<node_yes>* p, bool all, int index) {
	auto iter = node.child.begin();
	while (iter != node.child.end()) {
		if (iter->first == id) {
			if (all) {
				(*p).insert((*p).end(), iter->second.begin(), iter->second.end());
			}
			else {
				for (int i = 0; i < (int)iter->second.size(); i++) {
					(*p).push_back(iter->second[i]);
					if ((*p).size() > index) return;
				}
			}
		}
		for (int i = 0; i < (int)iter->second.size(); i++) {
			find_on_node(iter->second[i], id, p, all, index);
			if (!all && (int)(*p).size() > index) return;
		}
		iter++;
	}
}
node_yes Tree::find(string id, int index) {
	vector<node_yes>* p = new vector<node_yes>;
	find_on_node(pcb_yes, id, p, false, index);
	return (*p)[index];
}
vector<node_yes> Tree::find_all(string id, node_yes& node) {
	vector<node_yes>* p = new vector<node_yes>;
	if (node.data.size() || node.child.size())
		find_on_node(node, id, p, true, 0);
	else
		find_on_node(pcb_yes, id, p, true, 0);
	return (*p);
}

_path Tree::read_path(node_yes node) {
	_path p;
	p.layer_id = node.data[0];
	p.aperture_width = str_2_int(node.data[1]);
	for (int i = 2; i < node.data.size(); i += 2) {
		p.vertex.push_back(_vertex(str_2_double(node.data[i]), str_2_double(node.data[i + 1])));
	}
	return p;
}
_via Tree::read_via(node_yes node) {
	_via via;
	via.padstack_id.push_back(node.data[0]);
	via.vertex.x = str_2_double(node.data[1]);
	via.vertex.y = str_2_double(node.data[2]);
	return via;
}
_wire Tree::read_wire(node_yes node) {
	_wire wire;
	wire.net_id = node.child["net"][0].data[0];
	wire.polyline_path = read_path(node.child["path"][0]);
	return wire;
}
_wiring Tree::read_wiring(node_yes node) {
	_wiring wiring;
	vector<node_yes> wire_nodes = find_all("wire", node);
	for (int i = 0; i < wire_nodes.size(); i++) {
		wiring.wire.push_back(read_wire(wire_nodes[i]));
	}
	vector<node_yes> via_nodes = find_all("via", node);
	for (int i = 0; i < via_nodes.size(); i++) {
		wiring.via.push_back(read_via(via_nodes[i]));
	}
	return wiring;
}

void Tree::ClearWirings()
{
	pcb_yes.child.begin()->second[0].child["wiring"][0].child["wire"].clear();
	pcb_yes.child.begin()->second[0].child["wiring"][0].child["via"].clear();
}

void Tree::put_wiring_to_pcb(_wiring& w) {
	for (int i = 0; i < (int)w.wire.size(); i++) {
		node_yes wire_;
		node_yes p;	//polyline_path
		node_yes n;	//net
		node_yes c;	//clearance_class
		node_yes t;	//type
		p.data.push_back(w.wire[i].polyline_path.layer_id);
		p.data.push_back(to_string(w.wire[i].polyline_path.aperture_width));
		for (int j = 0; j < (int)w.wire[i].polyline_path.vertex.size(); j++) {
			string s = to_string(w.wire[i].polyline_path.vertex[j].x);
			s = s.substr(0, s.size() - 4);	//保留2位小数
			p.data.push_back(s);
			s = to_string(w.wire[i].polyline_path.vertex[j].y);
			s = s.substr(0, s.size() - 4);	//保留2位小数
			p.data.push_back(s);
		}
		wire_.child["path"].push_back(p);
		if (w.wire[i].net_id.size() > 0)
		{
			n.data.push_back(w.wire[i].net_id);
			wire_.child["net"].push_back(n);
		}
		if (w.wire[i].clearance_class.size() > 0)
		{
			c.data.push_back(w.wire[i].clearance_class);
			wire_.child["clearance_class"].push_back(c);
		}
		if (w.wire[i].type.size() > 0) {
			t.data.push_back(w.wire[i].type);
			wire_.child["type"].push_back(t);
		}
		pcb_yes.child.begin()->second[0].child["wiring"][0].child["wire"].push_back(wire_);
	}
}
void Tree::write_one_node(ofstream& out, node_yes& node, int k, vector<string> struct_, string name) {
	if (node.data.size() > 4) {
		int i = node.data.size() % 4;
		for (int m = 0; m < i; m++)
			out << node.data[m] << ' ';
		out << '\n';
		int j = node.data.size() / 4;
		for (int m = 0; m < j; m++) {
			for (int n = 0; n < k; n++)
				out << "  ";
			for (int n = 0; n < 4; n++)
				out << node.data[i + 4 * m + n] << ' ';
			out << '\n';
		}
		for (int n = 0; n < k - 1; n++)
			out << "  ";
	}
	else {
		for (int m = 0; m < (int)node.data.size(); m++)
			out << node.data[m] << ' ';
		if (node.child.size())
			out << '\n';
	}
	if (node.child.size()) {
		auto iter = node.child.begin();
		for (int i = 0, j = 0; i < (int)node.child.size(); i++) {
			if (node.child.size() > 1) {
				if (j >= (int)struct_.size()) {
					cout << "Something not define in: " << name << endl;
					break;
				}
				do {
					iter = node.child.find(struct_[j++]);
				} while (iter == node.child.end() && j < (int)struct_.size());
				if (iter == node.child.end()) {
					cout << "Something not define in: " << name << endl;
					break;
				}
			}
			for (int m = 0; m < (int)iter->second.size(); m++) {
				for (int n = 0; n < k; n++)
					out << "  ";
				out << '(' << iter->first << ' ';
				write_one_node(out, iter->second[m], k + 1, split(pcb_struct[iter->first], '/'), iter->first);
			}
		}
		for (int n = 0; n < k - 1; n++)
			out << "  ";
	}
	out << ")\n";
}
void Tree::write_new(string path) {
	ofstream ofile;
	ofile.open(path, ios::out);
	ofile << "(PCB ";
	write_one_node(ofile, pcb_yes.child.begin()->second[0], 1, split(pcb_struct["pcb"], '/'), "pcb");
	ofile.close();
}
void Tree::write_new_dsn(string path, vector<bus> &bus_vector, int bus_count) {
	_wiring wiring;
	cout << "write new dsn" << endl;
	for (int i = 0; i < bus_count; i++) {
		for (int m = 0; m < bus_vector[i].net_vector.size(); m++) {
			for (int h = 0; h < bus_vector[i].net_vector[m].wire_vector.size(); h++) {
				_wire wire;
				wire.net_id = bus_vector[i].net_vector[m].net_id;
				wire.type = "protect";
				wire.polyline_path.aperture_width = 7;
				wire.polyline_path.layer_id = bus_vector[i].net_vector[m].wire_vector[h].layer_id;
				for (int n = 0; n < bus_vector[i].net_vector[m].wire_vector[h].path.size(); n++) {
					wire.polyline_path.vertex.push_back({ bus_vector[i].net_vector[m].wire_vector[h].path[n].x, bus_vector[i].net_vector[m].wire_vector[h].path[n].y });
				}
				wiring.wire.push_back(wire);
			}			
		}		
	}
	put_wiring_to_pcb(wiring);
	write_new(path);
	cout << "after write" << endl;
}

void Tree::write_new_dsn_net(string path, vector<net>& net_vector, int net_count) {
	_wiring wiring;
	for (int i = 0; i < net_vector.size(); ++i) {//每一对线网
		for (int j = 0; j < net_vector[i].wire_vector.size(); ++j) {//每一个段线
			_wire wire;
			wire.net_id = net_vector[i].net_id;
			wire.type = "protect";
			wire.polyline_path.aperture_width = 7;
			wire.polyline_path.layer_id = net_vector[i].wire_vector[j].layer_id;
			for (int k = 0; k < net_vector[i].wire_vector[j].path.size(); k++) {//每一个线段的点
				wire.polyline_path.vertex.push_back({ net_vector[i].wire_vector[j].path[k].x, net_vector[i].wire_vector[j].path[k].y });
			}
			wiring.wire.push_back(wire);
		}
	}
	put_wiring_to_pcb(wiring);
	write_new(path);
	cout << "after write" << endl;
}