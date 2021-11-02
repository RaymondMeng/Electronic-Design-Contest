#include"test.h"
#include"BPNetwork.h"
#include"rand.h"
#include<stdio.h>
#include"BPSim.h"
extern int layer_all;
extern int* nodes_all;
extern struct netnode** network;
extern struct simnetnode** simnetwork;

void test_sim(void)
{
	float x[SIMROW_X], y[SIMROW_Y];
	float i1 = 0;
	for (i1 = 1; i1 < 30; i1+=0.5)
	{
		x[0] = i1;
		sim(x, y);
		printf("x: %f，y: %f\n",x[0],y[0]);
	}
}

void test_data()
{
	int i = 0, j = 0, k = 0;
	for (i = 0; i < layer_all; i++)
	{
		for (j = 0; j < nodes_all[i]; j++)
		{
			if (i != layer_all - 1)
			{
				if (network[i][j].b != simnetwork[i][j].b)
				{
					printf("原本b：%f  ,之后b：%f\n", network[i][j].b, simnetwork[i][j].b);
				}
				for (k = 0; k < nodes_all[i + 1]; k++)
				{

					if (network[i][j].w[k] != simnetwork[i][j].w[k])
					{
						printf("原本w：%f  ,之后w：%f\n", network[i][j].w[k], simnetwork[i][j].w[k]);
					}
				}
			}
		}
	}
}


void test_all1(void)
{
	simInit();
	test_sim();
	simFree();
}