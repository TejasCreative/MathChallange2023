#ifndef COORD_H
#define COORD_H
#include <ostream>
struct coord{
    int row;
    int col;
    int dist;
    coord(){
        row=0;
        col=0;
        dist=-1;
    }
    coord(int r, int c, int d){
        row=r;
        col=c;
        dist = d;
    }
    coord& operator=(const coord& other){
        row = other.row;
        col = other.col;
        dist = other.dist;
        return *this;
    }
};
std::ostream& operator<<(std::ostream& os, coord c){
    os << '('<<c.row <<','<<c.col << ','<< c.dist<<')';
    return os;
}

#endif