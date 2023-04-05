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

    for(int i=0;i<data.choices.size();i++){
        cout << data.choices[i] << endl;
    }


    for(int i=0;i<data.shift.size();i++){
        cout << data.shift[i] << endl;
    }
    
    
    return 0;
}