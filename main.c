//
// Created by Leo on 2023/4/21.
//

#include "bmpFile.h"
#include "string.h"
#include "stdio.h"
#include "grid.h"

#define OPEN_FAIL NULL
#define OPEN_FILE_READ_BIN ("rb")


void getName(int argc, char *const *argv, char *rawFileName, char *proFileName) {
    if (argc == 1) {
        printf("File name of the image to be processed: ");
        scanf("%s", rawFileName);
        strcpy(proFileName, "");
        strncat(proFileName, rawFileName, strlen(rawFileName) - 4);
        strcat(proFileName, "_result.bmp");
    } else if (argc == 2) {
        strcpy(rawFileName, argv[1]);
        strcpy(proFileName, "");
        strncat(proFileName, rawFileName, strlen(rawFileName) - 4);
        strcat(proFileName, "_result.bmp");
    } else {
        strcpy(rawFileName, argv[1]);
        strcpy(proFileName, argv[2]);
    }
}

int main(int argc, char **argv) {
    char rawFileName[FILENAME_MAX] = "02.bmp";
    char proFileName[FILENAME_MAX] = "02_EQ.bmp";
    getName(argc, argv, rawFileName, proFileName);

    FILE *file = fopen(rawFileName, OPEN_FILE_READ_BIN);
    if (OPEN_FAIL == file) {
        perror("open bmp file fail");
    }
    grid(V_P) *product = new_grid(V_P, 1, 1);
    BMP_HEADER *bmpHeader = malloc(sizeof(BMP_HEADER));;
    //读取头部
    product = dealWithBmp(file, bmpHeader, product, BMP_GRAY_EDGE);
    //写入文件
    fclose(file);
    file = fopen(proFileName, "wb");
    if (OPEN_FAIL == file) {
        perror("write file failed");
    }
    writeHeader(file, bmpHeader);
    writeGridData(file, TRUE, product);
    fclose(file);
    return 0;
}


