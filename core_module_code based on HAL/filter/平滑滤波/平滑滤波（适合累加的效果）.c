#include "stdio.h"
#include "math.h"
int a[10] = {10, 12, 32, 432, 53, 78664, 45, 67, 78, 675};
void main()
{
    int pwm_sum = 0, pwm_value = 0, pwm_avg = 0, pwm_value_final = 0;
    for (int i = 0; i < 10; i++)
    {
        pwm_value = a[i];
        pwm_sum += pwm_value * 10;   // pwm_sum累加
        pwm_sum -= pwm_avg;          // pwm_sum减去上次的平均值
        //pwm_avg = pwm_sum * 1.0 / (9*i+9); //更新pwm的平均值
        pwm_avg = pwm_sum * 1.0 / 5;
        pwm_value_final = pwm_avg;   // pwm_value_final的值即为当前pwm的频率
        pwm_value = 0;               //将pwm_value清空，重新计数
        printf("%d ", pwm_value_final);
    }
}


/*均值滤波
pwm_sum += pwm_value * 10; //pwm_sum累加
pwm_sum -= pwm_avg; //pwm_sum减去上次的平均值
pwm_avg = pwm_sum * 1.0 / 5; //更新pwm的平均值
pwm_value_final = pwm_avg; //pwm_value_final的值即为当前pwm的频率
pwm_value = 0; //将pwm_value清空，重新计数
*/