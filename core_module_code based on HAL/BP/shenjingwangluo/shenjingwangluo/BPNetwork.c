#include"BPNetwork.h"
#include<stdlib.h>
#include"rand.h"
#include<math.h>
#include<stdio.h>

//////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////�Ż��ռ�/////////////////////////////////////
//1�����Ӷ�����
//2��ѧϰ���ʿɵ�




struct netnode **network=NULL;//������Ľڵ������ά��̬����
struct netnode **network0 = NULL;//������Ľڵ������ά��̬����
int *nodes_all=NULL;
int layer_all=0;
long maxepoch;
float xminmax[ROW_X][2];
float yminmax[ROW_Y][2];
float momentum_w=0;//���Ӷ�����
float momentum_b = 0;//���Ӷ�����
float mseerror = 0;//���
float error0 = 100000;//��һ�����0
float speed = 0.1;//ѧϰ����
float rate_w = 0.00;//������ϵ��
float rate_b = 0.00;//������ϵ��
float rate_w0 = 0.9;//������ϵ��
float rate_b0 = 0.9;//������ϵ��
float rate_up = 0.00001;//ѧϰ������������
float rate_down =0.001;//ѧϰ�����½�����
void networkTrain(float x[ROW_X][COL_X], float y[ROW_Y][COL_Y], int layer, int nodes[], int mode1)
{   
	//���ȣ�����������ڵ�
	if (netSetup(ROW_X, layer, nodes, ROW_Y))
	{
//		error("����ڵ㽨��ʧ��");
	}

	//Ȼ�󣬳�ʼ������
	if (netInit())
	{
//		error("���������ʼ��ʧ��");
	}
	//����Ԥ����ӳ��
	minmax(x, y,xminmax,yminmax, mode1);
	//Ȼ�����������
	maxepoch = 8000;
	netSolve(x, y, maxepoch, 0.01, mode1);
	
	//�������
	if (OutPut())
	{
		printf("�������ʧ��\n");
	}
	else
	{
		printf("��������ɹ�\n");
	}
	//�ͷ��ڴ�
	if (freeNet())
	{
		printf("�ڴ��ͷ�ʧ��\n");
	}
	else
	{
		printf("�ڴ��ͷųɹ�\n");
	}
}



//���������ʼ��
int netInit(void)
{
	int i = 0, j = 0,k=0;
	for (i = 0; i < layer_all; i++)
	{
		for (j = 0; j < nodes_all[i]; j++)
		{
			network[i][j].b = random() / 65535.0 - 0.5;
			network[i][j].b_change = random() / 65535.0 - 0.5;
			network0[i][j].b_change = 0;
			if (i != layer_all - 1)
			{
				network[i][j].w = (double*)malloc(sizeof(double)*nodes_all[i + 1]);
				network0[i][j].w = (double*)malloc(sizeof(double)*nodes_all[i + 1]);
				network[i][j].w_change = (double*)malloc(sizeof(double)*nodes_all[i + 1]);
				network0[i][j].w_change = (double*)malloc(sizeof(double)*nodes_all[i + 1]);
			}
		}
	}
	for (i = 0; i < layer_all; i++)
	{
		for (j = 0; j < nodes_all[i]; j++)
		{
			if (i != layer_all - 1)
			{
				for (k = 0; k < nodes_all[i + 1]; k++)
				{
					network[i][j].w[k] = random() / 65535.0 - 0.5;;
					network[i][j].w_change[k] = random() / 65535.0 - 0.5;
					network0[i][j].w_change[k] = 0;
				}
			}
		}
	}
	return 0;
}







