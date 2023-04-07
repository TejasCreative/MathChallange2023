#include "matrix.h"
#include "coord.h"
#include <chrono>
using namespace std;

int main(){
    matrix data;
    data.generate2d("AM_challenge23_input.txt");
    data.findChoices();
    auto start = chrono::high_resolution_clock::now();
    data.trimGraph(3);
    data.solveMaze();
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    cout << "\nMicroseconds taken to trim graph and then solve: " << duration.count() << "\n";
    data.convertAdMatrix();
    data.displayAdjacencyMatrix("adjmatrix.txt");
    data.writeNodes("adjacencyList.txt");
    data.writeMap("map.txt");
    data.writeSolutionPaths("Solutions!.txt");
    cout << data.checkSolutions() << " correct solutions out of " << data.solutionPaths.size() <<" found\n\n";
    return 0;
}