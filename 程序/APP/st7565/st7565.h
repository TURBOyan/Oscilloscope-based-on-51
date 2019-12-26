#ifndef __ST7565_H
#define __ST7565_H

#include <STC12C5A60S2.H>
#include<intrins.h>

//---包含字库头文件
#define CHAR_CODE

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//--定时使用的IO口--//
#define DATA_PORT P2      //数据
sbit LCD12864_CS    = P3^6;//片选，低电平有效
sbit LCD12864_RESET = P3^7;//复位，低电平有效
sbit LCD12864_RS    = P3^3;//数据/指令选择，高电平为数据，低电平为指令
sbit LCD12864_WR    = P3^5;//写使能，低电平有效
sbit LCD12864_RD    = P3^4;//读使能，低电平有效

//--定义全局函数--//
void checkbusy();
void LcdSt7565_WriteCmd(cmd);
void LcdSt7565_WriteData(dat);
uchar LcdSt7565_ReadData(void);
void Lcd12864_Init();
void Lcd12864_ClearScreen(void);
uchar Lcd12864_Write16CnCHAR(uchar Page, uchar Column, uchar *cn);
uchar Lcd12864_Writestr(uchar x, uchar y,uchar *str);
void Lcd12864_WriteInt(uchar x, uchar y, int data1, int set);
void  Lcd12864_Writeflo(uchar x, uchar y,float data1,int set);
uchar Lcd12864_WritePoint(uchar x, uchar y);
void Lcd12864_WriteLine(uchar x1, uchar y1,uchar x2,uchar y2);
void Lcd12864_WriteRectangle(uchar x1, uchar y1, uchar x2, uchar y2);
void Lcd12864_Writecircle(uchar x0,uchar y0,uchar r);

#endif
