#include <complex.h>
extern double max;
void complex_fftInt16(short *data, int size, complex double *res)
{
    int logsize = intLog2(size);

    fftRecInt16(data, logsize, 0, &res[0]);
    for(int i = 0; i < size; i++) {
        if(max < fabs(res[i])) {
            max = fabs(res[i]);
//            maxh = i;
        }
    }
}
void complex_fftDouble(double *data, int size, complex double *res)
{
    int logsize = intLog2(size);

    fftRecDouble(data, logsize, 0, &res[0]);
    for(int i = 0; i < size; i++) {
        if(max < fabs(res[i])) {
            max = fabs(res[i]);
//            maxh = i;
        }
    }
}
