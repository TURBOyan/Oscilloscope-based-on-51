#include "osc_scope.h"
#include "TLC0820.h"

osc_data_typedef osc_data;

/*!
*  @描述       显示屏初始化
*  @参数       无
*  示例:       Osc_Init()；
*/
void Osc_Init(void)
{
	Lcd12864_Init();
	Lcd12864_ClearScreen();
	Delay10ms(1);
}

/*!
*  @描述       波形显示区坐标轴绘制
*  @参数       width  显示宽度（0到127）
*  示例:       Osc_FirstGUI(90)；	//在0到90的宽度范围内绘制波形坐标轴
*/
void Osc_FirstGUI(uchar width)		
{
	uchar x=0,y=0,x_last=0,y_last=0;

	Lcd12864_WriteLine(0,2,0,62);
	Lcd12864_WriteLine(width,2,width,62);	
	for(y=2;y<=62;y+=6)
	{
		Lcd12864_WriteLine(1,y,Cell_length,y);
		Lcd12864_WriteLine(width-Cell_length,y,width-1,y);
	}
	for(y=0;y<width;y+=9)
	{
		Lcd12864_WritePoint(y, 32);
	}
	for(y=2;y<=62;y+=3)
	{
		Lcd12864_WritePoint((uchar)(width/2.0), y);
	}
}

/*!
*  @描述       波形显示
*  @参数       level（触发电平）
							 jump（跳变沿触发，1是上升沿，0是下降沿）
							 show_weith(显示深度)
							 Wave_length（传入的采样波形的深度）
							 *wave（波形缓冲区指针）
*  示例:      Show_Wave(128,1,90,200,&ADC_BUF);		
			将采样深度为200的的ADC_BUF数组内的波形数据，上升沿触发（1），触发电平是128（也就是0V），显示大小是90的波形显示出来
*/
void Show_Wave(uchar level,uchar jump,uchar show_weith,uint Wave_length,uchar *wave)	
{
	uchar x,y;
	uchar show_y1,show_y2;
	switch(jump)
	{ 
		case 1:
					for(x=5;x<=Wave_length-show_weith;x++)
					{
						//当出现触发条件时（上升沿）
						if((wave[x] >= wave[x-1]) && wave[x] >= level-10 && wave[x] <= level+10)
						{
							for(y=1;y<show_weith;y++)
							{
								show_y1=(255-wave[x+y-1])>>2;
								show_y2=(255-wave[x+y])>>2;
								Lcd12864_WriteLine(y-1,show_y1 ,y,show_y2);
							}
							break;
						}
						//当从未出现触发条件时，从第一个开始显示
						if(x==(Wave_length-show_weith-1))
						{
							for(y=1;y<show_weith;y++)
							{
								show_y1=(255-wave[y-1])>>2;
								show_y2=(255-wave[y])>>2;
								Lcd12864_WriteLine(y-1,show_y1 ,y,show_y2);
							}
							break;
						}
					}break;
		
		case 0:
					for(x=5;x<=Wave_length-show_weith;x++)
					{
						//当出现触发条件时（下降沿）,从出现触发条件处开始显示
						if((wave[x] <= wave[x-1])&& wave[x] >= level-10 && wave[x] <= level+10)
						{
							for(y=1;y<show_weith;y++)
							{
								show_y1=(255-wave[x+y-1])>>2;
								show_y2=(255-wave[x+y])>>2;
								Lcd12864_WriteLine(y-1,show_y1 ,y,show_y2);
							}
							break;
						}
						//当从未出现触发条件时，从第一个开始显示
						if(x==(Wave_length-show_weith-1))
						{
							for(y=1;y<show_weith;y++)
							{
								show_y1=(255-wave[y-1])>>2;
								show_y2=(255-wave[y])>>2;
								Lcd12864_WriteLine(y-1,show_y1 ,y,show_y2);				
							}
							break;
						}		
					}break;
		default: break;
	}
}

