#ifndef BPNETWORK_H
#define BPNETWORK_H
//BP������ͷ�ļ�

#define ROW_X 2
#define COL_X 63
#define ROW_Y 1
#define COL_Y 63
#define LAYER 2

//��Ԫ�ṹ��
struct netnode
{
	double input,output,b,b_change,s1;//��Ԫ�����룬�����ƫ��
	double* w, *w_change;//��Ԫ��Ȩֵ
};


//����Ԥ����ת����0-1��-1��1������
void minmax(float x[ROW_X][COL_X], float y[ROW_Y][COL_Y], float xminmax[ROW_X][2],float yminmax[ROW_Y][2], int mode1);

//����ָ���ԭ��������
float recoverOutput(float output,int j,int mode1);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//������ѵ���������ܺ�����
//�����������������x���������y�����������layer��ÿ��ڵ���nodes��ģʽmode
void networkTrain(float x[ROW_X][COL_X], float y[ROW_Y][COL_Y], int layer, int nodes[], int mode1);


//�����罨���ṹ�ڵ㺯��
//��������������ڵ����num_x�����������layer��������ÿ��ڵ���nodes���Լ������ڵ���num_y
int netSetup(int num_x,int layer,int nodes[],int num_y);


//�������ʼ����������
//
int netInit(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��������⺯��ģ��
void netSolve(float x[ROW_X][COL_X], float y[ROW_Y][COL_Y],long epoch_max,float error_min,int mode1);

//�����ۻ�����
int clearChange(void);

//ѭ����������
float calAll(float x[ROW_X][COL_X], float y[ROW_Y][COL_Y], int mode1, long epoch);

//�����������
int calInputAndOutPut(float x[ROW_X][COL_X], float y[ROW_Y][COL_Y],int k,int mode1);

//��������
double calInspirit(double input, double b, int mode1);

//�������
float calError(float y[ROW_Y][COL_Y], int k, int mode1, long epoch);

//�������
int savedata(int mode);

//���㷴�����
int bachward(float y[ROW_Y][COL_Y], int k,int mode1);

//�������
int updata(void);

//�ͷ�������ڴ�
int freeNet(void);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ⲿ�����������,(����������������)
int OutPut(void);
#endif