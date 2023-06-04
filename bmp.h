//
// Created by 35098 on 2023/5/22.
//

#ifndef PICTUREDETECTOR_BMP_H
#define PICTUREDETECTOR_BMP_H

#include <corecrt.h>
#include <stdio.h>

typedef unsigned char byte;
typedef struct BITMAP_FILE_HEADER {
    unsigned short bfType;        // 19778，必须是BM字符串，对应的十六进制为0x4d42,十进制为19778，否则不是bmp格式文件
    unsigned int bfSize;        // 文件大小 以字节为单位(2-5字节)
    unsigned short bfReserved1;   // 保留，必须设置为0 (6-7字节)
    unsigned short bfReserved2;   // 保留，必须设置为0 (8-9字节)
    unsigned int bfOffBits;     // 从文件头到像素数据的偏移  (10-13字节)
} BITMAP_FILE_HEADER;


typedef struct BITMAP_INFO_HEADER {
    unsigned int biSize;          // 此结构体的大小 (14-17字节)
    unsigned int biWidth;         // 图像的宽  (18-21字节)
    unsigned int biHeight;        // 图像的高  (22-25字节)
    unsigned short biPlanes;        // 表示bmp图片的平面属，显然显示器只有一个平面，所以恒等于1 (26-27字节)
    unsigned short biBitCount;      // 一像素所占的位数，有1/4/8/24常见(28-29字节)
    /*biBitCount=1 表示位图最多有两种颜色，黑色和白色。图像数据阵列中的每一位表示一个像素。
    biBitCount=4 表示位图最多有16种颜色。每个像素用4位表示，并用这4位作为彩色表的表项来查找该像素的颜色。例如，如果位图中的第一个字节为0x1F，它表示有两个像素，第一像素的颜色就在彩色表的第2表项中查找，而第二个像素的颜色就在彩色表的第16表项中查找。
    biBitCount=8 表示位图最多有256种颜色。每个像素用8位表示，并用这8位作为彩色表的表项来查找该像素的颜色。例如，如果位图中的第一个字节为0x1F，这个像素的颜色就在彩色表的第32表项中查找。
    biBitCount=24 表示位图最多有224＝16 777 216种颜色。bmiColors (或者bmciColors)成员就为NULL。每3个字节代表一个像素，其颜色有R、G、B字节的相对强度决定。
     压缩方式可参考下列文章
     http://www.blogjava.net/georgehill/articles/6549.html#:~:text=(4)-,%E5%9B%BE%E5%83%8F%E6%95%B0%E6%8D%AE%E5%8E%8B%E7%BC%A9,-%E2%91%A0%20BI_RLE8%EF%BC%9A%E6%AF%8F*/
    unsigned int biCompression;   // 说明图像数据压缩的类型，0为不压缩。 (30-33字节)
    /*说明图像数据压缩的类型。其值可以是下述值之一：
        BI_RGB(0)：没有压缩；
        BI_RLE8(1)：每个像素8位的RLE压缩编码，压缩格式由2字节组成(重复像素计数和颜色索引)；
        BI_RLE4(2)：每个像素4位的RLE压缩编码，压缩格式由2字节组成*/
    unsigned int biSizeImage;     // 像素数据所占大小, 这个值应该等于上面文件头结构中bfSize-bfOffBits。当用BI_RGB格式时，可设置为0(34-37字节)
    unsigned int biXPelsPerMeter; // 说明水平分辨率，用像素/米表示。一般为0 (38-41字节)
    unsigned int biYPelsPerMeter; // 说明垂直分辨率，用像素/米表示。一般为0 (42-45字节)
    unsigned int biClrUsed;       // 说明位图实际使用的彩色表中的颜色索引数（设为0的话，则说明使用所有调色板项）。 (46-49字节)
    unsigned int biClrImportant;  // 说明对图像显示有重要影响的颜色索引的数目，如果是0，表示都重要。(50-53字节)
} BITMAP_INFO_HEADER;

enum {
    BMP_GRAY_BLACK = 0,
    BMP_GRAY_WHITE = 255,
};
enum {
    BI_RGB = 0,
    BI_RLE8 =1,
    BI_RLE4 = 2
};

#pragma pack(1)
typedef struct GRAY {
    byte gray_level;
} BMP_GRAY_BIT;

typedef struct RGB_QUAD24 {
    byte rgbBlue;
    byte rgbGreen;
    byte rgbRed;
} RGB_QUAD24;

typedef struct RGB_QUAD32 {
    byte rgbBlue;
    byte rgbGreen;
    byte rgbRed;
    byte rgbReserved;
} RGB_QUAD32;
#pragma pack()
typedef union {
    BMP_GRAY_BIT *grayBit;
    RGB_QUAD24 *rgbQuad;
} BMP_PIX_VALUE;

typedef enum {
    BMP_BIT1 = 0,
    BMP_BIT4,
    BMP_BIT8,
    BMP_BIT24,
    BMP_BIT32
} BMP_PIX_CLASS;

typedef struct {
    BMP_PIX_CLASS pixClass;
    void *value;//可以根据上面的类型来判断
} BMP_PIX;

typedef struct {
    BMP_PIX *(*get)(size_t x, size_t y, BMP_PIX *out_exceed_value);

    BMP_PIX *(*set)(size_t x, size_t y, BMP_PIX *value);
} BMP_STD_OPS;

typedef struct {
    BITMAP_FILE_HEADER *fileHeader;
    BITMAP_INFO_HEADER *infoHeader;
    byte *col_pix;//调色板
    void *pix_data;//具体的数据信息
} BMP_FILE_DATA;

BMP_FILE_DATA *read_bmp_file(FILE *file);

void write_bmp_file(FILE *file,BMP_FILE_DATA *data);

#endif //PICTUREDETECTOR_BMP_H
