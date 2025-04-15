#include "stc89xx.h"
#include "compile.h"
typedef unsigned char bool;
typedef unsigned char u8;
typedef unsigned int u16;


u8 gsmg_code[] = {0xcf,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xc0,0x88,0x83,0xc6,0xa1,0x86,0x8e}; // 共阴0-f

void ShowNixie(bool common_pole,u8 LED_num,u8 show_char)
{
    switch (LED_num)
    {
    case 1:  P22=0;P23=0,P24=0;  break;
    case 2:  P22=0;P23=0,P24=1;  break;
    case 3:  P22=0;P23=1,P24=0;  break;
    case 4:  P22=0;P23=1,P24=1;  break;
    case 5:  P22=1;P23=0,P24=0;  break;
    case 6:  P22=1;P23=0,P24=1;  break;
    case 7:  P22=1;P23=1,P24=0;  break;
    case 8:  P22=1;P23=1,P24=1;  break;
    }
    
    if(common_pole){//共阳极
        
    }
    else            //共阴极
    {
        
    }
}