#include <STC12C5A60S2.H>
#include <math.h>
#include "st7565.h"
#include "TLC0820.h"
#include "osc_scope.h"
#include "string.h"

void main(void)
{
	Osc_Init();			//ʾ������ʼ������ʾ����
	TLC0820_Init();	//ADת������ʼ��
	ADC_Start();		//��ʼADת��
	EA=1;						//�������ж�
	while(1)
	{
//	Lcd12864_Writestr(0, 1,"fhsdtwr");
		if(TLC0820_DATA.Trig_time)	//������ƽ����ɨ����50msһ�Σ��ȴ�50ms��ʱ��־λ��λ
		{
			TLC0820_DATA.Trig_time=0;	//�����־λ
			Trig_scan();							//ɨ�谴�������ô�����ƽ
		}
		if(TLC0820_DATA.show_time && TLC0820_DATA.Finish_Flag)	//���ν���ÿ1��ˢ��һ�Σ��ȴ�ʱ�䵽���һ�β��βɼ����
		{
			TLC0820_DATA.show_time =0;//�����־λ
			EA=0;											//�ر����жϣ����رղ��βɼ����ȴ��������ݴ�������ٿ���
			Lcd12864_ClearScreen();		//����
			Time_scan();							//�ж��Ƿ������ˮƽ��ʱ�䣩�����С����
			amp_scan();								//�ж��Ƿ�����˴�ֱ����ֵ�������С����
			Osc_FirstGUI(90);					//��ʾ�������棬��X,Y�����ϵĿ̶�ֵ
			Show_Wave(TLC0820_DATA.Trig_Vol,1,90,200,&ADC_BUF);		
			//���������Ϊ200�ĵ�ADC_BUF�����ڵĲ������ݣ������ش�����1����������ƽ��TLC0820_DATA.Trig_Vol����ʾ��С��90�Ĳ�����ʾ����
			Calcu_AmpFreq(0,90,&ADC_BUF);	//�������ɲ��εķ�ֵ��Ƶ�ʲ���ʾ
			memset(&ADC_BUF,0,sizeof(ADC_BUF));			//���㲨�����ݻ�����
			ADC_Start();	//��ʼ�ɼ���һ����������
			EA=1;					//�����ж�
		}
	}
}