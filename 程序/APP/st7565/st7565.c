#include"st7565.h"

/*******************************************************************************
* 函 数 名         : LCD12864_WriteCmd
* 函数功能		     : 写入一个命令到12864
* 输    入         : cmd
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

	LCD12864_WR = 1;	   //command writing 这句不能少，少了就不显示了，至于原因还在查资料
	LCD12864_CS = 0;	   //chip select
}

/*******************************************************************************
* 函 数 名         : LcdSt7565_WriteData
* 函数功能		     : 写入一个数据到12864
* 输    入         : dat
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

	DATA_PORT = dat;       //put data，放置数据
	_nop_();
	_nop_();

	LCD12864_WR = 1;	   //data writing，写数据 
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
	datal=DATA_PORT;       //读数据
	LCD12864_RD = 1;	      //disable read
	_nop_();
	LCD12864_CS = 1 ;       //chip select
	return datal;
}



/****************************************************************
* 函 数 名    : LCD12864_Init
* 函数功能		: 初始化12864
* 修改        ：相对原来的代码我们对初始化部分进行了较大的精简
*               指令2、8、9、10、11、20 复位后保持默认

*****************************************************************/

void Lcd12864_Init()
{
	uchar i;
	LCD12864_RESET = 0;     //低电平复位  硬件复位
	for (i=0; i<100; i++);
	//LCD12864_CS = 0;      //修改，认为原代码改行指令多余
	LCD12864_RESET = 1;     //复位置高

	//--指令14 -- 软件初始化 --//
	LcdSt7565_WriteCmd(0xE2);  //reset 
	for (i=0; i<100; i++);	   //延时一下
	
	//-- 指令1 -- 开启显示 --//
	LcdSt7565_WriteCmd(0xAF); 
	for (i=0; i<100; i++);

	//-- 指令8 显示列地址(左右)增减--//
	//LcdSt7565_WriteCmd(0xA0);  //正常：0xA0  反转：0xA1
	//该指令用0xA0，也就是从列地址从左到右，我们使用0~127列
	//因此设置列地址初始值时使用0x10，0x00
	
  //-- 指令15 -- 行(上下)扫描方向选择--// 
	LcdSt7565_WriteCmd(0xC8);  //正常：0xC0  反转：0xC8
	//我们认为液晶是硬件上下反转的，因此指令15对上下扫描进行取反
	//或者该指令保持复位后的默认，取字模时对字模进行上下取反
	 
	//-- 指令16-- 电源设置 --//
	LcdSt7565_WriteCmd(0x2F); 
	for (i=0; i<100; i++);
	
	//-- 指令17 -- 粗调对比度 --//0x20~0x27
	LcdSt7565_WriteCmd(0x25);  
	
	//-- 指令18 -- 微调对比度 --//
	LcdSt7565_WriteCmd(0x81); 
	LcdSt7565_WriteCmd(0x25);   //0x00~0x3F

}

/*******************************************************************************
* 函 数 名         : LCD12864_ClearScreen
* 函数功能		     : 清屏12864
*******************************************************************************/

void Lcd12864_ClearScreen(void)
{
	uchar i, j;

	for(i=0; i<8; i++)
	{
		//--表格第3个命令，设置X的坐标，也就是页地址，总共8页--//
		//所以一般我们使用的也就是从0xB0到0xB7,就够了--//	
		 LcdSt7565_WriteCmd(0xB0+i); //1011 0000 ---- 1011 0111

		//--表格第4个命令，设置Y坐标--//
		//--当你的段初始化为0xA1时，Y坐标从0x10,0x04到0x18,0x03,一共128位--//
		//--当你的段初始化为0xA0时，Y坐标从0x10,0x00到0x17,0x0F,一共128位--//
		//--在写入数据之后Y坐标的坐标是会自动加1的，我们初始化使用0xA0所以--//
		//--我们的X坐标从0x10,0x00开始---//
		LcdSt7565_WriteCmd(0x10);   //0001 0000
		LcdSt7565_WriteCmd(0x00);		//0000 0100			 		   
		
		//--X轴有128位，就一共刷128次，Y坐标会自动加1，所以我们不用再设置坐标--//
		for(j=0; j<128; j++)
		{
			LcdSt7565_WriteData(0x00);  //如果设置背景为白色时，清屏选择0XFF
		}
	}
}

/*******************************************************************************
* 函 数 名       : LCD12864_Write16CnCHAR
* 函数功能		   : 在12864上面书写16X16的汉字
* 输    入       : Page, Column, cn
*******************************************************************************/

#ifdef  CHAR_CODE

#include"charcode.h"

