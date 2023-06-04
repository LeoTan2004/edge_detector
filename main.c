//
// Created by Leo on 2023/4/21.
//

#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "bmp.h"
#include "grid.h"
#include "conv_std.h"

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

unsigned height_bmp = 0;
unsigned width_bmp = 0;

void *get_bmp8(void *data, unsigned x, unsigned y) {
    if (x >= width_bmp || y >= height_bmp)return NULL;
    byte *d = (byte *) data;
    return &d[x + y * height_bmp];
}

void set_bmp8(void *data, void *value, unsigned x, unsigned y) {
    if (x >= width_bmp || y >= height_bmp)return;
    byte v = *(byte *) value;
    byte *d = (byte *) data;
    d[x + y * height_bmp] = v;
}

void* pooling_plus8(void *pre, void *aft, unsigned x, unsigned y){
    byte *pre_data = (byte*)pre;
    byte *aft_data = (byte*)aft;
    *pre_data += (*aft_data);
    return pre;
}

int main(int argc, char **argv) {

    char rawFileName[FILENAME_MAX] = "02.bmp";
    char proFileName[FILENAME_MAX] = "02_EQ.bmp";
    getName(argc, argv, rawFileName, proFileName);


    FILE *file = fopen(rawFileName, OPEN_FILE_READ_BIN);
    if (OPEN_FAIL == file) {
        perror("open bmp file fail");
    }
    BMP_FILE_DATA *file_data = read_bmp_file(file);
    height_bmp = file_data->infoHeader->biHeight;
    width_bmp = file_data->infoHeader->biWidth;
    DATA_BLOCK *raw = create_data_block_a(file_data->pix_data, get_bmp8, set_bmp8, width_bmp, height_bmp);
    byte *pro = (byte *) malloc(
            (file_data->infoHeader->biWidth * file_data->infoHeader->biHeight * file_data->infoHeader->biBitCount)
                    >> 3);
    //处理函数
    CONV_CON_BLOCK *conv = default_init();
    pooling_handle* pooling = (pooling_handle*) get_handles(POOL_HANDLE);
    pooling[0] = pooling_plus8;
    //写入文件
    fclose(file);
    file = fopen(proFileName, "wb");
    if (OPEN_FAIL == file) {
        perror("write file failed");
    }

    fclose(file);
    return 0;
}


