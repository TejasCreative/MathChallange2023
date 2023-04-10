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
#include <random>
#include <chrono>
#include <thread>

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
    std::vector<std::string> LongestSolutions;
    std::string homeStretch;
    int T;
    matrix(){
        rows=0;
        cols=0;
        T=0;
        homeStretch = "";
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
        if(searchChoices(end->pos)!=-1){
            choices.erase(choices.begin()+searchChoices(end->pos));
            choices.push_back(end->pos);
        }
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
        if (vertex->letter=='*'){
            vertex->letter=',';
        }
    }
    int trimGraph(int size){
        int n=0;
        if(choices.back()==end->pos){
            for(int i=choices.size()-2;i>0;i--){//skip A and Z (0 and end)
                if(at(choices[i]).edges.size()<size){
                    prune(&at(choices[i]));
                    n++;
                }
            }
            if(end->edges.size()==1){           //prune Z if necessary
                homeStretch = flipPath(end->edges[0].path);
                node* temp = &(at(end->edges[0]));
                prune(end);
                n++;
                end->letter = 'Z';
                end = temp;
            }
            //could safely prune these loops
            // prune(&at(choices[20]));
            // prune(&at(choices[18]));
            // prune(&at(choices[17]));
            // prune(&at(choices[16]));
            // prune(&at(choices[7]));
            // prune(&at(choices[5]));
        }
        return n;
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
    void searchFrom(coord c, std::string path){ //T(C) C=choices.size()
        T++;
        visited.emplace(c.row,c.col,""); //O(1)
        path+=c.path; //+ "|"             //O(L) L total length of strings
        if((c==end->pos)){                 //O(1)
            path+=homeStretch;
            solutionPaths.push_back(path);
        }
        else{
            for(int i=0;i<at(c).edges.size();i++){          //edges.size() <= 4
                if(visited.find(at(c).edges[i])==visited.end()){        //false for at least 1 of 3
                    searchFrom(at(c).edges[i],path);            //recursive
                }
            }
        }
        visited.erase(c);
    }
    void solveMaze(){
        if(searchChoices(end->pos)!=-1){
            searchFrom(start->pos,"");
            int min = 0;
            int max = 0;
            for(int i=1;i<solutionPaths.size();i++){
                if(solutionPaths[i].size() < solutionPaths[min].size()){
                    min = i;
                }
                if(solutionPaths[i].size() > solutionPaths[max].size()){
                    max = i;
                }
            }
            for(int i=0;i<solutionPaths.size();i++){
                if(solutionPaths[i].size()==solutionPaths[min].size()){
                    bestSolutions.push_back(solutionPaths[i]);
                }
                if(solutionPaths[i].size()==solutionPaths[max].size()){
                    LongestSolutions.push_back(solutionPaths[i]);
                }
            }
        }
    }
    void writeSolutionPaths(std::string filename){
        std::ofstream mf;
        mf.open(filename);
        mf << "There are " << solutionPaths.size() << " paths.\n\n";
        if(solutionPaths.size()>0){
            mf << "The shortest path has " << bestSolutions[0].size() << " steps. They are: \n";
            for(int i=0;i<bestSolutions.size();i++){
                mf << bestSolutions[i] << "\n";
            }
            mf << "\nThe longest path has " << LongestSolutions[0].size() << " steps. They are: \n";
            for(int i=0;i<LongestSolutions.size();i++){
                mf << LongestSolutions[i] << "\n";
            }
            mf << "\n";
            for(int i=0;i<solutionPaths.size();i++){
                mf << "Solution " << i << " has " << solutionPaths[i].size() << " steps.\n";
                mf << solutionPaths[i] << "\n\n";
            }
        }
        mf.close();
    }
    bool verify(std::string path){
        visited.clear();
        coord myPosition = start->pos;
        for(int i=0;i<path.size();i++){
            if(visited.find(myPosition)!=visited.end()){
                return false;
            }
            visited.insert(myPosition);
            if(path[i]=='4'){
                myPosition = searchPortals(myPosition);
            }
            else{
                do{
                    myPosition = myPosition + shift[(int)(path[i]-48)];
                }while(at(myPosition).letter=='_');
            }
        }
        visited.clear();
        return at(myPosition).letter=='Z';
    }
    int checkSolutions(){
        std::unordered_set<std::string> verified;
        verified.reserve(solutionPaths.size());
        int n=0;
        for(int i=0;i<solutionPaths.size();i++){
            //each solution should be unique, not cross itself, and go from A to Z
            if(verified.find(solutionPaths[i])==verified.end() && verify(solutionPaths[i])){
                verified.insert(solutionPaths[i]);
                n++;
            }
        }
        return n;
    }
    bool checkPlaceability(coord c, char** arr, int n, int forward){
        coord check;
        for(int i=0;i<4;i++){
            check = c+shift[i];
            if(i==(forward+2)%4 || (i==forward && rand()%1000<200)){
                continue;
            }
            if(check.row >= n || check.row < 0 || check.col >= n || check.col <0 || arr[check.row][check.col]=='.'){
                // std::cout << " can't place at: " << c << " because " << check << "\n"; 
                return false;
            }
        }
        return true;
    }
    void generateMaze(int n){
        char** arr = new char*[n];
        for(int i=0;i<n;i++){
            arr[i]= new char[n];
            for(int j=0;j<n;j++){
                arr[i][j] = '#';
            }
        }
        std::ofstream mf;
        mf.open("newMaze.txt");
        for(int r=0;r<n;r++){
            for(int c=0;c<n;c++){
                mf << arr[r][c];
            }
            mf << "\n";
        }
        mf.close();

        int precision = 1000000;
        int probabilityOfIntersection = 100000;
        int probabilityOfQuadIntersection = 40000;
        int probabilityOfTurning = 110000;

        coord begin(1,rand() % n,"123");
        std::vector<coord> intersections;
        intersections.push_back(begin);
        coord current, next;
        int i=1,front=0,choiceDirection =0,length=0;
        while(front<intersections.size()){//breadth first
            current = intersections[front];
            i=(int)current.path[choiceDirection]-48;
            length=0;
            while(true){
                next = current+shift[i];
                if( next.row <= 0 || next.row >= n-1 || next.col >= n-1 || next.col<=0 || arr[next.row][next.col]=='.'){//if no new directions for some reason...
                    // std::cout << "out of bounds or next is a period\n";
                    break;
                }
                bool canPlace = checkPlaceability(next, arr, n, i);
                // std::cout << current;
                if(canPlace){
                    length++;
                    arr[next.row][next.col]='.';
                    if(rand()%precision < probabilityOfIntersection*(length)){//new intersection or dead end
                        int back = (i+2)%4;
                        std::string direction = std::to_string(back);
                        if(rand() % precision<probabilityOfQuadIntersection){
                            direction = "0123";
                        }
                        else{
                            int whichPaths = rand() % 3;
                            if(whichPaths==0){
                                direction+=std::to_string((1+back)%4);
                                direction+=std::to_string((2+back)%4);
                            }
                            else if(whichPaths==1){
                                direction+=std::to_string((1+back)%4);
                                direction+=std::to_string((3+back)%4);
                            }
                            else{
                                direction+=std::to_string((2+back)%4);
                                direction+=std::to_string((3+back)%4);
                            }
                        }
                        next.path = direction;
                        // std::cout << " intersection at " << next << "\n";
                        intersections.push_back(next);
                        break;
                    }
                    // std::cout << " continued " << i << "\n";
                    current = next;
                }
                if(!canPlace || rand() % precision < probabilityOfTurning*length){//turning
                    i = (i+((rand() % 3)+3))%4;
                    // std::cout << " turning to " << i << "\n"; 
                    length=0;
                }
                // mf.open("newMaze.txt");
                // for(int r=0;r<n;r++){
                //     for(int c=0;c<n;c++){
                //         mf << arr[r][c];
                //     }
                //     mf << "\n";
                // }
                // mf.close();
                // std::chrono::milliseconds duration(60);
                // std::this_thread::sleep_for(duration);
                // std::string test;
                // std::cin >> test;
            }
            choiceDirection++;
            if(choiceDirection>=intersections[front].path.size()){
                front++;
                choiceDirection=0;
            }
        }
        std::cout << intersections.size() << "\n";
        arr[begin.row][begin.col] = 'A';
        arr[begin.row-1][begin.col] = 'A';
        std::vector<int> possibleEnds;
        for(int i=1;i<n-1;i++){
            if(arr[n-2][i]=='.'){
                possibleEnds.push_back(i);
                // std::cout << possibleEnds.back() << "\n";
            }
        }
        coord ending(n-2,possibleEnds[rand() % possibleEnds.size()],"");
        arr[ending.row][ending.col] = 'Z';
        arr[ending.row+1][ending.col] = 'Z';
        
        //label portal pairs
        // for(int j=0;j<portals.size();j++){
        //     at(portals[j].pos).letter = (char)(66+j);
        //     at(portals[j].edges[0]).letter = (char)(66+j);
        // }
        // end->letter='Z';

        mf.open("newMaze.txt");
        for(int r=0;r<n;r++){
            for(int c=0;c<n;c++){
                mf << arr[r][c];
            }
            mf << "\n";
        }
        mf.close();
        for(int i=0;i<n;i++){
            delete[] arr[i];
        }
        delete[] arr;
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
/*
#############
##C#F#D#H#J##
#BA........B#
##.#.#.#.#.##
#G.........G#
##.#.#.#.#.##
#E.........E#
##.#.#.#.#.##
#K.........K#
##.#.#.#.#.##
#I........ZI#
##C#F#D#H#J##
#############

###########
##C#F#D#H##
#BA......B#
##.#.#.#.##
#G.......G#
##.#.#.#.##
#E.......E#
##.#.#.#.##
#I......ZI#
##C#F#D#H##
###########

#########
##C#F#D##
#BA....B#
##.#.#.##
#G.....G#
##.#.#.##
#E....ZE#
##C#F#D##
#########

#######
##C#D##
#BA..B#
##.#.##
#E..ZE#
##C#D##
#######
*/