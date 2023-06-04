//
// Created by Leo on 2023/5/22.
//

#include <signal.h>
#include <malloc.h>
#include <stdbool.h>
#include "bmp.h"

#define new(a) ((a*)malloc(sizeof(a)))
#define new_c(a, c) ((a*)malloc(sizeof(a)*(c)))
#define BYTE_BIT (8)
#define NOT_NULL(a) {if(NULL == a)raise(SIGSEGV);}
#define BEGIN(file) {fseek(file,0,SEEK_SET);}
#define F_READ(file, dest, count) {fread(dest,sizeof(*dest),count,file);}
#define F_WRITE(file, source, count) {fwrite(source,sizeof(*source),count,file);}
#define BM (19778)

bool valid_bmp_file_header(BITMAP_FILE_HEADER *fileHeader) {
    NOT_NULL(fileHeader);
    if (BM != fileHeader->bfType
        || 0 != fileHeader->bfReserved1
        || 0 != fileHeader->bfReserved2)
        return false;//文件类型校验
    return true;
}

bool valid_bmp_info_header(BITMAP_INFO_HEADER *infoHeader) {
    NOT_NULL(infoHeader);
    if (sizeof(*infoHeader) != infoHeader->biSize
        || 1 != infoHeader->biPlanes)
        return false;
    if (BI_RLE8 < infoHeader->biCompression)return false;
    return true;
}

void *read_data(FILE *file, BITMAP_INFO_HEADER *infoHeader, BITMAP_FILE_HEADER *fileHeader) {
    int size = infoHeader->biBitCount;
    unsigned long long count = infoHeader->biHeight * infoHeader->biWidth;
    byte *data = malloc(size * count / BYTE_BIT);
    fseek(file, fileHeader->bfOffBits, SEEK_SET);
    F_READ(file, data, count * size);
    return data;
}

void write_data(FILE *file, BITMAP_INFO_HEADER *infoHeader, BITMAP_FILE_HEADER *fileHeader, byte *data) {
    int size = infoHeader->biBitCount;
    unsigned long long count = infoHeader->biHeight * infoHeader->biWidth;
    fseek(file, fileHeader->bfOffBits, SEEK_SET);
    F_WRITE(file, data, count * size)
}

BMP_FILE_DATA *read_bmp_file(FILE *file) {
    NOT_NULL(file);
    BITMAP_FILE_HEADER *fileHeader = new(BITMAP_FILE_HEADER);
    BEGIN(file);
    F_READ(file, fileHeader, 1);
    if (!valid_bmp_file_header(fileHeader)) {
        free(fileHeader);
        return NULL;
    }
    BITMAP_INFO_HEADER *infoHeader = new(BITMAP_INFO_HEADER);
    F_READ(file, infoHeader, 1);
    if (!valid_bmp_info_header(infoHeader)) {
        free(fileHeader);
        free(infoHeader);
        return NULL;
    }
    // 这里需要完成压缩图像的处理，但是不是重点,所以这里遇到压缩图像自动结束程序
    if (BI_RGB != infoHeader->biCompression) {
        //todo 2023年5月23日 这里希望能对数据做个解压处理
        free(fileHeader);
        free(infoHeader);
        return NULL;
    }
//    调色板的读取
    byte *col_pix = NULL;
    int col_pix_count = fileHeader->bfOffBits - sizeof(BITMAP_INFO_HEADER) - sizeof(BITMAP_FILE_HEADER);
    if (col_pix_count > 0) {
        col_pix = new_c(byte, col_pix_count);
        F_READ(fileHeader, col_pix, col_pix_count);
    }
//    像素数据的读取
    void *pix_data = NULL;
    pix_data = read_data(file, infoHeader, fileHeader);
    if (NULL == pix_data) {
        free(fileHeader);
        free(infoHeader);
        return NULL;
    }
    BMP_FILE_DATA *fileData = new(BMP_FILE_DATA);
    fileData->infoHeader = infoHeader;
    fileData->fileHeader = fileHeader;
    fileData->pix_data = pix_data;
    fileData->col_pix = col_pix;
    return fileData;
}

void write_bmp_file(FILE *file, BMP_FILE_DATA *data) {
    NOT_NULL(file);
    NOT_NULL(data);
    NOT_NULL(data->fileHeader);
    NOT_NULL(data->infoHeader);
    NOT_NULL(data->pix_data);
    BEGIN(file);
    fwrite(data->fileHeader, sizeof(BITMAP_FILE_HEADER), 1, file);
    fwrite(data->infoHeader, sizeof(BITMAP_INFO_HEADER), 1, file);
    int col_pix_count = data->fileHeader->bfOffBits - sizeof(BITMAP_INFO_HEADER) - sizeof(BITMAP_FILE_HEADER);
    if (col_pix_count > 0 && NULL != data->col_pix) {
        fwrite(data->col_pix, sizeof(byte), col_pix_count, file);
    }
    write_data(file,data->infoHeader,data->fileHeader,data->pix_data);
}