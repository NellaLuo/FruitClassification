// WvltTrans.h: interface for the CWvltTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WVLTTRANS_H__37C34D77_D43B_4B90_A2B1_8F459DCCA048__INCLUDED_)
#define AFX_WVLTTRANS_H__37C34D77_D43B_4B90_A2B1_8F459DCCA048__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Dib.h"
class CWvltTrans :public CDib 
{
public:
	CWvltTrans();
	virtual ~CWvltTrans();

public:
	void DWT_Once(short** spOriginData, short** spTransData0, short** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);
	//图像小波变换的逆变换
	//三次小波变换
	void DWT_TriLayers(short** spOriginData, short** spTransData0, short** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);
	//两次小波变换
//	void DWT_TwoLayers(short** spOriginData, short** spTransData0, short** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);
	//一次小波变换
//	void DWT_Once(short** spOriginData, short** spTransData0, short** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);
	//竖直方向的小波变换
//	void Vert_Transform(short** spOriginData, short** spTransData1, int nHeight_H, int nWidth, float fRadius);
	//水平方向的小波变换
//	void Hori_Transform(short** spOriginData, short** spTransData0, int nHeight, int nWidth_H, float fRadius);
};

#endif // !defined(AFX_WVLTTRANS_H__37C34D77_D43B_4B90_A2B1_8F459DCCA048__INCLUDED_)
