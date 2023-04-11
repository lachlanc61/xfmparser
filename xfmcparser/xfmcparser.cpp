/* file : xfmcparser.cpp */
  
#include <iostream>
#include <vector>
#include <string>

//our header file
#include "xfmcparser.h"
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include "numpy/arrayobject.h"

//pybind11 includes
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


using std::cout;

//byte to int
inline unsigned int to_uint(char ch)
{
    //using namespace std;

    return static_cast<unsigned int>(static_cast<unsigned char>(ch));
}

//byte readout
char getbyte(const char* stream, int streamlen, int byteindex)
{

    cout << std::hex;
    cout << "C++ byte: 0x" << to_uint(stream[byteindex]) << "\n"; 
    
    return stream[byteindex];
}

//numpy readout
void mainline(double* dataPtr, int datasize) 
{
    for(size_t i = 0; i < datasize; ++i)
//        printf("[%zu] %lf\n",i,dataPtr[i]);
        cout << i << "\t" << dataPtr[i] << "\n";
}

/*
template <typename T>
py::array Vec2NpArray(std::vector<T> *data,
                      std::vector<size_t> shape) {

   // calculate stride of multidimensional data from shape
   std::vector<size_t> stride(shape.size(), 0);
   size_t elm_stride = sizeof(T);
   auto shape_it = shape.rbegin();
   auto stride_it = stride.rbegin();
   for (; stride_it != stride.rend(); stride_it++, shape_it++) {
      *stride_it = elm_stride;
      elm_stride *= *shape_it;
   }

   // tell python to delete pointer when deconstruction
   auto capsule = py::capsule(
       data, [](void *data) { delete reinterpret_cast<std::vector<T> *>(data); });

   return py::array_t<T>(shape, stride, data->data(), capsule);
}
*/

/*
py::array retarray(double* dataPtr, int datasize, const char* stream, int streamlen) 
/*
primary function recieving numpy array and bytestream
AND returning numpy array
https://stackoverflow.com/questions/72373186/how-to-return-numpy-array-by-reference-in-pybind11

{

    npy_intp dims[] = {datasize};

    PyObject* numpy_array = PyArray_SimpleNew(1, dims, NPY_INT); // create an uninitialized array of integers
    int* numpy_data = (int*)PyArray_DATA(numpy_array); // get a pointer to the array data

    cout << stream[1] << "\n";

    cout << "numpy array values:\n";
    for(size_t i = 0; i < datasize; ++i)
    {
//        printf("[%zu] %lf\n",i,dataPtr[i]);
        cout << i << "\t" << dataPtr[i] << "\n";
    }
    for (int i = 0; i < 5; i++)
    {
        numpy_data[i] = dataPtr[i]; // copy data from C++ array to NumPy array
    }

    // Return the NumPy array to Python
    return Py_BuildValue("O", numpy_array);
}
*/


void combine(double* dataPtr, int datasize, const char* stream, int streamlen, int byteindex) 
/*
primary function recieving numpy array and bytestream*/
{
    cout << std::hex;
    cout << "C++ byte: 0x" << to_uint(stream[byteindex]) << "\n"; 

    cout << "numpy array values:\n";
    for(size_t i = 0; i < datasize; ++i)
    {
//        printf("[%zu] %lf\n",i,dataPtr[i]);
        cout << i << "\t" << dataPtr[i] << "\n";
    }
}


int main()
{
    using namespace std;

    vector<string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};
    
    for (const string& word : msg)
    {
        cout << word << " ";
    }
    cout << endl;
}