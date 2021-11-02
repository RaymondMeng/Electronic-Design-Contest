#include"BPNetwork.h"
#include<stdlib.h>
#include"rand.h"
#include<math.h>
#include<stdio.h>

//////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////优化日记/////////////////////////////////////
//1、增加动量项
//2、学习速率可调




struct netnode **network=NULL;//神经网络的节点参数二维动态数组
struct netnode **network0 = NULL;//神经网络的节点参数二维动态数组
int *nodes_all=NULL;
int layer_all=0;
long maxepoch;
float xminmax[ROW_X][2];
float yminmax[ROW_Y][2];
float momentum_w=0;//增加动量项
float momentum_b = 0;//增加动量项
float mseerror = 0;//误差
float error0 = 100000;//上一个误差0
float speed = 0.1;//学习速率
float rate_w = 0.00;//动量项系数
float rate_b = 0.00;//动量项系数
float rate_w0 = 0.9;//动量项系数
float rate_b0 = 0.9;//动量项系数
float rate_up = 0.00001;//学习速率上升速率
float rate_down =0.001;//学习速率下降速率
void networkTrain(float x[ROW_X][COL_X], float y[ROW_Y][COL_Y], int layer, int nodes[], int mode1)
{   
	//首先，建立神经网络节点
	if (netSetup(ROW_X, layer, nodes, ROW_Y))
	{
//		error("网络节点建立失败");
	}

	//然后，初始化参数
	if (netInit())
	{
//		error("网络参数初始化失败");
	}
	//数据预处理，映射
	minmax(x, y,xminmax,yminmax, mode1);
	//然后进入迭代求解
	maxepoch = 8000;
	netSolve(x, y, maxepoch, 0.01, mode1);
	
	//输出参数
	if (OutPut())
	{
		printf("输出参数失败\n");
	}
	else
	{
		printf("输出参数成功\n");
	}
	//释放内存
	if (freeNet())
	{
		printf("内存释放失败\n");
	}
	else
	{
		printf("内存释放成功\n");
	}
}



//网络参数初始化
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







//神经网络建立结构节点函数
//输入参数，输入层节点个数num_x，隐含层层数layer，隐含层每层节点数nodes，以及输出层节点数num_y
int netSetup(int num_x, int layer, int nodes[], int num_y)
{
	int i = 0;

	network = (struct netnode**)malloc(sizeof(struct netnode*)*(layer + 2));//申请行数
	network0 = (struct netnode**)malloc(sizeof(struct netnode*)*(layer + 2));//申请行数
	for (i = 0; i < layer + 2; i++)
	{
		if (i == 0)
		{
			network[i] = (struct netnode*)malloc(sizeof(struct netnode)*num_x);//申请每行列数
			network0[i] = (struct netnode*)malloc(sizeof(struct netnode)*num_x);//申请每行列数
		}
		else if (i == layer + 1)
		{
			network[i] = (struct netnode*)malloc(sizeof(struct netnode)*num_y);//申请每行列数
			network0[i] = (struct netnode*)malloc(sizeof(struct netnode)*num_y);//申请每行列数
		}
		else
		{
			network[i] = (struct netnode*)malloc(sizeof(struct netnode)*nodes[i - 1]);//申请每行列数
			network0[i] = (struct netnode*)malloc(sizeof(struct netnode)*nodes[i - 1]);//申请每行列数
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
//神经网络求解函数
//步骤如下：
//第一，清零累积更新
//第二步，循环所有样本
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
		printf("误差 %f\n",error/COL_X*2);
		epoch++;
	}
}




//清零累积更新
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

//循环所有样本
float calAll(float x[ROW_X][COL_X], float y[ROW_Y][COL_Y], int mode1, long epoch)
{
	int i = 0,j=0,k=0;
	float error = 0;
	for (k = 0; k < COL_X; k++)
	{
		//计算输入输出
		calInputAndOutPut(x,y,k,mode1);

		//计算误差
		error += calError(y, k,mode1,epoch);

		//计算反向过程
		bachward(y,k,mode1);
	}
	mseerror = error;
	updata();
	error0 = mseerror;
	return error;
}

//计算输入输出
int calInputAndOutPut(float x[ROW_X][COL_X], float y[ROW_Y][COL_Y],int k,int mode1)
{
	int i = 0, j = 0,m=0;
	double input = 0;
	for (i = 0; i < layer_all; i++)
	{
		//如果是输入层
		if (i == 0)
		{
			for (j = 0; j < nodes_all[i]; j++)
			{
				network[i][j].input = x[j][k];
				network[i][j].output = calInspirit(network[i][j].input, network[i][j].b,mode1);
			}
		}
		//如果是其他层
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
				//如果不是输出层
				if (i != layer_all - 1)
					network[i][j].output = calInspirit(network[i][j].input, network[i][j].b,mode1);
				else
					network[i][j].output = network[i][j].input;
			}
		}
	}
	return 0;
}

//mode1==0,激励函数（sig）
//y=1/(1+exp(x))
//mode1==1,激励函数(正切tansig)
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

//计算误差函数
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
			printf("理想输出： %f , 实际输出： %f     ", recoverOutput(y[i][k], i, mode1), recoverOutput(network[layer_all - 1][i].output, i, mode1));
			if (i == ROW_Y - 1)
				printf("\n");
		}
	}
	if (epoch==maxepoch - 1)
	    printf("\n");
	error1 /= 2;
	return error1;
}

//计算反向更新
int bachward(float y[ROW_Y][COL_Y], int k,int mode1)
{
	int i = 0, j = 0,m=0;
	double s1 = 0, dy = 0;

	//计算累积分量
	for (i = layer_all-1; i >=0; i--)
	{
		for (j = 0; j < nodes_all[i]; j++)
		{
			//如果是输出层
			if (i == layer_all - 1)
			{
				s1 = network[i][j].output - y[j][k];
				network[i][j].s1 = s1;
			}
			//如果是其他层
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

	//计算w累积更新
	//从输出层的前一层开始
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

	//更新学习速率
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



//保存参数
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

	//处理x
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
			//压缩到-1到1
			{
				x[i][j] *= 2;
				x[i][j] -= 1;
			}
		}
	}

	//处理y
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
	//压缩到0-1区间
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
//参数输出
//创建一个txt文件，向里面写参数
int OutPut(void)
{
	//打开data.txt，如果没有就创建
	FILE *fpwrite = fopen("data.txt","w");
	if (fpwrite == NULL)
	{
		printf("打开文件失败\n");
		return 1;
	}

	//开始写文件(每写一个数，空一小格)

	//写入神经网络层数
	fprintf(fpwrite,"%.8f,",layer_all*1.0);

	//写入神经网络每层的网络节点数
	int i = 0;
	for (i = 0; i < layer_all; i++)
	{
		fprintf(fpwrite,"%.8f,",nodes_all[i]*1.0);
	}
	int j = 0, k = 0;

	//写入数据映射的数据
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


	//写入每个节点的参数

	//遍历神经网络每层每个节点，输出层除外
	for (i = 0; i < layer_all - 1; i++)
	{
		for (j = 0; j < nodes_all[i]; j++)
		{
			//写入w
			for (k = 0; k < nodes_all[i + 1]; k++)
			{
				fprintf(fpwrite, "%.8f,", network[i][j].w[k]);
			}

			//写入b
			fprintf(fpwrite, "%.8f,", network[i][j].b);
		}
	}
	//关闭文件
	fclose(fpwrite);
	return 0;
}
