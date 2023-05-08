//
// Created by Leo on 2023/4/24.
//
#include <string.h>
#include "bmpFile.h"

#define NULL_FILE NULL
#define RESET_F(file) fseek(file,0,SEEK_SET)
#define CHECK_FILE(file) if (NULL_FILE == file)return NULL


V_P fac_BIN(V_P num) {
    return num > (BMP_BLACK >> 1) ? BMP_BLACK : BMP_WHITE;

}

V_P fac_BIN_NULL(V_P num) {
    return (num);
}

#define con_size 3
factor_V_P_V_P pFunction_GRAY[con_size][con_size] = {
        fac_BIN_NULL, fac_BIN_NULL,// fac_BIN_NULL,
        fac_BIN_NULL, fac_BIN_NULL,// fac_BIN_NULL,
        fac_BIN_NULL, fac_BIN_NULL,// fac_BIN_NULL,
        fac_BIN_NULL, fac_BIN_NULL,// fac_BIN_NULL,
        fac_BIN_NULL//, fac_BIN_NULL//, fac_BIN,
};

factor_V_P_V_P pFunction_BIN[con_size][con_size] = {
        fac_BIN, fac_BIN,
        fac_BIN_NULL, fac_BIN_NULL,
        fac_BIN_NULL, fac_BIN_NULL,
        fac_BIN_NULL, fac_BIN_NULL,
        fac_BIN_NULL
};

V_P simulates_min(V_P _pre, V_P _late) {
    return MIN(_pre, _late);
}
V_P simulates_max(V_P _pre, V_P _late) {
    return MAX(_pre, _late);
}

V_P abs_dif(V_P raw,V_P pro){
    return abs(raw-pro);
}

V_P binary(V_P raw,V_P pro){
    return (fac_BIN(raw))== fac_BIN(pro)?BMP_WHITE:BMP_BLACK;
}

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


V_P *uncompress(V_P *data, U32 compression) {
    return data;
}

GRID_U8 *getGridData(FILE *file, BMP_HEADER *header, GRID_U8 *grid) {
    CHECK_FILE(file);
    RESET_F(file);
    BMP_HEADER bmpHeader;
    fread(&bmpHeader, sizeof(BMP_HEADER), 1, file);
    if (NULL != header) {
        memcpy(header, &bmpHeader, sizeof(BMP_HEADER));
    } else {
        header = &bmpHeader;
    }
    U32 length = header->biSizeImage == 0 ?
                 (header->biHeight * header->biWidth) : header->biSizeImage;
    fseek(file, (long) header->bfOffBits, SEEK_SET);
    V_P *data = malloc(sizeof(U8) * length);
    fread(data, sizeof(U8), length, file);
    data = uncompress(data, header->biCompression);
    grid->valueOf(data, header->biWidth, header->biHeight);
    return grid;
}

grid(V_P) *dealWithBmp(FILE *file, BMP_HEADER *bmpHeader, grid(V_P) *product, ACTION action) {
    conv_core(V_P, V_P) *conv_bin = new_conv_core(V_P, V_P, 3, 3);
    grid(V_P) *grid_con = new_grid(V_P, 1, 1);
    getGridData(file, bmpHeader, grid_con);
//    convolution grid_con(SE)
    conv_core(V_P, V_P) *conv = conv_bin;
    conv->core_x = 1;
    conv->core_y = 1;
    simulate_V_P_V_P result_simu = simulates_min;
    def:
    switch (action) {
        case BMP_BIN_EDGE:
            conv->factorGrid->valueOf(pFunction_BIN, con_size, con_size);
            result_simu = binary;
            conv->simulation = simulates_max;
            break;
        case BMP_GRAY_EDGE:
            conv->factorGrid->valueOf(pFunction_GRAY, con_size, con_size);
            result_simu = abs_dif;
            conv->simulation = simulates_min;
            break;
        default:
            action = BMP_BIN_EDGE;
            goto def;
            break;
    }
    V_P *p_v = grid_con->get_value();
    V_P *pro = malloc(sizeof(U8) * grid_con->width * grid_con->height);
    product->valueOf(pro, grid_con->height, grid_con->width);
    for (int i = 0; i < grid_con->width; ++i) {
        for (int j = 0; j < grid_con->height; ++j) {
            V_P result = conv->convolution(grid_con, i, j, BMP_NULL_DATA);
            V_P r = (grid_con->get(i, j, BMP_BLACK));
            result = result_simu(r,result);// - result ;
            product->set(i, j, result);
        }
    }
    return product;
}