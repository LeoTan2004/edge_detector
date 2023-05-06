//
// Created by Leo on 2023/4/21.
//

#include "bmpFile.h"
#include "string.h"
#include "stdio.h"

#define OPEN_FAIL NULL
#define OPEN_FILE_READ_BIN ("rb")

void getName(int argc, char *const *argv, char *rawFileName, char *proFileName) {
    if (argc == 1) {
        printf("File name of the image to be processed: ");
        scanf("%s", rawFileName);
        strcpy(proFileName, "");
        strncat(proFileName, rawFileName, strlen(rawFileName) - 4);
        strcat(proFileName, "_eq.bmp");
    } else if (argc == 2) {
        strcpy(rawFileName, argv[1]);
        strcpy(proFileName, "");
        strncat(proFileName, rawFileName, strlen(rawFileName) - 4);
        strcat(proFileName, "_eq.bmp");
    } else {
        strcpy(rawFileName, argv[1]);
        strcpy(proFileName, argv[2]);
    }
}

R_P factors (V_P* value){
    U8 temp = *value;
    return temp;
}

FACTOR factor = {
        factors
};

V_P *get_func(struct GRID *this, U32 x, U32 y, V_P *defaultValue) {
    if (NULL == this || NULL == this->values)return &NULL_DATA_V;
    U32 width = this->width;
    U32 height = this->height;
    if (x >= width || y >= height)return defaultValue;
    return factor;
}
int main(int argc, char **argv) {
    char rawFileName[FILENAME_MAX]="02.bmp";
    char proFileName[FILENAME_MAX] ="02_EQ.bmp" ;
//    getName(argc, argv, rawFileName, proFileName);

    FILE *file = fopen(rawFileName, OPEN_FILE_READ_BIN);
    if (OPEN_FAIL == file) {
        perror("open bmp file fail");
    }
    //读取头部
    BMP_HEADER *bmpHeader = malloc(sizeof(BMP_HEADER));
    GRID *grid = getGridData(file, bmpHeader);
    fclose(file);
    //处理函数
    CONV_CORE *core = init_CONV_CORE();
    core->height = 3;
    core->width = 3;
    for (int i = 0; i < grid->width; ++i) {
        for (int j = 0; j < grid->height; ++j) {
//            core->convolution()
            core->convolution(core,i,j,grid);
        }
    }
    //写入文件
    file = fopen(proFileName, "wb");
    if (OPEN_FAIL == file)
        perror("open x.bmp fail");
    writeHeader(file,bmpHeader);
    writeGridData(file,TRUE,grid);
    fclose(file);
    return 0;
}

