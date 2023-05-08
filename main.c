//
// Created by Leo on 2023/4/21.
//

#include "bmpFile.h"
#include "string.h"
#include "stdio.h"
#include "grid.h"

#define OPEN_FAIL NULL
#define OPEN_FILE_READ_BIN ("rb")


static struct {
    unsigned short target: 2;
    unsigned short output: 2;
    unsigned short isGray: 2;//gray==>1  bin==>0
} arguments = {0};

void getName(int argc, char *const *argv, char *rawFileName, char *proFileName) {
    for (int i = 1; i < argc; ++i) {
        if (strstr(argv[i], "-f")) {
            if (++i < argc) {
                strcpy(rawFileName, argv[i]);
                arguments.target = 1;
                continue;
            }
            printf("argument illegal");
            exit(__LINE__);
        }
        if (strstr(argv[i], "-o")) {
            if (++i < argc) {
                strcpy(proFileName, argv[i]);
                arguments.output = 1;
                continue;
            }
            printf("argument illegal");
            exit(__LINE__);
        }
        if (strstr(argv[i], "-g")) {
            arguments.isGray = 1;
            continue;
        }
        if (arguments.target == 0) {
            strcpy(rawFileName, argv[i]);
        } else if (arguments.output == 0) {
            strcpy(proFileName, argv[i]);
        } else {
            printf("argument illegal : %s\n", argv[i]);
            exit(__LINE__);
        }
    }
    if (arguments.target == 0) {
        printf("File name of the image to be processed: ");
        scanf("%s", rawFileName);
    }
    if (arguments.output == 0) {
        strcpy(proFileName, "");
        strncat(proFileName, rawFileName, strlen(rawFileName) - 4);
        strcat(proFileName, "_result.bmp");
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
    product = dealWithBmp(file, bmpHeader, product, arguments.isGray?BMP_GRAY_EDGE:BMP_BIN_EDGE);
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


