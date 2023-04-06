#ifndef COORD_H
#define COORD_H
#include <string>
#include <ostream>

class coord{
    public:
        int row;
        int col;
        std::string path;
        coord(){
            row=0;
            col=0;
            path = "";
        }
        coord(int r, int c, std::string p){
            row=r;
            col=c;
            path = p;
        }
        coord(const coord& other){
            row = other.row;
            col = other.col;
            path = other.path;
        }
        coord& operator=(const coord& other){
            row = other.row;
            col = other.col;
            path = other.path;
            return *this;
        }
        coord operator+(const coord& other) const{
            coord c;
            c.row=this->row+other.row;
            c.col=this->col+other.col;
            c.path = this->path+other.path;
            return c;
        }
        bool operator==(const coord& other) const{
            return row==other.row && col==other.col;
        }
        friend std::ostream& operator<<(std::ostream& os, coord c){
            os << '('<<c.row <<','<<c.col << ','<< c.path<<')';
            return os;
        }
};

class coordHash{
    public:
        std::size_t operator()(const coord& c) const{
            return 35*c.row+c.col;
        }
};



#endif