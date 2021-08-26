void fftRecInt16(int16_t *data, int logsize, int logstep, complex double *res) {
    if(logsize == 0) {
        *res = *data*ftcoef;
        return;
    }
    fftRecInt16(data, logsize-1, logstep+1, res);
    fftRecInt16(data+(1 << logstep), logsize-1, logstep+1, res+(1 << (logsize-1)));

    complex double proot = primeroot(logsize);
    complex double root = 1;
    for(int i = 0; i < (1 << (logsize-1)); i++) {
        complex double e = res[i];
        complex double o = res[i+(1 << (logsize-1))];
        res[i] = e + root * o;
        res[i+(1 << (logsize-1))] = e - root * o;
        root *= proot;
    }
}

void fftRecInt32(int32_t *data, int logsize, int logstep, complex double *res) {
    if(logsize == 0) {
        *res = *data*ftcoef;
        return;
    }
    fftRecInt32(data, logsize-1, logstep+1, res);
    fftRecInt32(data+(1 << logstep), logsize-1, logstep+1, res+(1 << (logsize-1)));

    complex double proot = primeroot(logsize);
    complex double root = 1;
    for(int i = 0; i < (1 << (logsize-1)); i++) {
        complex double e = res[i];
        complex double o = res[i+(1 << (logsize-1))];
        res[i] = e + root * o;
        res[i+(1 << (logsize-1))] = e - root * o;
        root *= proot;
    }
}
