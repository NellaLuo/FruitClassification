// Dib.h: interface for the CDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIB_H__0CD2F0F9_CFEA_4D51_AF8B_246ED5A5C989__INCLUDED_)
#define AFX_DIB_H__0CD2F0F9_CFEA_4D51_AF8B_246ED5A5C989__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDib : public CObject  
{
public:
    RGBQUAD* m_pRGB;
    BYTE* m_pData,*m_pData2;
    UINT m_numberOfColors;
	BOOL m_valid;
    BITMAPFILEHEADER bitmapFileHeader;
	
    BITMAPINFOHEADER* m_pBitmapInfoHeader;
    BITMAPINFO* m_pBitmapInfo;
	int byBitCount;
	DWORD dwWidthBytes;
	BYTE* pDib;
	DWORD size;

public:
	CDib();
	virtual ~CDib();

	char m_fileName[256];
    char* GetFileName();
    BOOL IsValid();
    DWORD GetSize();
    UINT GetWidth();
    UINT GetHeight();
    UINT GetNumberOfColors();
    RGBQUAD* GetRGB();
    BYTE* GetData();
	BYTE* GetData2();
	DWORD GetDibWidthBytes();
    BITMAPINFO* GetInfo();
	WORD PaletteSize(LPBYTE lpDIB);
	WORD DIBNumColors(LPBYTE lpDIB);
    void SaveFile(const CString filename);
	
public:
	void GradetoRGB();
	void RGBtoGrade();
    void LoadFile(const char* dibFileName);
	
	float R[256]; float G[256]; float B[256];           //ÑÕÉ«Ö±·½Í¼


};

#endif // !defined(AFX_DIB_H__0CD2F0F9_CFEA_4D51_AF8B_246ED5A5C989__INCLUDED_)
