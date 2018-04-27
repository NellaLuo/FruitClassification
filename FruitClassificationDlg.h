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

	/*******************ȫ�ֱ�������*************************/
	CDib dib;                 //��ʶ���ͼ��
	CString filename;         //ͼƬ����
	void DrawPhoto();
	CString m_strPhotoFilePath; //ȫ·����
	HBITMAP m_hBitmap;          //λͼ���
	bool m_bPhotoChange;        //��Ƭ�ļ��Ƿ�����ָ��
	CInterfaceDlg(CWnd* pParent = NULL);	// standard constructor

//	float R[256]; float G[256]; float B[256];           //��ɫֱ��ͼ
	CColorCoOccur cooccurFile;     //ͼ�������ͼƬ�Ķ���
	CColorCoOccur cooccur;         //���ɫ�ʹ������� ��������ͼ��
//	double guiyi[16];   //�ȴ���С �ٴ����     ����ֵ ���� * 4����ͬ������

	CDib ImgAlldib;
	int fileNum;            //�����ļ�������  ��ͼ��⣩
	CString filenameALL[900];   //��������ļ���·��
	DWORD m_dwJumlahFile;
	CString m_strExtension;
	CString m_strBeginPath;
	unsigned char* m_pBitmapALL;         //ͼ����е�ͼ��
	unsigned char* m_pTransferedALL;     //С���任������ͼ����е�ͼ��
	int pic;      //�������������±�
	double featureAll[1000][2];

	double edAll[900];  //С��
	double EDALLcolor[900];    //��ɫֱ��ͼ
	double tmpF1[900];         //����ɫֱ��ͼŷ�Ͼ������Ӧ

	double Iavg,Isd;     //��ʶ��ͼ�������ֵ
	double featureMerge[900][2];          //��Ŵ�ʶ��ͼ����ͼ�����ͼ���ŷ�Ͼ���1(��ɫֱ��ͼ) 2(С��)
	double f[900];       //�����ںϺ�����ս��

	/*********С����***/
	short** m_WvltCoeff; //С��ϵ��
	unsigned char* m_pBitmap;        //��ʶ��ͼ��
	unsigned char* m_pTransfered;     //С���任������ͼ��
	BOOL m_fDirty;
	CString m_strFileName;
	bool writeFile;              //�Ƿ������д���ļ�
//	bool shows=true;    //ͼ����еĲ�����ʾ ��ʶ�����ʾ

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
