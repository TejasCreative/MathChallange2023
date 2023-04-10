#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <fstream>
#include "node.h"
#include "coord.h"
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_set>

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
    std::vector<std::string> solutionPaths;
    std::unordered_set<coord,coordHash> visited;
    std::vector<std::string> bestSolutions;
    matrix(){
        rows=0;
        cols=0;
        info=nullptr;
        start=nullptr;
        end=nullptr;
        adjmat=nullptr;
        shift.emplace_back(-1,0,"");
        shift.emplace_back(0,1,"");
        shift.emplace_back(1,0,"");
        shift.emplace_back(0,-1,"");
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
                        node portal(row,col,line[col]);
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
    std::string flipPath(std::string s){
        std::string r = "";
        for(int i=s.length()-1;i>=0;i--){
            if(s[i]!='4'){
                r+=(char)((((s[i]-48)+2)%4)+48);
            }
            else{
                r+='4';
            }
        }
        return r;
    }
    void findChoices(){
        
        choices.push_back(start->pos);
        coord current,last;
        node* next;
        int i,front=0,choiceDirection =0;
        while(front<choices.size()){//breadth first
            std::string line = "";
            current = choices[front];
            i=choiceDirection;
            while(i<5){//depth first
                if(i==4){
                    if(at(current).letter=='P'){
                        next = &at(searchPortals(current)); //if currently at a portal go to the other portal
                    }
                    else{
                        break;
                    }
                }
                else{
                    next = &at(current+shift[i]);  
                    while(next->letter=='_'){
                        next = &at(next->pos+shift[i]);
                        

                    }
                }
                if((next->letter=='.' || next->letter=='P' || next->letter=='*' || next->letter=='Z') && !(next->pos==last)){
                    line+=(char)(i+48);
                    if(next->letter=='.'){ 
                        next->letter=',';//visited
                    }
                    if(next->letter=='*' || next->letter=='Z' || countOptions(next->pos)>2){//found a choice node
                        if(next->letter!='*'){                          //if a new choice node
                            choices.push_back(next->pos);
                            next->letter='*';
                        }
                        at(choices[front]).add(next->pos,line);
                        next->add(choices[front],flipPath(line));
                        break;
                    }
                    else if(countOptions(next->pos)==2){//only one option to continue
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
                at(vertex->edges[i]).add(vertex->edges[j],flipPath(vertex->edges[i].path) + vertex->edges[j].path, true);
            }
            for(int j=i+1;j<vertex->edges.size();j++){
                at(vertex->edges[i]).add(vertex->edges[j],flipPath(vertex->edges[i].path) + vertex->edges[j].path,true);
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
                adjmat[choice][n] = at(choices[choice]).edges[edge].path.length();
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
    void searchFrom(coord c, std::string path){
        visited.emplace(c.row,c.col,"");
        path+=c.path; //+ "|"
        if((c==end->pos)){
            solutionPaths.push_back(path);
        }
        for(int i=0;i<at(c).edges.size();i++){
            if(visited.find(at(c).edges[i])==visited.end()){
                searchFrom(at(c).edges[i],path);
            }
        }
        visited.erase(c);
    }
    void solveMaze(){
        searchFrom(start->pos,"");
        visited.clear();
        int min = 0;
        for(int i=1;i<solutionPaths.size();i++){
            if(solutionPaths[i].size() < solutionPaths[min].size()){
                min = i;
            }
        }
        for(int i=min;i<solutionPaths.size();i++){
            if(solutionPaths[i].size()==solutionPaths[min].size()){
                bestSolutions.push_back(solutionPaths[i]);
            }
        }
    }
    void writeSolutionPaths(std::string filename){
        std::ofstream mf;
        mf.open(filename);
        mf << "There are " << solutionPaths.size() << " paths.\n\n";
            mf << "The shortest path has " << bestSolutions[0].size() << " steps. They are: \n";
        for(int i=0;i<bestSolutions.size();i++){
            mf << bestSolutions[i] << "\n";
        }
        mf << "\n";
        for(int i=0;i<solutionPaths.size();i++){
            mf << "Solution " << i << " has " << solutionPaths[i].size() << " steps.\n";
            mf << solutionPaths[i] << "\n\n";
        }
        mf.close();
    }
    void writeCleanSolutionsPath(std::string filename){
        std::ofstream mf;
        mf.open(filename);
        for(int i=0;i<solutionPaths.size();i++){
            mf << solutionPaths[i].size() << "\n";
            mf << solutionPaths[i] << "\n";
        }
        mf.close();
    }
    bool verify(std::string path){
        std::cout << "Verifying " << path << "\n";
        visited.clear();
        coord myPosition = start->pos;
        char c;
        for(int i=0;i<path.size();i++){
            if(visited.find(myPosition)!=visited.end()){
                return false;
            }
            visited.insert(myPosition);
            c = path[i];
            if(c=='4'){
                myPosition = searchPortals(myPosition);
            }
            else{
                do{
                    myPosition = myPosition + shift[(int)(c-48)];
                }while(at(myPosition).letter=='_');
            }
        }
        visited.clear();
        return myPosition==end->pos;
    }
    void checkSolutions(){
        for(int i=0;i<solutionPaths.size();i++){
            if(!verify(solutionPaths[i])){
                std::cout << i << " Failed\n";
            }
        }
    }

    void prepVisual(){
        for(int i = 0; i < shift.size(); i++){
            std::cout << shift[i] << "\n";
        }
        std::cout << "Start: " << start->pos << "\n";
        std::ofstream mf;
        mf.open("info.txt");
        //write shift vector
        for(int i=0;i<shift.size();i++){
            mf << shift[i].col << " " << shift[i].row << "\n";  
        }
        //write start
        mf << start->pos.col << " " << start->pos.row << "\n";

    
    
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