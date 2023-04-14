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
        int size;
        std::string secret = "span style=\"color:#00FF00\">.</span>";
        Visual(){
            //std::cout << "Visual constructor" << std::endl;
        }
        void replicate(std::string filename){
            std::string line;
            std::ifstream file{"AM_challenge23_input.txt"};
            std::ofstream output{filename};
            
            if(file.is_open()){
                while(file.good()){
                    std::getline(file,line);
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
            std::ifstream file2{"AM_challenge23_input.txt"};
            std::string line2;
            int x;
            if(file2.is_open()){
                while(file2.good()){
                    std::getline(file2,line2);
                    x++;
                }
            }
            else{
                std::cout << "File not found" << std::endl;
            }
            file2.close();
            size = x;
        
        }
        void draw(std::string path, std::string filename){
            std::cout << "Drawing" << std::endl;
            std::cout << size << std::endl;
            std::cout << path << std::endl;
            std::ofstream file{filename};
            coord temp = start;
            node* next;
            std::vector<std::vector<int> > decisions;
            int y = start.col;
            int x = start.row;
            
            for(int i=0;i<size;i++){
                std::vector<int> temp;
                decisions.push_back(temp);
            }
            decisions[x].push_back(y);
            
            for(int i=0;i<path.size();i++){
                if(path[i] == '0'){
                    x--;
                }
                else if(path[i] == '1'){
                    y++;
                }
                else if(path[i] == '2'){
                    x++;
                }
                else if(path[i] == '3'){
                    y--;
                }
                if(path[i] == '4'){
                    break;
                }
                std::cout << x << " " << y << std::endl;
                decisions[x].push_back(y);
            }



            //sort each vector in decisions
            for(int i=0;i<decisions.size();i++){
                std::sort(decisions[i].begin(), decisions[i].end());
            }

            file << "<!DOCTYPE html> \n <html> \n head> \n Solution \n </head> \n <body> \n";
            std::ifstream file2{"AM_challenge23_input.txt"};
            std::string line;
            int count = 0;
            for(int i = 0; i< decisions.size();i++){
                file << "<pre>";
                for(int j=0;j<decisions[i].size();j++){
                    while(count < decisions[i][j]){
                        std::getline(file2,line);
                        file << line << "\n";
                        count++;
                    }
                    std::getline(file2,line);
                    file << line.substr(0,decisions[i][j]) << secret << line.substr(decisions[i][j]+1) << "\n";
                    count++;
                }
                while(count < size){
                    std::getline(file2,line);
                    file << line << "\n";
                    count++;
                }
                file << "</pre> \n";
                count = 0;
            }
            file << "</body> \n </html>";
            


            


            
        
            
            

        }

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
            replicate("overview.html");
            // drawOverview(smallest,longest, "overview.txt");

            //insert code to map on overview

            mkdir("Solutions", 0777);
            for(int i = 0; i < lines.size(); i++){
                replicate("Solutions/Solution_" + std::to_string(i+1) + ".html");
                draw(std::get<1>(lines[i]), "Solutions/Solution_" + std::to_string(i+1) + ".html");
            }
            std::cout << "Visualize complete" << std::endl;



        }




};




#endif