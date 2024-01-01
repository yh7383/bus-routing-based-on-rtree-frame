#include<iostream>
#include"data.h"
bool myComp(pair<vertex, vertex> p1, pair<vertex, vertex> p2) {
    return p1.first.y < p2.first.y;
}
bool myComp2(pair<vertex, vertex> p1, pair<vertex, vertex> p2) {
    return p1.first.x < p2.first.x;
}
