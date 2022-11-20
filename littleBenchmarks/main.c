#include <stdio.h>
#include <time.h>
#include <limits.h>
inline int mod(int f, int num) {
    return f%(1<<num);
}
inline int mod2(int f, int num) {
    return f&((1<<num)-1);
}
inline int mod3(int f, int num) {
    return f&(num-1);
}
inline int mod4(int f, int num) {
    return f==-1?num-1:f==num?0:f;
}
inline int mod5(int f, int num) {
    return f==num?0:f==-1?num-1:f;
}


int compar(int* a, int* b) {
    return *a < *b;
}
int solution(int A[], int N) {
    // write your code in C99 (gcc 6.2.0)
    qsort(a, N, sizeof(*A), compar);
    int smallest = 1;
    for(int i = 0; i < N; i++) {
        if(A[i] == smallest) {
            smallest++;
        } else if(A[i] > smallest) {
            break;
        }
    }
    return smallest;
}


int main()
{
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    int v = 1<<5;
    for(int i = 0; i < INT_MAX; i++) {
        if((i & 0xffffff) == 0) {
            fprintf(stderr, "did %d calls\n", i);
        }
        volatile int res = mod3(i, v);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    fprintf(stdout, "%lf seconds\n", cpu_time_used);
    return 0;
}