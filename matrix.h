#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <fstream>
#include "node.h"
#include "coord.h"
#include <string>
#include <vector>

struct matrix{
    int rows;
    int cols;
    coord start;
    coord end;
    node** info;
    std::vector<node> choices;
    matrix(){
        rows=0;
        cols=0;
        info=nullptr;
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
                    start.row = row;
                    start.col = col;
                }
                else if(line[col]=='Z'){
                    end.row = row;
                    end.col = col;
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
    void findChoices(){
        
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
            std::cout << choices[i] << "\n";
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