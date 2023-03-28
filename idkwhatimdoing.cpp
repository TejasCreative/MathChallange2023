#include <iostream>
using namespace std;
//n is the max length of the path (nxn for arr)
//i is the "choice index" we are currently on (A=0, B=1, C=2, ...)
//pnum= current path exploring
//j=current index of path we're on
//sum=array that has summed weight of each path
//global variables
//paths=number of paths currently
int paths=1;
int n=1;

void ඞ(int** arr, int i, int** a, int pnum, int j, int* sum)
{
    int count=0;
    if(i!=n)
    {
        for(int z=i+1; z<n-1; z++)
        {
            if(arr[i][z]!=-1)
            {
                if(count==0)
                {
                    sum[pnum]+=arr[i][z];
                    a[pnum][j]=z;
                    ඞ(arr, z, a, pnum, j+1, sum);
                }
                else
                {
                    paths++;
                    int* temp=new int[paths];
                    for(int x=0; x<paths; x++)
                    {
                        temp[x]=sum[x];
                    }
                    temp[paths-1]=0;
                    int* temp1=sum;
                    sum=temp;
                    delete temp1;
                    int** temp2=new int*[paths];
                    for(int x=0; x<paths-1; x++)
                    {
                        a[x]=new int[n];
                        for(int y=0; y<n; y++)
                        {
                            temp2[x][y]=a[x][y];
                        }
                    }
                    for(int y=0; y<n; y++)
                    {
                        temp[paths-1][y]=a[pnum][y];
                    }
                    int** temp3=a;
                    a=temp2;
                    delete temp3;
                    ඞ(arr, z, a, pnum+1, j+1, sum);
                }
                count++;
            }
        }
    }
}

//replicate this part in main
int main()
{
    int** arr;
    int sum[paths];
    sum[0]=0;
    //int n=2;
    int** a=new int*[paths];
    a[paths-1]=new int[n];
    for(int i=0; i<n; i++)
    {
        a[0][i]=-1;
    }
    a[0][0]=0;
    ඞ(arr, 0, a, 0, 1, sum);
    return 0;
}