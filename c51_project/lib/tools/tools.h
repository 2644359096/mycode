#ifndef __tools_h__
#define __tools_h__

typedef unsigned char u8;
typedef unsigned int u16;

#define Array(name, ...) uarray name[] = {__VA_ARGS__,'E','N','D','\0'}
#define CodeArray(name, ...) __code uarray  name[] = {__VA_ARGS__,'E','N','D','\0'}
enum NiXieChar {N1=1,N2=2,N3=3,N4=4,N5=5,N6=6,N7=7,N8=8,N9=9,N0=0,Aa='a',Bb='b',Cc='c',Dd='d',Ee='e',Ff='f'};
enum  NumberType {DEC='d',HEX='h',BIN='b'};


u16 ArrayLength(uarray Array[]);
void Delay10us(u8 xus);
void ShowNixie(u8 common_pole,u8 LED_num,enum NiXieChar show_char);
void Timer0_init();
INTERRUPT(Timer0_Routine,1);
void uart_init(u8 baud);
INTERRUPT(uart,4);

void LCD_Init(void);
void LCD_ShowChar(u8 Line,u8 Column,u8 Char);
void LCD_ShowString(u8 Line,u8 Column,u8 *String);
void LCD_ShowNum(u8 Line,u8 Column,u16 Number);
void LCD_ShowSignedNum(u8 Line,u8 Column,int Number);
void LCD_ShowHEXNum(u8 Line,u8 Column,u16 Number);
void LCD_ShowBINNum(u8 Line,u8 Column,u16 Number);
#endif