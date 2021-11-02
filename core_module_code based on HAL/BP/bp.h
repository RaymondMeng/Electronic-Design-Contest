#ifndef _BP_H_
#define _BP_H_

#define SIMROW_X 2
#define SIMROW_Y 1
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//BP�������ⲿ���

//������ڵ�
struct simnetnode
{
	double input, output, b;//��Ԫ�����룬�����ƫ��
	double* w;//��Ԫ��Ȩֵ
};

//��ȡ����
int readData(void);

//����������ڵ�
int simNetSetUp(void);

//��������㺯��
int sim(float x[SIMROW_X], float y[SIMROW_Y]);

//��������
double simInspirit(double x, double b, int mode1);

//��ʼ��
int simInit();

//����ӳ��
double simDataChange(double x, int j, int mode1);

//���ݻָ�
double simDataRecover(double y, int j, int mode1);


//�ͷ��ڴ�
int simFree(void);


#endif