/*!
*  @描述       计算波形峰峰值和频率
*  @参数       start（需要计算的波形的开始位置）
							 end(需要计算的波形的结束位置)
*  示例:       Calcu_AmpFreq(0,90,&ADC_BUF)
							从数组ADC_BUF内的0到90的波形开始计算峰峰值和频率
*/
void Calcu_AmpFreq(uchar start,uchar end,uchar *data_1)
{
	uchar i,j;
	uint xdata freq_buff[200],zero_num=0;
	long time=0;
	
	/************************以下为求峰峰值*********************************************************************************************************************************/
	int databuff_max[3]={0,0,0},databuff_min[3]={255,255,255};
	for(i=start;i<end/3;i++)		//一批波形数据分三份，分别求最大值和最小值，最终冒泡排序，去掉最大值和最小值，求取此批次波形最大值和最小值，为的是减小误差
	{
		if(data_1[i]>databuff_max[0])databuff_max[0]=data_1[i];
		if(data_1[i]<databuff_min[0])databuff_min[0]=data_1[i];
	}
	for(i=end/3;i<(2*end/3);i++)
	{
		if(data_1[i]>databuff_max[1])databuff_max[1]=data_1[i];
		if(data_1[i]<databuff_min[1])databuff_min[1]=data_1[i];
	}
	for(i=(2*end/3);i<end;i++)
	{
		if(data_1[i]>databuff_max[2])databuff_max[2]=data_1[i];
		if(data_1[i]<databuff_min[2])databuff_min[2]=data_1[i];
	}
	for(i=1;i<3;i++)
	{
		if(databuff_max[i]>databuff_max[i-1])
		{
			j=databuff_max[i-1];
			databuff_max[i-1]=databuff_max[i];
			databuff_max[i]=j;
		}
		if(databuff_min[i]>databuff_min[i-1])
		{
			j=databuff_min[i-1];
			databuff_min[i-1]=databuff_max[i];
			databuff_min[i]=j;
		}
	}
	TLC0820_DATA.amp=(((databuff_max[1]-databuff_min[1])/255.0)*5.0*2)/TLC0820_DATA.enlarge;//根据放大倍数换算输入波形的峰峰值，并保存在TLC0820_DATA.amp内
	
	if(TLC0820_DATA.amp<0)TLC0820_DATA.amp=0;		//峰峰值限幅
	
/************************以下为求频率*******************************************************************************************************************************/
	for(i=start+1;i<end;i++)			//求取所选择波形的上升沿过零点的次数，并保存每次零点时的时间
	{
		if(data_1[i+1]>=((databuff_max[1]+databuff_min[1])/2.0)&& (data_1[i] <= ((databuff_max[1]+databuff_min[1])/2.0)))
		{
			freq_buff[zero_num]=i;
			zero_num++;
		}
	}
	zero_num--;
	switch(TLC0820_DATA.Time_choose)		//获得此次波形的采样周期，也就是所采两个点之间的时间间隔
	{
		case 0:time=((freq_buff[zero_num]-freq_buff[0])/(zero_num*1.0))*10;
					 break;
		case 1:time=((freq_buff[zero_num]-freq_buff[0])/(zero_num*1.0))*20;
					 break;
		case 2:time=((freq_buff[zero_num]-freq_buff[0])/(zero_num*1.0))*50;
					 break;
		case 3:time=((freq_buff[zero_num]-freq_buff[0])/(zero_num*1.0))*100;
					 break;
		case 4:time=((freq_buff[zero_num]-freq_buff[0])/(zero_num*1.0))*200;
					 break;
		case 5:time=((freq_buff[zero_num]-freq_buff[0])/(zero_num*1.0))*500;
					 break;
		default: break;
	}
	TLC0820_DATA.freq=(long)1000000/(time*1.0);		//换算出频率，并保存在TLC0820_DATA.freq结构体变量内

/************************以下为频率和峰峰值显示**********************************************************************************************/
	if(TLC0820_DATA.amp < 1)	
	{
		
		if(TLC0820_DATA.amp*1000<100)
		{
			Lcd12864_WriteInt(95,0, (uint)(TLC0820_DATA.amp*1000), 2);
		}
		else
		{
			Lcd12864_WriteInt(95,0, (uint)(TLC0820_DATA.amp*1000), 3);
		}
		Lcd12864_Writestr(100,1,"mV");
	}
	else if(TLC0820_DATA.amp >= 10)			
	{
		Lcd12864_Writeflo(95,0,TLC0820_DATA.amp,2);
		Lcd12864_Writestr(105,1,"V");
	}
	else
	{
		Lcd12864_Writeflo(95,0,TLC0820_DATA.amp,1);
		Lcd12864_Writestr(105,1,"V");
	}
	
	if(TLC0820_DATA.freq>1000)
	{
		Lcd12864_Writeflo(95,4,TLC0820_DATA.freq/1000.0,1);
		Lcd12864_Writestr(100,5,"KHz");
	}
	else
	{
		if(TLC0820_DATA.freq<100)
		{
			Lcd12864_WriteInt(95,4, (uint)TLC0820_DATA.freq, 2);
		}
		else
		{
			 Lcd12864_WriteInt(95,4, (uint)TLC0820_DATA.freq, 3);
		}
		Lcd12864_Writestr(100,5,"Hz");
	}
	
}

/*!
*  @描述       扫描按键设置触发电平
*  @参数       无
*  示例:       Trig_scan()；
*/

