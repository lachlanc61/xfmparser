/* file : xfmcparser.cpp */
  
#include <iostream>
#include <vector>
#include <string>

//pybind11 includes
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;

using std::cout;
using std::endl;

int const NCHAN = 4096;
int const NDET = 2;
int const PXHEADERLEN = 16;


/* C++ function */
//void pbtest(py::array_t<double> input1) 
void pbtest(pybind11::array_t<double> input1)
{

	/*  read input arrays buffer_info */
	auto buf1 = input1.request();

	/*  variables */
	double *ptr1 = (double *) buf1.ptr;
	size_t N = buf1.shape[0];

	for ( int i = 0; i < N; i++ )
	{
		printf("val[%d] = %f \n", i, ptr1[i]);
	}

}

/* Wrapping routines with PyBind */
PYBIND11_MODULE(wrapper, m) {
	    m.doc() = ""; // optional module docstring
	    m.def("pbtest", &pbtest, "pbtest array");
}