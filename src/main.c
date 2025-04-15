#include "compile.h"
#include "stc89xx.h"
#include <tools.h>

typedef unsigned char u8;
typedef unsigned int u16;


void Delay1ms(unsigned int xms) //@11.0952MHz
{
    unsigned char i, j;

    for (unsigned int n = 0; n <= xms; n++)
    {

        i = 2;
        j = 199;
        do
        {
            while (--j)
                ;
        } while (--i);
    }
}

int main(void)
{   
    u8 count;
    Delay1ms(1000);
    while (1)
    {
        for(count=0;count<16;count++)
        {
            ShowNixie(false,1,count);
            Delay1ms(1000);
        }
    }
}