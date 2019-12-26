#ifndef __ST7565_H
#define __ST7565_H

#include <STC12C5A60S2.H>
#include<intrins.h>

//---�����ֿ�ͷ�ļ�
#define CHAR_CODE

//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//--��ʱʹ�õ�IO��--//
#define DATA_PORT P2      //����
sbit LCD12864_CS    = P3^6;//Ƭѡ���͵�ƽ��Ч
sbit LCD12864_RESET = P3^7;//��λ���͵�ƽ��Ч
sbit LCD12864_RS    = P3^3;//����/ָ��ѡ�񣬸ߵ�ƽΪ���ݣ��͵�ƽΪָ��
sbit LCD12864_WR    = P3^5;//дʹ�ܣ��͵�ƽ��Ч
sbit LCD12864_RD    = P3^4;//��ʹ�ܣ��͵�ƽ��Ч

//--����ȫ�ֺ���--//
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
