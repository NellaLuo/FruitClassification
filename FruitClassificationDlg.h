// FruitClassificationDlg.h : header file
//

#if !defined(AFX_FRUITCLASSIFICATIONDLG_H__3265D294_F203_416E_BF4A_0EE433B36FF0__INCLUDED_)
#define AFX_FRUITCLASSIFICATIONDLG_H__3265D294_F203_416E_BF4A_0EE433B36FF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define IDM_FILEFORMAT_ERROR            131
#define IDM_MEMORY_INSUFFICIENT         132
#define IDM_READFILE_ERROR              133
#define IDR_MENUWITHFILTER              134

#include "afxwin.h"
#include "Dib.h"
#include "ColorCoOccur.h"
/////////////////////////////////////////////////////////////////////////////
// CFruitClassificationDlg dialog

class CFruitClassificationDlg : public CDialog
{
// Construction
public:
	double GetED(CDib &dib1, CDib &dib2);
	void CalHistogram(CDib &dib);
	void Qsort(double a[], int low, int high);
	void WT_Three();
	void show();
	UINT _cdecl SiapCariRecursive();
	void ScanFiles(CString strNamaFile);
	void CariRecursive(CString strInitPath, CString strFileName);
	void OnDraw(CDC* pDC);
	bool ReadBitmap(LPCTSTR lpszPathName, unsigned char *& buffer);
	void ShowResult(CString fname,CWnd* pWnd);
	CFruitClassificationDlg(CWnd* pParent = NULL);	// standard constructor

	/*******************全局变量声明*************************/
	CDib dib;                 //待识别的图像
	CString filename;         //图片名称
	void DrawPhoto();
	CString m_strPhotoFilePath; //全路径名
	HBITMAP m_hBitmap;          //位图句柄
	bool m_bPhotoChange;        //照片文件是否重新指定
	CInterfaceDlg(CWnd* pParent = NULL);	// standard constructor

//	float R[256]; float G[256]; float B[256];           //颜色直方图
	CColorCoOccur cooccurFile;     //图像库里面图片的对象
	CColorCoOccur cooccur;         //针对色彩共生矩阵 待检索的图像
//	double guiyi[16];   //先存最小 再存最大     【均值 方差 * 4个不同特征】

	CDib ImgAlldib;
	int fileNum;            //遍历文件的总数  （图像库）
	CString filenameALL[900];   //存放所有文件的路径
	DWORD m_dwJumlahFile;
	CString m_strExtension;
	CString m_strBeginPath;
	unsigned char* m_pBitmapALL;         //图像库中的图像
	unsigned char* m_pTransferedALL;     //小波变换处理后的图像库中的图像
	int pic;      //存特征的数组下标
	double featureAll[1000][2];

	double edAll[900];  //小波
	double EDALLcolor[900];    //颜色直方图
	double tmpF1[900];         //与颜色直方图欧氏距离相对应

	double Iavg,Isd;     //待识别图像的特征值
	double featureMerge[900][2];          //存放待识别图像与图像库中图像的欧氏距离1(颜色直方图) 2(小波)
	double f[900];       //特征融合后的最终结果

	/*********小波儿***/
	short** m_WvltCoeff; //小波系数
	unsigned char* m_pBitmap;        //待识别图像
	unsigned char* m_pTransfered;     //小波变换处理后的图像
	BOOL m_fDirty;
	CString m_strFileName;
	bool writeFile;              //是否把特征写入文件
//	bool shows=true;    //图像库中的不用显示 待识别的显示

// 	CPalette hPalette;
// 	CPalette * CreateBitmapPalette(CDib *pBitmap);

// Dialog Data
	//{{AFX_DATA(CFruitClassificationDlg)
	enum { IDD = IDD_FRUITCLASSIFICATION_DIALOG };
	CEdit	m_Result;
	CString	m_RcgResult;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFruitClassificationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFruitClassificationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnselectImage();
	afx_msg void OnExit();
	afx_msg void OnWf();
	afx_msg void OnCalculateImgAll();
	afx_msg void OnFeatures();
	afx_msg void OnColorHist();
	afx_msg void OnColor();
	afx_msg void OnWave();
	afx_msg void OnColorCoOccur();
	afx_msg void OnCOcolorALL();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRUITCLASSIFICATIONDLG_H__3265D294_F203_416E_BF4A_0EE433B36FF0__INCLUDED_)
