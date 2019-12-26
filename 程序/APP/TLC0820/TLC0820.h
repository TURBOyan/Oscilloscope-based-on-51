#ifndef _TLC0820_H_
#define _TLC0820_H_

#include <STC12C5A60S2.H>
#include "osc_scope.h"

#define TLC0820_data P0

extern uint Timer[6];
extern uchar xdata ADC_BUF[250];
extern uchar TimeData[6][2];
extern float EnlargeData[6];

typedef struct
{
	uchar Trig_Now;
	uchar Trig_Last;
	uchar Trig_Flag;
	uint  Trig_Vol;
	
	float amp;
	float enlarge;
	int enlarge_choose;
	long  freq;
	
  uint Set_Timer_us_H;
	uint Set_Timer_us_L;
	uchar Finish_Flag;
	uchar Point;
	
	uint show_time;
	uint Trig_time;
	
	int Time_choose;
}TLC0820_DATA_Typedef;

extern TLC0820_DATA_Typedef xdata TLC0820_DATA;

sbit TLC0820_WR=P1^0;
sbit TLC0820_INT=P3^2;

void TLC0820_Init(void);
void ADC_Start(void);


#endif