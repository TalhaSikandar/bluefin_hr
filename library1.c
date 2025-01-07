#include <stdio.h>

void func1(void *params) {
    int *num = (int *)params;
    printf("Library 1, Function 1 called with number: %d\n", *num);
}

void func2(void *params) {
    double *dnum = (double *)params;
    printf("Library 1, Function 2 called with double: %.2f\n", *dnum);
}
