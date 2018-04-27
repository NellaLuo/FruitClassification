// ColorCoOccur.cpp: implementation of the CColorCoOccur class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FruitClassification.h"
#include "ColorCoOccur.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CColorCoOccur::CColorCoOccur()
{

}

CColorCoOccur::~CColorCoOccur()
{

}

void CColorCoOccur::RGBtoColorType(HDC dc, CString m_strPhotoFilePath)
{
	//将图象各像素点的RGB转化为HSV
	if (!IsValid())
		return;
	ASSERT(m_pBitmapInfo);
	ASSERT(m_pBitmapInfo->bmiHeader.biSize == sizeof(BITMAPINFOHEADER));
	// 复制源图
	HBITMAP m_hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_strPhotoFilePath,
		IMAGE_BITMAP, 0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION); 
	
	HDC compDC=CreateCompatibleDC(dc);
    SelectObject(compDC,m_hBitmap);
	
	COLORREF clr; //定义一个COLORREF结构，因为提取的象素点的颜色是以RGB形式表示的
	int R,G,B;
	double H,S,V;
	for(int i=0;i<384;i++)
	{
		for(int j=0;j<256;j++)
		{
			clr=GetPixel(compDC,i,j);
			R=GetRValue(clr);
			G=GetGValue(clr);
			B=GetBValue(clr);
			int rgbMax=Max3(R,G,B);
			int rgbMin=Min3(R,G,B);
			V=(double)100*rgbMax/255;    //V
			S=(double)100*(rgbMax-rgbMin)/rgbMax;  //S
			//H
			if(rgbMax == R)
			{
				H=(double)(G-B)*60/(rgbMax-rgbMin);  
			}else if(rgbMax == G)
			{
				H=(double)120+(B-R)*60/(rgbMax-rgbMin);
			}else{
				H=(double)240+(R-G)*60/(rgbMax-rgbMin);
			}
			if(H < 0)
			{
				H+=360;
			}
			colortype[i][j]=DefineColorType(H,S,V);
		}
	}
}

int CColorCoOccur::Max3(int a, int b, int c)
{
	//返回3个数中的最大值
	int max=0;
	if(a>b)
	{
		max=a;
	}
	else
	{
		max=b;
	}
	if(c>max)
	{
		max=c;
	}
	return max;
}

int CColorCoOccur::Min3(int a, int b, int c)
{
	//返回3个数中的最小值
	int min=0;
	if(a<b)
	{
		min=a;
	}
	else
	{
		min=b;
	}
	if(c<min)
	{
		min=c;
	}
	return min;
}

int CColorCoOccur::DefineColorType(double H, double S, double V)
{
	if(V<=25)
	{
		return 15;   //黑
	}else if(S<=20){
		if(V>=75){
			return 12;    //白
		}else if(V>=50){
			return 13;    //亮灰
		}else{
			return 14;    //暗灰
		}
	}else{
		int SR=(100-S)/20;   //饱和度补偿
		int VR=(100-V)/20;   //亮度补偿
		if(H>=330+SR-VR | H<=20-2*SR+2*VR)
		{
			return 0;     //红
		}else if(H<=35){
			return 1;     //橙
		}else if(H<=70){
			return 2;     //黄
		}else if(H<=80+SR-VR){
			return 3;     //黄绿
		}else if(H<=155-SR+VR){
			return 4;     //绿
		}else if(H<=165){
			return 5;     //青绿
		}else if(H<=190){
			return 6;     //青
		}else if(H<=35){
			return 7;     //橙
		}else if(H<=205+SR-2*VR){
			return 8;     //青蓝
		}else if(H<=270-SR+2*VR){
			return 9;     //蓝
		}else if(H<=285){
			return 10;     //蓝紫
		}else if(H<=320){
			return 11;     //紫
		}else{
			return 12;     //紫红
		}
	}
}

