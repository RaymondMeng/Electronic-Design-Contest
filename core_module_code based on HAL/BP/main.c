#include "bp.h"
#include "stdio.h"

float BP_in[2],BP_out[1];
float ObjectTemperature = 37.0;
unsigned short int Distance = 2000;
void main()
{
    simInit();
    BP_in[0] = ObjectTemperature;
    BP_in[1] = Distance; 
    sim(BP_in, BP_out); 
    printf("物体温度%.2f距离 %dmm",BP_out[0],Distance);    
}