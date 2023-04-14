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
    cout << "=" << typeid(stream[byteindex]).name() << std::endl;   
    cout << std::hex;
    cout << "===C++ byte: 0x" << to_uint(stream[byteindex]) << std::endl; 

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
        cout << "=" << i << "\t" << ptr1[i] << std::endl;

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

    cout << "===cpp input numpy" << std::endl;
    cout << "=" << indexes_p << std::endl;
    cout << "=" << typeid(indexes_p).name() << std::endl;

    for(size_t i = 0; i < indexes_size; ++i)
    {
        printf("[%zu] %lu\n", i, indexes_p[i]);   //need to use printf, uint64 too large for cout apparently
    }

    //int *p = &d_vector[0]

    cout << "===cpp vector" << std::endl;
    cout << "=" << &d_vector[0] << std::endl;
    cout << "=" << typeid(d_vector).name() << std::endl;
    cout << "=" << typeid(&d_vector[0]).name() << std::endl;

    for(size_t i = 0; i < indexes_size; ++i)
    {
        printf("[%zu] %lu\n", i, d_vector[i]);   //need to use printf, uint64 too large for cout apparently
    }

    cout << "=" << d_vector.size() << std::endl;
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

    cout << "===C++ values===" << std::endl;
    for (size_t i = 0 ; i < indexes_size ; i++ )
    {
        cout << "= 0x";
        printf("[%zu] %lu", i, indexes_p[i]);
        cout << std::hex;
        cout << to_uint(stream[indexes_p[i]]) << std::endl;
        cout << std::dec;
    }
}

inline unsigned short swap_16bit(unsigned short us)
{
    return (unsigned short)(((us & 0xFF00) >> 8) |
                            ((us & 0x00FF) << 8));
}


void printpixel(const char* stream, const int streamlen)
/*
    minimal converter, prints out series of uint16_t from raw bytes
    https://stackoverflow.com/questions/47816805/c-how-to-cast-multiple-bytes-from-char-into-one-integer

    NOTE: system MUST BE little-endian at the moment
        https://stackoverflow.com/questions/19751130/how-to-read-big-endian-integers-from-file-in-c
    possible solutions in OP comments:
        https://stackoverflow.com/questions/66762830/reading-little-endian-from-binary-file-into-integers-in-c
        length = sizeof(int); my_input_file.read(reinterpret_cast<char*>(&num), length);
        OR
        Read into a uint8_t bytes[4] then compute the result as bytes[0] + (bytes[1]<<8) + (bytes[2]<<16) + ...).
*/
{
    auto voidPtr = static_cast<void const *>(stream);
    auto intPtr = static_cast<uint16_t const *>(voidPtr);

    for (size_t i = 0; i < streamlen / sizeof(uint16_t); ++i) {
    cout << "uint16_t: " << intPtr[i] << endl;
    }
}


py::array_t<uint16_t> readpixel1D(const char* stream, const int streamlen) 
/*
recieves numpy array of indexes and bytestream
prints byte value at each position in indexes_p
*/
{
    int NCOLS = 2;

    size_t X = streamlen/2;
    //size_t Y = NCOLS;
    size_t Y = 1;

    py::array_t<uint16_t> result = py::array_t<uint16_t>(X*Y);

    auto result_buf = result.request();
    uint16_t *result_ptr = (uint16_t *) result_buf.ptr;

    cout << "===reading pixel from stream===" << std::endl;

    auto voidPtr = static_cast<void const *>(stream);
    auto intPtr = static_cast<uint16_t const *>(voidPtr);

    for (size_t i = 0; i < streamlen / sizeof(uint16_t); ++i) {
        result_ptr[i] = intPtr[i];
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

    cout << "===generating chan/det array===" << std::endl;
    for (size_t idx = 0; idx < X; idx++)
        for (size_t idy = 0; idy < Y; idy++)
            result_ptr[idx*Y + idy] = idx;

    result.resize({X,Y});

    return result;
}

/* Wrapping routines with PyBind */
PYBIND11_MODULE(parsercore_lib, m) {
	    m.doc() = ""; // optional module docstring
	    m.def("indexbyte", &indexbyte, "indexbyte array");
        m.def("doubleprint", &doubleprint, "doubleprint array");
        m.def("indexret", &indexret, "indexret array");
        m.def("returnchanarray", &returnchanarray, "returnchanarray");
        m.def("printbyte", &printbyte, "printbyte");
        m.def("printpixel", &printpixel, "printpixel");
        m.def("readpixel1D", &readpixel1D, "readpixel1D");
}