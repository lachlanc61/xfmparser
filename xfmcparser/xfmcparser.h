#ifndef MYLIBRARY_H
#define MYLIBRARY_H

/*
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
*/
char getbyte(const char* stream, int streamlen, int byteindex);

double doubleprint(double* dataPtr, int datasize);

unsigned long indexret(uint64_t* indexes_p, int indexes_size);

/*
//setup pybind11 - not working currently, possibly pybind library not being passed to swig?

namespace py = pybind11;

template <typename T>
py::array Vec2NpArray(std::vector<T> *data, std::vector<size_t> shape);
*/

//PyObject* retarray(double* dataPtr, int datasize, const char* stream, int streamlen) 

char indexbyte(uint64_t* indexes_p, int indexes_size, const char* stream, int streamlen);

#endif // MYLIBRARY_H