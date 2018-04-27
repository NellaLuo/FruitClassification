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
*����������	DWT_Once���һ��ͼ���С���任										*
*����������	short **spOriginData����άָ�룬ָ��ԭʼ��ͼ������					*
*			short **spTransData0��С���任ϵ�������һ��ˮƽ�任���С��ϵ��	*
*			short **spTransData1��С���任ϵ�������һ����ֵ�任���С��ϵ��	*
*			int   nHeight		��ͼ�����Բ�������ֵΪԭʼͼ��ĸ߶�ֵ			*
*			int	  nHeight_H		��ͼ�����Բ�������ֵΪԭʼͼ��߶�ֵ��һ��		*
*			int   nWidth		��ͼ�����Բ�������ֵΪԭʼͼ��Ŀ��ֵ			*
*			int	  nWidth_H		��ͼ�����Բ�������ֵΪԭʼͼ����ֵ��һ��		*
*			int   layer			��С���任�Ĳ�������ֵΪ1��						*
*			float fRadius		��С���任���ӣ��ڵ���ʱ����ָ����ֵΪ1.414		*
********************************************************************************/


/********************************************************************************
*����������	DWT_TriLayers�������ͼ���С���任									*	
*����������	short **spOriginData����άָ�룬ָ��ԭʼ��ͼ������					*
*			short **spTransData0��С���任ϵ�������һ��ˮƽ�任���С��ϵ��	*
*			short **spTransData1��С���任ϵ�������һ����ֵ�任���С��ϵ��	*
*			int   nHeight		��ͼ�����Բ�������ֵΪԭʼͼ��ĸ߶�ֵ			*
*			int	  nHeight_H		��ͼ�����Բ�������ֵΪԭʼͼ��߶�ֵ��һ��		*
*			int   nWidth		��ͼ�����Բ�������ֵΪԭʼͼ��Ŀ��ֵ			*
*			int	  nWidth_H		��ͼ�����Բ�������ֵΪԭʼͼ����ֵ��һ��		*
*			int   layer			��С���任�Ĳ���,��ֵΪ3��						*
*			float fRadius		��С���任���ӣ��ڵ���ʱ����ָ����ֵΪ1.414		*
********************************************************************************/

void CWvltTrans::DWT_TriLayers(short** spOriginData, short** spTransData0, short** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int i;
	short **pData, **pTran0, **pTran1;
	//ͼ������Բ���
	int iWidth, iHeight, iWidth_H, iHeight_H;
	float fr = fRadius;
	//������ݿռ��ָ��
	pData = spOriginData;
	pTran0 = spTransData0;
	pTran1 = spTransData1;
	//ͼ�����Բ�����ֵ
	iWidth = nWidth;	iWidth_H = nWidth_H;
	iHeight = nHeight;	iHeight_H = nHeight_H;
	//����ѭ���������С���任
	for(i=1; i<=layer; i++)
	{
		DWT_Once(pData,pTran0,pTran1,iHeight,iHeight_H,iWidth,iWidth_H,i,fr);
		iHeight=iHeight>>1;		iWidth=iWidth>>1;
		iHeight_H=iHeight/2;	iWidth_H=iWidth/2;
	}
}

