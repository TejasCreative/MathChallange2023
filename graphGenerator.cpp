#include "matrix.h"
#include "coord.h"
#include "visual.h"
#include <chrono>
using namespace std;

int main(){
    matrix data;
    auto start = chrono::high_resolution_clock::now();
    // data.generateMaze(40);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    std::cout << "Generated Maze in " << duration.count() << " microseconds.\n";

    int n=0;
    start = chrono::high_resolution_clock::now();
    data.generate2d("AM_challenge23_input.txt");
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    std::cout << "Generated Matrix in " << duration.count() << " microseconds.\n";
    
    start = chrono::high_resolution_clock::now();
    data.findChoices();
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    std::cout << "Found " << data.choices.size() << " choices in " << duration.count() << " microseconds.\n";

    start = chrono::high_resolution_clock::now();
    n = data.trimGraph(3);
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    std::cout << "Eliminated " << n << " choices in " << duration.count() << " microseconds.\n";

    start = chrono::high_resolution_clock::now();
    data.solveMaze();
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    cout << "Solved Maze in: " << duration.count() << " microseconds.\n";
    cout << "Made " << data.T << " recursive calls.\n";
     
    start = chrono::high_resolution_clock::now();
    n = data.checkSolutions();
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    cout << "Verified " << n << " correct solutions out of " << data.solutionPaths.size() << " in " << duration.count() << " microseconds.\n";
    
    // // data.convertAdMatrix();
    // // data.displayAdjacencyMatrix("adjmatrix.txt");
    data.writeNodes("adjacencyList.txt");
    data.writeMap("map.txt");
    data.writeSolutionPaths("Solutions!.txt");

    data.writeCleanSolutionsPath("CleanSolutions!.txt");
    data.checkSolutions();
    data.prepVisual();
    Visual v;
    v.visualize("CleanSolutions!.txt");


    return 0;
}