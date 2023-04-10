/* file : xfmcparser.cpp */
  
#include <iostream>
#include <vector>
#include <string>

//our header file
#include "xfmcparser.h"
  
//double tdouble = 3.4;

// return first byte
char getbyte(const char* data, int len)
{
    return data[1];
}
  
//find mod
int my_mod(int n, int m)
{
  return(n % m);
}

//numpy example
void mainline(double* dataPtr, int datasize, char* headerPtr, int headersize) {
    for(size_t i = 0; i < datasize; ++i)
        printf("[%zu] %lf\n",i,dataPtr[i]);
    for(size_t i = 0; i < headersize; ++i)
        printf("[%zu] %hhd\n",i,headerPtr[i]);
}


using namespace std;

int main()
{
    vector<string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};
    
    for (const string& word : msg)
    {
        cout << word << " ";
    }
    cout << endl;
}