void CColorCoOccur::calCoOccurMatrix()
{
	//色彩共生矩阵初始化
	for(int k=0;k<12;k++)
	{
		for(int m=0;m<16;m++)
		{
			for(int n=0;n<16;n++)
			{
				co_occur[k][m][n]=0;
			}
		}
	}

	int x=0,y=0,i=0,j=0;
	//0度,d=1,3,9
	for( x=0;x<256-1;x++)
	{
		for( y=0;y<256;y++)
		{
			int i=colortype[x][y];
			int j=colortype[x+1][y];
			co_occur[0][i][j]++;
		}
	}
	for(i=0;i<16;i++)  //求出概率
	{
		for( j=0;j<16;j++)
		{
			co_occur[0][i][j]/=(255*256);
		}
	}
	for( x=0;x<256-3;x++)
	{
		for( y=0;y<256;y++)
		{
			int i=colortype[x][y];
			int j=colortype[x+3][y];
			co_occur[1][i][j]++;
		}
	}
	for(i=0;i<16;i++)
	{
		for(j=0;j<16;j++)
		{
			co_occur[1][i][j]/=(256*256);
		}
	}
	for(x=0;x<256-9;x++)
	{
		for( y=0;y<256;y++)
		{
			int i=colortype[x][y];
			int j=colortype[x+9][y];
			co_occur[2][i][j]++;
		}
	}
	for( i=0;i<16;i++)
	{
		for(j=0;j<16;j++)
		{
			co_occur[2][i][j]/=(247*256);
		}
	}
	//45度,d=1,3,9
	for( x=0;x<256-1;x++)
	{
		for(y=0;y<256-1;y++)
		{
			int i=colortype[x][y];
			int j=colortype[x+1][y+1];
			co_occur[3][i][j]++;
		}
	}
	for(i=0;i<16;i++)
	{
		for(j=0;j<16;j++)
		{
			co_occur[3][i][j]/=(255*255);
		}
	}
	for( x=0;x<256-3;x++)
	{
		for( y=0;y<256-3;y++)
		{
			int i=colortype[x][y];
			int j=colortype[x+3][y+3];
			co_occur[4][i][j]++;
		}
	}
	for( i=0;i<16;i++)
	{
		for(j=0;j<16;j++)
		{
			co_occur[4][i][j]/=(253*253);
		}
	}
	for(x=0;x<256-9;x++)
	{
		for( y=0;y<256-9;y++)
		{
			int i=colortype[x][y];
			int j=colortype[x+9][y+9];
			co_occur[5][i][j]++;
		}
	}
	for(i=0;i<16;i++)
	{
		for(j=0;j<16;j++)
		{
			co_occur[5][i][j]/=(247*247);
		}
	}
	//90度,d=1,3,9
	for( x=0;x<256;x++)
	{
		for( y=0;y<256-1;y++)
		{
			int i=colortype[x][y];
			int j=colortype[x][y+1];
			co_occur[6][i][j]++;
		}
	}
	for( i=0;i<16;i++)
	{
		for( j=0;j<16;j++)
		{
			co_occur[6][i][j]/=(256*255);
		}
	}
	for( x=0;x<256;x++)
	{
		for( y=0;y<256-3;y++)
		{
			int i=colortype[x][y];
			int j=colortype[x][y+3];
			co_occur[7][i][j]++;
		}
	}
	for( i=0;i<16;i++)
	{
		for( j=0;j<16;j++)
		{
			co_occur[7][i][j]/=(256*253);
		}
	}
	for( x=0;x<256;x++)
	{
		for( y=0;y<256-9;y++)
		{
			int i=colortype[x][y];
			int j=colortype[x][y+9];
			co_occur[8][i][j]++;
		}
	}
	for( i=0;i<16;i++)
	{
		for( j=0;j<16;j++)
		{
			co_occur[8][i][j]/=(256*247);
		}
	}
	//135度,d=1,3,9
	for( x=0+1;x<256;x++)
	{
		for( y=0;y<256-1;y++)
		{
			int i=colortype[x][y];
			int j=colortype[x-1][y+1];
			co_occur[9][i][j]++;
		}
	}
	for( i=0;i<16;i++)
	{
		for( j=0;j<16;j++)
		{
			co_occur[9][i][j]/=(255*255);
		}
	}
	for( x=0+3;x<256;x++)
	{
		for( y=0;y<256-3;y++)
		{
			int i=colortype[x][y];
			int j=colortype[x-3][y+3];
			co_occur[10][i][j]++;
		}
	}
	for( i=0;i<16;i++)
	{
		for( j=0;j<16;j++)
		{
			co_occur[10][i][j]/=(253*253);
		}
	}
	for( x=0+9;x<256;x++)
	{
		for( y=0;y<256-9;y++)
		{
			int i=colortype[x][y];
			int j=colortype[x-9][y+9];
			co_occur[11][i][j]++;
		}
	}
	for( i=0;i<16;i++)
	{
		for( j=0;j<16;j++)
		{
			co_occur[11][i][j]/=(247*247);
		}
	}
}

