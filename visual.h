#ifndef VISUAL_H
#define VISUAL_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <tuple>
#include <sys/stat.h>
#include <sys/types.h>

#include "coord.h"
#include "matrix.h"
#include "node.h"


class Visual{
    public:
        std::vector<coord> shift;
        coord start;
        Visual(){
            //std::cout << "Visual constructor" << std::endl;
        }
        void replicate(std::string filename){
            std::string line;
            std::ifstream file{"AM_challenge23_input.txt"};
            std::ofstream output{filename};
            
            std::cout << filename << std::endl;
            if(file.is_open()){
                while(file.good()){
                    std::getline(file,line);
                    std::cout << line << std::endl;
                    output << line << "\n";
                }
            }
            else{
                std::cout << "File not found" << std::endl;
            }
            output << "0" << "\n";
            file.close();
            output.close();

        }
        void setter(){
            std::ifstream file{"info.txt"};
            std::string line;
            coord temp;
            for(int i=0;i<4;i++){
                file >> temp.col >> temp.row;
                shift.push_back(temp);
            }
            file >> start.col >> start.row;
            file.close();
            for(int i=0;i<shift.size();i++){
                std::cout << shift[i] << std::endl;
            }
            std::cout << start << std::endl;
        
        }
        void draw(std::string path, std::string filename){
            std::ofstream file{filename};
            coord temp = start;
            node* next;
            vector<vector <int> > positions;
            int x = start.col;
            int y = start.row;
            
            

        // }

        void visualize(std::string filename){
            setter();
            std::ifstream file;
            file.open(filename);
            std::string line;
            int x;
            std::vector<std::tuple<int, std::string> > lines;
            if(file.is_open()){
                while(file.good()){
                    file >> x;
                    file >> line;
                    lines.emplace_back(x,line);
                }
            }
            else{
                std::cout << "File not found" << std::endl;
            }
            file.close();
            std::vector<std::tuple<int, std::string> > smallest;
            smallest.emplace_back(lines[0]);
            for(int i=1;i<lines.size();i++){
                if(std::get<0>(lines[i]) < std::get<0>(smallest[0])){
                    smallest.clear();
                    smallest.emplace_back(lines[i]);
                }
                else if(std::get<0>(lines[i]) == std::get<0>(smallest[0])){
                    smallest.emplace_back(lines[i]);
                }
            }
            std::vector<std::tuple<int, std::string> > longest;
            longest.emplace_back(lines[0]);
            for(int i=1;i<lines.size();i++){
                if(std::get<0>(lines[i]) > std::get<0>(longest[0])){
                    longest.clear();
                    longest.emplace_back(lines[i]);
                }
                else if(std::get<0>(lines[i]) == std::get<0>(longest[0])){
                    longest.emplace_back(lines[i]);
                }
            }
            replicate("overview.rtf");
            // drawOverview(smallest,longest, "overview.txt");

            //insert code to map on overview

            mkdir("Solutions", 0777);
            for(int i = 0; i < lines.size(); i++){
                replicate("Solutions/Solution_" + std::to_string(i+1) + ".rtf");
                draw(std::get<1>(lines[i]), "Solutions/Solution_" + std::to_string(i+1) + ".rtf");
            }
            std::cout << "Visualize complete" << std::endl;



        }




};




#endif