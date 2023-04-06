#include <iostream>
using namespace std;
//n is the max length of the path, number of choices bascally (nxn for arr)
//i is the "choice index" we are currently on (A=0, B=1, C=2, ...)
//pnum= current path exploring
//j=current index of path we're on
//sum=array that has summed weight of each path
//a is the 2D array of paths
//if a[pnum][0] and sum[pnum] are -1 its a dead end path, should be removed at the end of this code
//global variables
//paths=number of paths currently
int paths=1;
int n=1;

void ඞ(int** arr, int i, int** a, int pnum, int j, int* sum)
{
    int count=0;
    //make sure we're not at the end
    if(i!=n)
    {
        for(int z=0; z<n; z++)
        {
            int check=0;
            //check if its a choice A=A index
            if(i!=z)
            {
                //check if choice connection exists
                if(arr[i][z]!=-1)
                {
                    //check if z has appeared in the path before
                    for(int v=0; v<=j; v++)
                    {
                        if(a[pnum][v]==z)
                        {
                            check=1;
                        }
                    }
                    if(check==0)
                    {
                        //if first branch of pathing we dont create new path
                        if(count==0)
                        {
                            sum[pnum]+=arr[i][z];
                            a[pnum][j]=z;
                            ඞ(arr, z, a, pnum, j+1, sum);
                        }
                        //if >1st branch of path we redefine sum and a to fit
                        else
                        {
                            // temp and temp1 are used to swap sum around, and temp1 and temp2 are used to swap path around
                            paths++;
                            int* temp=new int[paths];
                            //copy over sum
                            for(int x=0; x<paths; x++)
                            {
                                temp[x]=sum[x];
                            }
                            temp[paths-1]=0;
                            int* temp1=sum;
                            sum=temp;
                            delete temp1;
                            int** temp2=new int*[paths];
                            //copy over a
                            for(int x=0; x<paths-1; x++)
                            {
                                //temp2 changed from a fix
                                temp2[x]=new int[n];
                                for(int y=0; y<n; y++)
                                {
                                    temp2[x][y]=a[x][y];
                                }
                            }
                            //copy current path into new a via duplication
                            for(int y=0; y<n; y++)
                            {
                                temp2[paths-1][y]=a[pnum][y];
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
        //dead end check
        if (count==0)
        {
            sum[pnum]=-1;
            a[pnum][0]=-1;
        }
    }
}

//clean up a array via copying non dead end paths
void Ω(int** a, int* sum)
{
    //# of real paths
    int real=0;
    for(int i=0; i<paths; i++)
    {
        if(sum[i]!=-1)
        {
            real++;
        }
    }
    int* temp=new int[real];
    //keep track of real sum index currently copying
    int count=0;
    //copy over sum
    for(int x=0; x<paths; x++)
    {
        if(sum[x]!=-1)
        {
            temp[count]=sum[x];
            count++;
        }
    }
    int* temp1=sum;
    sum=temp;
    delete temp1;
    //reset count
    count=0;
    int** temp2=new int*[real];
    //copy over a
    for(int x=0; x<paths-1; x++)
    {
        if(a[x][0]!=-1)
        {
            temp2[count]=new int[n];
            for(int y=0; y<n; y++)
            {
                temp2[count][y]=a[x][y];
            }
            count++;
        }
    }
    int** temp3=a;
    a=temp2;
    delete temp3;
    //set paths to correct value of real paths (without dead ends)
    paths=real;
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
    Ω(a, sum);
    return 0;
}