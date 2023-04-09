#ifndef VISUAL_H
#define VISUAL_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <tuple>

#include "coord.h"
#include "matrix.h"
#include "node.h"


class Visual{
    public:
        Visual(){
            //std::cout << "Visual constructor" << std::endl;
        }
        void visualize(std::string filename){
            std::ifstream file;
            file.open(filename);
            std::string line;
            int x;
            std::vector<std::tuple<int, std::string> > lines;
            if(file.is_open()){
                while(file.good()){
                    file >> x;
                    file >> line;
                    std::cout << x << " " << line << std::endl;
                    lines.emplace_back(x,line);
                }
            }
            else{
                std::cout << "File not found" << std::endl;
            }
            std::cout << "Size of lines: " << lines.size() << std::endl;
            file.close();
            for(auto i : lines){
                std::cout << std::get<0>(i) << " " << std::get<1>(i) << std::endl;
            }
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
            std::cout << "Size of smallest: " << smallest.size() << std::endl;
            for(auto i : smallest){
                std::cout << std::get<0>(i) << " " << std::get<1>(i) << std::endl;
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
            std::cout << "Size of longest: " << longest.size() << std::endl;
            for(auto i : longest){
                std::cout << std::get<0>(i) << " " << std::get<1>(i) << std::endl;
            }

            

        }



};




#endif