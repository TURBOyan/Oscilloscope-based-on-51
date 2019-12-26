#include "TLC0820.h"
#include "st7565.h"
#include "osc_scope.h"
uchar TimeData[6][2]=
{
		{0xff,0xf9},	//10us
		{0xff,0xef},  //20us
		{0xff,0xd1},  //50us
		{0xff,0x9f},  //100us
		{0xff,0x3b},  //200us
		{0xfe,0x0d},  //500us
};

float EnlargeData[6]=		//�Ŵ���ѡ��
{
	 1,    
	 1.99,   
	 5.66,  
	 10.13,  
	 17.7,   
	 48      
};

uchar xdata ADC_BUF[250];									//���λ�����
TLC0820_DATA_Typedef xdata TLC0820_DATA;	//���������й�AD�������ݵĽṹ��	

void TLC0820_Init(void)
{
	AUXR = 0x00;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x00;		
	TMOD |= 0x01;		//���ö�ʱ��1��0Ϊ16λ��ʱ��ģʽ 
	TMOD |= 0X10;

	TH1  =0x3c;	//���ö�ʱ��1��ʱ50ms
	TL1  =0xaf;
	TR1=1;			//������ʱ��1
	ET1=1;			//������ʱ��1�ж�
	TF1=0;			//�����ʱ��1�жϱ�־λ
	
	TLC0820_DATA.Time_choose=0;	//��ʼ����ʱ��0��ʱʱ��ѡ��λ
	
	TLC0820_DATA.Set_Timer_us_H=TimeData[TLC0820_DATA.Time_choose][0];	//���ó�ʼ����ʱ��Ϊ10us
	TLC0820_DATA.Set_Timer_us_L=TimeData[TLC0820_DATA.Time_choose][1];
	
	TLC0820_DATA.Trig_Vol=127;		//���ó�ʼ������ƽ
	TLC0820_DATA.Trig_Flag=0;
	TLC0820_DATA.show_time=0;
	
	TLC0820_DATA.enlarge_choose=0;	//��ʼ���Ŵ���ѡ��λ
	TLC0820_DATA.enlarge=EnlargeData[TLC0820_DATA.enlarge_choose];		//���ó�ʼ�Ŵ���Ϊ1��
	
	TH0=TLC0820_DATA.Set_Timer_us_H;		//����֮ǰ���������ò���ʱ��
	TL0=TLC0820_DATA.Set_Timer_us_L;		
	TF0 = 0;		//���TF0��־
  IT0=1;         	 //�½��ش���
	IPH=0x03;	//�����ж����ȼ�
	IP=0x02;
}

void ADC_Start(void)
{
	TLC0820_DATA.Point=0;		//�����������
	TLC0820_DATA.Finish_Flag=0;	//���������ɱ�־λ
	TH0=TLC0820_DATA.Set_Timer_us_H;	//���¸���ʱ��0����ֵ
	TL0=TLC0820_DATA.Set_Timer_us_L;
	ET0=1;	//����������ʱ���ж�
	TR0=1;
}

void Timer1_isr(void) interrupt 3	//�������ڶ�ʱ���ж�
{
	static uint time;
	TH1  =0x3C;	//���¸���ʱ��1����ֵ
	TL1  =0xAF;
	time++;

	TLC0820_DATA.Trig_time=1;		//��λ50ms��־λ
	
	if(time >= 20)
	{
		time=0;
		TLC0820_DATA.show_time=1;	//��λ1���־λ
	}
}

void Timer0_isr(void) interrupt 1	//�������ڶ�ʱ���ж�
{
	TH0=TLC0820_DATA.Set_Timer_us_H;		//��ʱ��1���¸���ֵ
	TL0=TLC0820_DATA.Set_Timer_us_L;	
	TF0=0;	//�����־λ
	TLC0820_WR=0;		//��TLC0820��WR����һ��1us�ĵ͵�ƽ����һ��AD����
	TLC0820_WR=0;
	TLC0820_WR=1;
	while(TLC0820_INT == 0);		//�ȴ��������
	ADC_BUF[TLC0820_DATA.Point++]=TLC0820_data;		//����˴β��εĵ�ѹֵ
	if(TLC0820_DATA.Point>=199)		//ֱ�������ﵽ������ȣ���һ�β��βɼ���ɣ��رղ����жϣ���λ������ɱ�־λ����ʼ��������
	{
		ET0=0;
		TR0=0;
		TLC0820_DATA.Point=0;
		TLC0820_DATA.Finish_Flag=1;
	}
}