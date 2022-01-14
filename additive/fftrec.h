#ifndef FFTREC_H
#define FFTREC_H
#include <inttypes.h>
#include <complex.h>
void fftRecInt16(int16_t *data, int logsize, int logstep, complex double *res);
void fftRecDouble(double *data, int logsize, int logstep, complex double *res);
#endif // FFTREC_H
