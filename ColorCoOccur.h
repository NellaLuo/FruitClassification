// ColorCoOccur.h: interface for the CColorCoOccur class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORCOOCCUR_H__A7FD8049_19E8_4B40_94F5_C8E1E68D973B__INCLUDED_)
#define AFX_COLORCOOCCUR_H__A7FD8049_19E8_4B40_94F5_C8E1E68D973B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dib.h"

class CColorCoOccur : public CDib  
{
public:
	int calParallax(int i,int j);
	void calMatrixFeature();
	void calCoOccurMatrix(void);
	int DefineColorType(double H, double S, double V);
	int Min3(int a, int b, int c);
	int Max3(int a, int b, int c);
	void RGBtoColorType(HDC dc, CString m_strPhotoFilePath);
	CColorCoOccur();
	virtual ~CColorCoOccur();

	int colortype[384][256];         //全图颜色分类
	
	double co_occur[12][16][16];      //12个色彩共生矩阵，依次为 0度,d=1,3,9  45度,d=1,3,9  90度,d=1,3,9  135度,d=1,3,9
	
	double F[4][12];       //一次存放4个特征向量，能量 惯性矩 逆差矩 熵
	double feature[4][2];     //图片对应的4个特征向量的均值、标准差
};

#endif // !defined(AFX_COLORCOOCCUR_H__A7FD8049_19E8_4B40_94F5_C8E1E68D973B__INCLUDED_)
