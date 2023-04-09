#include "matrix.h"
#include "coord.h"
#include "visual.h"
using namespace std;

int main(){
    matrix data;
    data.generate2d("AM_Challenge_packet_participant/AM_challenge23_input.txt");
    data.findChoices();
    data.trimGraph(3);
    data.convertAdMatrix();
    data.displayAdjacencyMatrix("adjmatrix.txt");
    data.writeNodes("adjacencyList.txt");
    data.writeMap("map.txt");
    data.solveMaze();
    data.writeSolutionPaths("Solutions!.txt");
    data.writeCleanSolutionsPath("CleanSolutions!.txt");
    data.checkSolutions();
    Visual v;
    v.visualize("CleanSolutions!.txt");

    return 0;
}