/* file : xfmcparser.cpp */
  
#include <iostream>
#include <vector>
#include <string>
#include <tuple>

#include <typeinfo>
#include <type_traits>

//our header file
//#include "xfmcparser.h"

//#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
//#include "numpy/arrayobject.h"

//pybind11 includes
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;

using std::cout;
using std::endl;

int const NCHAN = 4096;
int const NDET = 2;
int const PXHEADERLEN = 16;


//byte to int
inline unsigned int to_uint(char ch)
{
    //using namespace std;

    return static_cast<unsigned int>(static_cast<unsigned char>(ch));
}


inline unsigned int to_uint16(char ch)
{
    //using namespace std;

    return static_cast<uint16_t>(static_cast<unsigned char>(ch));
}


//byte readout
void printbyte(const char* stream, const int streamlen, int byteindex)
{
    cout << typeid(stream[byteindex]).name() << std::endl;   
    cout << std::hex;
    cout << "C++ byte: 0x" << to_uint(stream[byteindex]) << std::endl; 

    //not sure how to return to python as a bytes object yet
    //char should work but need to convert on python end somehow
    //not important for application so leave it
}

//numpy readout
double doubleprint(py::array_t<double> data) 
{
	/*  read input arrays buffer_info */
	auto buf1 = data.request();

	/*  variables */
	double *ptr1 = (double *) buf1.ptr;
	size_t N = buf1.shape[0];

    for(int i = 0; i < int(N); ++i)
        cout << i << "\t" << ptr1[i] << std::endl;

    return(ptr1[2]);
}


//numpy readout
//unsigned long indexret(const uint64_t* indexes_p, int indexes_size) 
unsigned long indexret(py::array_t<uint64_t> indexes) 
{

	/*  read input arrays buffer_info */
	auto indexes_buf = indexes.request();

	/*  variables */
	uint64_t *indexes_p = (uint64_t *) indexes_buf.ptr;
	size_t indexes_size = indexes_buf.shape[0];

    // vectors are declared via start and end+1 pointers
    //warning: creatng a vector copies the array
    std::vector<uint64_t> d_vector(indexes_p, indexes_p + indexes_size);

    cout << "cpp input numpy" << std::endl;
    cout << indexes_p << std::endl;
    cout << typeid(indexes_p).name() << std::endl;

    for(size_t i = 0; i < indexes_size; ++i)
    {
        printf("[%zu] %lu\n", i, indexes_p[i]);   //need to use printf, uint64 too large for cout apparently
    }

    //int *p = &d_vector[0]

    cout << "cpp vector" << std::endl;
    cout << &d_vector[0] << std::endl;
    cout << typeid(d_vector).name() << std::endl;
    cout << typeid(&d_vector[0]).name() << std::endl;

    for(size_t i = 0; i < indexes_size; ++i)
    {
        printf("[%zu] %lu\n", i, d_vector[i]);   //need to use printf, uint64 too large for cout apparently
    }

    cout << d_vector.size() << std::endl;
    //cout << indexes_p.size();

    return(indexes_p[indexes_size-1]);
}


void indexbyte(const py::array_t<uint64_t> indexes, const char* stream, const int streamlen) 
//char indexbyte(const uint64_t* indexes_p, int indexes_size, const char* stream, int streamlen, uint64_t* data_p, int data_size) 
/*
recieves numpy array of indexes and bytestream
prints byte value at each position in indexes_p
*/
{

    /*  read input arrays buffer_info */
	auto indexes_buf = indexes.request();

	/*  variables */
	uint64_t *indexes_p = (uint64_t *) indexes_buf.ptr;
	size_t indexes_size = indexes_buf.shape[0];

    cout << "---C++ values---" << std::endl;
    for (size_t i = 0 ; i < indexes_size ; i++ )
    {
        printf("[%zu] %lu", i, indexes_p[i]);
        cout << std::hex;
        cout << " 0x" << to_uint(stream[indexes_p[i]]) << std::endl;
    }
}

inline unsigned short swap_16bit(unsigned short us)
{
    return (unsigned short)(((us & 0xFF00) >> 8) |
                            ((us & 0x00FF) << 8));
}


#include <limits.h>

uint16_t bytes_to_int_little_endian(const char* bytes)
{
    int i;
    int result;

    result = 0;
    for (i = 0; i < sizeof(uint16_t); ++i)
        result += bytes[i] << (i * CHAR_BIT);
    return result;
}


py::array_t<uint16_t> readpixel(const char* stream, const int streamlen) 
/*
recieves numpy array of indexes and bytestream
prints byte value at each position in indexes_p
*/
{
    int NCOLS = 2;

    size_t X = NCHAN;
    //size_t Y = NCOLS;

    py::array_t<uint16_t> result = py::array_t<uint16_t>(X*2);

    auto result_buf = result.request();
    uint16_t *result_ptr = (uint16_t *) result_buf.ptr;

    //const uint16_t n = *(reinterpret_cast<const uint16_t *>(stream));

    cout << "---reading pixel from stream---" << std::endl;

    //for ( int i = 0 ; i < streamlen-1 ; i+=2 )
    //for ( int i = 0 ; i < 49 ; i+=2 )
    for ( int i = 0 ; i < 49 ; i++ )
    {
        /*
        uint16_t resval = 0;
        for (int i = 0; i < sizeof(uint16_t); ++i)
        {
            result_ptr[j] += stream[j+i] << (i * CHAR_BIT);        //little endian
        }
        */
        //std::cout << i << " = " << std::bitset<8>(stream[i])  << std::endl;
        result_ptr[i] = to_uint16(stream[i]);
        //result_ptr[i] = *n[i]
        //result_ptr[i] = (stream[i] << 8) | stream[i+1] ;
        cout << result_ptr[i] << endl;
    }

    return result;
}



py::array_t<uint64_t> returnchanarray(const py::array_t<uint64_t> indexes, const char* stream, int streamlen) 
/*
recieves numpy array of indexes and bytestream
prints byte value at each position in indexes_p
*/
{

    /*  read input arrays buffer_info */
	auto indexes_buf = indexes.request();

    size_t X = NCHAN;
    size_t Y = NDET;

    py::array_t<uint64_t> result = py::array_t<uint64_t>(X*Y);

    auto result_buf = result.request();
    uint64_t *result_ptr = (uint64_t *) result_buf.ptr;

    cout << "---generating chan/det array---" << std::endl;
    for (size_t idx = 0; idx < X; idx++)
        for (size_t idy = 0; idy < Y; idy++)
            result_ptr[idx*Y + idy] = idx;

    result.resize({X,Y});

    return result;
}


/*
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
*/


/* Wrapping routines with PyBind */
PYBIND11_MODULE(parsercore_lib, m) {
	    m.doc() = ""; // optional module docstring
	    m.def("indexbyte", &indexbyte, "indexbyte array");
        m.def("doubleprint", &doubleprint, "doubleprint array");
        m.def("indexret", &indexret, "indexret array");
        m.def("returnchanarray", &returnchanarray, "returnchanarray");
        m.def("printbyte", &printbyte, "printbyte");
        m.def("readpixel", &readpixel, "readpixel");
}