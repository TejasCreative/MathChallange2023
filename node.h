#ifndef NODE_H
#define NODE_H
#include "coord.h"
#include <vector>
#include <ostream>
// struct node{
//     coord pos;
//     std::vector<coord> edges;
//     char letter;
//     node(){
//         pos.row=0;
//         pos.col=0;
//         letter = ' ';
//     }
//     node(coord p,char l){
//         letter =l;
//         pos = p;
//     }

// };
// std::ostream& operator<<(std::ostream& os, node n){
//     os << n.pos << " | ";
//     for(int i=0;i<n.edges.size();i++){
//         os << n.edges[i] << ",";
//     }
//     return os;
// }

class node{
    public:
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
        node(int r, int c, int d, char l){
            pos.row = r;
            pos.col = c;
            pos.dist = d;
            letter =l;
        }
        bool noDuplicates(coord p){
            for(int i=0;i<edges.size();i++){
                if(p==edges[i]){
                    return false;
                }
            }
            return true;
        }
        bool add(coord p,int d){
            if(noDuplicates(p)){
                edges.emplace_back(p.row,p.col,d);
                return true;
            }
            return false;
        }
        bool operator==(const node other){
            return pos==other.pos && letter==other.letter;
        }
        friend std::ostream& operator<<(std::ostream& os, node n){
            os << n.pos << " | " << n.letter << " | ";
            for(int i=0;i<n.edges.size();i++){
                os << n.edges[i] << ",";
            }
            return os;
        }
};


#endif