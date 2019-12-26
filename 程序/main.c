#include <STC12C5A60S2.H>
#include <math.h>
#include "st7565.h"
#include "TLC0820.h"
#include "osc_scope.h"
#include "string.h"

void main(void)
{
	Osc_Init();			//示波器初始化（显示屏）
	TLC0820_Init();	//AD转换器初始化
	ADC_Start();		//开始AD转换
	EA=1;						//开启总中断
	while(1)
	{
//	Lcd12864_Writestr(0, 1,"fhsdtwr");
		if(TLC0820_DATA.Trig_time)	//触发电平设置扫描大概50ms一次，等待50ms计时标志位置位
		{
			TLC0820_DATA.Trig_time=0;	//清零标志位
			Trig_scan();							//扫描按键并设置触发电平
		}
		if(TLC0820_DATA.show_time && TLC0820_DATA.Finish_Flag)	//波形界面每1秒刷新一次，等待时间到达和一次波形采集完成
		{
			TLC0820_DATA.show_time =0;//清零标志位
			EA=0;											//关闭总中断，即关闭波形采集，等待波形数据处理完成再开启
			Lcd12864_ClearScreen();		//清屏
			Time_scan();							//判断是否进行了水平（时间）方向大小设置
			amp_scan();								//判断是否进行了垂直（幅值）方向大小设置
			Osc_FirstGUI(90);					//显示基础界面，即X,Y方向上的刻度值
			Show_Wave(TLC0820_DATA.Trig_Vol,1,90,200,&ADC_BUF);		
			//将采样深度为200的的ADC_BUF数组内的波形数据，上升沿触发（1），触发电平是TLC0820_DATA.Trig_Vol，显示大小是90的波形显示出来
			Calcu_AmpFreq(0,90,&ADC_BUF);	//计算所采波形的幅值和频率并显示
			memset(&ADC_BUF,0,sizeof(ADC_BUF));			//清零波形数据缓冲区
			ADC_Start();	//开始采集下一批波形数据
			EA=1;					//开启中断
		}
	}
}