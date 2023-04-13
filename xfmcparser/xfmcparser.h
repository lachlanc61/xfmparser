#ifndef MYLIBRARY_H
#define MYLIBRARY_H


//#include <vector>
/*
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
*/


char getbyte(const char* stream, int streamlen, int byteindex);

double doubleprint(double* dataPtr, int datasize);

unsigned long indexret(const uint64_t* indexes_p, int indexes_size);

//namespace py = pybind11;

//void display(py::array_t<double> input1);

/*
//setup pybind11 - not working currently, possibly pybind library not being passed to swig?

namespace py = pybind11;

template <typename T>
py::array Vec2NpArray(std::vector<T> *data, std::vector<size_t> shape);
*/

//PyObject* retarray(double* dataPtr, int datasize, const char* stream, int streamlen) 

char indexbyte(const uint64_t* indexes_p, int indexes_size, const char* stream, int streamlen);


void ptrtrial();

#endif // MYLIBRARY_H