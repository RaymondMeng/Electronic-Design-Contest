#include "bp.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "string.h"

//ȫ�ֱ�������
double data[]={5.00000000,2.00000000,8.00000000,5.00000000,3.00000000,1.00000000,35.68000031,56.84999847,35.68000031,800.00000000,37.00000000,58.09999847,0.64570207,0.27466848,0.35617979,0.35306763,0.42616859,-0.57195933,-0.00338672,0.31871169,-0.13533976,-0.05711701,-0.33677429,0.09802239,-0.82668032,0.26457487,-0.56926808,0.24048530,0.11704168,0.01491545,-0.31724332,0.10334364,-0.41470029,-0.65812621,-0.47970036,-0.54455216,-0.14360282,0.33909995,0.04813299,-0.76213588,-0.14589079,-0.37810510,-0.34273172,0.09997717,-0.42062101,-0.31452459,-0.11080456,0.04833866,-0.49966498,0.19002831,-0.30373946,0.13546240,0.38934161,0.71503494,-0.44128676,-0.06909855,-0.30003388,-0.45262708,-0.08322956,0.09263252,0.14998757,0.14101498,0.76294917,0.18201768,0.11916190,-0.34936277,-0.02110520,-0.13052297,0.00458996,0.17567657,0.30070337,0.12678518,-0.25401242,-0.12242658,-0.49955737,-0.30070769,-0.08527430,0.50817849,-0.51871682,0.74023877,-0.35639791,-0.26908021,0.04315437,0.14877746,0.24689973,0.09298783,-0.45415254,1.08256501,-0.46348926,0.12244601,-1.02686950,-0.12518663,0.29334037,0.31632787,-0.55261821,-0.25678814,0.00361281,0.22295608,1.01961281,-0.63257711,-1.05411055,-0.15684291,0.48211594,-0.22252784};
int layer_all1 = 0;
int *nodes_all1 = NULL;
int num_all = 0;
struct simnetnode **simnetwork=NULL;
float simxminmax[SIMROW_X][2];
float simyminmax[SIMROW_Y][2];

//������ڵ㽨������
int simNetSetUp(void)
{
	int i = 0,j=0,k=0,n=0;
	//���������
	layer_all1 = (int)data[n];
	n++;

	//������ÿ��ڵ���
	nodes_all1 = (int *)malloc(sizeof(int)*layer_all1);
	for (i = 0; i < layer_all1; i++)
	{
		nodes_all1[i] = data[n];
		n++;
	}
	//�������
	simnetwork = (struct simnetnode**)malloc(sizeof(struct simnetnode*)*(layer_all1));
	//����������ÿ��ڵ�
	for (i = 0; i < layer_all1; i++)
	{
		simnetwork[i] = (struct simnetnode*)malloc(sizeof(struct simnetnode)*nodes_all1[i]);
	}

	//ӳ�����
	for (i = 0; i < SIMROW_X; i++)
	{
		simxminmax[i][0] = data[n];
		n++;
		simxminmax[i][1] = data[n];
		n++;
	}
	for (i = 0; i < SIMROW_Y; i++)
	{
		simyminmax[i][0] = data[n];
		n++;
		simyminmax[i][1] = data[n];
		n++;
	}

	//������ڵ������ʼ��
	for (i = 0; i < layer_all1; i++)
	{
		for (j = 0; j < nodes_all1[i]; j++)
		{
			if (i != layer_all1 - 1)
			{
				simnetwork[i][j].w = (double*)malloc(sizeof(double)*nodes_all1[i + 1]);
				for (k = 0; k < nodes_all1[i + 1]; k++)
				{
					simnetwork[i][j].w[k] = data[n];
					n++;
				}

				simnetwork[i][j].b = data[n];
				n++;
			}
		}
	}
	return 0;

}

int sim(float x[SIMROW_X], float y[SIMROW_Y])
{
	if ((SIMROW_X != nodes_all1[0]) && (SIMROW_Y != nodes_all1[layer_all1 - 1]))
	{
		printf("������������������ȷ\n");
		return 1;
	}
	int i = 0, j = 0, k = 0;
	for (i = 0; i < layer_all1; i++)
	{
		//����������
		if (i == 0)
		{
			for (j = 0; j < nodes_all1[i]; j++)
			{
				simnetwork[i][j].input = simDataChange(x[j],j,1);
				simnetwork[i][j].output = simInspirit(simnetwork[i][j].input, simnetwork[i][j].b, 1);
			}
		}
		else
		{
			float s = 0;
			for (j = 0; j < nodes_all1[i]; j++)
			{
				//����
				s = 0;
				for (k = 0; k < nodes_all1[i - 1]; k++)
				{
					s += (simnetwork[i-1][k].output*simnetwork[i-1][k].w[j]);
				}
				simnetwork[i][j].input = s;
				if (i<(layer_all1-1))
				    simnetwork[i][j].output = simInspirit(simnetwork[i][j].input, simnetwork[i][j].b, 1);
				else
				{
					simnetwork[i][j].output = simnetwork[i][j].input;
					y[j] = simDataRecover(simnetwork[i][j].output,j,1); 
				}
			}
		}
	}
}

//mode1==0,����������sig��
//y=1/(1+exp(x))
//mode1==1,��������(����tansig)
//y=2/(1+e(-2x))-1
double simInspirit(double x, double b, int mode1)
{
	double y = 0;
	x -= b;
	if (mode1 == 0)
	{
		y = 1 + exp(-x);
		y = 1 / y;
	}
	else if (mode1 == 1)
	{
		y = 1 + exp(-2 * x);
		y = 2 / y - 1;
	}
	return y;
}

//int readData(void)
//{
//	FILE *fpread = fopen("data.txt", "r");
//	if (fpread == NULL)
//	{
//		return 1;
//	}
//	double k = 0;
//	num_all = 0;
//	while (fscanf(fpread, "%lf", &k) != EOF)
//		num_all++;
//	fclose(fpread);

//	int i = 0;
//	data = (double*)malloc(num_all*sizeof(double));
//	fpread = fopen("data.txt", "r");
//	while (fscanf(fpread, "%lf", &data[i]) != EOF)
//	{
//		i++;
//	}
//	return 0;
//}

int simInit()
{

//	if (readData())
//	{
//		printf("��ȡ����\n");
//		return 1;
//	}
	if (simNetSetUp())
	{
		printf("�����罨������\n");
		return 2;
	}
	return 0;
}

//����ӳ��
double simDataChange(double x,int j,int mode1)
{
	float dis = simxminmax[j][1] - simxminmax[j][0];
	x -= simxminmax[j][0];
	x /= dis;
	if (mode1 == 1)
	{
		x *= 2;
		x -= 1;
	}
	return x;
}

//���ݻָ�
double simDataRecover(double y, int j,int mode1)
{
	float dis = simyminmax[j][1] - simyminmax[j][0];
	if (mode1 == 1)
	//ѹ����0-1����
	{
		y += 1;
		y /= 2;
	}
	y *= dis;
	y += simyminmax[j][0];
	return y;
}

int simFree(void)
{
	int i = 0, j = 0;
	//�ͷ�data
	
	for (i = 0; i < layer_all1; i++)
	{
		for (j = 0; j < nodes_all1[i]; j++)
		{
			if (i != layer_all1 - 1)
			{
					free(simnetwork[i][j].w);
					simnetwork[i][j].w = NULL;
			}

		}
		free(simnetwork[i]);
		simnetwork[i] = NULL;
	}
	free(simnetwork);
	simnetwork = NULL;
	free(nodes_all1);
	nodes_all1 = NULL;
	return 0;
}