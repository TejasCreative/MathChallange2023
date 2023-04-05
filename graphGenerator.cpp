#include "matrix.h"
#include "coord.h"
using namespace std;

int main(){
    matrix data;
    data.generate2d("AM_Challenge_packet_participant/AM_challenge23_input.txt");
    data.findChoices();
    data.convertAdMatrix();
    data.displayAdjacencyMatrix("adjmatrix.txt");
    data.trimGraph(3);
    data.writeNodes("adjacencyList.txt");
    data.writeMap("map.txt");
    return 0;
}