#include "compile.h"
#include "stc89xx.h"
#include <public.h>
#include <uart.h>
#include <MatrixLED8_8.h>


int main(void)
{   
    Uart1_Init(RELOAD_COUNT);
    MatrixLED_Init();
    
    // static TaskContext_t context = { tasks, 3 };
    // tasks[2].params = &context;

    

    Loop_Tasks();
    
    return 0;
}