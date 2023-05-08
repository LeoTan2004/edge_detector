//
// Created by Leo on 2023/4/21.
//

#include "bmpFile.h"
#include "string.h"
#include "stdio.h"
#include "grid.h"
#define OPEN_FAIL NULL
#define OPEN_FILE_READ_BIN ("rb")
#define BMP_BLACK 255
#define BMP_WHITE 0
#define MAX(a,b) (a>b?a:b)

CONVOLUTION_CORE_SET(V_P, V_P);
V_P fac(V_P num) {
    return num>(BMP_BLACK>>1)?BMP_BLACK:BMP_WHITE;
}

factor_V_P_V_P abc[3][3] = {
        {fac, fac, fac},
        {fac, fac, fac},
        {fac, fac, fac}
};
typedef int R;

V_P simulates(V_P _pre,V_P _late){
    return MAX(_pre,_late);
}

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
#define TARGETFILE 03

int main(int argc, char **argv) {
    char rawFileName[FILENAME_MAX]="01.bmp";
    char proFileName[FILENAME_MAX] ="01_EQ.bmp" ;
//    getName(argc, argv, rawFileName, proFileName);

    FILE *file = fopen(rawFileName, OPEN_FILE_READ_BIN);
    if (OPEN_FAIL == file) {
        perror("open bmp file fail");
    }
    //读取头部
    BMP_HEADER *bmpHeader = malloc(sizeof(BMP_HEADER));
    grid(V_P) *grid = new_grid(V_P,1,1);
    getGridData(file, bmpHeader,grid);
    fclose(file);
    //处理函数
    conv_core_V_P_V_P *conv = new_conv_core(V_P, V_P, 3, 3);
    conv->core_x = 1;
    conv->core_y = 1;
    conv->factorGrid->valueOf(abc,3,3);
    conv->simulation = simulates;
    V_P *p_v = grid->get_value();
    V_P *pro = malloc(sizeof(U8)*grid->width*grid->height);
    grid(V_P) *product = new_grid(V_P,grid->height,grid->width);

    product->valueOf(pro,grid->height,grid->width);
    for (int i = 0; i < grid->width; ++i) {
        for (int j = 0; j < grid->height; ++j) {
            V_P temp = conv->convolution(grid,i,j,0);
            V_P r = fac(grid->get(i,j,BMP_BLACK));
            temp = temp == r?BMP_WHITE:BMP_BLACK;
            product->set(i,j,temp);
        }
    }
    //写入文件
    file = fopen(proFileName, "wb");
    if (OPEN_FAIL == file) {
        perror("open x.bmp fail");
    }
    writeHeader(file,bmpHeader);
    writeGridData(file,TRUE,product);
    fclose(file);
    return 0;
}

