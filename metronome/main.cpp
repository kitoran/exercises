#include <stdio.h>
int main(int argc, char** argv)
{
    if(argc == 1) {
        printf("need temp\n");
        return 1;
    }
    double tempo;
    sscanf(argv[1], "%lf", &tempo);


    printf("%lf\n", tempo);


    return 0;
}
