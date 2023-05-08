//
// Created by Leo on 2023/4/24.
//
#include <string.h>
#include "bmpFile.h"

#define NULL_FILE NULL
#define RESET_F(file) fseek(file,0,SEEK_SET)
#define CHECK_FILE(file) if (NULL_FILE == file)return NULL

BMP_HEADER *readHeader(FILE *file) {
    CHECK_FILE(file);
    RESET_F(file);
    BMP_HEADER *bmpHeader = malloc(sizeof(BMP_HEADER));
    fread(bmpHeader, sizeof(BMP_HEADER), 1, file);
    return bmpHeader;
}

void *writeGridData(FILE *file, bool isGray, GRID_U8 *grid) {
    CHECK_FILE(file);
    size_t offset = sizeof(BMP_HEADER);
    fseek(file, offset, SEEK_SET);
    fwrite(grid->get_value(), sizeof(U8), grid->height * grid->width, file);
    return grid;
}

void *writeHeader(FILE *file, BMP_HEADER *header) {
    CHECK_FILE(file);
    if (NULL == header)return NULL;
    RESET_F(file);
    fwrite(header, sizeof(BMP_HEADER), 1, file);
    return header;
}


V_P * uncompress(V_P *data, U32 compression) {
    return data;
}

GRID_U8 *getGridData(FILE *file, BMP_HEADER *header,GRID_U8 *grid) {
    CHECK_FILE(file);
    RESET_F(file);
    BMP_HEADER bmpHeader;
    fread(&bmpHeader, sizeof(BMP_HEADER), 1, file);
    if (NULL != header) {
        memcpy(header, &bmpHeader, sizeof(BMP_HEADER));
    }else{
        header = &bmpHeader;
    }
    U32 length = header->biSizeImage == 0 ?
            (header->biHeight * header->biWidth) : header->biSizeImage;
    fseek(file, (long) header->bfOffBits, SEEK_SET);
    V_P *data = malloc(sizeof(U8) * length);
    fread(data, sizeof(U8), length, file);
    data = uncompress(data,header->biCompression);

    printf("%x\n",data);
    grid->valueOf(data,header->biWidth,header->biHeight);
    return grid;
}