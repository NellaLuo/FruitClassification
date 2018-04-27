// WvltTrans.cpp: implementation of the CWvltTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FruitClassification.h"
#include "WvltTrans.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CWvltTrans::CWvltTrans()
{

}

CWvltTrans::~CWvltTrans()
{

}

/********************************************************************************
*函数描述：	DWT_Once完成一次图像的小波变换										*
*函数参数：	short **spOriginData：二维指针，指向原始的图像数据					*
*			short **spTransData0：小波变换系数，存放一次水平变换后的小波系数	*
*			short **spTransData1：小波变换系数，存放一次数值变换后的小波系数	*
*			int   nHeight		：图像属性参数，数值为原始图像的高度值			*
*			int	  nHeight_H		：图像属性参数，数值为原始图像高度值的一半		*
*			int   nWidth		：图像属性参数，数值为原始图像的宽度值			*
*			int	  nWidth_H		：图像属性参数，数值为原始图像宽度值的一半		*
*			int   layer			：小波变换的层数，数值为1层						*
*			float fRadius		：小波变换因子，在调用时候已指定数值为1.414		*
********************************************************************************/


/********************************************************************************
*函数描述：	DWT_TriLayers完成三次图像的小波变换									*	
*函数参数：	short **spOriginData：二维指针，指向原始的图像数据					*
*			short **spTransData0：小波变换系数，存放一次水平变换后的小波系数	*
*			short **spTransData1：小波变换系数，存放一次数值变换后的小波系数	*
*			int   nHeight		：图像属性参数，数值为原始图像的高度值			*
*			int	  nHeight_H		：图像属性参数，数值为原始图像高度值的一半		*
*			int   nWidth		：图像属性参数，数值为原始图像的宽度值			*
*			int	  nWidth_H		：图像属性参数，数值为原始图像宽度值的一半		*
*			int   layer			：小波变换的层数,数值为3层						*
*			float fRadius		：小波变换因子，在调用时候已指定数值为1.414		*
********************************************************************************/

void CWvltTrans::DWT_TriLayers(short** spOriginData, short** spTransData0, short** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int i;
	short **pData, **pTran0, **pTran1;
	//图像的属性参数
	int iWidth, iHeight, iWidth_H, iHeight_H;
	float fr = fRadius;
	//获得数据空间的指针
	pData = spOriginData;
	pTran0 = spTransData0;
	pTran1 = spTransData1;
	//图像属性参数赋值
	iWidth = nWidth;	iWidth_H = nWidth_H;
	iHeight = nHeight;	iHeight_H = nHeight_H;
	//利用循环完成两次小波变换
	for(i=1; i<=layer; i++)
	{
		DWT_Once(pData,pTran0,pTran1,iHeight,iHeight_H,iWidth,iWidth_H,i,fr);
		iHeight=iHeight>>1;		iWidth=iWidth>>1;
		iHeight_H=iHeight/2;	iWidth_H=iWidth/2;
	}
}

