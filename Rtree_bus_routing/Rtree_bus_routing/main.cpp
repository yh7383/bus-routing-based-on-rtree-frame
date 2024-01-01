#include<iostream>
#include<stack>
#include<set>
#include "data.h"
#include "routing.h"
using namespace std;

int main() {
    Routing routing_instance;
    routing_instance.init_pcb("input/test.dsn");
    routing_instance.rec_link();
    //routing_instance.Rtree_bus_routing();
    system("pause");
    return 0;
}