//�����罨���ṹ�ڵ㺯��
//��������������ڵ����num_x�����������layer��������ÿ��ڵ���nodes���Լ������ڵ���num_y
int netSetup(int num_x, int layer, int nodes[], int num_y)
{
	int i = 0;

	network = (struct netnode**)malloc(sizeof(struct netnode*)*(layer + 2));//��������
	network0 = (struct netnode**)malloc(sizeof(struct netnode*)*(layer + 2));//��������
	for (i = 0; i < layer + 2; i++)
	{
		if (i == 0)
		{
			network[i] = (struct netnode*)malloc(sizeof(struct netnode)*num_x);//����ÿ������
			network0[i] = (struct netnode*)malloc(sizeof(struct netnode)*num_x);//����ÿ������
		}
		else if (i == layer + 1)
		{
			network[i] = (struct netnode*)malloc(sizeof(struct netnode)*num_y);//����ÿ������
			network0[i] = (struct netnode*)malloc(sizeof(struct netnode)*num_y);//����ÿ������
		}
		else
		{
			network[i] = (struct netnode*)malloc(sizeof(struct netnode)*nodes[i - 1]);//����ÿ������
			network0[i] = (struct netnode*)malloc(sizeof(struct netnode)*nodes[i - 1]);//����ÿ������
		}
	}
	int j = 0;
	for (i = 0; i < layer + 2; i++)
	{
		if (network[i]== NULL)
			return 1;
	}
	nodes_all = (int*)malloc(sizeof(int)*(layer + 2));
	for (i = 0; i < layer + 2; i++)
	{
		if (i == 0)
			nodes_all[i] = ROW_X;
		else if (i==layer+1)
			nodes_all[i] = ROW_Y;
		else
			nodes_all[i] = nodes[i-1];
	}
	layer_all = layer + 2;
	return 0;
}




///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//��������⺯��
//�������£�
//��һ�������ۻ�����
//�ڶ�����ѭ����������
void netSolve(float x[ROW_X][COL_X], float y[ROW_Y][COL_Y], long epoch_max, float error_min, int mode1)
{
	int epoch = 0;
	float error = 0;
	while (epoch < epoch_max)
	{
		if (clearChange())
		{
			;
		}
		error=calAll(x, y,mode1,epoch);
		printf("��� %f\n",error/COL_X*2);
		epoch++;
	}
}




//�����ۻ�����
int clearChange(void)
{
	int i = 0,j=0,k=0;
	for (i = 0; i < layer_all; i++)
	{
		for (j = 0; j < nodes_all[i]; j++)
		{
			network[i][j].b_change = 0;
			if (i != layer_all - 1)
			{
				for (k = 0; k < nodes_all[i + 1]; k++)
				{
					network[i][j].w_change[k] = 0;
				}
			}
		}
	}
	return 0;
}

//ѭ����������
float calAll(float x[ROW_X][COL_X], float y[ROW_Y][COL_Y], int mode1, long epoch)
{
	int i = 0,j=0,k=0;
	float error = 0;
	for (k = 0; k < COL_X; k++)
	{
		//�����������
		calInputAndOutPut(x,y,k,mode1);

		//�������
		error += calError(y, k,mode1,epoch);

		//���㷴�����
		bachward(y,k,mode1);
	}
	mseerror = error;
	updata();
	error0 = mseerror;
	return error;
}

//�����������
int calInputAndOutPut(float x[ROW_X][COL_X], float y[ROW_Y][COL_Y],int k,int mode1)
{
	int i = 0, j = 0,m=0;
	double input = 0;
	for (i = 0; i < layer_all; i++)
	{
		//����������
		if (i == 0)
		{
			for (j = 0; j < nodes_all[i]; j++)
			{
				network[i][j].input = x[j][k];
				network[i][j].output = calInspirit(network[i][j].input, network[i][j].b,mode1);
			}
		}
		//�����������
		else
		{
			for (j = 0; j < nodes_all[i]; j++)
			{
				input = 0;
				for (m = 0; m < nodes_all[i - 1]; m++)
				{
					input += network[i - 1][m].output*network[i - 1][m].w[j];
				}
				network[i][j].input = input;
				//������������
				if (i != layer_all - 1)
					network[i][j].output = calInspirit(network[i][j].input, network[i][j].b,mode1);
				else
					network[i][j].output = network[i][j].input;
			}
		}
	}
	return 0;
}

//mode1==0,����������sig��
//y=1/(1+exp(x))
//mode1==1,��������(����tansig)
//y=2/(1+e(-2x))-1
double calInspirit(double x, double b, int mode1)
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

