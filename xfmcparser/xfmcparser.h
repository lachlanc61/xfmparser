#ifndef MYLIBRARY_H
#define MYLIBRARY_H

/*
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
*/
char getbyte(const char* stream, int streamlen, int byteindex);

double mainline(double* dataPtr, int datasize);

unsigned long long indexin(uint64_t* index_p, int indexsize);

/*
//setup pybind11 - not working currently, possibly pybind library not being passed to swig?

namespace py = pybind11;

template <typename T>
py::array Vec2NpArray(std::vector<T> *data, std::vector<size_t> shape);
*/

//PyObject* retarray(double* dataPtr, int datasize, const char* stream, int streamlen) 

void combine(double* dataPtr, int datasize, const char* stream, int streamlen, int byteindex);

#endif // MYLIBRARY_H