void CWvltTrans::DWT_Once(short **spOriginData, short **spTransData0, short **spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int Trans_W,				//ͼ��ɨ���߿��ƣ�������
		Trans_H,				//ͼ��ɨ���߿��ƣ�������
		Trans_M,				//ͼ�����ĺ�����
		Trans_N;				//ͼ������������
	short Trans_Coeff0;			//С���任ϵ��
    signed short Trans_Coeff1;
	fRadius=1.414;				//�任�˲�ϵ��
	//��ģ����ɱ任ϵ���ĸ�ֵ����
	//�б任,��һ�Σ�layer=1ʱ��ʱnHeight��Ϊԭʼͼ��ĸ߶�ֵ
    for(Trans_H=0; Trans_H<nHeight; Trans_H++)            
	{
		if(layer == 1)
			 //layer=1ʱ��nWidth_HΪԭʼͼ����ֵ��һ��
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
		//���任��������1,���������Ƶ��С��ϵ��
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
			//��ż��ֵ�͵�һ��
			Trans_Coeff1 = ((spTransData0[Trans_H][Trans_N]+spTransData0[Trans_H][Trans_N+1])>>1);	
			//�߼��ǲ�������ֵ��1
			Trans_Coeff1=~Trans_Coeff1+1;	
			//ϵ��Ԥ��
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N]+Trans_Coeff1;	
		}
		//���һ��żϵ���ı߽紦��
		Trans_Coeff1 = ((spTransData0[Trans_H][nWidth_H-1]+spTransData0[Trans_H][nWidth_H-2])>>1);
		Trans_Coeff1=~Trans_Coeff1+1;
		spTransData0[Trans_H][nWidth-1] = spTransData0[Trans_H][nWidth-1]+Trans_Coeff1;
		//���һ����ϵ���ı߽紦��
		Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H]+spTransData0[Trans_H][nWidth_H+1])>>2);
		spTransData0[Trans_H][0] = spTransData0[Trans_H][0]+Trans_Coeff0;
		//�����������������ı任
		for(Trans_N=1; Trans_N<nWidth_H; Trans_N++)
		{
			Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H+Trans_N]+spTransData0[Trans_H][nWidth_H+Trans_N-1])>>2);
			spTransData0[Trans_H][Trans_N] = spTransData0[Trans_H][Trans_N]+Trans_Coeff0;
		}

	}//ˮƽ����ı任����
	//��ֱ����ı任��ʼ������Դδˮƽ�任���С��ϵ��
	for(Trans_M=0; Trans_M<nHeight; Trans_M++)
	{
		for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
		{
			spTransData0[Trans_M][Trans_N]*=fRadius;
			spTransData0[Trans_M][Trans_N+nWidth_H]/=fRadius;
		}
	}
	//���������������spTransData0�У�spTransData0�е�������Ȼ��ż����
	for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
	{
		//�б任
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_H =Trans_M<<1;
			//Ƶ��LL����
			spTransData1[Trans_M][Trans_N] = spTransData0[Trans_H][Trans_N];
			//Ƶ��HL����
			spTransData1[nHeight_H+Trans_M][Trans_N] = spTransData0[Trans_H+1][Trans_N];
			//Ƶ��LH����
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N];	
			//Ƶ��HH����
			spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N] = spTransData0[Trans_H+1][nWidth_H+Trans_N];
		}
		//��һ��������������ϵ��
		for(Trans_M=0; Trans_M<nHeight_H-1; Trans_M++)
		{
		//��ֱ����ı任 
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
		//�߽紦��
		spTransData1[nHeight-1][nWidth_H+Trans_N] = spTransData1[nHeight-1][nWidth_H+Trans_N]+Trans_Coeff1;

		Trans_Coeff0 = ((spTransData1[nHeight_H][Trans_N]+spTransData1[nHeight_H+1][Trans_N])>>2);
		spTransData1[0][Trans_N] = spTransData1[0][Trans_N]+Trans_Coeff0;
		Trans_Coeff0 = ((spTransData1[nHeight_H][nWidth_H+Trans_N]+spTransData1[nHeight_H+1][nWidth_H+Trans_N])>>2);
		//�߽紦��
		spTransData1[0][nWidth_H+Trans_N] = spTransData1[0][nWidth_H+Trans_N]+Trans_Coeff0;
		//��һ������ż������ϵ��
		for(Trans_M=1; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_Coeff0 = ((spTransData1[nHeight_H+Trans_M][Trans_N]+spTransData1[nHeight_H+Trans_M-1][Trans_N])>>2);
			spTransData1[Trans_M][Trans_N] = spTransData1[Trans_M][Trans_N]+Trans_Coeff0;
			Trans_Coeff0 = ((spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+spTransData1[nHeight_H+Trans_M-1][nWidth_H+Trans_N])>>2);
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData1[Trans_M][nWidth_H+Trans_N]+Trans_Coeff0;
		}
	}
	//���С��ϵ����LLƵ����ϵ�����з�ֵ��ǿ����������ƵƵ����ϵ�����������ֵ
	for(Trans_N=0; Trans_N<nWidth; Trans_N++)
	{
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			spTransData1[Trans_M][Trans_N]*=fRadius;
			spTransData1[Trans_M+nHeight_H][Trans_N]/=fRadius;
		}
	}
}
