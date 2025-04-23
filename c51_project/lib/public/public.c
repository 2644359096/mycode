#include "compile.h"
#include "stc89xx.h"
#include "public.h"

LoopFunction_t *Tasks;
u8 Task_Count;

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
//计算数组长度
u8 ArrayLength(uarray Array[])
{   
    u8 count=0;
    while (1)
    {
        if (Array[count]=='E')
        {
            count++;
            if (Array[count]=='N')
            {
                count++;
                if (Array[count]=='D')
                {
                    count++;
                    if (Array[count]=='\0')
                    {
                        return count-3;
                    }
                }
            }
        }
        else
        {
            count++;
        }
    }
    
}

void Task_init(LoopFunction_t *tasks,u8 task_count)
{   Task_Count=task_count;
    for(u8 i=0;i<Task_Count;i++)
    {
        Tasks[i]=tasks[i];
    }
    
}
void Loop_Tasks()
{   
    while (1)
    {
        for(u8 i=0 ;i<Task_Count;i++)
        {   
            
            if(Tasks[i].state == RUNNING)
            {
                Tasks[i].func(Tasks[i].params);
            }
        }
    }
    
}