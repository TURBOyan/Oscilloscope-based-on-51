#include "osc_scope.h"
#include "TLC0820.h"

osc_data_typedef osc_data;

/*!
*  @����       ��ʾ����ʼ��
*  @����       ��
*  ʾ��:       Osc_Init()��
*/
void Osc_Init(void)
{
	Lcd12864_Init();
	Lcd12864_ClearScreen();
	Delay10ms(1);
}

/*!
*  @����       ������ʾ�����������
*  @����       width  ��ʾ��ȣ�0��127��
*  ʾ��:       Osc_FirstGUI(90)��	//��0��90�Ŀ�ȷ�Χ�ڻ��Ʋ���������
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
*  @����       ������ʾ
*  @����       level��������ƽ��
							 jump�������ش�����1�������أ�0���½��أ�
							 show_weith(��ʾ���)
							 Wave_length������Ĳ������ε���ȣ�
							 *wave�����λ�����ָ�룩
*  ʾ��:      Show_Wave(128,1,90,200,&ADC_BUF);		
			���������Ϊ200�ĵ�ADC_BUF�����ڵĲ������ݣ������ش�����1����������ƽ��128��Ҳ����0V������ʾ��С��90�Ĳ�����ʾ����
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
						//�����ִ�������ʱ�������أ�
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
						//����δ���ִ�������ʱ���ӵ�һ����ʼ��ʾ
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
						//�����ִ�������ʱ���½��أ�,�ӳ��ִ�����������ʼ��ʾ
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
						//����δ���ִ�������ʱ���ӵ�һ����ʼ��ʾ
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
*  @����       ���㲨�η��ֵ��Ƶ��
*  @����       start����Ҫ����Ĳ��εĿ�ʼλ�ã�
							 end(��Ҫ����Ĳ��εĽ���λ��)
*  ʾ��:       Calcu_AmpFreq(0,90,&ADC_BUF)
							������ADC_BUF�ڵ�0��90�Ĳ��ο�ʼ������ֵ��Ƶ��
*/
void Calcu_AmpFreq(uchar start,uchar end,uchar *data_1)
{
	uchar i,j;
	uint xdata freq_buff[200],zero_num=0;
	long time=0;
	
	/************************����Ϊ����ֵ*********************************************************************************************************************************/
	int databuff_max[3]={0,0,0},databuff_min[3]={255,255,255};
	for(i=start;i<end/3;i++)		//һ���������ݷ����ݣ��ֱ������ֵ����Сֵ������ð������ȥ�����ֵ����Сֵ����ȡ�����β������ֵ����Сֵ��Ϊ���Ǽ�С���
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
	TLC0820_DATA.amp=(((databuff_max[1]-databuff_min[1])/255.0)*5.0*2)/TLC0820_DATA.enlarge;//���ݷŴ����������벨�εķ��ֵ����������TLC0820_DATA.amp��
	
	if(TLC0820_DATA.amp<0)TLC0820_DATA.amp=0;		//���ֵ�޷�
	
/************************����Ϊ��Ƶ��*******************************************************************************************************************************/
	for(i=start+1;i<end;i++)			//��ȡ��ѡ���ε������ع����Ĵ�����������ÿ�����ʱ��ʱ��
	{
		if(data_1[i+1]>=((databuff_max[1]+databuff_min[1])/2.0)&& (data_1[i] <= ((databuff_max[1]+databuff_min[1])/2.0)))
		{
			freq_buff[zero_num]=i;
			zero_num++;
		}
	}
	zero_num--;
	switch(TLC0820_DATA.Time_choose)		//��ô˴β��εĲ������ڣ�Ҳ��������������֮���ʱ����
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
	TLC0820_DATA.freq=(long)1000000/(time*1.0);		//�����Ƶ�ʣ���������TLC0820_DATA.freq�ṹ�������

/************************����ΪƵ�ʺͷ��ֵ��ʾ**********************************************************************************************/
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
*  @����       ɨ�谴�����ô�����ƽ
*  @����       ��
*  ʾ��:       Trig_scan()��
*/

void Trig_scan(void)
{
	if(Button_TrigUp == 0)//����ϼ����£�������ƽ����
	{
			TLC0820_DATA.Trig_Vol++;		
			if(TLC0820_DATA.Trig_Vol>254)TLC0820_DATA.Trig_Vol=254;		
	}
	if(Button_TrigDown == 0)	//����¼����£�������ƽ����
	{	
			TLC0820_DATA.Trig_Vol--;		
			if(TLC0820_DATA.Trig_Vol<=1)TLC0820_DATA.Trig_Vol=1;	
	}
	//��ʾָʾ������ƽ��С��ͷ
	Lcd12864_WriteLine(90, (255-TLC0820_DATA.Trig_Vol)>>2,93,(255-TLC0820_DATA.Trig_Vol+12)>>2);
	Lcd12864_WriteLine(90, (255-TLC0820_DATA.Trig_Vol)>>2,93,(255-TLC0820_DATA.Trig_Vol-12)>>2);
	Lcd12864_WriteLine(90, (255-TLC0820_DATA.Trig_Vol)>>2,94,(255-TLC0820_DATA.Trig_Vol)>>2);
}



/*!
*  @����       ɨ�谴������ˮƽ�����С
*  @����       ��
*  ʾ��:       Time_scan()��
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
*  @����       ɨ�谴�����ô�ֱ�����С
*  @����       ��
*  ʾ��:       amp_scan()��
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
* �� �� ��         : Delay10ms
* ��������		     : ��ʱ��������ʱ10ms
************************************************************/
void Delay10ms(unsigned int c)   //��� 0us
{
    unsigned char a,b;
    for(;c>0;c--)
        for(b=38;b>0;b--)
            for(a=130;a>0;a--);
}