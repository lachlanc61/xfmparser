/* file : xfmcparser.i */

/* name of module to use*/
%module xfmcparser
%{
    /* Every thing in this file is being copied in 
     wrapper file. We include the C header file necessary
     to compile the interface */
    #include "xfmcparser.h"

    //numpy init
    #define SWIG_FILE_WITH_INIT
    
    //treat bytes as char*
    #define SWIG_PYTHON_STRICT_BYTE_CHAR

%}

/*recieve <bytes> objects as strings
%begin %{

%}*/

/*  
%include "typemaps.i"

%apply (char *STRING, size_t LENGTH) { (const char* data, int len) }
*/



%include "numpy.i"
%init %{
import_array();
%}

%apply (double* IN_ARRAY1, int DIM1) {(double* dataPtr, int datasize)}
//%apply (char* IN_ARRAY1, int DIM1) {(char* headerPtr, int headersize)}
//%apply (signed char* IN_ARRAY1, int DIM1) {(char* headerPtr, int headersize)}

%include "xfmcparser.h"


/*char getbyte(const char* data, int len);
double tdouble;*/

/* explicitly list functions and variables to be interfaced 

long long int fact(long long int n1);
int my_mod(int m, int n);
*/

/* or if we want to interface all functions then we can simply
   include header file like this - 
   %include "gfg.h"
*/