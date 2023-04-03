#include "matrix.h"
#include "coord.h"
using namespace std;

int main(){
    matrix data;
    data.generate2d("AM_Challenge_packet_participant/AM_challenge23_input.txt");
    data.findChoices();
    data.trimGraph(3);
    data.writeNodes("matrix.txt");
    data.writeMap("map.txt");
    data.convertAdMatrix();
    data.displayAdjacencyMatrix("adjmatrix.txt");
    return 0;
}