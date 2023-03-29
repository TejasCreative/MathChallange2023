#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <fstream>
#include "node.h"
#include "coord.h"
#include <string>
#include <vector>

struct matrix{
    //multi-portals, clean code
    int rows;
    int cols;
    node start;
    node end;
    node** info;
    int** adjmat;
    std::vector<coord> choices;
    std::vector<coord> shift;
    std::vector<node> portals;
    matrix(){
        rows=0;
        cols=0;
        info=nullptr;
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
                else if((int)line[col]>65 && (int)line[col]<90){
                    if(((int)info[row-1][col].letter>65 && (int)info[row-1][col].letter<90) || ((int)info[row][col-1].letter>65 && (int)info[row][col-1].letter<90)){
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
                    if(start.letter==' '){
                        start.pos.row = row;
                        start.pos.col = col;
                        start.letter = 'A';
                    }
                    else{
                        line[col] = '_';
                    }
                }
                else if(line[col]=='Z'){
                    if(end.letter==' '){
                        end.pos.row = row;
                        end.pos.col = col;
                        end.letter = 'Z';
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
        choices.push_back(start.pos);
        coord current,last;
        node next;
        int distance,i,front=0,choiceDirection =0;
        while(front<choices.size()){//breadth first
            current = choices[front];
            i=choiceDirection;
            distance=0;
            while(i<5){//depth first
                if(i==4){
                    if(at(current).letter=='P'){
                        next = at(searchPortals(current));
                    }
                    else{
                        break;
                    }
                }
                else{
                    next = at(current+shift[i]);
                    while(next.letter=='_'){
                        next = at(next.pos+shift[i]);
                    }
                }
                if((next.letter=='.' || next.letter=='P' || next.letter=='*' || next.letter=='Z') && !(next.pos==last)){
                    distance++;
                    if(at(next.pos).letter=='.'){
                        at(next.pos).letter=',';//visited
                    }
                    if(countOptions(next.pos)>2 || next.letter=='Z'){//found a choice node
                        at(choices[front]).add(next.pos,distance);
                        at(next.pos).add(choices[front],distance);
                        if(at(next.pos).letter!='*'){
                            choices.push_back(next.pos);
                            at(next.pos).letter='*';
                        }
                        break;
                    }
                    else if(countOptions(next.pos)==2){//only one option to continue
                        last = current;
                        current = next.pos;
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
        //ensures end point is last
        choices.erase(choices.begin()+searchChoices(end.pos));
        choices.push_back(end.pos);
        at(end.pos).letter='Z';
    }
    node& operator()(int row, int col){
        return info[row][col];
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
    // std::pair<int**, coord*> makeAdjacencyMatrix(){
    //     int** adjmat = new int*[choices.size()];
    //     coord* coords = new coord[choices.size()];
    //     int n;
    //     for(int choice=0;choice<choices.size();choice++){
    //         adjmat[choice] = new int[choices.size()];
    //         for(int j=0;j<choices.size();j++){
    //             adjmat[choice][j]=-1;
    //         }
    //         for(int edge=0;edge<at(choices[choice]).edges.size();edge++){
    //             n = searchChoices(at(choices[choice]).edges[edge]);
    //             adjmat[choice][n] = at(choices[choice]).edges[edge].dist;
    //         }
    //         coords[choice] = choices[choice];
    //     }
    //     return std::make_pair(adjmat,coords);
    // }
    
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