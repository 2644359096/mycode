#include "compile.h"
#include "stc89xx.h"
#include "public.h"
#include "MatrixLED8_8.h"



SBIT(RCK,_P3,5);        //RCLK      锁存
SBIT(SCK,_P3,6);        //SRCLK     进位
SBIT(SER,_P3,4);        //SER       数据输入标志
//移位寄存器控制LED点阵屏-->
//写数据
void _74HC595_WriteByte(u8 Byte)
{  
    u8 i;
    SCK=0;                  //初始化进位标志位低电平
    RCK=0;                  //初始化锁存标志位低电平
    for ( i = 0; i < 8; i++)
    {
        SER=Byte&(0x80>>i);      //Byte最高位是0就是0，是1就是1,右移i位是指每次循环把下一位当作最高位
        SCK=1;                   //将进位标志设为高电平使最高位数据进位
        SCK=0;                   //数据进位后重置进位标志为低电平
    }
    RCK=1;                      //每次输出8位字节后锁存设置高电平，数据输出
    RCK=0;                      //输出数据后重置锁存为低电平
}


void MatrixLED_Init(void)
{
    
}
//8*8LED点阵屏显示
void MatrixLED_ShowColumn(u8 Column,u8 data)
{
    
    _74HC595_WriteByte(data);
    
    P0=~(0x80>>Column);         //0x80右移column列，然后进行取反，例如:1000 0000 >> 1 = 0100 0000 ~ = 1011 1111 相当于第二列设置低电平，显示
    Delay10us(10000);
    P0=0xff;                    //显示完成后恢复成高电平
}




//动态8*8LED点阵屏显示
void Dynamic_MatrixLED(void *param)
{   
    DynamicLED_Params* p = (DynamicLED_Params*)param;
    u16 Animation_Length = ArrayLength(p->Animation);

    static u8 count=0;
    static u8 Offset_Target=0;
    for (u8 i = 0; i < p->ColumnNum; i++)
        {
            MatrixLED_ShowColumn(i,p->Animation[(i+Offset_Target+Animation_Length)%Animation_Length]);
        }
    count++;
    if (count>p->Speed)
    {
        count=0;
        Offset_Target+=p->Offset;
        if(Offset_Target>=Animation_Length)
        {
            Offset_Target=0;
        }

    }

}
//<-- 移位寄存器控制LED点阵屏