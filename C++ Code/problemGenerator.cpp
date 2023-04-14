#include <fstream>
#include <stdlib.h>
using namespace std;


int main(){
    //generate maps like AM_Challenge_packet_participant/AM_challenge23_input.txt

    ofstream mf;
    mf.open("AM_Challenge_packet_participant/AM_challenge23_input.txt");
    int n=100;
    int w=100;
    for(int row=0;row<n;row++){
        for(int col=0;col<w;col++){
            if(rand()%2==0){
                mf << '#';
            }
            else{
                mf << ' ';
            }
        }
        mf << endl;
    }
    mf.close();
    
    
    


    return 0;
}