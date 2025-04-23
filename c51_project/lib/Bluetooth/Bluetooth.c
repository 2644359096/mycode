#include "compile.h"
#include "stc89xx.h"
#include <Bluetooth.h>

u8 rxBuffer[RX_BUFFER_SIZE];
u8 rxIndex = 0;




void UART_SendByte(u8 data) {
    SBUF = data;            // 将数据写入发送缓冲区
    while (!TI);            // 等待发送完成（TI标志置位）
    TI = 0;                 // 清除发送完成标志
}

void UART_SendBytes(u8 *str) {
    for (u8 i = 0; str[i]!='\0'; i++) {
        UART_SendByte(str[i]); // 调用单字节发送函数逐个发送
    }
    UART_SendByte('\r');        // 发送回车换行符
    UART_SendByte('\n');
}

// 串口接收中断服务程序
INTERRUPT(UART_ISR,4)
{
    if(RI)
	{
		RI=0;
		rxBuffer[0]=SBUF;//读取接收到的数据
        if (rxBuffer[0]==LED_ON)
            {
                P22=0;
            }
        else if (rxBuffer[0]==LED_OFF)
        {
                P22=1;
        }
        
    }
   
}
void Bluetooth_Init(void)   // 蓝牙初始化
{   
    UART_Init(RELOAD_COUNT); //初始化串口
    ES=1;
}
