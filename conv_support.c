//
// Created by Leo on 2023/5/24.
//
/**
 * 这里定义的一个大类是用来做代理的，也就是用户可以自定义处理过程，形成一个链式的函数，然后交给代理器处理
 * 在这里面需要做的就是实现代理
 */
#include <stdlib.h>
#include "conv_std.h"

#define new(a) ((a*)malloc(sizeof(a)))
//=============================
#define MAX_LEN_FUNC 100
static pre_handle preHandles[MAX_LEN_FUNC] = {};
static after_handle afterHandles[MAX_LEN_FUNC] = {};
static pooling_handle poolHandle = NULL;
static conv_calc convHandle[MAX_LEN_FUNC] = {};
//函数指针数组
static void **handles = {preHandles, afterHandles, poolHandle, convHandle};

void *get_handles(HANDLE_TYPE type) {
    if (type > AFTER_HANDLE)return NULL;
    return handles[type];//返回他的一级指针，这样在实现隔离的同时也实现了对外暴露
}


//=============================


void *pre_handles(void *value, unsigned x, unsigned y) {
    for (int i = 0; i < MAX_LEN_FUNC; ++i) {
        if (NULL == preHandles[i] || pre_handles == preHandles[i]) {
            break;
        }
        value = preHandles[i](value, x, y);
    }
    return value;
}

//=============================

void *conv_calculations(void *value, unsigned x, unsigned y) {
    for (int i = 0; i < MAX_LEN_FUNC; ++i) {
        if (NULL == convHandle[i] || conv_calculations == convHandle[i]) {
            break;
        }
        value = convHandle[i](value, x, y);
    }
    return value;
}

//=============================
void *pooling_handles(void *preValue, void *aftValue, int x, int y) {
    if (NULL == poolHandle || poolHandle == pooling_handles)return NULL;
    return poolHandle(preValue, aftValue, x, y);
}

//=============================
void *after_handles(void *value, unsigned x, unsigned y) {
    for (int i = 0; i < MAX_LEN_FUNC; ++i) {
        if (NULL == afterHandles[i] || after_handles == afterHandles[i]) {
            break;
        }
        value = afterHandles[i](value, x, y);
    }
    return value;
}

DATA_BLOCK *create_data_block_a(void *data, get gets, set sets, unsigned width, unsigned height) {
    DATA_BLOCK *dataBlock = new(DATA_BLOCK);
    dataBlock->data = data;
    dataBlock->width = width;
    dataBlock->height = height;
    dataBlock->gets = gets;
    dataBlock->sets = sets;
    return dataBlock;
}

CONV_CON_BLOCK *
create_conv_block_a(pre_handle preHandle, CONV_CORE *convCore, pooling_handle poolingHandle, after_handle afterHandle) {
    CONV_CON_BLOCK *block = new(CONV_CON_BLOCK);
    block->afterHandle = afterHandle;
    block->preHandle = preHandle;
    block->poolingHandle = poolingHandle;
    block->convCore = convCore;
    return block;
}


// 默认初始化
CONV_CON_BLOCK *default_init() {
    CONV_CORE *core = new(CONV_CORE);
    core->core_x = 1;
    core->core_y = 1;
    core->calc_width = 3;
    core->calc_height = 3;
    conv_calc *calc = (conv_calc *) malloc(sizeof(conv_calc) * core->calc_width * core->calc_height);
    for (int i = 0; i < core->calc_width * core->calc_height; ++i) {
        calc[i] = conv_calculations;
    }
    core->calculations = calc;
    return create_conv_block_a(pre_handles, core, pooling_handles, after_handles);
}


