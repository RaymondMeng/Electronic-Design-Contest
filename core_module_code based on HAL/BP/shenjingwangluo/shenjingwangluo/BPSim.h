#ifndef BPSIM_H
#define BPSIM_H

#define SIMROW_X 2
#define SIMROW_Y 1
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//BP神经网络外部输出

//神经网络节点
struct simnetnode
{
	double input, output, b;//神经元的输入，输出，偏置
	double* w;//神经元的权值
};

//读取数据
int readData(void);

//建立神经网络节点
int simNetSetUp(void);

//神经网络计算函数
int sim(float x[SIMROW_X], float y[SIMROW_Y]);

//激励函数
double simInspirit(double x, double b, int mode1);

//初始化
int simInit();

//数据映射
double simDataChange(double x, int j, int mode1);

//数据恢复
double simDataRecover(double y, int j, int mode1);


//释放内存
int simFree(void);


#endif