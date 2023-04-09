#ifndef VISUAL_H
#define VISUAL_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "coord.h"
#include "matrix.h"
#include "node.h"


class Visual{
    public:
        Visual(){
            //std::cout << "Visual constructor" << std::endl;

        }
        void visualize(std::string filename){
            std::fstream file;
            file.open(filename);
            std::string line;
            std::vector<std::string> lines;
            while(std::getline(file, line)){
                lines.push_back(line);
            }
            file.close();
            for(int i = 0; i < lines.size(); i++){
                std::cout << lines[i] << std::endl;
            }

        }



};




#endif