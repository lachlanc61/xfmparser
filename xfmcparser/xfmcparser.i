/* file : xfmcparser.i */

/* name of module to use*/
%module xfmcparser
%{
    /* Every thing in this file is being copied in 
     wrapper file. We include the C header file necessary
     to compile the interface */
    #include "xfmcparser.h"

    #define SWIG_PYTHON_STRICT_BYTE_CHAR

    double tdouble;
%}

/*recieve <bytes> objects as strings
%begin %{

%}*/

/*  
%include "typemaps.i"

%apply (char *STRING, size_t LENGTH) { (const char* data, int len) }
*/

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