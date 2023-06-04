//
// Created by 35098 on 2023/5/23.
//
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include "conv_std.h"

#define IS_NULL(a) (NULL == a)

inline bool valid_con_block(CONV_CON_BLOCK *block) {
    return !(IS_NULL(block)
             || IS_NULL(block->convCore)
             || IS_NULL(block->afterHandle)
             || IS_NULL(block->preHandle)
             || IS_NULL(block->poolingHandle)
             || IS_NULL(block->convCore->calculations));
}


inline bool valid_data(DATA_BLOCK *data) {
    return !(IS_NULL(data)
             || IS_NULL(data->gets)
             || IS_NULL(data->sets));
}

//这里只有内部使用，所以不需要数据校验
void *conv_exe(CONV_CON_BLOCK *block, DATA_BLOCK *data, unsigned int x, unsigned int y) {
    void *pre = NULL;
    unsigned int width = block->convCore->calc_width;
    unsigned int height = block->convCore->calc_height;
    unsigned int dx = x + block->convCore->core_x;
    unsigned int dy = y + block->convCore->core_y;
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            void *handled = data->gets(data->data,dx + i, dy + j);
            handled = block->preHandle(handled, dx + i, dy + j);
            handled = block->convCore->calculations[i * width + j](handled, i, j);
            handled = block->poolingHandle(pre, handled, i, j);
            pre = handled;
        }
    }
    return block->afterHandle(pre, x, y);
}

DATA_BLOCK *execute_convolution(CONV_CON_BLOCK *conBlock,
                                DATA_BLOCK *rawData, DATA_BLOCK *pro_data) {
    if (!valid_con_block(conBlock) ||
        !valid_data(rawData) || !valid_data(pro_data)) {
        raise(SIGSEGV);
    }
    for (int i = 0; i < pro_data->width; ++i) {
        for (int j = 0; j < pro_data->height; ++j) {
            void *result = conv_exe(conBlock, rawData, i, j);
            pro_data->sets(pro_data->data,result, i, j);
        }
    }
    return pro_data;
}


