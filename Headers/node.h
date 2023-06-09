#ifndef NODE_H
#define NODE_H
#include "coord.h"
#include <vector>
#include <ostream>
#include <unordered_set>
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
            pos = p;
            letter =l;
        }
        node(int r, int c, char l){
            pos.row = r;
            pos.col = c;
            letter =l;
        }
        bool noDuplicates(coord p,std::string l){//needs improvement
            for(int i=0;i<edges.size();i++){
                if(p==edges[i] && l==edges[i].path){
                    return false;
                }
            }
            return true;
        }
        bool add(coord p, std::string path, bool fine = false){//needs improvement
            if(!(pos==p) && (fine || noDuplicates(p,path))){
                edges.emplace_back(p.row,p.col,path);
                return true;
            }
            return false;
        }
        bool remove(coord p){//needs improvement
            for(int i=0;i<edges.size();i++){
                if(p==edges[i]){
                    edges.erase(edges.begin()+i);
                    return true;
                }
            }
            return false;
        }
        bool operator==(const node other){//may need to consider distance here
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