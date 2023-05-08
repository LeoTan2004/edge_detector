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

CONVOLUTION_CORE_SET(int, int);

int nums[3][3] = {121212,2,3,4,5,6,7,8,9};
int fac(int num) {
    printf("%d\n",num);
    return num + 1;
}

factor_int_int a[3][3] = {
        {fac, fac, fac},
        {fac, fac, fac},
        {fac, fac, fac}
};
typedef int R;

int simulates(int _pre,int _late){
    return _pre + _late;
}

int mains() {
    grid(int) *intGrid = new_grid(int,2,2);
    intGrid->valueOf((int *) nums, 3, 3);
    conv_core_int_int *that = new_conv_core(int, int, 3, 3);
    that->factorGrid->valueOf(a, 3, 3);
    that->simulation = simulates;
    that->core_x = 1;
    that->core_y = 1;
    int *hh = intGrid->get_value();
    int b = that->convolution(intGrid,0,0,1);
    return 0;
}