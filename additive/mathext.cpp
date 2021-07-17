#include "mathext.h"
#include "math.h"
#include "inttypes.h"


const double tau = M_PI*2;
double *makeSinLookupTable()
{
    static bool initialized = false;
    static double lookup[LOOKUP_TABLE_SIZE];
    if(!initialized) {
        for(int i = 0; i < LOOKUP_TABLE_SIZE; i++) {
          lookup[i] = /*(*/sin(i*tau/LOOKUP_TABLE_SIZE)/*/2+1)*UINT16_MAX*/;
        }
        initialized = true;
    }
    return lookup;
}
double *sinLookupTable = makeSinLookupTable();
int * makeSinLookupTableInt()
{
    static bool initialized = false;
    static int lookup[LOOKUP_TABLE_SIZE];
    if(!initialized) {
        for(int i = 0; i < LOOKUP_TABLE_SIZE; i++) {
          lookup[i] = /*(*/sin(i*tau/LOOKUP_TABLE_SIZE)/*/2+1)*/*INT16_MAX;
        }
        initialized = true;
    }
    return lookup;
}
int* sinLookupTableInt = makeSinLookupTableInt();
inline double fastSin(double x)
{
    return sinLookupTable
            [int(x/tau*LOOKUP_TABLE_SIZE)%LOOKUP_TABLE_SIZE];
}
int intLog2(int l) {
    int res = 0;
    while(l >>= 1) {
        res++;
    }
    return res;
}

