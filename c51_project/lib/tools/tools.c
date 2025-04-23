#include "stc89xx.h"
#include "compile.h"
#include <tools.h>
#include <stdio.h>










//软件延时
void Delay10us(u8 xus)		//@11.0592MHz
{
	u8 i;
    u16 n;
    for (n = 0; n <= xus; n++)
    {
        i = 2;
        while (--i);
    }
}

u8 gsmg_code[] = {0xcf,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xc0,0x88,0x83,0xc6,0xa1,0x86,0x8e}; // 共阴0-f

void ShowNixie(u8 common_pole,u8 LED_num,enum NiXieChar show_char)
{
    switch (LED_num)
    {
        case 1:  P22=1;P23=1;P24=1;  break;
        case 2:  P22=0;P23=1;P24=1;  break;
        case 3:  P22=1;P23=0;P24=1;  break;
        case 4:  P22=0;P23=0;P24=1;  break;
        case 5:  P22=1;P23=1;P24=0;  break;
        case 6:  P22=0;P23=1;P24=0;  break;
        case 7:  P22=1;P23=0;P24=0;  break;
        case 8:  P22=0;P23=0;P24=0;  break;
    }
    switch (show_char)
    {
        case N0: P0=gsmg_code[0]; break;
        case N1: P0=gsmg_code[1]; break;
        case N2: P0=gsmg_code[2]; break;
        case N3: P0=gsmg_code[3]; break;
        case N4: P0=gsmg_code[4]; break;
        case N5: P0=gsmg_code[5]; break;
        case N6: P0=gsmg_code[6]; break;
        case N7: P0=gsmg_code[7]; break;
        case N8: P0=gsmg_code[8]; break;
        case N9: P0=gsmg_code[9]; break;
        case Aa: P0=gsmg_code[10]; break;
        case Bb: P0=gsmg_code[11]; break;
        case Cc: P0=gsmg_code[12]; break;
        case Dd: P0=gsmg_code[13]; break;
        case Ee: P0=gsmg_code[14]; break;
        case Ff: P0=gsmg_code[15]; break;
    }
    if(common_pole){//共阳极
        P0=P0;
    }
    else            //共阴极
    {
        P0=~P0;
    }
}

//计时器0中断器-->
void Timer0_init(void)
{
    TMOD |=0x01; //0000 0001 选择定时器0模式，模式1

    TH0=0xfc;   //给计时器赋初值，定时1ms
    TL0=0x66;
    ET0=1;      //打开计时器0中断允许
    EA=1;       //打开总中断
    TR0=1;      //打开计时器
}
u8 T0count;
// void  Timer0_Routine (void) __interrupt (1)

INTERRUPT(Timer0_Routine,1)
{   
    static u16 i=0;

    TH0=0xfc;   //给计时器赋初值，定时1ms
    TL0=0x66;

    i++;
    if (i==1000)//1s
    {   
        i=0;
        P20=!P20;
    }
}
//<-- 计时器0中断器==dealy()


//串口通信-->
//串口初始化
void uart_init(u8 baud)
{
    TMOD|=0x20;     //定时器1模式2
    SCON=0x50;      //串口控制方式1
    PCON=0x80;      //倍频
    TH1=baud;       //设置初始波特率,0xfa
    TL1=baud;
    ES=1;           //打开串口中断
    EA=1;           //打开总中断
    TR1=1;          //打开计时器1
}


INTERRUPT(uart,4)
{
    //接收数据
    u8 rec_data=0;  //定义接收数据变量
    RI=0;           //接收中断标志清零
    rec_data=SBUF;  //赋值接收数据变量

    
    //发送数据
    SBUF=cmd_str;  //把接收数据原封不动赋值给SBUF用于发送
    while (!TI);    //等待数据发送完毕，TI:发送中断标志
    TI=0;           //发送中断标志清零
    


}
//<--串口通信





//LCD1602液晶显示屏
SBIT(LCD_RS,_P2,6);         //1为显示数据,0为指令数据
SBIT(LCD_RW,_P2,5);         //1为读,0为写
SBIT(LCD_E,_P2,7);          //1为数据有效,0为执行指令
#define LCD_DataPort P0     //写入数据的端口,为8位




