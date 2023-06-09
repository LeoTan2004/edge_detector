//
// Created by Leo on 2023/4/23.
//

#ifndef PICTUREDETECTOR_BMPFILE_H
#define PICTUREDETECTOR_BMPFILE_H

#ifdef __cplusplus
extern "C" {
#endif
#define REDEFINE_V_P
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;


typedef U8 V_P ;
const static U8 _NONE_DATA = 0;
const static V_P NULL_DATA_V = (V_P)_NONE_DATA;
#include <stdio.h>
#include "grid.h"
#define COL_DATA 1024

typedef U8 col_data[COL_DATA];//调色板
#pragma  pack(1)
typedef struct {
    //bmp header
    U16 bfType;         // B  M
    U32 bfSize;         //文件大小
    U16 bfReserved1;
    U16 bfReserved2;
    U32 bfOffBits;      //文件头偏移量
    //DIB header
    U32 biSize;         //DIB头大小
    U32 biWidth;        //文件宽度
    U32 biHeight;       //文件高度
    U16 biPlanes;
    U16 biBitCount;     //每个相素点的位数
    U32 biCompression;
    U32 biSizeImage;    //图文件大小
    U32 biXPelsPerMeter;
    U32 biYPelsPerMeter;
    U32 biClrUsed;
    U32 biClrImportant;
    col_data colData;//1024位调色板
} BMP_HEADER;
#pragma  pack()
#define BMP_BLACK 255
#define BMP_WHITE 0
#define MAX(a,b) (a>b?a:b)
#define MIN(a,b) (a<b?a:b)
#define BMP_NULL_DATA BMP_BLACK
typedef enum{
    BMP_GRAY_EDGE,
    BMP_BIN_EDGE
}ACTION;
GRID_TYPE_SET(V_P);
CONVOLUTION_CORE_SET(V_P, V_P);
typedef grid(V_P) GRID_U8;

grid(V_P) *dealWithBmp(FILE* file,BMP_HEADER *bmpHeader, grid(V_P) *product,ACTION action);

BMP_HEADER *readHeader(FILE *file);

void * writeGridData(FILE *file, bool isGray, GRID_U8 *grid);

void * writeHeader(FILE *file, BMP_HEADER *header);


grid(V_P) *getGridData(FILE *file, BMP_HEADER *header,GRID_U8 *grid);
#ifdef __cplusplus
}
#endif
#endif //PICTUREDETECTOR_BMPFILE_H
