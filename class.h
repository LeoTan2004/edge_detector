//
// Created by Leo on 2023/4/23.
//

#ifndef PICTUREDETECTOR_CLASS_H
#define PICTUREDETECTOR_CLASS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;

typedef _Bool bool;
#define TRUE (1)
#define FALSE (0)

#define MAX(a, b) (a>b?a:b)
#define MIN(a, b) (a>b?b:a)
#define SUM(a, b) (a+b)
#define MUL(a, b) ((a)*(b))
#define DIV(a, b) ((a)/(b))



typedef void* V_P;
static const V_P NULL_DATA_V = NULL;
struct GRID;

typedef V_P (*get_M)(struct GRID *this, U32 x, U32 y,const V_P default_value);

typedef V_P (*set_M)(struct GRID *this, U32 x, U32 y,const V_P value);

typedef bool(*doIt_M)(U32 x, U32 y, V_P *value);

typedef struct GRID {
    U8 type;
    V_P *values;//数组存值

    U32 width;

    U32 height;

    get_M get;//获取值,如果出界,会返回defaultValue

    set_M set;//设置数,下标出界时会将value原样返回

} GRID;//网格类

enum {
    BASIC_TYPE_INT,
    BASIC_TYPE_DOUBLE,
    BASIC_TYPE_FLOAT,
    BASIC_TYPE_FUNC,
    BASIC_TYPE_REF,
    BASIC_TYPE_OTHER,
    BASIC_TYPE_DEFAULT
};

GRID *init_GRID(int type);

typedef V_P R_P;
static const R_P NULL_DATA_R = NULL_DATA_V;
static R_P DEFAULT_R = NULL_DATA_R;
typedef struct {
    R_P (*d)(V_P*);
} FACTOR;
struct CONV_CORE;

typedef void (*simulate_M)(R_P *previous, const R_P *target);

typedef R_P (*convolution_M)(struct CONV_CORE *this, U32 x, U32 y, GRID *rawData);

typedef struct CONV_CORE {
    U32 core_x;//核心坐标x
    U32 core_y;//黑心坐标y
    GRID *factors;//处理的函数集,这里放的类型是FACTOR
    simulate_M simulate;
    convolution_M convolution;
} CONV_CORE;

CONV_CORE *init_CONV_CORE();

#ifdef __cplusplus
}
#endif
#endif //PICTUREDETECTOR_CLASS_H
