#ifndef NODE_H
#define NODE_H
#include "coord.h"
#include <vector>
#include <ostream>
struct node{
    coord pos;
    std::vector<coord> edges;
    char letter;
    node(){
        pos.row=0;
        pos.col=0;
        letter = ' ';
    }
    node(coord p,char l){
        letter =l;
        pos = p;
    }

};
std::ostream& operator<<(std::ostream& os, node n){
    os << n.pos << " | ";
    for(int i=0;i<n.edges.size();i++){
        os << n.edges[i] << ",";
    }
    return os;
}

#endif