void Trig_scan(void)
{
	if(Button_TrigUp == 0)//如果上键按下，触发电平上移
	{
			TLC0820_DATA.Trig_Vol++;		
			if(TLC0820_DATA.Trig_Vol>254)TLC0820_DATA.Trig_Vol=254;		
	}
	if(Button_TrigDown == 0)	//如果下键按下，触发电平下移
	{	
			TLC0820_DATA.Trig_Vol--;		
			if(TLC0820_DATA.Trig_Vol<=1)TLC0820_DATA.Trig_Vol=1;	
	}
	//显示指示触发电平的小箭头
	Lcd12864_WriteLine(90, (255-TLC0820_DATA.Trig_Vol)>>2,93,(255-TLC0820_DATA.Trig_Vol+12)>>2);
	Lcd12864_WriteLine(90, (255-TLC0820_DATA.Trig_Vol)>>2,93,(255-TLC0820_DATA.Trig_Vol-12)>>2);
	Lcd12864_WriteLine(90, (255-TLC0820_DATA.Trig_Vol)>>2,94,(255-TLC0820_DATA.Trig_Vol)>>2);
}



/*!
*  @描述       扫描按键设置水平方向大小
*  @参数       无
*  示例:       Time_scan()；
*/
void Time_scan(void)
{
	if(Button_TimeUp == 0)
	{
			TLC0820_DATA.Time_choose++;
			if(TLC0820_DATA.Time_choose>5)TLC0820_DATA.Time_choose=5;
	}
	if(Button_TimeDown == 0)
	{
			TLC0820_DATA.Time_choose--;
			if(TLC0820_DATA.Time_choose<0)TLC0820_DATA.Time_choose=0;
	}
	TLC0820_DATA.Set_Timer_us_H = TimeData[TLC0820_DATA.Time_choose][0];
	TLC0820_DATA.Set_Timer_us_L = TimeData[TLC0820_DATA.Time_choose][1];
	switch(TLC0820_DATA.Time_choose)
	{
		case 0:Lcd12864_WriteInt(100, 6, 100, 3);
					 Lcd12864_Writestr(91, 7,"us/div");break;
		case 1:Lcd12864_WriteInt(100, 6, 200, 3);
					 Lcd12864_Writestr(91, 7,"us/div");break;
		case 2:Lcd12864_WriteInt(100, 6, 500, 3);
					 Lcd12864_Writestr(91, 7,"us/div");break;
		case 3:Lcd12864_WriteInt(100, 6, 1, 2);
					 Lcd12864_Writestr(91, 7,"ms/div");break;
		case 4:Lcd12864_WriteInt(100, 6, 2, 2);
					 Lcd12864_Writestr(91, 7,"ms/div");break;
		case 5:Lcd12864_WriteInt(100, 6, 5, 2);
					 Lcd12864_Writestr(91, 7,"ms/div");break;
		default: break;
	}
}

/*!
*  @描述       扫描按键设置垂直方向大小
*  @参数       无
*  示例:       amp_scan()；
*/
void amp_scan(void)
{
	if(Button_wait2 == 0)
	{
			TLC0820_DATA.enlarge_choose++;		
			if(TLC0820_DATA.enlarge_choose>5)TLC0820_DATA.enlarge_choose=5;		
	}
	if(Button_wait1 == 0)
	{	
			TLC0820_DATA.enlarge_choose--;		
			if(TLC0820_DATA.enlarge_choose<0)TLC0820_DATA.enlarge_choose=0;	
	}
	TLC0820_DATA.enlarge=EnlargeData[TLC0820_DATA.enlarge_choose];
	
	switch(TLC0820_DATA.enlarge_choose)
	{
		case 0:Lcd12864_Writeflo(95,2,1.0,1);
					 Lcd12864_Writestr(95,3,"v/div");break;
		case 1:Lcd12864_WriteInt(95,2,500,3);
					 Lcd12864_Writestr(91,3,"mv/div");break;
		case 2:Lcd12864_WriteInt(95,2,200,3);
					 Lcd12864_Writestr(91,3,"mv/div");break;
		case 3:Lcd12864_WriteInt(95,2,100,3);
					 Lcd12864_Writestr(91,3,"mv/div");break;
		case 4:Lcd12864_WriteInt(95,2,50,3);
					 Lcd12864_Writestr(91,3,"mv/div");break;
		case 5:Lcd12864_WriteInt(95,2,20,3);
					 Lcd12864_Writestr(91,3,"mv/div");break;
	}
}


/************************************************************
* 函 数 名         : Delay10ms
* 函数功能		     : 延时函数，延时10ms
************************************************************/
void Delay10ms(unsigned int c)   //误差 0us
{
    unsigned char a,b;
    for(;c>0;c--)
        for(b=38;b>0;b--)
            for(a=130;a>0;a--);
}