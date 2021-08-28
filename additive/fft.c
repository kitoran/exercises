void fftInt32(int32_t *data, int size, double *res)
{
    complex double actualRes[size];
    int logsize = intLog2(size);

    fftRecInt32(data, logsize, 0, &actualRes[0]);
    for(int i = 0; i < size; i++) {
        res[i] = cabs(actualRes[i]);
        if(max < (res[i])) {
            max = (res[i]);
//            maxh = i;
        }
    }
}
void fftInt16(int16_t *data, int size, double *res)
{
    complex double actualRes[size];
    int logsize = intLog2(size);

    fftRecInt16(data, logsize, 0, &actualRes[0]);
    for(int i = 0; i < size; i++) {
        res[i] = cabs(actualRes[i]);
        if(max < (res[i])) {
            max = (res[i]);
//            maxh = i;
        }
    }
}

void fftDouble(double *data, int size, double *res)
{
    complex double actualRes[size];
    int logsize = intLog2(size);

    fftRecDouble(data, logsize, 0, &actualRes[0]);
    for(int i = 0; i < size; i++) {
        res[i] = cabs(actualRes[i]);
        if(max < (res[i])) {
            max = (res[i]);
//            maxh = i;
        }
    }
}

void fftIComplexDouble(complex double *data, int size, double *res)
{
    complex double actualRes[size];
    int logsize = intLog2(size);

    fftRecComplexDouble(data, logsize, 0, &actualRes[0]);
    for(int i = 0; i < size; i++) {
        res[i] = cabs(actualRes[i]);
        if(max < (res[i])) {
            max = (res[i]);
//            maxh = i;
        }
    }
}
