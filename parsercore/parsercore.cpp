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

//byte readout
void printbyte(const char* stream, int streamlen, int byteindex)
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


void ptrtrial() 
{
    int val = 10;
    int *p = &val;
   // int *p = 10;    
    cout << "reference" <<endl;
    cout << val <<endl;
    cout << &val << endl;

    cout << "pointer" <<endl;
    cout << p << endl;
    cout << *p << endl;
    cout << &p << endl;

}

void indexbyte(const py::array_t<uint64_t> indexes, const char* stream, int streamlen) 
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

py::array_t<uint64_t> getchanarray(const py::array_t<uint64_t> indexes, const char* stream, int streamlen) 
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

    py::array_t<uint64_t> result = py::array_t<uint64_t>(NCHAN);

    auto result_buf = result.request();
    uint64_t *result_ptr = (uint64_t *) result_buf.ptr;
    size_t X = NCHAN;
    size_t Y = NDET;

    cout << "---generating chan/det array---" << std::endl;
    for (size_t idx = 0; idx < X; idx++)
        for (size_t idy = 0; idy < Y; idy++)
            result_ptr[idx*Y + idy] = idx;

    result.resize({X,Y});

    return result;
}






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

/* Wrapping routines with PyBind */
PYBIND11_MODULE(parsercore_lib, m) {
	    m.doc() = ""; // optional module docstring
	    m.def("pbtest", &pbtest, "pbtest array");
	    m.def("indexbyte", &indexbyte, "indexbyte array");
        m.def("doubleprint", &doubleprint, "doubleprint array");
        m.def("indexret", &indexret, "indexret array");
        m.def("getchanarray", &getchanarray, "getchanarray");
        m.def("printbyte", &printbyte, "printbyte");
        m.def("ptrtrial", &ptrtrial, "ptrtrial");
}