void CColorCoOccur::calMatrixFeature()
{
	//初始化
	for(int m=0;m<4;m++)
	{
		for(int n=0;n<12;n++)
		{
			F[m][n]=0;
		}
	}
	
	int z=0,i=0,j=0;
	//计算能量
	for( z=0;z<12;z++)
	{
		for( i=0;i<16;i++)
		{
			for( j=0;j<16;j++)
			{
				F[0][z]+=co_occur[z][i][j]*co_occur[z][i][j];
			}
		}
	}
	//计算惯性矩
	for( z=0;z<12;z++)
	{
		for( i=0;i<16;i++)
		{
			for( j=0;j<16;j++)
			{
				F[1][z]+=co_occur[z][i][j]*calParallax(i,j)*calParallax(i,j);
			}
		}
	}
	//计算逆差矩
	for( z=0;z<12;z++)
	{
		for( i=0;i<16;i++)
		{
			for( j=0;j<16;j++)
			{
				F[2][z]+=co_occur[z][i][j]/( 1+calParallax(i,j)*calParallax(i,j) );
			}
		}
	}
	//计算熵
	for( z=0;z<12;z++)
	{
		for( i=0;i<16;i++)
		{
			for( j=0;j<16;j++)
			{
				if(co_occur[z][i][j] != 0)
				{
					F[3][z]+=-co_occur[z][i][j]*( log10(co_occur[z][i][j])/log10(2.0) );
				}
			}
		}
	}
	
	for( z=0;z<4;z++)
	{
		double ave=0;   //均值
		double V=0;     //方差
		for(int m=0;m<12;m++)
		{
			ave+=F[z][m]/12;
		}
		for(int m1=0;m1<12;m1++)
		{
			V+=(F[z][m1]-ave)*(F[z][m1]-ave);
		}
		feature[z][0]=ave;
		feature[z][1]=sqrt(V);
	}
}

int CColorCoOccur::calParallax(int i, int j)
{
	//计算视差
	if( (i == 0 && j== 12)||(i == 12 && j== 0)||(i == 11 && j== 12)||(i == 12 && j== 11) )
	{//红与紫红 紫与紫红 0.707
		return 0.707;
	}
	if( (i == 0 && j== 11)||(i == 11 && j== 0) )
	{//红与紫 1.414
		return 1.414;
	}
	if( (i>=12&&i<=15) && (j>=12&&j<=15) )
	{//i j都是灰度类
		if(abs(i-j) == 1)
		{//i j相邻
			return 1;
		}else{
			//i j不相邻
			return 2;
		}
	}else if(i<12 && j<12)
	{//i j都是颜色类
		if(abs(i-j) == 1)
		{//i j相邻
			return 1;
		}else{
			//i j不相邻
			return 2;
		}
	}else{
		//i j 一个是灰度类 一个是颜色类
		return 2;
	}
}
