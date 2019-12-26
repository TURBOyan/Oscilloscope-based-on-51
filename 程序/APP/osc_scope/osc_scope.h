#ifndef _OSC_SCOPE_H_
#define _OSC_SCOPE_H_

#include "st7565.h"

#define Cell_length 2

#define XTAL_freq_Mhz 12

sbit Button_TimeUp  =P1^1;
sbit Button_TimeDown=P1^2;
sbit Button_TrigUp  =P1^3;
sbit Button_TrigDown=P1^4;
sbit Button_wait1   =P1^5;
sbit Button_wait2   =P1^6;

typedef struct
{
	uchar data_t[15];
	uint  amp;
	uint  freq;
}osc_data_typedef;

void Osc_Init(void);
void Osc_FirstGUI(uchar width);
void Calcu_AmpFreq(uchar start,uchar end,uchar *data_1);
void Show_Wave(uchar level,uchar jump,uchar show_weith,uint Wave_length,uchar *wave);
void Trig_scan(void);
void Time_scan(void);
void amp_scan(void);
void Delay10ms(unsigned int c);

#endif