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
    matrix(){
        rows=0;
        cols=0;
        info=nullptr;
        shift.emplace_back(-1,0,0);
        shift.emplace_back(0,1,0);
        shift.emplace_back(1,0,0);
        shift.emplace_back(0,-1,0);
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
    int countDots(coord curr){
        int n=0;
        for(int i=0;i<4;i++){
            if(at(curr+shift[i]).letter=='.'){
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
    void findChoices(){
        std::vector<coord> lasts; 
        choices.push_back(start);
        lasts.push_back(start.pos);
        coord current, last;
        node next;
        int front=0;
        int distance=0;
        std::string test;
        int choiceDirection =0;
        //check cardinal directions, NESW
        while(front!=choices.size()){
            current = choices[front].pos;
            //create a new list that stores last that updates for each new choice
            last = lasts[front];
            int i=choiceDirection;
            while(i<4){
                next = at(current+shift[i]);
                // std::cout << "\ni: " << i << "\ncurrent: " << current << "\nnext: " << next << "\ncd: " << choiceDirection << "\nAck: ";
                if(next.letter=='.' && !(next.pos==last)){
                    distance++;
                    if(countDots(next.pos)>2){//found a choice node
                        // std::cout << "new choice\n";
                        distance = 0;
                        next.pos.dist=distance;
                        choices[front].edges.push_back(next.pos);
                        next.edges.push_back(choices[front].pos);
                        next.edges.back().dist=distance;
                        if(noDuplicates(next.pos)){
                            choices.push_back(next);
                            lasts.push_back(current);
                        }
                        // for(int q=0;q<choices.size();q++){
                        //     std::cout<<choices[q].pos << " | ";
                        // }
                        // std::cout << "\n";
                        // for(int q=0;q<lasts.size();q++){
                        //     std::cout<<lasts[q] << " | ";
                        // }
                        // std::cout << "\n";
                        // std::cin >> test;
                        break;
                    }
                    else if(countDots(next.pos)==2){
                        // std::cout << "continuing\n";
                        last = current;
                        current = next.pos;
                        i=0;
                        continue;
                    }
                    else{// < 2 dead end so reset to last choice currently exploring
                        // std::cout << "dead end\n";
                        current = choices[front].pos;
                        last = lasts[front];
                        break;
                    }
                }
                else{
                    // if(next.letter==' '){
                    //     std::cout<< "not a path\n";
                    // }
                    // else{
                    //     std::cout<<"came from there\n";
                    // }
                    if(current==choices[front].pos){
                        break;
                    }
                }
                i++;
            }
            choiceDirection++;
            if(choiceDirection>3){
                front++;
                choiceDirection=0;
            }
        }
        for(int j=0;j<choices.size();j++){
            at(choices[j].pos).letter = '0';
        }
        //fixify end cords
        //include portal connections
    }
    node& operator()(int row, int col){
        return info[row][col];
    }
    void print(){
        for(int r=0;r<rows;r++){
            for(int c=0;c<cols;c++){
                std::cout << info[r][c].letter;
            }
            std::cout << "\n";
        }
        for(int i=0;i<choices.size();i++){
            std::cout << choices[i] << " | ";
        }
        std::cout << start << '\n' << end << "\n";
    }
    ~matrix(){
        for(int i=0;i<rows;i++){
            delete[] info[i];
        }
        delete[] info;
    }
};


#endif