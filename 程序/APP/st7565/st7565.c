#include"st7565.h"

/*******************************************************************************
* �� �� ��         : LCD12864_WriteCmd
* ��������		     : д��һ�����12864
* ��    ��         : cmd
*******************************************************************************/

void checkbusy(void)
{
	LCD12864_CS = 0;	   //chip select
	_nop_();
	LCD12864_RS = 0;     //select command
	LCD12864_WR = 1;     //select write
	DATA_PORT=0xff;
	_nop_();
	LCD12864_RD = 0;	   //disable read
	_nop_();
	while(DATA_PORT & 0x80);
	_nop_();
	LCD12864_RD = 1;	   //disable read
	_nop_();
	LCD12864_CS = 1;	   //chip select
}


void LcdSt7565_WriteCmd(cmd)
{
	checkbusy();
	LCD12864_CS = 0;	   //chip select
	LCD12864_RD = 1;	   //disable read
	LCD12864_RS = 0;     //select command
	LCD12864_WR = 0;     //select write
	_nop_();
	_nop_();

	DATA_PORT = cmd;     //put command
	_nop_();
	_nop_();

	LCD12864_WR = 1;	   //command writing ��䲻���٣����˾Ͳ���ʾ�ˣ�����ԭ���ڲ�����
	LCD12864_CS = 0;	   //chip select
}

/*******************************************************************************
* �� �� ��         : LcdSt7565_WriteData
* ��������		     : д��һ�����ݵ�12864
* ��    ��         : dat
*******************************************************************************/

void LcdSt7565_WriteData(dat)
{	
	checkbusy();
	LCD12864_CS = 0 ;       //chip select
	LCD12864_RD = 1;	      //disable read
	LCD12864_RS = 1;        //select data
	LCD12864_WR = 0;        //select write
	_nop_();
	_nop_();

	DATA_PORT = dat;       //put data����������
	_nop_();
	_nop_();

	LCD12864_WR = 1;	   //data writing��д���� 
	LCD12864_CS = 0;	   //chip select
}

uchar LcdSt7565_ReadData(void)
{	
	uchar datal=0;
	checkbusy();
	LCD12864_CS = 0 ;       //chip select
	_nop_();
	LCD12864_WR = 1;        //select write
	LCD12864_RS = 1;        //select data
	_nop_();
	LCD12864_RD = 0;	      //disable read
	_nop_();
	datal=DATA_PORT;       //������
	LCD12864_RD = 1;	      //disable read
	_nop_();
	LCD12864_CS = 1 ;       //chip select
	return datal;
}



/****************************************************************
* �� �� ��    : LCD12864_Init
* ��������		: ��ʼ��12864
* �޸�        �����ԭ���Ĵ������ǶԳ�ʼ�����ֽ����˽ϴ�ľ���
*               ָ��2��8��9��10��11��20 ��λ�󱣳�Ĭ��

*****************************************************************/

void Lcd12864_Init()
{
	uchar i;
	LCD12864_RESET = 0;     //�͵�ƽ��λ  Ӳ����λ
	for (i=0; i<100; i++);
	//LCD12864_CS = 0;      //�޸ģ���Ϊԭ�������ָ�����
	LCD12864_RESET = 1;     //��λ�ø�

	//--ָ��14 -- �����ʼ�� --//
	LcdSt7565_WriteCmd(0xE2);  //reset 
	for (i=0; i<100; i++);	   //��ʱһ��
	
	//-- ָ��1 -- ������ʾ --//
	LcdSt7565_WriteCmd(0xAF); 
	for (i=0; i<100; i++);

	//-- ָ��8 ��ʾ�е�ַ(����)����--//
	//LcdSt7565_WriteCmd(0xA0);  //������0xA0  ��ת��0xA1
	//��ָ����0xA0��Ҳ���Ǵ��е�ַ�����ң�����ʹ��0~127��
	//��������е�ַ��ʼֵʱʹ��0x10��0x00
	
  //-- ָ��15 -- ��(����)ɨ�跽��ѡ��--// 
	LcdSt7565_WriteCmd(0xC8);  //������0xC0  ��ת��0xC8
	//������ΪҺ����Ӳ�����·�ת�ģ����ָ��15������ɨ�����ȡ��
	//���߸�ָ��ָ�λ���Ĭ�ϣ�ȡ��ģʱ����ģ��������ȡ��
	 
	//-- ָ��16-- ��Դ���� --//
	LcdSt7565_WriteCmd(0x2F); 
	for (i=0; i<100; i++);
	
	//-- ָ��17 -- �ֵ��Աȶ� --//0x20~0x27
	LcdSt7565_WriteCmd(0x25);  
	
	//-- ָ��18 -- ΢���Աȶ� --//
	LcdSt7565_WriteCmd(0x81); 
	LcdSt7565_WriteCmd(0x25);   //0x00~0x3F

}

/*******************************************************************************
* �� �� ��         : LCD12864_ClearScreen
* ��������		     : ����12864
*******************************************************************************/

