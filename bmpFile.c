//
// Created by Leo on 2023/4/24.
//
#include <string.h>
#include "bmpFile.h"

#define NULL_FILE NULL
#define BI_CMP_UNCOMPRESS 0
#define RESET_F(file) fseek(file,0,SEEK_SET)
#define CHECK_FILE(file) if (NULL_FILE == file)return NULL

BMP_HEADER *readHeader(FILE *file) {
    CHECK_FILE(file);
    RESET_F(file);
    BMP_HEADER *bmpHeader = malloc(sizeof(BMP_HEADER));
    fread(bmpHeader, sizeof(BMP_HEADER), 1, file);
    return bmpHeader;
}

void *writeGridData(FILE *file, bool isGray, GRID *grid) {
    CHECK_FILE(file);
    long offset = sizeof(BMP_HEADER);
    fseek(file, offset, SEEK_SET);
    fwrite(grid->values, sizeof(*(grid->values)), MUL(grid->height, grid->height), file);
    return grid;
}

void *writeHeader(FILE *file, BMP_HEADER *header) {
    CHECK_FILE(file);
    if(NULL == header)return NULL;
    RESET_F(file);
    fwrite(header, sizeof(BMP_HEADER),1,file);
    return header;
}

void *uncompress(FILE *file) {
    exit(__LINE__);
}

GRID *getGridData(FILE *file, BMP_HEADER *header) {
    CHECK_FILE(file);
    RESET_F(file);
    BMP_HEADER bmpHeader;
    fread(&bmpHeader, sizeof(BMP_HEADER), 1, file);
    if (NULL != header) {
        memcpy(header, &bmpHeader, sizeof(BMP_HEADER));
    }
    GRID *grid = init_GRID();
    U32 w = header->biWidth;
    U32 h = header->biHeight;
    U16 bitcount = header->biBitCount;
    U32 length = bmpHeader.biSizeImage;
    V_P *data;
    fseek(file, (long) header->bfOffBits, SEEK_SET);
    if (BI_CMP_UNCOMPRESS == header->biCompression) {//没有压缩的情况
        data = malloc(sizeof(U8)*length);
        fread(data, sizeof(U8), length, file);

    } else {
        data = uncompress(file);
    }
    grid->width = w;
    grid->height = h;
    grid->values = data;
    return grid;
}