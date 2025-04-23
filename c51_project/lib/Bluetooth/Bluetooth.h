#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__



#define RELOAD_COUNT 0xFA //宏定义波特率发生器的载入值9600
// 定义接收缓冲区
#define RX_BUFFER_SIZE 10
#define Buletooth_STA P33

enum LED_STA {LED_OFF=0x01,LED_ON=0x02};


void Bluetooth_Init(void);   // 蓝牙初始化
void UART_SendByte(u8 data);
void UART_SendBytes(u8 *str);


#endif // __BLUETOOTH_H__