void Lcd12864_ClearScreen(void)
{
	uchar i, j;

	for(i=0; i<8; i++)
	{
		//--����3���������X�����꣬Ҳ����ҳ��ַ���ܹ�8ҳ--//
		//����һ������ʹ�õ�Ҳ���Ǵ�0xB0��0xB7,�͹���--//	
		 LcdSt7565_WriteCmd(0xB0+i); //1011 0000 ---- 1011 0111

		//--����4���������Y����--//
		//--����Ķγ�ʼ��Ϊ0xA1ʱ��Y�����0x10,0x04��0x18,0x03,һ��128λ--//
		//--����Ķγ�ʼ��Ϊ0xA0ʱ��Y�����0x10,0x00��0x17,0x0F,һ��128λ--//
		//--��д������֮��Y����������ǻ��Զ���1�ģ����ǳ�ʼ��ʹ��0xA0����--//
		//--���ǵ�X�����0x10,0x00��ʼ---//
		LcdSt7565_WriteCmd(0x10);   //0001 0000
		LcdSt7565_WriteCmd(0x00);		//0000 0100			 		   
		
		//--X����128λ����һ��ˢ128�Σ�Y������Զ���1���������ǲ�������������--//
		for(j=0; j<128; j++)
		{
			LcdSt7565_WriteData(0x00);  //������ñ���Ϊ��ɫʱ������ѡ��0XFF
		}
	}
}

/*******************************************************************************
* �� �� ��       : LCD12864_Write16CnCHAR
* ��������		   : ��12864������д16X16�ĺ���
* ��    ��       : Page, Column, cn
*******************************************************************************/

#ifdef  CHAR_CODE

#include"charcode.h"

uchar Lcd12864_Write16CnCHAR(uchar Page, uchar Column, uchar *cn)
{
	uchar j, x1, x2, wordNum;
	
	if(Page > 7)  return 0;      //ҳ����ֻ�ܴ�0��7��������ֱ�ӷ���
	if(Column > 128) 	return 0;  //�е�����ֻ�ܴ�0��127��������ֱ�ӷ���
	
  Page += 0xB0;	             //��ȡҳ�����ֵ ���ܷ���while����

	while ( *cn != '\0')	     //��C�������ַ��������ԡ�\0����β
	{	
		LcdSt7565_WriteCmd(Page);    //����ҳ����

		x1 = (Column >> 4)& 0x0F;       //��ȡ����4λ  ע��1
		x2 = Column & 0x0F;             //ȡ����λ 
		LcdSt7565_WriteCmd(0x10 + x1);  //���������� ��4
		LcdSt7565_WriteCmd(0x00 + x2);	//           ��4

		for (wordNum=0; wordNum<50; wordNum++)
		{
		    //--��ѯҪд�������ֿ��е�λ��--//
			if ((CN16CHAR[wordNum].Index[0] == *cn) && (CN16CHAR[wordNum].Index[1] == *(cn+1)))
			{
				for (j=0; j<32; j++) //дһ����
				{		
					if (j == 16) //16X16�õ�����ҳ���꣬�����ڵ���16ʱ���л�ҳ����
					{
			   		LcdSt7565_WriteCmd(Page + 1);    //����ҳ����			
						LcdSt7565_WriteCmd(0x10 + x1);   //��4λ ����������
						LcdSt7565_WriteCmd(0x00 + x2);	 //��4λ
					}
					LcdSt7565_WriteData(CN16CHAR[wordNum].Msk[j]);
				}
				Column += 16; //��һ���ֵ��µ��е�ַ
				break;  //������⣺һ���ֿ����ҵ��ַ��������ֿⲻ������
			}  
		} 	
		cn += 2;
	}	
	return 1;
}

#endif

uchar  Lcd12864_Writestr(uchar x, uchar y,uchar *str)
{
	uchar j,x1, x2,wordNum;
	if(y > 7)  return 0;      //ҳ����ֻ�ܴ�0��7��������ֱ�ӷ���
	if(x > 128) 	return 0;  //�е�����ֻ�ܴ�0��127��������ֱ�ӷ���
	
  y += 0xB0;	             //��ȡҳ�����ֵ ���ܷ���while����

	LcdSt7565_WriteCmd(y);    //����ҳ����
	x1 = (x >> 4)& 0x0F;       //��ȡ����4λ  ע��1
	x2 = x & 0x0F;             //ȡ����λ 
	LcdSt7565_WriteCmd(0x10 + x1);  //���������� ��4
	LcdSt7565_WriteCmd(0x00 + x2);	
	while(*str != '\0')
	{
		for (wordNum=0; wordNum<50; wordNum++)
		{
		    //--��ѯҪд�������ֿ��е�λ��--//
			if (CHAR6x8[wordNum].Index[0] == *str)
			{
				for (j=0; j<6; j++) //дһ����
				{		
					LcdSt7565_WriteData(CHAR6x8[wordNum].Msk[j]);
				}
				x += 6; //��һ���ֵ��µ��е�ַ
				break;  //������⣺һ���ֿ����ҵ��ַ��������ֿⲻ������
			}  
		} 	
		str += 1;
	}
	return 1;
}
//��ʾ��������
void Lcd12864_WriteInt(uchar x, uchar y, int data1, int set)
{
  uchar result[7];
  if(data1<0)
  {
    data1 *= -1;
    result[0] = '-';
  }
  else
  {
    result[0] = '+';
  }
  result[1] = (uchar)((data1%100000)/10000+48);
  result[2] = (uchar)((data1%10000)/1000+48);
  result[3] = (uchar)((data1%1000)/100+48);
  result[4] = (uchar)((data1%100)/10+48);
  result[5] = (uchar)(data1%10+48);
  result[6] = '\0';
  if(set < 0)
  {
    result[5 + set] = result[0];
    Lcd12864_Writestr(x, y, (char *)&(result[5 + set]));
  }
  else if(set > 0)
  {
    Lcd12864_Writestr(x, y, (char *)&(result[6 - set]));
  }
}

