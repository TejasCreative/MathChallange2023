#include "matrix.h"
using namespace std;

int main(){
    matrix data;
    data.generate2d("AM_Challenge_packet_participant/AM_challenge23_input.txt");
    data.findChoices();
    data.writeNodes("matrix.txt");
    data.writeMap("map.txt");
    return 0;
}