//��������
float calError(float y[ROW_Y][COL_Y], int k, int mode1, long epoch)
{
	int i = 0;
	float error1 = 0;
	for (i = 0; i < ROW_Y; i++)
	{
		error1 += pow((recoverOutput(network[layer_all - 1][i].output, i, mode1) - recoverOutput(y[i][k], i, mode1)), 2);
//		error1 += fabs((recoverOutput(network[layer_all - 1][i].output, i, mode1) - recoverOutput(y[i][k], i, mode1)));
		if (epoch == maxepoch - 1)
		{
			printf("��������� %f , ʵ������� %f     ", recoverOutput(y[i][k], i, mode1), recoverOutput(network[layer_all - 1][i].output, i, mode1));
			if (i == ROW_Y - 1)
				printf("\n");
		}
	}
	if (epoch==maxepoch - 1)
	    printf("\n");
	error1 /= 2;
	return error1;
}

//���㷴�����
int bachward(float y[ROW_Y][COL_Y], int k,int mode1)
{
	int i = 0, j = 0,m=0;
	double s1 = 0, dy = 0;

	//�����ۻ�����
	for (i = layer_all-1; i >=0; i--)
	{
		for (j = 0; j < nodes_all[i]; j++)
		{
			//����������
			if (i == layer_all - 1)
			{
				s1 = network[i][j].output - y[j][k];
				network[i][j].s1 = s1;
			}
			//�����������
			else
			{
				s1 = 0;
				for (m = 0; m < nodes_all[i + 1];m++)
				{
					if (mode1 == 0)
						dy = network[i][j].output*(1 - network[i][j].output);
					else
						dy = 1 - network[i][j].output*network[i][j].output;
					s1 += network[i+1][m].s1*network[i][j].w[m]*dy;
				}
				network[i][j].s1 = s1;
			}

		}
	}

	//����w�ۻ�����
	//��������ǰһ�㿪ʼ
	double w1 = 0, d1 = 0, b1 = 0;
	for (i = layer_all - 2; i >= 0; i--)
	{
		for (j = 0; j < nodes_all[i]; j++)
		{
			for (m = 0; m < nodes_all[i + 1]; m++)
			{
				w1 = network[i + 1][m].s1*network[i][j].output;
				network[i][j].w_change[m] += w1;
			}
			network[i][j].b_change += network[i][j].s1;
		}
	}
	return 0;
}

int updata()
{
	int i = 0, j = 0,m=0;
	float s = 0;
	for (i = 0; i < layer_all - 1; i++)
	{
		for (j = 0; j < nodes_all[i]; j++)
		{
			s = (speed*network[i][j].b_change / COL_X);
			network[i][j].b += s + rate_b*network0[i][j].b_change;
			network0[i][j].b_change = s + rate_b*network0[i][j].b_change;
			for (m = 0; m < nodes_all[i + 1]; m++)
			{
				s = (speed*network[i][j].w_change[m] / COL_X);
				network[i][j].w[m] -= s + rate_w*network0[i][j].w_change[m];
				network0[i][j].w_change[m] = s + rate_w*network0[i][j].w_change[m];
			}
		}
	}	

	//����ѧϰ����
	if (mseerror<error0)
	{
		speed *= (1+rate_up);
		savedata(0);
		rate_b = rate_b0;
		rate_w = rate_w0;
	}
	else if (mseerror>error0)
	{
		speed *= (1-rate_down);
		savedata(1);
		rate_b = 0;
		rate_w = 0;
	}
	return 0;
}



