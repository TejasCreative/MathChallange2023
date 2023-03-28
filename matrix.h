#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <fstream>
#include "node.h"
#include "coord.h"
#include <string>
#include <vector>
#include <queue>

struct matrix{
    int rows;
    int cols;
    node start;
    node end;
    node** info;
    std::vector<node> choices;
    std::vector<coord> shift;
    std::vector<node> portals;
    matrix(){
        rows=0;
        cols=0;
        info=nullptr;
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
        int n=0, w=0;
        while(std::getline(mf,line)){
            n++;
            if(line.length()>w){
                w=line.length();
            }
        }
        mf.close();
        mf.open(filename);
        w--;
        node** data = new node*[n];
        for(int row=0;row<n;row++){
            std::getline(mf,line);
            data[row] = new node[w];
            for(int col=0;col<w;col++){
                if(col>=line.length()-1 || line[col]=='#'){
                    line[col]=' ';
                }
                else if(line[col]=='A'){
                    line[col] = '.';
                    start.pos.row = row;
                    start.pos.col = col;
                }
                else if(line[col]=='Z'){
                    line[col] = '.';
                    end.pos.row = row;
                    end.pos.col = col;
                }
                else if((int)line[col]>65 && (int)line[col]<90){
                    if(((int)data[row-1][col].letter>65 && (int)data[row-1][col].letter<90) || ((int)data[row][col-1].letter>65 && (int)data[row][col-1].letter<90)){
                        line[col]='p';
                    }
                    else{
                        node portal(row,col,0,line[col]);
                        linkPortals(portal);
                        portals.push_back(portal);
                        line[col]='P';
                    }
                }
                data[row][col].letter = line[col];
                data[row][col].pos.row = row;
                data[row][col].pos.col = col;
            }
        }
        mf.close();
        rows = n;
        cols = w;
        info = data;
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
            if(l=='p'){
                l=at(curr+shift[i]+shift[i]).letter;
            }
            if(l=='.' || l=='P'){
                n++;
            }
        }
        return n;
    }
    bool noDuplicates(coord coordinate){
        for(int i=0;i<choices.size();i++){
            if(choices[i].pos==coordinate){
                return false;
            }
        }
        return true;
    }
    coord searchPortals(coord current,int whichPortal=0){
        for(int i=0;i<portals.size();i++){
            if(portals[i].pos == current){
                return portals[i].edges[whichPortal];
            }
        }
        return current;
    }
    void findChoices(){
        std::vector<coord> lasts; 
        choices.push_back(start);
        lasts.push_back(start.pos);
        coord current, last;
        node next;
        int front=0;
        int distance;
        int choiceDirection =0;
        while(front<=choices.size()){//check cardinal directions, NESW
            if(front==choices.size()){
                if(noDuplicates(end.pos)){
                    choices.push_back(end);
                    lasts.push_back(end.pos);
                }
                else{
                    break;
                }
            }
            current = choices[front].pos;
            last = lasts[front];
            int i=choiceDirection;
            distance=0;
            while(i<5){
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
                    if(next.letter=='p'){
                        next = at(next.pos+shift[i]);
                    }
                }
                if((next.letter=='.' || next.letter=='P')&& !(next.pos==last)){
                    distance++;
                    if(countOptions(next.pos)>2){//found a choice node
                        choices[front].edges.emplace_back(next.pos.row,next.pos.col,distance);
                        next.edges.emplace_back(choices[front].pos.row,choices[front].pos.col,distance);
                        if(noDuplicates(next.pos)){
                            choices.push_back(next);
                            lasts.push_back(current);
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
                        current = choices[front].pos;
                        last = lasts[front];
                        break;
                    }
                }
                else if(current==choices[front].pos){
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
        //label choice nodes with 0
        at(choices.front().pos).letter='A';
        at(choices.back().pos).letter='Z';
        for(int j=1;j<choices.size()-1;j++){
            if(choices[j].edges.size()>2){   
               at(choices[j].pos).letter = '0';
            }
        }
        //label portal pairs
        for(int j=0;j<portals.size();j++){
            at(portals[j].pos).letter = (char)(66+j);
            at(portals[j].edges[0]).letter = (char)(66+j);
        }
    }
    node& operator()(int row, int col){
        return info[row][col];
    }
    void writeNodes(std::string filename){
        std::ofstream mf;
        mf.open(filename);
        mf << "Choices:\n";
        for(int i=0;i<choices.size();i++){
            mf << choices[i] << "\n";
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
    ~matrix(){
        for(int i=0;i<rows;i++){
            delete[] info[i];
        }
        delete[] info;
    }
};


#endif