//
// Created by Leo on 2023/5/4.
//

#ifndef PICTUREDETECTOR_GRID_H
#define PICTUREDETECTOR_GRID_H

#include <stdint.h>
#include <stdbool.h>
#include <windows.h>
#include <signal.h>

#ifdef __cplusplus
extern "C" {
#endif
#define reflect(x,y) (x + y * this.height)
#define grid(T) T##_grid
typedef struct position position;
struct position {
    size_t x;
    size_t y;
};
/*******************************/
#define GRID_TYPE_SET(T) \
                         \
typedef struct grid(T) grid(T);\
struct grid(T) {\
    T (*get)(size_t y, size_t x,const T default_value);\
    void (*set)(size_t y, size_t x,const T value);\
    size_t width;\
    size_t height;\
    position (*indexOf)(const T *ele);                 \
    T *(*valueOf)(T* target,size_t w, size_t h);\
}
/*******************************/

/*******************************/
#define GRID_BODY_FUNCTION(T)\
inline _Bool isValid(size_t y, size_t x) { \
    return cells != NULL && x < this.width && y < this.height; \
}                            \
T get_s(size_t y, size_t x, const T default_value) {          \
    if (!isValid(x, y)) \
    return default_value;    \
    if(IsBadReadPtr(&cells[reflect(x,y)],sizeof(T)))  {  \
        raise(SIGSEGV);      \
        return default_value;\
    }                        \
    return cells[reflect(x,y)];     \
}                            \
T *value_of(T* target,size_t w, size_t h){ \
    T *pre = cells;          \
    if(IsBadReadPtr(target, w * h * sizeof(T))){                   \
          return target;     \
    }                        \
    cells = target;          \
    this.height = w;         \
    this.width = h;          \
    return pre;              \
}                            \
void set_s(size_t y, size_t x, const T value) {\
    if (!isValid(x, y))      \
    return ;                  \
    if( IsBadWritePtr(&cells[reflect(x,y)],sizeof(T))){  \
    raise(SIGSEGV);          \
    return ;                 \
    }                        \
    cells[reflect(x,y)] = value;    \
}                            \
                             \
position get_position(const T *ele) {      \
    const static position out = {-1, -1};  \
    T *begin = &cells[0];    \
    if (ele < begin)return out;            \
    size_t index = (ele - begin);          \
    index /= sizeof(T);      \
    if (index > (this.width * this.height))return out;         \
    position pos = {index % this.width, index / this.width};   \
    return pos;              \
}

/*******************************/


/*******************************/
#define GRID_BODY_INIT(T, w, h) \
({                              \
    T *cells = NULL;            \
    grid(T) this;               \
    this.height = w;            \
    this.width = h;             \
    GRID_BODY_FUNCTION(T)       \
    this.set = set_s;           \
    this.get = get_s;           \
    this.indexOf = get_position;\
    this.valueOf = value_of;\
    this;                       \
})
#define new_grid(T, w, h) (GRID_BODY_INIT(T,w,h))
/*******************************/
#define conv_core(T, R) conv_core_##T##_##R
/*******************************/
#define CONVOLUTION_CORE_SET(T, R, w, h) \
typedef R (*factor_##T##_##R)(const T fact); \
typedef R (*simulate_##T##_##R)(const R fact1,const R fact2); \
typedef R (*do_conv_##T##_##R)(const grid(T) *rawData,size_t x,size_t y,const T outOfEdge); \
GRID_TYPE_SET(factor_##T##_##R);         \
typedef struct {                         \
    size_t core_x;                       \
    size_t core_y;                       \
    simulate_##T##_##R simulation;       \
    factor_##T##_##R##_grid *factorGrid; \
    do_conv_##T##_##R convolution;\
} conv_core_##T##_##R                    \
/*******************************/
/*******************************/
#define CONVOLUTION_CORE_BODY_FUNCTION(T, R, w, h) \
R do_conv_##T##_##R(const grid(T) *rawData,size_t x,size_t y,const T outOfEdge){ \
    R result = outOfEdge;                          \
    _Bool flags = FALSE;                          \
    for (int i = 0; i < this.factorGrid->width; ++i) {                           \
        for (int j = 0; j < this.factorGrid->height; ++j) {                      \
            R temp = this.factorGrid->get(i, j, NULL)(     \
            rawData->get(x - this.core_x + i, y - this.core_y + j, outOfEdge));  \
            if (flags) {                               \
                result = this.simulation(result, temp);\
            } else {                                   \
                result = temp;                         \
                flags = TRUE;                          \
            }                                      \
        }                                          \
    }                                              \
    return result;                                 \
}                                                  \
/*******************************/
/*******************************/
#define CONVOLUTION_CORE_BODY_INIT(T, R, w, h) \
({                                             \
conv_core(int,int) this;                       \
grid(factor_##T##_##R) grid_i = new_grid(factor_##T##_##R ,w,h); \
this.core_y = 0;                               \
this.core_x = 0;                               \
CONVOLUTION_CORE_BODY_FUNCTION(T,R,w,h);       \
this.factorGrid = &grid_i;                     \
this.simulation = NULL;                        \
this.convolution = do_conv_##T##_##R;          \
this;                                          \
})                                             \
/*******************************/

#define new_conv_core(T,R,w,h) CONVOLUTION_CORE_BODY_INIT(T, R, w, h)
#ifdef __cplusplus
}
#endif
#endif //PICTUREDETECTOR_GRID_H
