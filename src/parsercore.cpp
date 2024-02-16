/* file : parsercore.cpp */

#include <iostream>
#include <vector>
#include <string>
#include <tuple>

#include <typeinfo>
#include <type_traits>

//our header file
//#include "xfmcparser.h"

//pybind11 includes
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;

using std::cout;
using std::endl;

bool const DEBUG = false;
int const NCHAN = 4096;
int const NDET = 2;
int const PXHEADERLEN = 16;
int const NDIM = 3;
std::string const VERSION = "0.0.1";


struct EndianError : public std::exception {
   const char * what () const throw () {
      return "Incompatible system byte-order - must be little-endian";
   }
};

struct ArrayContiguousError : public std::exception {
   const char * what () const throw () {
      return "Numpy array not C-contiguous";
   }
};

struct ArrayShapeError : public std::exception {
   const char * what () const throw () {
      return "Input array shapes differ";
   }
};


bool is_big_endian(void)
/*
checks endianness of system
*/
{
    union {
        uint32_t i;
        char c[4];
    } binary = {0x01020304};

    return binary.c[0] == 1;
}

bool check_system()
/*
checks system compatability
*/
{
    if ( is_big_endian() )
    {
        throw EndianError();
        //"v " + VERSION + 
    }
    else
    {
        return true;
    }
}

#define C_CONTIGUOUS py::detail::npy_api::constants::NPY_ARRAY_C_CONTIGUOUS_

bool check_ccontig(const py::array nparray)
/*
checks array compatability
WARNNIG: might trigger on 1D array
*/
{

    if (!(C_CONTIGUOUS == (nparray.flags() & C_CONTIGUOUS))) {
        throw ArrayContiguousError();
    }
    else
    {
        return true;
    }

    //alternative: cast F-contig to C-contig:
    // Actual casting, notice that type needs to be known and adding `c_style` is required to force memory alignment
    //auto c_array = nparray.cast<py::array_t<double, py::array::c_style | py::array::forcecast>>();

}


bool check_inputs(const py::array_t<uint64_t> indexlist, const py::array_t<uint16_t> pxlens)
/*
check the inputs for compatability
*/
{
    bool system = check_system();
    bool np1 = check_ccontig(indexlist);
    bool np2 = check_ccontig(pxlens);
    bool shapes1 = false;

    //array shapes
    auto index_info = indexlist.request();
    std::vector<ssize_t> index_shape = index_info.shape;

    auto pxlens_info = pxlens.request();
    std::vector<ssize_t> pxlens_shape = pxlens_info.shape;
    
    if (index_shape == pxlens_shape)
    {
        shapes1 = true;
    }
    else
    {
        throw ArrayShapeError();
    }

    if (system && np1 && np2 && shapes1)
    {
        return true;
    }
    else
    {
        return false;
    }
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
    if (DEBUG == true)
    {
        for (size_t i = 140; i < 160; i++)
        {
            cout << "inner " << i << "  " << result_vector[i] << endl;
        }
    
        cout << "----BREAK----" << endl;
    }
    return result_vector;
}


py::array_t<uint16_t> readstream(const py::array_t<uint64_t> indexlist, const py::array_t<uint16_t> pxlens, const py::bytes in_stream, const int streamlen) 
/*
reads pixeldata stream as:
    uint16(chan) uint16(counts)
returns 1D numpy array of counts, w/ missing = 0

WARNING: currently system MUST BE little-endian 
*/
{

    //check inputs and system for compatability
    bool accepted = check_inputs(indexlist, pxlens);

    std::string str_stream = in_stream;  // Convert py::bytes to std::string
    const char* stream = str_stream.data();
    //set up pointers and info for input np arrays
    
    //index array
    auto index_info = indexlist.request();
    uint64_t *index_ptr = static_cast<uint64_t *>(index_info.ptr);
    std::vector<ssize_t> index_shape = index_info.shape;

    //pixel length array
    auto pxlens_info = pxlens.request();
    uint16_t *pxlens_ptr = static_cast<uint16_t *>(pxlens_info.ptr);
    std::vector<ssize_t> pxlens_shape = pxlens_info.shape;

    //check shapes of pxlens and indexes match
    //  & get dimensionality and n. pixels 

    size_t ndim;
    size_t npixels;
    if ( index_shape == pxlens_shape ) {
        ndim = index_shape.size(); 
        npixels = index_shape[0];
    }
    else  {
        throw std::runtime_error("indexes and pixel-length arrays are not the same shape");
    }
    if (DEBUG == true) { cout << "----DIMS-" << npixels << "--" << endl; }


    size_t npoints = npixels*NDET*NCHAN;
    size_t nspectra = npixels*NDET;

    //initialise result arrays
    std::vector<uint16_t> full_result(npixels*NDET*NCHAN, 5);   //init to "5" so different to zeros produced by empty steam
    std::vector<uint16_t> working_result;

    //loop through all indexed pixels 
    //  and read each into result array
    for ( size_t i = 0; i < nspectra; i++ )    
    {
        if (DEBUG == true) { cout << "----INNER-" << i << "--" << endl; }

        uint64_t index = index_ptr[i] + PXHEADERLEN;
        uint16_t length = pxlens_ptr[i] - PXHEADERLEN;  
        const char* substream = stream + index;

        //read pixel
        working_result = readpixelcounts(substream, length);

        if (i % 10000 == 0)
            {
                std::cout << "Iteration: " << i << "of " << nspectra << std::endl;
            }

        if (DEBUG == true) 
        { 
            cout << "----OUTER-" << i << "--" << endl; 

            for (size_t j = 140; j < 160; j++)
            {
                cout << "outer " << j << "  " << working_result[j] << endl;
            }
        }
        //copy to result array
        std::copy(working_result.begin(), working_result.end(), full_result.begin()+i*NCHAN);
    }

    //initialise python export params
    //init shape
    std::vector<ssize_t> result_shape(NDIM);
    result_shape[0] = npixels;
    result_shape[1] = NDET;
    result_shape[2] = NCHAN;
    std::vector<ssize_t> result_strides(NDIM);
    //init strides
    result_strides[0] = result_shape[1]*result_shape[2]*sizeof(uint16_t);
    result_strides[1] = result_shape[2]*sizeof(uint16_t);
    result_strides[2] = sizeof(uint16_t);        
    uint16_t* pointer=full_result.data();

    //return as python object  
    return pybind11::array_t<uint16_t>(
        result_shape,
        result_strides,
        pointer); 
}

/* Wrapping routines with PyBind */
PYBIND11_MODULE(parsercore, m) {
	    m.doc() = ""; // optional module docstring
        m.def("readpixelcounts", &readpixelcounts, "pixel cound reader");        
        m.def("readstream", &readstream, "primary stream reader");   
}