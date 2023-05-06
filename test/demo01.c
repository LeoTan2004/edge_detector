//
// Created by Leo on 2023/5/4.
//
#include <stdio.h>
#include <stdlib.h>
#include "../grid.h"

GRID_TYPE_SET(int);

void test(int w, int h);

void sigsegv_handle(int num) {
    printf("out of ptr\n");
    exit(num);
}

CONVOLUTION_CORE_SET(int, int, 1, 1);

int fac(int num) {
    return num + 1;
}

factor_int_int a[3][3] = {
        {fac, fac, fac},
        {fac, fac, fac},
        {fac, fac, fac}
};
typedef int R;

int main() {
    conv_core_int_int this = new_conv_core(int, int, 3, 3);
    this.factorGrid->valueOf(a, 3, 3);
    grid(int) rawData;
    int x, y;
    R outOfEdge;


    return 0;
}
