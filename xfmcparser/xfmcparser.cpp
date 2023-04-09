/* file : gfg.c */
  
#include <stdio.h>
#include <math.h>
  
//our header file
#include "xfmcparser.h"
#define ll long long
  
double tdouble = 3.4;
  
// return first byte
char getbyte(const char* data, int len)
{
    return data[0];
}
  
//find mod
int my_mod(int n, int m)
{
  return(n % m);
}