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

	int colortype[384][256];         //ȫͼ��ɫ����
	
	double co_occur[12][16][16];      //12��ɫ�ʹ�����������Ϊ 0��,d=1,3,9  45��,d=1,3,9  90��,d=1,3,9  135��,d=1,3,9
	
	double F[4][12];       //һ�δ��4���������������� ���Ծ� ���� ��
	double feature[4][2];     //ͼƬ��Ӧ��4�����������ľ�ֵ����׼��
};

#endif // !defined(AFX_COLORCOOCCUR_H__A7FD8049_19E8_4B40_94F5_C8E1E68D973B__INCLUDED_)
