#include "stdio.h"
#define N 10
int a[10] = {10, 12, 32, 432, 53, 78664, 45, 67, 78, 675};
unsigned char value_buf[N];

unsigned char *MiddlevalueFilter()
{
    
    unsigned char i, j, k, temp;
    for(i = 0; i < N; i++)
    {
        value_buf[i] = a[i];
    }
    for(j = 0; j < N-1; j++)
    {
        for(k = 0; k < N-j; k++)
        {
            if(value_buf[k] > value_buf[k+1])
            {
                temp = value_buf[k];
                value_buf[k] = value_buf[k+1];
                value_buf[k+1] = temp;
            }    
        }
    }
}

void main()
{
    MiddlevalueFilter();
    for(int i = 0; i < 10; i++)
    {
        printf("%d ", value_buf[i]);
    }
    
}