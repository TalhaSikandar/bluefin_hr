#include <stdio.h>

void func3(void *params) {
    int *num = (int *)params;
    printf("Library 2, Function 3 called with number: %d\n", *num);
}

void func4(void *params) {
    double *dnum = (double *)params;
    printf("Library 2, Function 4 called with double: %.2f\n", *dnum);
}