//��ʾ������������
void  Lcd12864_Writeflo(uchar x, uchar y,float data1,int set)
{
  uchar result[9];
  uint data2;
  if(data1<0)
  {			
    result[0] = '-';
    data1 *= -1;
  }
  else
  {
    result[0]='+';
  }
  data1 += 0.000001;
  data2 = (uint)(data1*100);
  result[1] = (uchar)((data2%1000000)/100000+48);
  result[2] = (uchar)((data2%100000)/10000+48);
  result[3] = (uchar)((data2%10000)/1000+48);
  result[4] = (uchar)((data2%1000)/100+48);
  result[5] = '.';
  result[6] = (uchar)((data2%100)/10+48);
  result[7] = (uchar)(data2%10+48);
  result[8] = '\0'; 
  if(set < 0)
  {
    result[4 + set] = result[0];
    Lcd12864_Writestr(x, y, (char *)&(result[4 + set]));
  }
  else if(set > 0)
  {
    Lcd12864_Writestr(x, y, (char *)&(result[5 - set]));
  }
}


//���㺯��
uchar  Lcd12864_WritePoint(uchar x, uchar y)
{
	uchar Page, x1, x2,i,m;
	if(y > 64)  return 0;      //ҳ����ֻ�ܴ�0��7��������ֱ�ӷ���
	if(x > 128) 	return 0;  //�е�����ֻ�ܴ�0��127��������ֱ�ӷ���
	Page=y/8;
  Page += 0xB0;	             //��ȡҳ�����ֵ ���ܷ���while����
	LcdSt7565_WriteCmd(Page);    //����ҳ����
	x1 = (x >> 4)& 0x0F;       //��ȡ����4λ  ע��1
	x2 = x & 0x0F;             //ȡ����λ 
	LcdSt7565_WriteCmd(0x10 + x1);  //���������� ��4
	LcdSt7565_WriteCmd(0x00 + x2);	//           ��4
	m=LcdSt7565_ReadData();
	m=LcdSt7565_ReadData();
	
	i=(1<<((y)%8));

	Page=y/8;
  Page += 0xB0;	             //��ȡҳ�����ֵ ���ܷ���while����
	LcdSt7565_WriteCmd(Page);    //����ҳ����
	x1 = (x >> 4)& 0x0F;       //��ȡ����4λ  ע��1
	x2 = x & 0x0F;             //ȡ����λ 
	LcdSt7565_WriteCmd(0x10 + x1);  //���������� ��4
	LcdSt7565_WriteCmd(0x00 + x2);	//           ��4
	
	LcdSt7565_WriteData(m | i);
	return 1;
}

//���ߺ���
void Lcd12864_WriteLine(uchar x1, uchar y1,uchar x2,uchar y2)
{
	uchar t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1;
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		Lcd12864_WritePoint(uRow, uCol);
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	} 
}
//�����κ���
void Lcd12864_WriteRectangle(uchar x1, uchar y1, uchar x2, uchar y2)
{
	Lcd12864_WriteLine(x1,y1,x2,y1);
	Lcd12864_WriteLine(x1,y1,x1,y2);
	Lcd12864_WriteLine(x1,y2,x2,y2);
	Lcd12864_WriteLine(x2,y1,x2,y2);
}
//��Բ����
void Lcd12864_Writecircle(uchar x0,uchar y0,uchar r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		Lcd12864_WritePoint(x0+a,y0-b);             //5
 		Lcd12864_WritePoint(x0+b,y0-a);             //0           
		Lcd12864_WritePoint(x0+b,y0+a);             //4               
		Lcd12864_WritePoint(x0+a,y0+b);             //6 
		Lcd12864_WritePoint(x0-a,y0+b);             //1       
 		Lcd12864_WritePoint(x0-b,y0+a);             
		Lcd12864_WritePoint(x0-a,y0-b);             //2             
  	Lcd12864_WritePoint(x0-b,y0-a);             //7     	         
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
}