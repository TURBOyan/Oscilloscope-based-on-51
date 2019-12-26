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

float EnlargeData[6]=		//放大倍数选择
{
	 1,    
	 1.99,   
	 5.66,  
	 10.13,  
	 17.7,   
	 48      
};

uchar xdata ADC_BUF[250];									//波形缓冲区
TLC0820_DATA_Typedef xdata TLC0820_DATA;	//保存所有有关AD采样数据的结构体	

void TLC0820_Init(void)
{
	AUXR = 0x00;		//定时器时钟1T模式
	TMOD &= 0x00;		
	TMOD |= 0x01;		//设置定时器1和0为16位定时器模式 
	TMOD |= 0X10;

	TH1  =0x3c;	//设置定时器1定时50ms
	TL1  =0xaf;
	TR1=1;			//开启定时器1
	ET1=1;			//开启定时器1中断
	TF1=0;			//清除定时器1中断标志位
	
	TLC0820_DATA.Time_choose=0;	//初始化定时器0定时时间选择位
	
	TLC0820_DATA.Set_Timer_us_H=TimeData[TLC0820_DATA.Time_choose][0];	//设置初始采样时间为10us
	TLC0820_DATA.Set_Timer_us_L=TimeData[TLC0820_DATA.Time_choose][1];
	
	TLC0820_DATA.Trig_Vol=127;		//设置初始触发电平
	TLC0820_DATA.Trig_Flag=0;
	TLC0820_DATA.show_time=0;
	
	TLC0820_DATA.enlarge_choose=0;	//初始化放大倍数选择位
	TLC0820_DATA.enlarge=EnlargeData[TLC0820_DATA.enlarge_choose];		//设置初始放大倍数为1倍
	
	TH0=TLC0820_DATA.Set_Timer_us_H;		//根据之前的设置设置采样时间
	TL0=TLC0820_DATA.Set_Timer_us_L;		
	TF0 = 0;		//清除TF0标志
  IT0=1;         	 //下降沿触发
	IPH=0x03;	//设置中断优先级
	IP=0x02;
}

void ADC_Start(void)
{
	TLC0820_DATA.Point=0;		//清零采样计数
	TLC0820_DATA.Finish_Flag=0;	//清零采样完成标志位
	TH0=TLC0820_DATA.Set_Timer_us_H;	//重新给定时器0赋初值
	TL0=TLC0820_DATA.Set_Timer_us_L;
	ET0=1;	//开启采样定时器中断
	TR0=1;
}

void Timer1_isr(void) interrupt 3	//采样周期定时器中断
{
	static uint time;
	TH1  =0x3C;	//重新给定时器1赋初值
	TL1  =0xAF;
	time++;

	TLC0820_DATA.Trig_time=1;		//置位50ms标志位
	
	if(time >= 20)
	{
		time=0;
		TLC0820_DATA.show_time=1;	//置位1秒标志位
	}
}

void Timer0_isr(void) interrupt 1	//采样周期定时器中断
{
	TH0=TLC0820_DATA.Set_Timer_us_H;		//定时器1重新赋初值
	TL0=TLC0820_DATA.Set_Timer_us_L;	
	TF0=0;	//清零标志位
	TLC0820_WR=0;		//给TLC0820的WR引脚一个1us的低电平触发一次AD采样
	TLC0820_WR=0;
	TLC0820_WR=1;
	while(TLC0820_INT == 0);		//等待采样完成
	ADC_BUF[TLC0820_DATA.Point++]=TLC0820_data;		//保存此次波形的电压值
	if(TLC0820_DATA.Point>=199)		//直到采样达到采样深度，即一次波形采集完成，关闭采样中断，置位采样完成标志位，开始后续处理
	{
		ET0=0;
		TR0=0;
		TLC0820_DATA.Point=0;
		TLC0820_DATA.Finish_Flag=1;
	}
}