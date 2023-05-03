//
// Created by Leo on 2023/4/23.
//

#ifndef PICTUREDETECTOR_CLASS_H
#define PICTUREDETECTOR_CLASS_H

#include <stdlib.h>

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;

typedef int bool;
#define TRUE (1)
#define FALSE (0)

#define MAX(a, b) (a>b?a:b)
#define MIN(a, b) (a>b?b:a)
#define SUM(a, b) (a+b)
#define MUL(a, b) ((a)*(b))
#define DIV(a, b) ((a)/(b))
#ifndef REDEFINE_VALUE
#define REDEFINE_VALUE
typedef U8 V_P;//可以交给外部定义也行
const static U8 _NONE_DATA = 0;
const static V_P NULL_DATA_V = (V_P)_NONE_DATA;
#endif //REDEFINE_VALUE
struct GRID;

typedef V_P (*get_M)(struct GRID *this, U32 x, U32 y, V_P defaultValue);

typedef V_P (*set_M)(struct GRID *this, U32 x, U32 y, V_P value);

typedef U32 (*iterate_M)(struct GRID *this, bool(*doIt)(U32 x, U32 y, V_P value));

typedef bool(*doIt_M)(U32 x, U32 y, V_P value);

typedef U32 (*iterate_M)(struct GRID *this, bool(*doIt)(U32 x, U32 y, V_P value));

typedef struct GRID {
    V_P *values;//数组存值

    U32 width;

    U32 height;

    get_M get;//获取值,如果出界,会返回defaultValue,未初始化返回NULL

    set_M set;//设置数,下标出界时会将value原样返回,否则返回空值或者之前该位置所有的值

    iterate_M iterate;

    struct GRID *(*clone)(struct GRID *this);
} GRID;//网格类

GRID *init_GRID();
#ifndef REDEFINE_RESULT
#define REDEFINE_RESULT
typedef V_P R_P;
static const R_P NULL_DATA_R = NULL_DATA_V;
const static R_P DEFAULT_R = NULL_DATA_R;
#endif //REDEFINE_RESULT
typedef struct {
    R_P (*d)(V_P);
} FACTOR;
struct CONV_CORE;

typedef void (*simulate_M)(R_P* previous, const R_P *target);

typedef R_P (*convolution_M)(struct CONV_CORE *this, U32 x, U32 y, GRID *rawData);

typedef struct CONV_CORE {
    U32 width;
    U32 height;
    U32 core_x;//核心坐标x
    U32 core_y;//黑心坐标y
    GRID *factors;//处理的函数集,这里头放的类型是FACTOR
    simulate_M simulate;

    convolution_M convolution;
} CONV_CORE;

CONV_CORE *init_CONV_CORE();

#endif //PICTUREDETECTOR_CLASS_H