void CWvltTrans::DWT_Once(short **spOriginData, short **spTransData0, short **spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int Trans_W,				//图像扫描线控制：横坐标
		Trans_H,				//图像扫描线控制：纵坐标
		Trans_M,				//图像矩阵的横坐标
		Trans_N;				//图像矩阵的纵坐标
	short Trans_Coeff0;			//小波变换系数
    signed short Trans_Coeff1;
	fRadius=1.414;				//变换滤波系数
	//本模块完成变换系数的赋值采样
	//行变换,第一次（layer=1时）时nHeight即为原始图像的高度值
    for(Trans_H=0; Trans_H<nHeight; Trans_H++)            
	{
		if(layer == 1)
			 //layer=1时，nWidth_H为原始图像宽度值的一半
			for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)          
			{
				Trans_W=Trans_N<<1;
	            if (fRadius==2)
				{
					spTransData0[Trans_H][Trans_N] = (spOriginData[Trans_H][Trans_W]);
                    spTransData0[Trans_H][nWidth_H+Trans_N] = (spOriginData[Trans_H][Trans_W+1]);
				}
	            else
				{
                    spTransData0[Trans_H][Trans_N] = (spOriginData[Trans_H][Trans_W]-128);		
                    spTransData0[Trans_H][nWidth_H+Trans_N] = (spOriginData[Trans_H][Trans_W+1]-128);	
				}
	   		}
		//若变换层数大于1,则仅采样低频的小波系数
		if(layer > 1)
		for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
		{
			Trans_W=Trans_N<<1;
			spTransData0[Trans_H][Trans_N] = spTransData1[Trans_H][Trans_W];
            spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData1[Trans_H][Trans_W+1];
		}
	}
	for(Trans_H=0; Trans_H<nHeight; Trans_H++)
	{
		for(Trans_N=0; Trans_N<nWidth_H-1; Trans_N++)
		{
			//奇偶数值和的一半
			Trans_Coeff1 = ((spTransData0[Trans_H][Trans_N]+spTransData0[Trans_H][Trans_N+1])>>1);	
			//逻辑非操作后数值加1
			Trans_Coeff1=~Trans_Coeff1+1;	
			//系数预测
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N]+Trans_Coeff1;	
		}
		//完成一个偶系数的边界处理
		Trans_Coeff1 = ((spTransData0[Trans_H][nWidth_H-1]+spTransData0[Trans_H][nWidth_H-2])>>1);
		Trans_Coeff1=~Trans_Coeff1+1;
		spTransData0[Trans_H][nWidth-1] = spTransData0[Trans_H][nWidth-1]+Trans_Coeff1;
		//完成一个奇系数的边界处理
		Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H]+spTransData0[Trans_H][nWidth_H+1])>>2);
		spTransData0[Trans_H][0] = spTransData0[Trans_H][0]+Trans_Coeff0;
		//提升，整数到整数的变换
		for(Trans_N=1; Trans_N<nWidth_H; Trans_N++)
		{
			Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H+Trans_N]+spTransData0[Trans_H][nWidth_H+Trans_N-1])>>2);
			spTransData0[Trans_H][Trans_N] = spTransData0[Trans_H][Trans_N]+Trans_Coeff0;
		}

	}//水平方向的变换结束
	//竖直方向的变换开始，数据源未水平变换后的小波系数
	for(Trans_M=0; Trans_M<nHeight; Trans_M++)
	{
		for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
		{
			spTransData0[Trans_M][Trans_N]*=fRadius;
			spTransData0[Trans_M][Trans_N+nWidth_H]/=fRadius;
		}
	}
	//行提升后的数据在spTransData0中，spTransData0中的数据自然奇偶有序
	for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
	{
		//列变换
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_H =Trans_M<<1;
			//频带LL部分
			spTransData1[Trans_M][Trans_N] = spTransData0[Trans_H][Trans_N];
			//频带HL部分
			spTransData1[nHeight_H+Trans_M][Trans_N] = spTransData0[Trans_H+1][Trans_N];
			//频带LH部分
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N];	
			//频带HH部分
			spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N] = spTransData0[Trans_H+1][nWidth_H+Trans_N];
		}
		//第一次提升奇数坐标系数
		for(Trans_M=0; Trans_M<nHeight_H-1; Trans_M++)
		{
		//竖直方向的变换 
			Trans_Coeff1 = ((spTransData1[Trans_M][Trans_N]+spTransData1[Trans_M+1][Trans_N])>>1);
			Trans_Coeff1=~Trans_Coeff1+1;
			spTransData1[nHeight_H+Trans_M][Trans_N] = spTransData1[nHeight_H+Trans_M][Trans_N]+Trans_Coeff1;
			Trans_Coeff1 = ((spTransData1[Trans_M][nWidth_H+Trans_N]+spTransData1[Trans_M+1][nWidth_H+Trans_N])>>1);
			Trans_Coeff1=~Trans_Coeff1+1;
			spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N] = spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+Trans_Coeff1;
		}
		Trans_Coeff1 = ((spTransData1[nHeight_H-1][Trans_N]+spTransData1[nHeight_H-2][Trans_N])>>1);
		Trans_Coeff1=~Trans_Coeff1+1;
		spTransData1[nHeight-1][Trans_N] = spTransData1[nHeight-1][Trans_N]+Trans_Coeff1;
		Trans_Coeff1 = ((spTransData1[nHeight_H-1][nWidth_H+Trans_N]+spTransData1[nHeight_H-2][nWidth_H+Trans_N])>>1);
		Trans_Coeff1=~Trans_Coeff1+1;
		//边界处理
		spTransData1[nHeight-1][nWidth_H+Trans_N] = spTransData1[nHeight-1][nWidth_H+Trans_N]+Trans_Coeff1;

		Trans_Coeff0 = ((spTransData1[nHeight_H][Trans_N]+spTransData1[nHeight_H+1][Trans_N])>>2);
		spTransData1[0][Trans_N] = spTransData1[0][Trans_N]+Trans_Coeff0;
		Trans_Coeff0 = ((spTransData1[nHeight_H][nWidth_H+Trans_N]+spTransData1[nHeight_H+1][nWidth_H+Trans_N])>>2);
		//边界处理
		spTransData1[0][nWidth_H+Trans_N] = spTransData1[0][nWidth_H+Trans_N]+Trans_Coeff0;
		//第一次提升偶数坐标系数
		for(Trans_M=1; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_Coeff0 = ((spTransData1[nHeight_H+Trans_M][Trans_N]+spTransData1[nHeight_H+Trans_M-1][Trans_N])>>2);
			spTransData1[Trans_M][Trans_N] = spTransData1[Trans_M][Trans_N]+Trans_Coeff0;
			Trans_Coeff0 = ((spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+spTransData1[nHeight_H+Trans_M-1][nWidth_H+Trans_N])>>2);
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData1[Trans_M][nWidth_H+Trans_N]+Trans_Coeff0;
		}
	}
	//存放小波系数，LL频带的系数进行幅值增强处理，其它高频频带的系数则削弱其幅值
	for(Trans_N=0; Trans_N<nWidth; Trans_N++)
	{
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			spTransData1[Trans_M][Trans_N]*=fRadius;
			spTransData1[Trans_M+nHeight_H][Trans_N]/=fRadius;
		}
	}
}
