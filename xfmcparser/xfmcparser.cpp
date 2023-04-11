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
    cout << "C++ byte: 0x" << to_uint(stream[byteindex]) << std::endl; 
    
    return stream[byteindex];
}

//numpy readout
double mainline(double* dataPtr, int datasize) 
{
    for(size_t i = 0; i < datasize; ++i)
//        printf("[%zu] %lf\n",i,dataPtr[i]);
        cout << i << "\t" << dataPtr[i] << std::endl;

    return(dataPtr[2]);
}

//numpy readout
unsigned long long indexin(uint64_t* index_p, int indexsize) 
{
    unsigned long long retval;

    for(size_t i = 0; i < indexsize; ++i)
    {
//        printf("[%zu] %lf\n",i,dataPtr[i]);
        retval = index_p[i];
    //    cout << i << "\t" << retval << std::endl;
        printf("[%zu] %llu\n", i, retval);   //need to use printf, uint64 too large for cout apparently
    }
    return(retval);
}



namespace py = pybind11;
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

/*
int createnp() {
    // Initialize data pointer
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int* data_ptr = data.data();

    // Initialize shape pointer
    std::vector<size_t> shape = {data.size()};
    size_t* shape_ptr = shape.data();

    // Convert vector to numpy array
    auto np_array = Vec2NpArray<int>(&data, &shape);

    // Print numpy array
    for (int i = 0; i < data.size(); i++) {
        std::cout << np_array.at<int>(i) << " ";
    }

    return 0;
}
*/

void combine(double* dataPtr, int datasize, const char* stream, int streamlen, int byteindex) 
/*
primary function recieving numpy array and bytestream*/
{
    cout << std::hex;
    cout << "C++ byte: 0x" << to_uint(stream[byteindex]) << std::endl; 

    cout << "numpy array values:\n";
    for(size_t i = 0; i < datasize; ++i)
    {
//        printf("[%zu] %lf\n",i,dataPtr[i]);
        cout << i << "\t" << dataPtr[i] << std::endl;
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