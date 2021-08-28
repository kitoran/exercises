void complex_fftInt16(int16_t *data, int size, complex double *res)
{
    int logsize = intLog2(size);

    fftRecInt16(data, logsize, 0, &res[0]);
    for(int i = 0; i < size; i++) {
        if(max < std::abs(res[i])) {
            max = std::abs(res[i]);
//            maxh = i;
        }
    }
}
void complex_fftDouble(double *data, int size, complex double *res)
{
    int logsize = intLog2(size);

    fftRecDouble(data, logsize, 0, &res[0]);
    for(int i = 0; i < size; i++) {
        if(max < std::abs(res[i])) {
            max = std::abs(res[i]);
//            maxh = i;
        }
    }
}
