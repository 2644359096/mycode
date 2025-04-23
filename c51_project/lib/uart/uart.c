#include "compile.h"
#include <public.h>
#include <uart.h>
#include <string.h>
#include <MatrixLED8_8.h>



Uart_t Uart1 =
{
    {0},
    0,
    Rx_Idle
};


INTERRUPT(Uart1_Isr,4)
{
    static u8 enter = 0;
    if(RI)
    {
        RI=0;
        if(Uart1.state == Rx_Idle || Uart1.state == Rx_Running)
        {
            Uart1.state = Rx_Running;
            Uart1.rx_buf[Uart1.rx_len] = SBUF;
            

            if(Uart1.rx_buf[Uart1.rx_len] == '\r')
            {   
                
                enter = 1;
            }
            if(enter==1)
            {
                if(Uart1.rx_buf[Uart1.rx_len] == '\n')
                {
                    
                    Uart1.state = Rx_End;
                }
            }
            else
            {
                enter = 0;

            }
            Uart1.rx_len++;
            if(Uart1.rx_len >= RX_BUFFER_SIZE)
            {
                Uart1.state = Rx_Overflow;
            }
        }
    }
}


void Uart1_Init(u8 burd)		//9600bps@11.0592MHz
{
	PCON |= 0x80;		//使能波特率倍速位SMOD
	SCON = 0x50;		//8位数据,可变波特率
	// AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	// AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = burd;		//设定定时初值
	TH1 = burd;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
    ES=1;           //串口1中断使能
    EA=1;            //总中断使能
}

void Uart1_SendByte(u8 ch)
{   
    SBUF=ch;
    while(!TI);
    TI=0;
}

void Uart1_Send_String(u8 *str) 
{
    for (u8 i = 0; str[i]!='\0'; i++) {
        Uart1_SendByte(str[i]); // 调用单字节发送函数逐个发送
    }
    Uart1_SendByte('\r');        // 发送回车换行符
    Uart1_SendByte('\n');
}

void Uart1_Send_cmd(u8 *cmd,CMD_t cmd_state) 
{   
    u8 cmd_str[30];
    switch (cmd_state)
    {
    case HELP:  strcpy((u8 *)cmd_str, "[HELP]:");strcat((u8 *)cmd_str, (u8 *)cmd);break;
    case OK:    strcpy((u8 *)cmd_str, "[OK]:");strcat((u8 *)cmd_str, (u8 *)cmd);break;
    case ERROR: strcpy((u8 *)cmd_str, "[ERROR]:");strcat((u8 *)cmd_str, (u8 *)cmd);break;
    }
    Uart1_Send_String(cmd_str);
}
char PutChar(char ch)
{
    Uart1_SendByte(ch);
    return ch;
}

void Uart1_MainFunction(void *params)
{   
    TaskContext_t *context = (TaskContext_t *)params;
    LoopFunction_t *tasks = context->tasks;
    switch (Uart1.state)
    {   
        case Rx_End:            //接收完成
            Uart1.rx_buf[Uart1.rx_len] = '\0';
            /*识别指令，进行对应动作*/
            if(strncmp(Uart1.rx_buf, "/HELP", Uart1.rx_len-2)==0)
                {
                    Uart1_Send_cmd("CMD1:/LED ON\r\nCMD4:/LED OFF",HELP);
                }
                
            else if (strncmp(Uart1.rx_buf, "/LED ON", Uart1.rx_len-2)==0)
                {
                    P21 = 0;
                    Uart1_Send_cmd("LED ON!",OK);
                    
                }
            else if (strncmp(Uart1.rx_buf, "/LED OFF", Uart1.rx_len-2)==0)
                {   
                    P21 = 1;
                    Uart1_Send_cmd("LED OFF!",OK);
                }
            else if (strncmp(Uart1.rx_buf, "/MatrixLED hello", Uart1.rx_len-2)==0)
                {   

                    tasks[0].state=END;
                    char state1=(char)tasks[0].state;
                    Uart1_SendByte(state1);
                    Uart1_Send_cmd("MatrixLED show HELLO",OK);
                }
            else if (strncmp(Uart1.rx_buf, "/MatrixLED kunkun", Uart1.rx_len-2)==0)
                {   
                    
                    Uart1_Send_cmd("MatrixLED show kunkun",OK);
                }
            else
                {   
                    Uart1.rx_len =0;
                    Uart1.state = Rx_Idle;
                    Uart1_Send_cmd("CMD ERROR!",ERROR);
                    
                }
            Uart1.rx_len =0;
            Uart1.state = Rx_Idle;
            break;
        case Rx_Idle:           //空闲状态
            
            /*...*/
            break;
        case Rx_Running:        //接收状态
            
            /*...*/
            break;
        
        case Rx_Overflow:       //接收缓冲区溢出
            Uart1.rx_len =0;
            Uart1.state = Rx_Idle;   
            
            break;
    }
    
    
}

