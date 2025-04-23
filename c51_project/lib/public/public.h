#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#include "compile.h"
#include "stc89xx.h"

#define RELOAD_COUNT 0xFA //宏定义波特率发生器的载入值9600
#define NULL 0

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned char uarray;

#define Array(name, ...) uarray name[] = {__VA_ARGS__,'E','N','D','\0'}
#define CodeArray(name, ...) __code uarray  name[] = {__VA_ARGS__,'E','N','D','\0'}

typedef enum
{
    RUNNING,
    END
}Loop_Function_State_t;




typedef struct
{   
    void (*func)(void *params);
    void *params;
    Loop_Function_State_t state;
}LoopFunction_t;

typedef enum
{
    _u8,
    _u16,
    _uarray
} datatype;



void Delay10us(u8 xus);
u8 ArrayLength(uarray Array[]);
void Task_init(LoopFunction_t *tasks,u8 task_count);
void Loop_Tasks();

#endif 
