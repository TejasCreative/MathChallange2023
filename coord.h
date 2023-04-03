#ifndef COORD_H
#define COORD_H
#include <ostream>

class coord{
    public:
        int row;
        int col;
        int dist;
        coord(){
            row=0;
            col=0;
            dist=0;
        }
        coord(int r, int c, int d){
            row=r;
            col=c;
            dist = d;
        }
        coord(const coord& other){
            row = other.row;
            col = other.col;
            dist = other.dist;
        }
        coord& operator=(const coord& other){
            row = other.row;
            col = other.col;
            dist = other.dist;
            return *this;
        }
        coord operator+(const coord other){
            coord c;
            c.row=this->row+other.row;
            c.col=this->col+other.col;
            return c;
        }
        bool operator==(const coord other){
            return row==other.row && col==other.col;
        }
        friend std::ostream& operator<<(std::ostream& os, coord c){
            os << '('<<c.row <<','<<c.col << ','<< c.dist<<')';
            return os;
        }
};



#endif