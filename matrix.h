#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <fstream>
#include "node.h"
#include "coord.h"
#include <string>
#include <vector>

struct matrix{
    //assuming only movement in cardinal directions and teleporting
    //multi-portals, clean code
    int rows;
    int cols;
    node* start;
    node* end;
    node** info;
    int** adjmat;
    std::vector<coord> choices;
    std::vector<coord> shift;
    std::vector<node> portals;
    matrix(){
        rows=0;
        cols=0;
        info=nullptr;
        start=nullptr;
        end=nullptr;
        adjmat=nullptr;
        shift.emplace_back(-1,0,0);
        shift.emplace_back(0,1,0);
        shift.emplace_back(1,0,0);
        shift.emplace_back(0,-1,0);
    }
    void linkPortals(node& n){
        for(int i=0;i<portals.size();i++){
            if(portals[i].letter==n.letter){
                portals[i].edges.push_back(n.pos);
                n.edges.push_back(portals[i].pos);
            }
        }
    }
    void generate2d(std::string filename){
        std::ifstream mf;
        mf.open(filename);
        std::string line;
        while(std::getline(mf,line)){
            rows++;
            if(line.length()>cols){
                cols=line.length();
            }
        }
        mf.close();
        mf.open(filename);
        cols--;
        info = new node*[rows];
        for(int row=0;row<rows;row++){
            std::getline(mf,line);
            info[row] = new node[cols];
            for(int col=0;col<cols;col++){
                if(col>=line.length()-1 || line[col]=='#'){
                    line[col]=' ';
                }
                else if('A'<line[col] && line[col]<'Z'){
                    if((info[row-1][col].letter>'A' && info[row-1][col].letter<'Z') || (info[row][col-1].letter>'A' && info[row][col-1].letter<'Z')){
                        line[col]='_';
                    }
                    else{
                        node portal(row,col,0,line[col]);
                        linkPortals(portal);
                        portals.push_back(portal);
                        line[col]='P';
                    }
                }
                else if(line[col]=='A'){
                    if(start==nullptr){
                        start = &info[row][col];
                    }
                    else{
                        line[col] = '_';
                    }
                }
                else if(line[col]=='Z'){
                    if(end==nullptr){
                        end = &info[row][col];
                    }
                    else{
                        line[col] = '_';
                    }
                }
                info[row][col].letter = line[col];
                info[row][col].pos.row = row;
                info[row][col].pos.col = col;
            }
        }
        mf.close();
    }
    node& at(coord coordinate){
        if(coordinate.row>=0 && coordinate.row<rows && coordinate.col>=0 && coordinate.col<cols){
            return info[coordinate.row][coordinate.col];
        }
        return info[0][0];
    }
    int countOptions(coord curr){
        int n=0;
        char l;
        if(at(curr).letter=='P'){
            n++;
        }
        for(int i=0;i<4;i++){
            l = at(curr+shift[i]).letter;
            if(l=='_'){//while
                l=at(curr+shift[i]+shift[i]).letter;
            }
            if(l!=' '){
                n++;
            }
        }
        return n;
    }
    coord searchPortals(coord current,int whichPortal=0){
        for(int i=0;i<portals.size();i++){
            if(portals[i].pos == current){
                return portals[i].edges[whichPortal];
            }
        }
        return current;
    }
    int searchChoices(coord coordinate){
        for(int i=0;i<choices.size();i++){
            if(coordinate==choices[i]){
                return i;
            }
        }
        return -1;
    }
    void findChoices(){
        
        choices.push_back(start->pos);
        coord current,last;
        node* next;
        int distance,i,front=0,choiceDirection =0;
        while(front<choices.size()){//breadth first
            std::string line = "";
            current = choices[front];
            i=choiceDirection;
            distance=0;
            while(i<5){//depth first
                if(i==4){
                    if(at(current).letter=='P'){
                        line = line + "p";
                        next = &at(searchPortals(current)); //if currently at a portal go to the other portal
                    }
                    else{
                        break;
                    }
                }
                else{
                    next = &at(current+shift[i]);  
                    while(next->letter=='_'){
                        line = line + "p";
                        next = &at(next->pos+shift[i]);
                        

                    }
                }
                if((next->letter=='.' || next->letter=='P' || next->letter=='*' || next->letter=='Z') && !(next->pos==last)){
                    distance++;
                    if(i == 0){
                        line = line + "l";
                    }
                    else if(i == 1){
                        line = line + "d";
                    }
                    else if(i == 2){
                        line = line + "r";
                    }
                    else if(i == 3){
                        line = line + "u";
                    }
                    if(next->letter=='.'){ 
                        next->letter=',';//visited
                    }
                    if(next->letter=='*' || next->letter=='Z' || countOptions(next->pos)>2){//found a choice node
                        if(next->letter!='*'){                          //if a new choice node
                            next->pos.path = line;
                            choices.push_back(next->pos);
                            next->letter='*';
                        }
                        at(choices[front]).add(next->pos,distance);
                        next->add(at(choices[front]).pos,distance);
                        break;
                    }
                    else if(countOptions(next->pos)==2){//only one option to continue
                        if(i == 0){
                            line = line + "l";
                        }
                        else if(i == 1){
                            line = line + "d";
                        }
                        else if(i == 2){
                            line = line + "r";
                        }
                        else if(i == 3){
                            line = line + "u";
                        }
                        last = current;
                        current = next->pos;
                        i=0;
                        continue;
                    }
                    else{// < 2 dead end so reset to last choice currently exploring
                        current = choices[front];
                        break;
                    }
                }
                else if(current==choices[front]){
                    break;
                }
                i++;
            }
            choiceDirection++;
            if(choiceDirection>3){
                front++;
                choiceDirection=0;
            }
        }
        //label portal pairs
        for(int j=0;j<portals.size();j++){
            at(portals[j].pos).letter = (char)(66+j);
            at(portals[j].edges[0]).letter = (char)(66+j);
        }
        choices.erase(choices.begin()+searchChoices(end->pos));
        choices.push_back(end->pos);
        end->letter='Z';
    }
    void prune(node* vertex){
        for(int i=0;i<vertex->edges.size();i++){
            for(int j=0;j<i;j++){
                at(vertex->edges[i]).add(vertex->edges[j],vertex->edges[i].dist+vertex->edges[j].dist, true);
            }
            for(int j=i+1;j<vertex->edges.size();j++){
                at(vertex->edges[i]).add(vertex->edges[j],vertex->edges[i].dist+vertex->edges[j].dist,true);
            }
            at(vertex->edges[i]).remove(vertex->pos);
        }
        vertex->edges.clear();
        choices.erase(choices.begin() + searchChoices(vertex->pos));
        vertex->letter=',';
    }
    void trimGraph(int size){
        for(int i=choices.size()-2;i>0;i--){//skip A and Z (0 and end)
            if(at(choices[i]).edges.size()<size){
                prune(&at(choices[i]));
            }
        }
    }
    void convertAdMatrix(){
        adjmat = new int*[choices.size()];
        int n;
        for(int choice=0;choice<choices.size();choice++){
            adjmat[choice] = new int[choices.size()];
            for(int j=0;j<choices.size();j++){
                adjmat[choice][j]=-1;
            }
            for(int edge=0;edge<at(choices[choice]).edges.size();edge++){
                n = searchChoices(at(choices[choice]).edges[edge]);
                adjmat[choice][n] = at(choices[choice]).edges[edge].dist;
            }
        }
    }
    void writeNodes(std::string filename){
        std::ofstream mf;
        mf.open(filename);
        mf << "Choices:\n";
        for(int i=0;i<choices.size();i++){
            mf << at(choices[i]) << "\n";
        }
        mf << "\nPortals:\n";
        for(int i=0;i<portals.size();i++){
            mf << portals[i] << "\n";
        }
        mf.close();
    }
    void writeMap(std::string filename){
        std::ofstream mf;
        mf.open(filename);
        for(int r=0;r<rows;r++){
            for(int c=0;c<cols;c++){
                mf << info[r][c].letter;
            }
            mf << "\n";
        }
        mf.close();
    }
    void displayAdjacencyMatrix(std::string filename){
        std::ofstream mf;
        mf.open(filename);
        for(int i=0;i<choices.size();i++){
            for(int j=0;j<choices.size();j++){
                mf << adjmat[i][j] << " ";
            }
            mf << "\n";
        }
        mf.close();
    }
    
    ~matrix(){
        if(info!=nullptr){
            for(int i=0;i<rows;i++){
                delete[] info[i];
            }
            delete[] info;
        }
        if(adjmat!=nullptr){
            for(int i=0;i<choices.size();i++){
                delete[] adjmat[i];
            }
            delete[] adjmat;
        }
    }
};


#endif