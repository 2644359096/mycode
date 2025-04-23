#ifndef __UART_H__
#define __UART_H__
#include <public.h>

#define RX_BUFFER_SIZE 32

typedef enum{
    Rx_Idle,
    Rx_Running,
    Rx_End,
    Rx_Overflow
}UartState_t;
typedef struct {
    LoopFunction_t *tasks; // 指向任务数组的指针
    u8 task_count;         // 任务数量
} TaskContext_t;

typedef enum{
    HELP,
    OK,
    ERROR
}CMD_t;

typedef struct
{
    u8 rx_buf[RX_BUFFER_SIZE];
    u8 rx_len;
    UartState_t state;
}Uart_t;




void Uart1_Init(u8 burd);

INTERRUPT(Uart1_Isr,4);
void Uart1_MainFunction(void *params);



#endif
