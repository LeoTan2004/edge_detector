//
// Created by Leo on 2023/5/23.
//

#ifndef PICTUREDETECTOR_CONV_STD_H
#define PICTUREDETECTOR_CONV_STD_H
/**
 * 这些是规范，但是需要外部自定义
 */

//预处理函数               （指向原始数据格） （当前数据在卷积核中的坐标位置）
typedef void *(*pre_handle)(void *value, unsigned x, unsigned y);

//卷积核中的算子
typedef void *(*conv_calc)(void *value, unsigned x, unsigned y);

//池化
typedef void *(*pooling_handle)(void *pre, void *aft, unsigned x, unsigned y);

//重处理函数                （指向处理结果数据格） （当前数据在卷积核中的坐标位置）
typedef void *(*after_handle)(void *value, unsigned x, unsigned y);

typedef void *(*get)(void *data, unsigned x, unsigned y);

typedef void (*set)(void *data, void *value, unsigned x, unsigned y);

//数据块
typedef struct {
    void *data;
    get gets;
    set sets;
    unsigned width;
    unsigned height;
} DATA_BLOCK;


//卷积核
typedef struct {
    conv_calc *calculations;
    unsigned calc_width;
    unsigned calc_height;
    long long core_x;
    long long core_y;
} CONV_CORE;
//卷积控制块
typedef struct {
    pre_handle preHandle;
    CONV_CORE *convCore;
    pooling_handle poolingHandle;
    after_handle afterHandle;
} CONV_CON_BLOCK;

typedef enum {
    PRE_HANDLE = 0,
    AFTER_HANDLE,
    POOL_HANDLE,
    CONV_HANDLE
} HANDLE_TYPE;

DATA_BLOCK *execute_convolution(CONV_CON_BLOCK *conBlock, DATA_BLOCK *rawData, DATA_BLOCK *pro_data);

void *get_handles(HANDLE_TYPE type);

CONV_CON_BLOCK *default_init();

// 默认初始化，默认为3x3的卷积，但是不会产生任何默认无任何效果
CONV_CON_BLOCK *
create_conv_block_a(pre_handle
                    preHandle,
                    CONV_CORE *convCore, pooling_handle
                    poolingHandle,
                    after_handle afterHandle
);

// 生成数据对象
DATA_BLOCK *create_data_block_a(void *data, get gets, set sets, unsigned width, unsigned height);

#endif //PICTUREDETECTOR_CONV_STD_H
