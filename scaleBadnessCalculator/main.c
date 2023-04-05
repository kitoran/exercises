#include <stdio.h>
#include <math.h>
#include "stb_ds.h"
#include "misc.h"
int main()
{
    typedef struct fr {
       int num, den;
    } fr ;
    int limit = 10;
    fr* fractions = NULL;
    for(int i = 1; i <= limit; i++) {
        for(int j = i+1; j <= limit; j++) {
            fr f = {j,i};
            if(j%3 != 0 && i%3!=0/* && j*1.0/i < 1.9*/) {
                arrput(fractions, f);
            }
        }
    }
// 5 ed6/5 is good
// 17ed11/8, 11ed11/8




    //15ed7/5
//    double period = 7.0/5;
//    double period = 2;
    int n = 1;
    double threeCents = 1.00173437023 ;
    fr prevf;
    double stepSize = pow(2,16.70239/1200);
    double prevprev = 0;
    double prev = 0;
    double prevStep;
    double prevCents;
    while(stepSize <= 1.40) {
//        double stepSize = pow(period, 1.0/n);
        double stepSizeCents = log(stepSize) / (log(2)/1200);
        double max = 0;
        fr maxf;
        FOR_STB_ARRAY(fr*, f, fractions) {
            double fd = f->num*1.0/f->den;
            double step = round(log(fd) / (log(stepSize)));
//            step = round(step);
            double diffA = fd/pow(stepSize, round(step));
            double diff = fabs(log(diffA)/(log(2)/1200));
//            diff =
//            threeCents
//            double dif
            if(diff > max) {
                max = diff;
                maxf = *f;
            }
        }
        if(prev < prevprev && prev < max && prev < 30) {
            printf("badness for %lf cents (%lf edp): %lf cents, ratio %d/%d\n",
                   prevCents, log(2)/log(prevStep), /*max,*/ prev,
                    prevf.num, prevf.den);

        }
        prevprev = prev;
        prev = max;
        prevStep = stepSize;
        prevCents = stepSizeCents;
        prevf = maxf;
        stepSize+=0.00001;//*=threeCents;
        n++;
    }
    return 0;
}
