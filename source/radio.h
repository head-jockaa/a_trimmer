#ifndef RADIO__
#define RADIO__

#include "util.h"

void estimate();
void receiveAll(double X, double Y);
void startSMR(int st);
double asin_q(double D, double X, double Y);
double acos_q(double D, double X, double Y);
double atan_q(double X, double Y);
int receive_mg(int aim_tower, int CH, int DIR);

extern BOOL saveSMR, SMRscanned;
extern int SMRcount,SMRfast;
extern Uint16 atan_table[60000], asin_table[10001], acos_table[10001];

struct RadioData{
	BOOL received,estimated;
	int bias_dir;
	double tower_dis_multi2;
	int seeDIS[1000], seeH[1000], seeNUM[1000], seeM[1000], see_mounts;
	BOOL seeCITY[1000];
	double seeDIR[1000];
};
extern RadioData rd;

#endif