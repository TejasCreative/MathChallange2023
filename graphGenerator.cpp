#include "matrix.h"
#include "coord.h"
using namespace std;

int main(){
    matrix data;
    data.generate2d("AM_Challenge_packet_participant/AM_challenge23_input.txt");
    data.findChoices();
    data.writeNodes("matrix.txt");
    data.writeMap("map.txt");
    data.convertAdMatrix();
    data.displayAdjacencyMatrix("adjmatrix.txt");

    pair<int**,coord*> adjmat = data.makeAdjacencyMatrix();
    for (int i = 0; i < data.choices.size(); i++) {
        for (int j = 0; j < data.choices.size(); j++) {
            cout << adjmat.first[i][j] << " ";
        }
        cout << endl;
    }
    cout << "Coords: " << endl;
    for(int i=0;i<data.choices.size();i++){
        cout << "(" << adjmat.second[i].row << "," << adjmat.second[i].col << "," << adjmat.second[i].dist << ")" << endl;
    }
    return 0;
}