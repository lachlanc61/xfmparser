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
	uint64_t *indexes_p = static_cast<uint64_t *>(indexes_buf.ptr);
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
	uint64_t *indexes_p = static_cast<uint64_t *>(indexes_buf.ptr);
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
    uint64_t *result_ptr = static_cast<uint64_t *>(result_buf.ptr);

    cout << "===generating chan/det array===" << std::endl;
    for (size_t idx = 0; idx < X; idx++)
        for (size_t idy = 0; idy < Y; idy++)
            result_ptr[idx*Y + idy] = idx;

    result.resize({X,Y});

    return result;
}


py::array_t<uint16_t> readpixel1D(const char* stream, const int streamlen) 
/*
recieves numpy array of indexes and bytestream
prints byte value at each position in indexes_p
*/
{
    int NCOLS = 2;

    size_t X = streamlen/sizeof(uint16_t);
    //size_t Y = NCOLS;
    size_t Y = 1;

    py::array_t<uint16_t> result = py::array_t<uint16_t>(X*Y);

    auto result_buf = result.request();
    uint16_t *result_ptr = static_cast<uint16_t *>(result_buf.ptr);

    cout << "===reading pixel from stream===" << std::endl;

    auto voidPtr = static_cast<void const *>(stream);
    auto uintPtr = static_cast<uint16_t const *>(voidPtr);

    for (size_t i = 0; i < streamlen / sizeof(uint16_t); ++i) {
        result_ptr[i] = uintPtr[i];
    }

    return result;
}

std::vector<uint16_t> readpixelcounts(const char* stream, const uint64_t streamlen) 
/*
reads pixeldata stream as:
    uint16(chan) uint16(counts)
returns 1D numpy array of counts, w/ missing = 0

WARNING: currently system MUST BE little-endian 
*/
{
    //initialise result vector and pointers
    std::vector<uint16_t> result_vector(NCHAN, 0);

    //zeroresult array
    for (size_t i = 0; i < NCHAN; i++)
    {
        
        result_vector[i]=0;
    }

    //setup conversion casts
    auto voidPtr = static_cast<void const *>(stream);
    auto uintPtr = static_cast<uint16_t const *>(voidPtr);

    //fill result array from stream
    uint16_t channel = NCHAN+1;    //init to value outside array JIC
    for (size_t i = 0; i < streamlen / sizeof(uint16_t); ++i) {
        if ( i % sizeof(uint16_t) == 0 )
        { channel = uintPtr[i]; }
        else
        { result_vector[channel] = uintPtr[i]; }
    }

    for (size_t i = 140; i < 160; i++)
    {
        cout << "inner " << i << "  " << result_vector[i] << endl;
    }

    return result_vector;
}


py::array_t<uint16_t> readbuffer(const py::array_t<uint64_t> indexlist, const py::array_t<uint16_t> pxlens, const char* stream, const int streamlen) 
/*
reads pixeldata stream as:
    uint16(chan) uint16(counts)
returns 1D numpy array of counts, w/ missing = 0

WARNING: currently system MUST BE little-endian 
*/
{
    size_t npixels = 1; //start with 1 pixel for *=

    //set up pointers and info for input np arrays
    auto index_info = indexlist.request();
    uint64_t *index_ptr = static_cast<uint64_t *>(index_info.ptr);
    std::vector<ssize_t> index_shape = index_info.shape;

    auto pxlens_info = pxlens.request();
    uint16_t *pxlens_ptr = static_cast<uint16_t *>(pxlens_info.ptr);
    std::vector<ssize_t> pxlens_shape = pxlens_info.shape;

    //check shapes of pxlens and indexes match
    //get dimensionality and n. pixels 
    if ( index_shape == pxlens_shape ) {

        size_t ndim = index_shape.size(); 

        for ( size_t i=0; i < ndim; i++ ) {
            npixels *= index_shape[i];
        }
        cout << "----NPIXELS----" << endl;
        cout << npixels << endl;
    }
    else  {
        throw std::runtime_error("indexes and pixel-length arrays are not the same shape");
    }

    //initialise and zero result array
    py::array_t<uint16_t> full_result = py::array_t<uint16_t>(npixels*NDET*NCHAN);
    auto full_result_info = full_result.request();
    uint16_t *full_result_ptr = static_cast<uint16_t *>(full_result_info.ptr);

    //init temp result array 

    std::vector<uint16_t> working_result;

    cout << "----INNER----" << endl;
    //for ( size_t i = 0; i < npixels; i++ )
    for ( size_t i = 0; i < 1; i++ )
    {
        uint64_t index = index_ptr[i] + PXHEADERLEN;
        uint16_t length = pxlens_ptr[i] - PXHEADERLEN;
        //uint64_t nextstart = index_ptr[i+1];
        //uint64_t currend = index+length;
        //cout << "index " << index_ptr[i] << " len " << pxlens_ptr[i] << endl;
        //cout << "nextstart " << nextstart << " currend " << currend << endl;        
        const char* substream = stream + index;
        //cout << index_ptr[i] << endl;
        //py::array_t<uint16_t> 
        working_result = readpixelcounts(substream, length);
    }
    cout << "----OUTER----" << endl;
    for (size_t i = 140; i < 160; i++)
    {
        cout << "outer " << i << "  " << working_result[i] << endl;
    }

    return full_result;
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
        m.def("readpixelcounts", &readpixelcounts, "readpixelcounts");        
        m.def("readbuffer", &readbuffer, "readbuffer");   
}