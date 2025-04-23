#ifndef __MATRIXLED8_8_H__
#define __MATRIXLED8_8_H__

#include "public.h"

typedef struct {
    uarray* Animation; // 动画数据数组指针
    u8 ColumnNum;      // 列数
    u8 Offset;         // 偏移量
    u8 Speed;          // 动画速度
} DynamicLED_Params;

void _74HC595_WriteByte(u8 Byte);
void MatrixLED_Init();
void MatrixLED_ShowColumn(u8 Column,u8 data);
void Dynamic_MatrixLED(void *param);

#endif // __MATRIXLED8_8_H__
