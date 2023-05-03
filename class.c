//
// Created by Leo on 2023/4/23.
//

#include "class.h"

#include <stdlib.h>
#include "string.h"

typedef char* string;
V_P get(struct GRID *this, U32 x, U32 y, V_P defaultValue) {
    if (NULL == this || NULL == this->values)return NULL_DATA_V;
    U32 width = this->width;
    U32 height = this->height;
    if (x >= width || y >= height)return defaultValue;
    return this->values[x + y * height];
}

V_P set(struct GRID *this, U32 x, U32 y, V_P value) {
    if (NULL == this || NULL == this->values)return NULL_DATA_V;
    U32 width = this->width;
    U32 height = this->height;
    if (x >= width || y >= height)return value;
    V_P temp = this->values[x + y * height];
    this->values[x + y * height] = value;
    return temp;
}

U32 iterate(struct GRID *this, bool(*doIt)(U32 x, U32 y, V_P value)) {
    if (NULL == this || NULL == this->values)return 0;
    U32 counter = 0;
    U32 width = this->width;
    U32 height = this->height;
    bool flag = TRUE;
    for (int i = 0; flag && i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            ++counter;
            flag = doIt(i, j, this->get(this, i, j, NULL_DATA_V));
        }
    }
    return counter;
}

GRID *clone(struct GRID *this) {
    GRID *grid = malloc(sizeof(GRID));
    memcpy(grid, this, sizeof(GRID));
    return grid;
}

GRID *init_GRID() {
    GRID *grid = malloc(sizeof(GRID));
    if (NULL == grid) {
        perror("内存分配错误");
        exit(__LINE__);
    }
    GRID this = *grid;
    this.height = 0;
    this.width = 0;
    this.values = NULL;
    this.get = get;
    this.set = set;
    this.iterate = iterate;
    this.clone = clone;
    return grid;
}

R_P convolution(struct CONV_CORE *this, U32 x, U32 y, GRID *rawData) {
    if (NULL == this || NULL == this->factors ||
        0 == this->width || 0 == this->height ||
        NULL == rawData || NULL == rawData->values||
        NULL == this->simulate)
        return NULL_DATA_R;
    U32 start_x = x - this->core_x;
    U32 start_y = y - this->core_y;
    U32 width = this->factors->width;
    U32 height = this->factors->height;
    R_P *s = malloc(sizeof(R_P));
    memcpy(s,&DEFAULT_R,sizeof(R_P));
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            V_P v = rawData->get(rawData, start_x + i, start_y + height, NULL_DATA_V);
            FACTOR *factor = (FACTOR *) this->factors->get(this->factors, i, j, NULL_DATA_V);
            if (NULL == factor)exit(__LINE__);
            R_P t = factor->d(v);
            this->simulate((R_P *) *s, (const R_P *) t);
            free((void *) t);
        }
    }
    return *s;
}

void simulate(R_P* previous, const R_P* target){
    *previous = SUM(*previous,*target);
}

CONV_CORE *init_CONV_CORE() {
    CONV_CORE *convCore = malloc(sizeof(CONV_CORE));
    if (NULL == convCore) {
        perror("内存分配错误");
        exit(__LINE__);
    }
    CONV_CORE this = *convCore;
    this.height = 0;
    this.width = 0;
    this.core_x = 0;
    this.core_y = 0;
    this.factors = NULL;
    this.simulate = simulate;
    this.convolution = convolution;
    return convCore;
}