uchar Lcd12864_Write16CnCHAR(uchar Page, uchar Column, uchar *cn)
{
	uchar j, x1, x2, wordNum;
	
	if(Page > 7)  return 0;      //页坐标只能从0到7，大于则直接返回
	if(Column > 128) 	return 0;  //列的坐标只能从0到127，大于则直接返回
	
  Page += 0xB0;	             //求取页坐标的值 不能放在while里面

	while ( *cn != '\0')	     //在C语言中字符串结束以‘\0’结尾
	{	
		LcdSt7565_WriteCmd(Page);    //设置页坐标

		x1 = (Column >> 4)& 0x0F;       //先取出高4位  注释1
		x2 = Column & 0x0F;             //取低四位 
		LcdSt7565_WriteCmd(0x10 + x1);  //设置列坐标 高4
		LcdSt7565_WriteCmd(0x00 + x2);	//           低4

		for (wordNum=0; wordNum<50; wordNum++)
		{
		    //--查询要写的字在字库中的位置--//
			if ((CN16CHAR[wordNum].Index[0] == *cn) && (CN16CHAR[wordNum].Index[1] == *(cn+1)))
			{
				for (j=0; j<32; j++) //写一个字
				{		
					if (j == 16) //16X16用到两个页坐标，当大于等于16时，切换页坐标
					{
			   		LcdSt7565_WriteCmd(Page + 1);    //设置页坐标			
						LcdSt7565_WriteCmd(0x10 + x1);   //高4位 设置列坐标
						LcdSt7565_WriteCmd(0x00 + x2);	 //低4位
					}
					LcdSt7565_WriteData(CN16CHAR[wordNum].Msk[j]);
				}
				Column += 16; //下一个字的新的列地址
				break;  //我们理解：一旦字库中找到字符，余下字库不用再找
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
	if(y > 7)  return 0;      //页坐标只能从0到7，大于则直接返回
	if(x > 128) 	return 0;  //列的坐标只能从0到127，大于则直接返回
	
  y += 0xB0;	             //求取页坐标的值 不能放在while里面

	LcdSt7565_WriteCmd(y);    //设置页坐标
	x1 = (x >> 4)& 0x0F;       //先取出高4位  注释1
	x2 = x & 0x0F;             //取低四位 
	LcdSt7565_WriteCmd(0x10 + x1);  //设置列坐标 高4
	LcdSt7565_WriteCmd(0x00 + x2);	
	while(*str != '\0')
	{
		for (wordNum=0; wordNum<50; wordNum++)
		{
		    //--查询要写的字在字库中的位置--//
			if (CHAR6x8[wordNum].Index[0] == *str)
			{
				for (j=0; j<6; j++) //写一个字
				{		
					LcdSt7565_WriteData(CHAR6x8[wordNum].Msk[j]);
				}
				x += 6; //下一个字的新的列地址
				break;  //我们理解：一旦字库中找到字符，余下字库不用再找
			}  
		} 	
		str += 1;
	}
	return 1;
}
//显示整型数据
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

//显示浮点类型数据
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


//画点函数
uchar  Lcd12864_WritePoint(uchar x, uchar y)
{
	uchar Page, x1, x2,i,m;
	if(y > 64)  return 0;      //页坐标只能从0到7，大于则直接返回
	if(x > 128) 	return 0;  //列的坐标只能从0到127，大于则直接返回
	Page=y/8;
  Page += 0xB0;	             //求取页坐标的值 不能放在while里面
	LcdSt7565_WriteCmd(Page);    //设置页坐标
	x1 = (x >> 4)& 0x0F;       //先取出高4位  注释1
	x2 = x & 0x0F;             //取低四位 
	LcdSt7565_WriteCmd(0x10 + x1);  //设置列坐标 高4
	LcdSt7565_WriteCmd(0x00 + x2);	//           低4
	m=LcdSt7565_ReadData();
	m=LcdSt7565_ReadData();
	
	i=(1<<((y)%8));

	Page=y/8;
  Page += 0xB0;	             //求取页坐标的值 不能放在while里面
	LcdSt7565_WriteCmd(Page);    //设置页坐标
	x1 = (x >> 4)& 0x0F;       //先取出高4位  注释1
	x2 = x & 0x0F;             //取低四位 
	LcdSt7565_WriteCmd(0x10 + x1);  //设置列坐标 高4
	LcdSt7565_WriteCmd(0x00 + x2);	//           低4
	
	LcdSt7565_WriteData(m | i);
	return 1;
}

//画线函数
void Lcd12864_WriteLine(uchar x1, uchar y1,uchar x2,uchar y2)
{
	uchar t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
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
//画矩形函数
void Lcd12864_WriteRectangle(uchar x1, uchar y1, uchar x2, uchar y2)
{
	Lcd12864_WriteLine(x1,y1,x2,y1);
	Lcd12864_WriteLine(x1,y1,x1,y2);
	Lcd12864_WriteLine(x1,y2,x2,y2);
	Lcd12864_WriteLine(x2,y1,x2,y2);
}
//画圆函数
void Lcd12864_Writecircle(uchar x0,uchar y0,uchar r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
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
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
}