//写指令
void LCD_WirteCommand(u8 Command)
{
    LCD_RS=0;               //数据指定为指令数据
    LCD_RW=0;               //设置为写入
    LCD_DataPort=Command;   //写入指令数据
    LCD_E=1;                //先让指令数据有效
    Delay10us(200);          //延时2豪秒
    LCD_E=0;                //再执行指令
    Delay10us(200);          //延时2豪秒

}
//写数据
void LCD_WirteData(u8 Data)
{
    LCD_RS=1;               //数据指定为显示数据
    LCD_RW=0;               //设置为写入
    LCD_DataPort=Data;   //写入显示数据
    LCD_E=1;                //先让显示数据有效
    Delay10us(200);          //延时2豪秒
    LCD_E=0;                //再显示数据
    Delay10us(200);          //延时2豪秒
}

//初始化
void LCD_Init(void)
{
    LCD_WirteCommand(0x38);     //指令0x38:八位数据接口,l两行显示,5*7点阵
    LCD_WirteCommand(0x0c);     //指令0x0c:显示开,光标关,闪烁关
    LCD_WirteCommand(0x06);     //指令0x06:数据读写操作后,光标自动加一,画面不动
    LCD_WirteCommand(0x01);     //指令0x01:清屏
}
// 设置光标位置
void LCD_SetCursor(u8 Line,u8 Column)
{
    if (Line == 1)                                  //第一列
    {
        LCD_WirteCommand(0x80|(Column-1));          //设置光标地址
    }
    else                                            //第二列
    {
        LCD_WirteCommand(0x80|(Column-1)+0x40);     //设置光标地址
    }
}
//显示字符
void LCD_ShowChar(u8 Line,u8 Column,u8 Char)
{
    LCD_SetCursor(Line,Column);
    LCD_WirteData(Char);
    
}
//显示字符串
void LCD_ShowString(u8 Line,u8 Column,u8 *String)
{   
    u8 i;
    LCD_SetCursor(Line,Column);
    for(i=0;String[i] != '\0';i++)
    {
        LCD_WirteData(String[i]);
    }
    
}

//显示数字

//辅助函数:计算数字长度
u8 Number_Length(int Number,enum NumberType numtype)
{   
    u8 i;
    int ABS_Number = (Number < 0) ? -Number : Number;
    
    for(i=0; ABS_Number >0 ; i++)
    {   
        switch (numtype)
        {
        case DEC: ABS_Number /= 10; break;
        case HEX: ABS_Number /= 16; break;
        case BIN: ABS_Number /= 2; break;
        
        }
        
    }
    return i;
}
//辅助函数:计算x^y
u16 Pow(u16 X,u16 Y)
{
    u8 i;
    u16 Result=1;
    for(i=0;i<Y;i++)
    {
        Result*=X;
    }
    return Result;
}


//显示无符号十进制数字函数
void LCD_ShowNum(u8 Line,u8 Column,u16 Number)
{   
    u8 i;
    u8 NumberLength=Number_Length(Number,DEC);
    LCD_SetCursor(Line,Column);
    for(i=NumberLength;i>0;i--)
    {   
        LCD_WirteData('0'+Number/Pow(10,i-1)%10);   
        
    }
    
}

//显示有符号十进制数字函数
void LCD_ShowSignedNum(u8 Line,u8 Column,int Number)
{   
    u8 i;
    u16 Number1;
    u8 NumberLength=Number_Length(Number,DEC);

    LCD_SetCursor(Line,Column);

    if(Number>=0)
    {
        LCD_WirteData('+');
        Number1=Number;
    }
    else
    {
        LCD_WirteData('-');
        Number1=-Number;
    }
    for(i=NumberLength;i>0;i--)
    {   
        LCD_WirteData('0'+Number1/Pow(10,i-1)%10);   
        
    }
    
}
//显示16进制数字函数
void LCD_ShowHEXNum(u8 Line,u8 Column,u16 Number)
{   
    u8 i;
    u8 NumberLength=Number_Length(Number,HEX);
    u8 SingleNumber;
    LCD_SetCursor(Line,Column);


    for(i=NumberLength;i>0;i--)
    {   
        SingleNumber=Number/Pow(16,i-1)%16;
        
        if (SingleNumber<10)
        {
            LCD_WirteData('0'+ SingleNumber);
        }
        else
        {
            LCD_WirteData('A'+ SingleNumber-10);
        }
           
    }
    
}
//显示二进制数字函数
void LCD_ShowBINNum(u8 Line,u8 Column,u16 Number)
{   
    u8 i;
    u8 NumberLength=Number_Length(Number,BIN);
    LCD_SetCursor(Line,Column);
    for(i=NumberLength;i>0;i--)
    {   
        LCD_WirteData('0'+Number/Pow(2,i-1)%2);   
        
    }
    
}