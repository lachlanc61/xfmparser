/* file : xfmcparser.cpp */
  
#include <iostream>
#include <vector>
#include <string>

//our header file
#include "xfmcparser.h"
  
using namespace std;

//byte example
char getbyte(const char* data, int len)
{
    return data[1];
}

//numpy example
void mainline(double* dataPtr, int datasize) 
{
    for(size_t i = 0; i < datasize; ++i)
//        printf("[%zu] %lf\n",i,dataPtr[i]);
        cout << i << "\t" << dataPtr[i] << "\n";
}


void combine(double* dataPtr, int datasize, const char* stream, int streamlen) 
/*
primary function recieving numpy array and bytestream*/
{
    cout << stream[1] << "\n";

    cout << "numpy array values:\n";
    for(size_t i = 0; i < datasize; ++i)
    {
//        printf("[%zu] %lf\n",i,dataPtr[i]);
        cout << i << "\t" << dataPtr[i] << "\n";
    }
}


int main()
{
    vector<string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};
    
    for (const string& word : msg)
    {
        cout << word << " ";
    }
    cout << endl;
}