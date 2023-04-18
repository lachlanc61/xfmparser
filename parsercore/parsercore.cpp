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


py::array_t<uint16_t> readstream(const py::array_t<uint64_t> indexlist, const py::array_t<uint16_t> pxlens, const char* stream, const int streamlen) 
/*
reads pixeldata stream as:
    uint16(chan) uint16(counts)
returns 1D numpy array of counts, w/ missing = 0

WARNING: currently system MUST BE little-endian 
*/
{
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
PYBIND11_MODULE(parsercore_lib, m) {
	    m.doc() = ""; // optional module docstring
        m.def("readpixelcounts", &readpixelcounts, "readpixelcounts");        
        m.def("readstream", &readstream, "readstream");   
}