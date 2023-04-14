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
        std::vector<std::tuple<coord, coord> > portals;
        int size;
        std::string secret = "<span style=\"color:#00FF00\">.</span>";
        std::string green = "<span style=\"color:#00FF00\">.</span>";
        std::string red = "<span style=\"color:#FF0000\">.</span>";
        std::string purple = "<span style=\"color:#BF40BF\">.</span>";

        Visual(){
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
            int xx, yy, aa, bb;
            file >> xx >> yy >> aa >> bb;
            portals.push_back(std::make_tuple(coord(xx,yy, ""),coord(aa,bb, "")));
            while(file >> xx >> yy >> aa >> bb){
                for(int i = 0; i< portals.size();i++){
                    if(std::get<0>(portals[i])==coord(xx,yy,"") || std::get<1>(portals[i])==coord(xx,yy,"")){
                        break;
                    }
                }
                portals.push_back(std::make_tuple(coord(xx,yy, ""),coord(aa,bb, "")));
            }
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
            char initial = path[0];
            char last = path[path.size()-1];
            path = initial + path + last;

            
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
                    bool found = false;
                    for(int j=0;j<portals.size();j++){
                        if(std::get<0>(portals[j]) == coord(x,y,"")){
                            x = std::get<1>(portals[j]).row;
                            y = std::get<1>(portals[j]).col;
                            found = true;
                            break;
                        }
                    }
                    if(!found){
                        x++;
                        for(int j=0;j<portals.size();j++){
                            if(std::get<0>(portals[j]) == coord(x,y,"")){
                                x = std::get<1>(portals[j]).row;
                                y = std::get<1>(portals[j]).col;
                                found = true;
                                break;
                            }
                        }
     
                    }
                    if(!found){
                        x--;
                        x--;
                        for(int j=0;j<portals.size();j++){
                            if(std::get<0>(portals[j]) == coord(x,y,"")){
                                x = std::get<1>(portals[j]).row;
                                y = std::get<1>(portals[j]).col;
                                found = true;
                                break;
                            }
                        }
     
                    }
                    if(!found){
                        x++;
                        y++;
                        for(int j=0;j<portals.size();j++){
                            if(std::get<0>(portals[j]) == coord(x,y,"")){
                                x = std::get<1>(portals[j]).row;
                                y = std::get<1>(portals[j]).col;
                                found = true;
                                break;
                            }
                        }
     
                    }
                    if(!found){
                        y--;
                        y--;
                        for(int j=0;j<portals.size();j++){
                            if(std::get<0>(portals[j]) == coord(x,y,"")){
                                x = std::get<1>(portals[j]).row;
                                y = std::get<1>(portals[j]).col;
                                found = true;
                                break;
                            }
                        }
     
                    }
                }
                
                decisions[x].push_back(y);
                
            }


            //sort each vector in decisions backwards
            for(int i=0;i<decisions.size();i++){
                std::sort(decisions[i].begin(), decisions[i].end());
                std::reverse(decisions[i].begin(), decisions[i].end());
            }

            file << "<!DOCTYPE html> \n <html> \n <body> \n";
            std::ifstream file2{"AM_challenge23_input.txt"};
            std::string line;
            for(int i = 0; i< decisions.size();i++){
                file << "<pre>";
                getline(file2,line);
                    for(int j = 0; j< decisions[i].size();j++){
                        line = line.substr(0,decisions[i][j]) + secret + line.substr(decisions[i][j]+1);
                        
                    }
                if(!file.good()){
                    line = "";
                }
                file << line;
                file << "</pre> \n";

            }
            file << "</body> \n </html>";
            file.close();
            file2.close();
            


            


            
        
            
            

        }
        void drawOverview(std::string path, std::string longPath, std::string filename){
            std::ofstream file{filename};
            coord temp = start;
            node* next;
            std::vector<std::vector<int> > greenDecisions;
            std::vector<std::vector<int> > redDecisions;
            std::vector<std::vector<int> > purpleDecisions;
            int y = start.col;
            int x = start.row;
            
            for(int i=0;i<size;i++){
                std::vector<int> temp;
                greenDecisions.push_back(temp);
                redDecisions.push_back(temp);
                purpleDecisions.push_back(temp);
            }
            greenDecisions[x].push_back(y);
            char initial = path[0];
            char last = path[path.size()-1];
            path = initial + path + last;

            
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
                else if(path[i] == '4'){
                    bool found = false;
                    for(int j=0;j<portals.size();j++){
                        if(std::get<0>(portals[j]) == coord(x,y,"")){
                            x = std::get<1>(portals[j]).row;
                            y = std::get<1>(portals[j]).col;
                            found = true;
                            break;
                        }
                    }
                    if(!found){
                        x++;
                        for(int j=0;j<portals.size();j++){
                            if(std::get<0>(portals[j]) == coord(x,y,"")){
                                x = std::get<1>(portals[j]).row;
                                y = std::get<1>(portals[j]).col;
                                found = true;
                                break;
                            }
                        }
     
                    }
                    if(!found){
                        x--;
                        x--;
                        for(int j=0;j<portals.size();j++){
                            if(std::get<0>(portals[j]) == coord(x,y,"")){
                                x = std::get<1>(portals[j]).row;
                                y = std::get<1>(portals[j]).col;
                                found = true;
                                break;
                            }
                        }
     
                    }
                    if(!found){
                        x++;
                        y++;
                        for(int j=0;j<portals.size();j++){
                            if(std::get<0>(portals[j]) == coord(x,y,"")){
                                x = std::get<1>(portals[j]).row;
                                y = std::get<1>(portals[j]).col;
                                found = true;
                                break;
                            }
                        }
     
                    }
                    if(!found){
                        y--;
                        y--;
                        for(int j=0;j<portals.size();j++){
                            if(std::get<0>(portals[j]) == coord(x,y,"")){
                                x = std::get<1>(portals[j]).row;
                                y = std::get<1>(portals[j]).col;
                                found = true;
                                break;
                            }
                        }
     
                    }
                }
                
                greenDecisions[x].push_back(y);
                
            }
            std::cout << "green done" << std::endl;
            
            //finish green
            bool instart = false;
            //start red
            temp = start;
            x = start.row;
            y = start.col;
            purpleDecisions[x].push_back(y);
            initial = longPath[0];
            last = longPath[longPath.size()-1];
            longPath = initial + longPath + last;
            for(int i =0; i < longPath.size();i++){
                
                if(longPath[i] == '0'){
                    x--;
                }
                else if(longPath[i] == '1'){
                    y++;
                }
                else if(longPath[i] == '2'){
                    x++;
                }
                else if(longPath[i] == '3'){
                    y--;
                }
                else if(path[i] == '4'){
                    bool found = false;
                    for(int j=0;j<portals.size();j++){
                        if(std::get<0>(portals[j]) == coord(x,y,"")){
                            x = std::get<1>(portals[j]).row;
                            y = std::get<1>(portals[j]).col;
                            found = true;
                            break;
                        }
                    }
                    if(!found){
                        x++;
                        for(int j=0;j<portals.size();j++){
                            if(std::get<0>(portals[j]) == coord(x,y,"")){
                                x = std::get<1>(portals[j]).row;
                                y = std::get<1>(portals[j]).col;
                                found = true;
                                break;
                            }
                        }
     
                    }
                    if(!found){
                        x--;
                        x--;
                        for(int j=0;j<portals.size();j++){
                            if(std::get<0>(portals[j]) == coord(x,y,"")){
                                x = std::get<1>(portals[j]).row;
                                y = std::get<1>(portals[j]).col;
                                found = true;
                                break;
                            }
                        }
     
                    }
                    if(!found){
                        x++;
                        y++;
                        for(int j=0;j<portals.size();j++){
                            if(std::get<0>(portals[j]) == coord(x,y,"")){
                                x = std::get<1>(portals[j]).row;
                                y = std::get<1>(portals[j]).col;
                                found = true;
                                break;
                            }
                        }
     
                    }
                    if(!found){
                        y--;
                        y--;
                        for(int j=0;j<portals.size();j++){
                            if(std::get<0>(portals[j]) == coord(x,y,"")){
                                x = std::get<1>(portals[j]).row;
                                y = std::get<1>(portals[j]).col;
                                found = true;
                                break;
                            }
                        }
     
                    }
                }

                instart = false;
                std::cout << path[i] << std::endl;
                std::cout << "x: " << x << " y: " << y << std::endl;



                for(int j = 0; j < greenDecisions[x].size();j++){
                    if(greenDecisions[x][j] == y){
                        purpleDecisions[x].push_back(y);
                        instart = true;
                    }
                }
                if(!instart){
                    redDecisions[x].push_back(y);
                }
            }

            
            std::cout << "Decisions" << std::endl;
            //sort each vector in decisions backwards
            for(int i=0;i<greenDecisions.size();i++){
                std::sort(greenDecisions[i].begin(), greenDecisions[i].end());
                std::reverse(greenDecisions[i].begin(), greenDecisions[i].end());
            }
            for(int i=0;i<redDecisions.size();i++){
                std::sort(redDecisions[i].begin(), redDecisions[i].end());
                std::reverse(redDecisions[i].begin(), redDecisions[i].end());
            }
            for(int i=0;i<purpleDecisions.size();i++){
                std::sort(purpleDecisions[i].begin(), purpleDecisions[i].end());
                std::reverse(purpleDecisions[i].begin(), purpleDecisions[i].end());
            }
            
            int sizer = 0;
            std::string line2 = ""; 
            file << "<!DOCTYPE html> \n <html> \n <body> \n";
            std::ifstream file2{"AM_challenge23_input.txt"};
            std::string line;
            for(int i = 0; i< greenDecisions.size();i++){
                file << "<pre>";
                getline(file2,line);
                    //set sizer to longest vector
                    if(greenDecisions[i].size() >= redDecisions[i].size() && greenDecisions[i].size() >= purpleDecisions[i].size()){
                        sizer = greenDecisions[i].size();
                    }
                    else if(redDecisions[i].size() >= greenDecisions[i].size() && redDecisions[i].size() >= purpleDecisions[i].size()){
                        sizer = redDecisions[i].size();
                    }
                    else{
                        sizer = purpleDecisions[i].size();
                    }
                    for(int j = 0; j< sizer; j++){
                        line2 = "";
                        line2 += line[j];
                        if(j < greenDecisions[i].size()){
                            if(greenDecisions[i][j] == j){
                                line2 += "<span style=\"color:green\">*</span>";
                            }
                            else{
                                line2 += "<span style=\"color:green\"> </span>";
                            }
                        }
                        else{
                            line2 += "<span style=\"color:green\"> </span>";
                        }
                        if(j < redDecisions[i].size()){
                            if(redDecisions[i][j] == j){
                                line2 += "<span style=\"color:red\">*</span>";
                            }
                            else{
                                line2 += "<span style=\"color:red\"> </span>";
                            }
                        }
                        else{
                            line2 += "<span style=\"color:red\"> </span>";
                        }
                        if(j < purpleDecisions[i].size()){
                            if(purpleDecisions[i][j] == j){
                                line2 += "<span style=\"color:purple\">*</span>";
                            }
                            else{
                                line2 += "<span style=\"color:purple\"> </span>";
                            }
                        }
                        else{
                            line2 += "<span style=\"color:purple\"> </span>";
                        }




                    }
                if(!file.good()){
                    line = "";
                }
                file << line2;
                file << "</pre> \n";

            }
            file << "</body> \n </html>";
            file.close();
            file2.close();
            


            


            
        
            
            

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
            drawOverview(std::get<1>(smallest[0]), std::get<1>(longest[0]), "overview.html");

            mkdir("Solutions", 0777);
            for(int i = 0; i < lines.size(); i++){
                replicate("Solutions/Solution_" + std::to_string(i+1) + ".html");
                draw(std::get<1>(lines[i]), "Solutions/Solution_" + std::to_string(i+1) + ".html");
            }
            std::cout << "Visualize complete" << std::endl;



        }




};




#endif