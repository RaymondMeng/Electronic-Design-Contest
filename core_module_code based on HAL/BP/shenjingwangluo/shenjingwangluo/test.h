#ifndef TEST_H
#define TEST_H
#include"BPNetwork.h"
//≤‚ ‘Õ∑Œƒº˛

//≤‚ ‘netSetup()
void test_netSetup(void);

void test_netInit(void);

void test_clearChange(void);

void test_calInspirit(void);

void test_calInputAndOutPut(float x[ROW_X][COL_X], float y[ROW_X][COL_X], int k);

float test_calError(float x[ROW_X][COL_X], float y[ROW_X][COL_X], int k);

void test_bachward(float x[ROW_X][COL_X], float y[ROW_Y][COL_Y], int k);

void test_sim(void);

void test_data();

void test_all1(void);
#endif