//�������
int savedata(int mode)
{
	int i = 0, j = 0, k = 0;
	for (i = 0; i < layer_all; i++)
	{
		if (i != layer_all - 1)
		{
			for (j = 0; j < nodes_all[i]; j++)
			{
				if (mode == 0)
				{
					network0[i][j].b = network[i][j].b;
					for (k = 0; k < nodes_all[i + 1]; k++)
					{
						network0[i][j].w[k] = network[i][j].w[k];
					}
				}
				else if (mode == 1)
				{
					network[i][j].b = network0[i][j].b;
					for (k = 0; k < nodes_all[i + 1]; k++)
					{
						network[i][j].w[k] = network0[i][j].w[k];
					}
				}
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


void minmax(float x[ROW_X][COL_X], float y[ROW_Y][COL_Y], float xminmax[ROW_X][2],float yminmax[ROW_Y][2], int mode1)
{
	int i = 0, j = 0;
	float min = 10000, max = -10000,dis=0;

	//����x
	for (i = 0; i < ROW_X; i++)
	{

		for (j = 0; j < COL_X; j++)
		{
			if (min>x[i][j])
				min = x[i][j];
			if (max<x[i][j])
				max = x[i][j];
		}
		dis = max - min;
		xminmax[i][0] = min;
		xminmax[i][1] = max;
		for (j = 0; j < COL_X; j++)
		{
			x[i][j] -= min;
			x[i][j] /= dis;
			if (mode1 == 1)
			//ѹ����-1��1
			{
				x[i][j] *= 2;
				x[i][j] -= 1;
			}
		}
	}

	//����y
	min = 10000;
	max = -10000;
	for (i = 0; i < ROW_Y; i++)
	{
		for (j = 0; j < COL_Y; j++)
		{
			if (min>y[i][j])
				min = y[i][j];
			if (max < y[i][j])
				max = y[i][j];
		}
		dis = max - min;
		yminmax[i][0] = min;
		yminmax[i][1] = max;
		for (j = 0; j < COL_Y; j++)
		{
			y[i][j] -= min;
			y[i][j] /= dis;
			if (mode1 == 1)
			{
				y[i][j] *= 2;
				y[i][j] -= 1;
			}
		}
	}
}


float recoverOutput(float output, int j,int mode1)
{
	float dis = yminmax[j][1] - yminmax[j][0];
	if (mode1 == 1)
	//ѹ����0-1����
	{
		output += 1;
		output /= 2;
	}
	output *= dis;
	output += yminmax[j][0];
	
	return output;
}

int freeNet(void)
{
	int i = 0, j = 0;
	for (i = 0; i < layer_all; i++)
	{
		for (j = 0; j < nodes_all[i]; j++)
		{
			if (i != layer_all - 1)
			{
				free(network[i][j].w);
				free(network[i][j].w_change);
				network[i][j].w = NULL;
				free(network0[i][j].w_change);
				network[i][j].w_change = NULL;

				free(network0[i][j].w);
				network0[i][j].w = NULL;

				if (network[i][j].w != NULL)
					return 1;
			}
		}
		free(network[i]);
		free(network0[i]);
		network[i] = NULL;
		network0[i] = NULL;
		if (!(network[i] == NULL))
			return 1;
	}
	free(network);
	network = NULL;
	free(network0);
	network0 = NULL;
	if (!(network==NULL))
		return 1;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//�������
//����һ��txt�ļ���������д����
int OutPut(void)
{
	//��data.txt�����û�оʹ���
	FILE *fpwrite = fopen("data.txt","w");
	if (fpwrite == NULL)
	{
		printf("���ļ�ʧ��\n");
		return 1;
	}

	//��ʼд�ļ�(ÿдһ��������һС��)

	//д�����������
	fprintf(fpwrite,"%.8f,",layer_all*1.0);

	//д��������ÿ�������ڵ���
	int i = 0;
	for (i = 0; i < layer_all; i++)
	{
		fprintf(fpwrite,"%.8f,",nodes_all[i]*1.0);
	}
	int j = 0, k = 0;

	//д������ӳ�������
	for (i = 0; i < ROW_X; i++)
	{
		fprintf(fpwrite, "%.8f,", xminmax[i][0] * 1.0);//min
		fprintf(fpwrite, "%.8f,", xminmax[i][1] * 1.0);//max
	}
	for (i = 0; i < ROW_Y; i++)
	{
		fprintf(fpwrite, "%.8f,", yminmax[i][0] * 1.0);//min
		fprintf(fpwrite, "%.8f,", yminmax[i][1] * 1.0);//max
	}


	//д��ÿ���ڵ�Ĳ���

	//����������ÿ��ÿ���ڵ㣬��������
	for (i = 0; i < layer_all - 1; i++)
	{
		for (j = 0; j < nodes_all[i]; j++)
		{
			//д��w
			for (k = 0; k < nodes_all[i + 1]; k++)
			{
				fprintf(fpwrite, "%.8f,", network[i][j].w[k]);
			}

			//д��b
			fprintf(fpwrite, "%.8f,", network[i][j].b);
		}
	}
	//�ر��ļ�
	fclose(fpwrite);
	return 0;
}
