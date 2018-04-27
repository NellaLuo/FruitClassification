// FruitClassificationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FruitClassification.h"
#include "FruitClassificationDlg.h"
#include "Dib.h"
#include "WvltTrans.h"
#include "DirDialog.h"
#include <fstream>
#include "math.h"
#include <iostream>
#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFruitClassificationDlg dialog

CFruitClassificationDlg::CFruitClassificationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFruitClassificationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFruitClassificationDlg)
	m_RcgResult = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFruitClassificationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFruitClassificationDlg)
	DDX_Control(pDX, ID_Result, m_Result);
	DDX_Text(pDX, ID_Result, m_RcgResult);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFruitClassificationDlg, CDialog)
	//{{AFX_MSG_MAP(CFruitClassificationDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_selectImage, OnselectImage)
	ON_BN_CLICKED(ID_EXIT, OnExit)
	ON_BN_CLICKED(IDC_WF, OnWf)
	ON_BN_CLICKED(ID_CalculateImgAll, OnCalculateImgAll)
	ON_BN_CLICKED(ID_Features, OnFeatures)
	ON_BN_CLICKED(IDC_ColorHist, OnColorHist)
	ON_BN_CLICKED(IDC_Color, OnColor)
	ON_BN_CLICKED(IDC_Wave, OnWave)
	ON_BN_CLICKED(IDC_ColorCoOccur, OnColorCoOccur)
	ON_BN_CLICKED(IDC_COcolorALL, OnCOcolorALL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFruitClassificationDlg message handlers

BOOL CFruitClassificationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	if (!m_strPhotoFilePath.IsEmpty())	{
		m_hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_strPhotoFilePath, 
			IMAGE_BITMAP, 0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION); 
		// ���ⲿBMP�ļ�װ�ز�����HBITMAP���
	}
	m_bPhotoChange = FALSE;
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	m_fDirty = FALSE;
	m_pBitmap = NULL ;
	m_pTransfered = NULL;
	m_pBitmapALL=NULL;
//	memset(m_pBitmapALL,0,900);
	m_pTransferedALL=NULL;
	pic=0;
	for (int i=0;i<900;i++)
	{
		featureMerge[i][0]=0;
		featureMerge[i][1]=0;
//		featureMerge[i][2]=0;
		EDALLcolor[i]=0;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFruitClassificationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFruitClassificationDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFruitClassificationDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

	CPalette hPalette;
	CPalette * CreateBitmapPalette(CDib *pBitmap);

void CFruitClassificationDlg::OnselectImage() 
{
	// TODO: Add your control notification handler code here
	Invalidate(1);
	static char BASED_CODE szFilter[] = "ͼƬ�ļ�(*.bmp)|*.bmp||";
	CFileDialog dlg( TRUE,NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
		szFilter);
	
	 dlg.m_ofn.lpstrInitialDir=_T("F:\\��ҵ���\\codes\\fruitUnknown");                             //��������Ĭ��·��
	if (IDOK != dlg.DoModal()) return;
	m_bPhotoChange = TRUE;
	m_strPhotoFilePath = dlg.GetPathName();
	m_hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_strPhotoFilePath,
		IMAGE_BITMAP, 0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION); 
	
	filename.Format ("%s",dlg.GetPathName() ); 

	dib.LoadFile(filename);
	cooccur.LoadFile(filename);

	Invalidate();	// ǿ�ƶԻ�����Ч������OnPaint
	
	CWnd* pWnd = GetDlgItem(IDC_DRAW);	// ���IDC_DRAW�ؼ�����ָ��
	pWnd->UpdateWindow();
	
	CDC* pDC = pWnd->GetDC();			// ��ô��ڵ�ǰ���豸����ָ��

	/************************/
	m_strFileName=filename;
	ReadBitmap(filename, m_pBitmap);
//	m_pTransfered=dib.m_pData;
	/**************************/

	BYTE* pBitmapData = dib.GetData();
	LPBITMAPINFO pBitmapInfo = dib.GetInfo();
	float m_scale=1;
	int bitmapHeight = dib.GetHeight();
	int bitmapWidth = dib.GetWidth();
	int scaledWidth = (int)(bitmapWidth * m_scale);
	int scaledHeight = (int)(bitmapHeight * m_scale);
	
    float fScale;						// ��/��
    fScale = (float)bitmapHeight / (float)bitmapWidth;
	// ����������ʾ�ľ��δ�С
	CRect rcClient;
	int nWidth, nHeight, nX = 0, nY = 0;
	pWnd->GetClientRect( rcClient );
	nWidth = rcClient.Width();
	nHeight = (int)((float)nWidth * fScale);
	nX = 0;	nY = ( rcClient.Height() - nHeight ) / 2;
	if ( nHeight > rcClient.Height() )	
	{
		nHeight = rcClient.Height();		
		nWidth = (int)((float)nHeight/fScale);
        nX = ( rcClient.Width() - nWidth )/2;		
		nY = 0;
		rcClient.DeflateRect(-1, -1);
		pDC->Rectangle( rcClient );
	}
	
	if (dib.GetRGB()) // Has a color table
	{
		CPalette * hPalette=CreateBitmapPalette(&dib);
		//	CPalette * CreateBitmapPalette(CDib *pBitmap)
		CPalette * hOldPalette =
			pDC->SelectPalette(hPalette, true);
		pDC->RealizePalette();
		::StretchDIBits(pDC->GetSafeHdc(),nX, nY, scaledWidth, scaledHeight,
			0, 0, bitmapWidth, bitmapHeight,
			pBitmapData, pBitmapInfo,
			DIB_RGB_COLORS, SRCCOPY);
		pDC->SelectPalette(hOldPalette, true);
		::DeleteObject(hPalette);
	}
	else
	{
		StretchDIBits(pDC->GetSafeHdc(),
			nX, nY, scaledWidth, scaledHeight,
			0, 0, bitmapWidth, bitmapHeight,
			pBitmapData, pBitmapInfo,
			DIB_RGB_COLORS, SRCCOPY);
	}
}

CPalette * CreateBitmapPalette(CDib *pBitmap)
{
	CPalette hPalette;
	struct
	{
		WORD Version;
		WORD NumberOfEntries;
		PALETTEENTRY aEntries[256];
	} palette = { 0x300, 256 };
	
	LPRGBQUAD pRGBTable = pBitmap->GetRGB();
	UINT numberOfColors = pBitmap->GetNumberOfColors();
	
	for(UINT x=0; x<numberOfColors; ++x)
	{
		palette.aEntries[x].peRed =
			pRGBTable[x].rgbRed;
		palette.aEntries[x].peGreen =
			pRGBTable[x].rgbGreen;
		palette.aEntries[x].peBlue =
			pRGBTable[x].rgbBlue;
		palette.aEntries[x].peFlags = 0;
	}
	
	hPalette.CreatePalette((LPLOGPALETTE)&palette);
	
	return &hPalette;
	::DeleteObject(hPalette);   
}



void CFruitClassificationDlg::OnExit()        //�˳�ϵͳ
{
	// TODO: Add your control notification handler code here
// 	if( m_pBitmap != NULL)
// 		free( m_pBitmap);
// 	if( m_pTransfered != NULL)
// 		free( m_pTransfered	);
// 	if( m_pBitmapALL != NULL)
// 		free( m_pBitmapALL);
// 	if( m_pTransferedALL != NULL)
// 		free( m_pTransferedALL	);
	exit(1);
}

/**************С���任��Ӧ����******************************/
void CFruitClassificationDlg::OnWf()       //3��С���任
{
	// TODO: Add your control notification handler code here
	WT_Three();  //С���任
	//��ʾͼ���С���任
//	dib=*m_pTransfered;
// 	CString filenew=filename+"(2)";             //��������ͼƬ�ԣ�2������ʽ����
// 	m_pTransfered->SaveFile(filenew);
// 	CWnd* pWnd = GetDlgItem(IDC_DRAW2);	      // ���IDC_DRAW�ؼ�����ָ��
// 	ShowResult(filenew,pWnd);  
	
	show();
	
// 	CWnd* pWnd = GetDlgItem(IDC_DRAW2);	// ���IDC_DRAW�ؼ�����ָ��
// 	pWnd->UpdateWindow();		
// 	CDC* pDC = pWnd->GetDC();			// ��ô��ڵ�ǰ���豸����ָ��
// 	OnDraw(pDC);
}


void CFruitClassificationDlg::ShowResult(CString fname, CWnd *pWnd)
{
	pWnd->UpdateWindow();
	CDC* pDC = pWnd->GetDC();			// ��ô��ڵ�ǰ���豸����ָ��
	
	dib.LoadFile(fname);
	BYTE* pBitmapData = dib.GetData();
	LPBITMAPINFO pBitmapInfo = dib.GetInfo();
	float m_scale=1;
	int bitmapHeight = dib.GetHeight();
	int bitmapWidth = dib.GetWidth();
	int scaledWidth = (int)(bitmapWidth * m_scale);
	int scaledHeight = (int)(bitmapHeight * m_scale);
	
    float fScale;						// ��/��
    fScale = (float)bitmapHeight / (float)bitmapWidth;
	// ����������ʾ�ľ��δ�С
	CRect rcClient;
	int nWidth, nHeight, nX = 0, nY = 0;
	pWnd->GetClientRect( rcClient );
	nWidth = rcClient.Width();
	nHeight = (int)((float)nWidth * fScale);
	nX = 0;	nY = ( rcClient.Height() - nHeight ) / 2;
	if ( nHeight > rcClient.Height() )	
	{
		nHeight = rcClient.Height();		
		nWidth = (int)((float)nHeight/fScale);
        nX = ( rcClient.Width() - nWidth )/2;		
		nY = 0;
		rcClient.DeflateRect(-1, -1);
		pDC->Rectangle( rcClient );
	}
	
	if (dib.GetRGB()) // Has a color table
	{
		CPalette * hPalette=CreateBitmapPalette(&dib);
		CPalette * hOldPalette =
			pDC->SelectPalette(hPalette, true);
		pDC->RealizePalette();
		::StretchDIBits(pDC->GetSafeHdc(),nX, nY, scaledWidth, scaledHeight,
			0, 0, bitmapWidth, bitmapHeight,
			pBitmapData, pBitmapInfo,
			DIB_RGB_COLORS, SRCCOPY);
		pDC->SelectPalette(hOldPalette, true);
		::DeleteObject(hPalette);
	}
	else
	{
		StretchDIBits(pDC->GetSafeHdc(),
			nX, nY, scaledWidth, scaledHeight,
			0, 0, bitmapWidth, bitmapHeight,
			pBitmapData, pBitmapInfo,
			DIB_RGB_COLORS, SRCCOPY);
	}
}

bool CFruitClassificationDlg::ReadBitmap(LPCTSTR lpszPathName, unsigned char *&buffer)
{
	FILE *file;
	file = fopen( lpszPathName, "rb");
	if(!file)
		return FALSE;
	fpos_t posend, posbegin;
	
	fseek(file, 0, SEEK_END);
	fgetpos(file, &posend);
	fseek(file, 0, SEEK_SET);
	fgetpos(file,&posbegin);
	size_t filesize = (size_t)(posend - posbegin);
	
	if( buffer == NULL )
		buffer = (unsigned char*)malloc( filesize);
	if( buffer == NULL )
	{
		AfxMessageBox(IDM_MEMORY_INSUFFICIENT, MB_OK, 0);
		return FALSE;
	}
	memset(buffer, 0, filesize);
	size_t sizeread = fread(buffer, sizeof(unsigned char), filesize, file);
	if(sizeread != filesize)
	{
		AfxMessageBox(IDM_READFILE_ERROR, MB_OK, 0);
		free(buffer);
		buffer = NULL;
		fclose(file);
		return FALSE;
	}
	if( buffer[0] != 'B'||buffer[1] != 'M')
	{
		AfxMessageBox(IDM_FILEFORMAT_ERROR, MB_OK, 0);
		free(buffer);
		buffer = NULL;
		fclose(file);
		return FALSE;
	}
	fclose(file);
	return TRUE;
}

void CFruitClassificationDlg::OnDraw(CDC *pDC)
{
	//	Display the transfered image
	if( m_pTransfered == NULL)
		return ;
	
	/***********ԭ����****/        //��ʾλ�ò���
// 	unsigned char* pBitmap =m_pBitmap;
// 	LPBITMAPINFO lpBitmapInfo = (LPBITMAPINFO)(pBitmap + 14);
// 	unsigned char* pBitmapData = m_pTransfered;
// 	unsigned long biHeight = ((LPBITMAPINFOHEADER)lpBitmapInfo)->biHeight;
// 	unsigned long biWidth = ((LPBITMAPINFOHEADER)lpBitmapInfo)->biWidth;
// 	
// 	CRect rect;
// 	GetClientRect(&rect);
// 	long originx = 0;
// 	long originy = 0;
// 	if((unsigned long)(rect.Width()) > biWidth)
// 		originx = (rect.Width() - biWidth)>>1;
// 	if((unsigned long)(rect.Height()) > biHeight)
// 		originy = (rect.Height() - biHeight)>>1;
// 	
// 	SetDIBitsToDevice(pDC->m_hDC, originx, originy, biWidth, biHeight, 0, 0, 0, biHeight,
// 					  pBitmapData, lpBitmapInfo, DIB_RGB_COLORS);


	/**************�ĺ�**********/
	CWnd* pWnd = GetDlgItem(IDC_DRAW2);	// ���IDC_DRAW�ؼ�����ָ��
	pWnd->UpdateWindow();	
	pDC = pWnd->GetDC();			// ��ô��ڵ�ǰ���豸����ָ��

//	BYTE* pBitmapData = dib.GetData();
	BYTE* pBitmapData =m_pTransfered;                      //��������޸�
//	LPBITMAPINFO pBitmapInfo = dib.GetInfo();                       
	LPBITMAPINFO pBitmapInfo = (LPBITMAPINFO)(m_pBitmap + 14);
	float m_scale=1;
	int bitmapHeight = dib.GetHeight();
	int bitmapWidth = dib.GetWidth();
	int scaledWidth = (int)(bitmapWidth * m_scale);
	int scaledHeight = (int)(bitmapHeight * m_scale);
	
    float fScale;						// ��/��
    fScale = (float)bitmapHeight / (float)bitmapWidth;
	// ����������ʾ�ľ��δ�С
	CRect rcClient;
	int nWidth, nHeight, nX = 0, nY = 0;
	pWnd->GetClientRect( rcClient );
	nWidth = rcClient.Width();
	nHeight = (int)((float)nWidth * fScale);
	nX = 0;	nY = ( rcClient.Height() - nHeight ) / 2;
	if ( nHeight > rcClient.Height() )	
	{
		nHeight = rcClient.Height();		
		nWidth = (int)((float)nHeight/fScale);
        nX = ( rcClient.Width() - nWidth )/2;		
		nY = 0;
		rcClient.DeflateRect(-1, -1);
		pDC->Rectangle( rcClient );
	}
	
	if (dib.GetRGB()) // Has a color table
	{
		CPalette * hPalette=CreateBitmapPalette(&dib);
		CPalette * hOldPalette =
			pDC->SelectPalette(hPalette, true);
		pDC->RealizePalette();
		::StretchDIBits(pDC->GetSafeHdc(),nX, nY, scaledWidth, scaledHeight,
			0, 0, bitmapWidth, bitmapHeight,
			pBitmapData, pBitmapInfo,
			DIB_RGB_COLORS, SRCCOPY);
		pDC->SelectPalette(hOldPalette, true);
		::DeleteObject(hPalette);
	}
	else
	{
		StretchDIBits(pDC->GetSafeHdc(),
			nX, nY, scaledWidth, scaledHeight,
			0, 0, bitmapWidth, bitmapHeight,
			pBitmapData, pBitmapInfo,
			DIB_RGB_COLORS, SRCCOPY);
	}
}

void CFruitClassificationDlg::OnCalculateImgAll()       //����ͼ�����ͼ���������С����
{
	// TODO: Add your control notification handler code here
	CDirDialog dlg;
	dlg.m_strSelDir  = "F:\\��ҵ���\\codes\\fruit\\";
	dlg.m_strWindowTitle= "Select .BMP File(s) Directory";
	dlg.m_strTitle = "Browse Folder for .BMP files location";
	m_strExtension = "*.BMP";
	
	if( dlg.DoBrowse(this) )
	{	
		m_strBeginPath = dlg.m_strPath;
		m_dwJumlahFile = 0x0;
		SiapCariRecursive();
	}
	/******************ѡ��ͼ���·��***************************/

	/********��������ֵ********************/
	int i=0;
	for (i=0;i<fileNum;i++)
	{
//		ImgAlldib.LoadFile(filenameALL[i]);        //����ͼ����е�ͼƬ
		//С������
//		m_strFileName=filenameALL[i];

		ReadBitmap(filenameALL[i], m_pBitmapALL);              
		writeFile=true; //��Ҫ������д���ļ�
		WT_Three();
	}

	int p=0;
	/*********ͳһд���ļ�*****/                                                    //ͼ���ȷ����ֻ������һ�μ���ע�͵�
	ofstream out("F://��ҵ���//codes//FruitClassification//WTfeature.txt");	
	for(p=0;p<fileNum;p++)
		out<<featureAll[p][0]<<" "<<featureAll[p][1]<<endl;
	out.close();
	
	writeFile=false;

 	MessageBox("�����ļ������ɡ�");
}

void CFruitClassificationDlg::CariRecursive(CString strInitPath, CString strFileName)
{
	CFileFind ff;
	CString strPath;
	BOOL bFound = TRUE;
	int temp=0;
	
	strPath = strInitPath + "\\*";
	if( ff.FindFile(strPath) )
	{		
		while(bFound)
		{
			bFound = ff.FindNextFile();
			if(!ff.IsDots())
			{
				CString strFile = ff.GetFilePath();
				filenameALL[temp]=strFile;
				temp++;
				if(ff.IsDirectory())
				{
					CariRecursive(strFile, strFileName);
				}
			}
		}
		
		ScanFiles(strInitPath + "\\" + strFileName );
	}
	fileNum=temp-1;
	ff.Close();

}

void CFruitClassificationDlg::ScanFiles(CString strNamaFile)
{
	CFileFind ff;
	BOOL bFound = TRUE;
	
	if( ff.FindFile(strNamaFile) )
	{
		while(bFound)
		{
			bFound = ff.FindNextFile();
			if((!ff.IsDots()) && (!ff.IsDirectory()))
			{
				CString strFile = ff.GetFilePath();			
				m_dwJumlahFile++;				
			}
		}
	}
	
	ff.Close();
}

UINT _cdecl CFruitClassificationDlg::SiapCariRecursive()
{
	CariRecursive(m_strBeginPath, m_strExtension);
	return true;
}

void CFruitClassificationDlg::show()
{
	 	CWnd* pWnd = GetDlgItem(IDC_DRAW2);	// ���IDC_DRAW�ؼ�����ָ��
	 	pWnd->UpdateWindow();		
	 	CDC* pDC = pWnd->GetDC();			// ��ô��ڵ�ǰ���豸����ָ��
	 	OnDraw(pDC);
}

void CFruitClassificationDlg::WT_Three()
{
	if (writeFile==true)               //д���ļ�
	{
		//��ȡ����ͼ����ļ�ͷ����ȡͼ������Բ���
		LPBITMAPINFOHEADER lpBitmapInfoHeader = (LPBITMAPINFOHEADER)(m_pBitmapALL+14);
		LPBITMAPFILEHEADER lpBitmapFileHeader = (LPBITMAPFILEHEADER)m_pBitmapALL;
		unsigned char *lpData = m_pBitmapALL + lpBitmapFileHeader->bfOffBits;
		unsigned long biHeight = lpBitmapInfoHeader->biHeight;
		unsigned long biWidth = lpBitmapInfoHeader->biWidth;
		unsigned long biAlign = (biWidth*3+3)/4 *4;
		unsigned long bmSize = biHeight * biAlign;
		if(m_pTransferedALL==NULL)
			m_pTransferedALL=(unsigned char*) malloc (bmSize);
		if(m_pTransferedALL==NULL)
			return;
		//ͼ�������������������
		int x,y,cur;
		unsigned char tempR, tempG, tempB;
		float fTempBufforDisp;
		short MaxPixVal,MinPixVal,Diff;
		short **spOriginData, **spTransData0, **spTransData1;  //ָ��ԭʼͼ�����ݣ�С���任ϵ����ˮƽ�任�󣩣�С���任ϵ������ֱ�任��
		//����ͼ��С���任�������ڴ�ռ�
		spOriginData = new short* [biHeight];
		spTransData0 = new short* [biHeight];
		spTransData1 = new short* [biHeight];
		m_WvltCoeff	= new short * [biHeight];
		for(int i = 0; i < biHeight; i ++)
		{
			spOriginData[i] = new short [biWidth];
			spTransData0[i] = new short [biWidth];
			spTransData1[i] = new short [biWidth];
			m_WvltCoeff[i] = new short [biWidth];
		}
		//����ͼ��С����
		CWvltTrans *pTrans;
		//���豸��ͼ�񻺴��л�ȡԭʼͼ�������
		for(y=0; y<(int)biHeight; y++)
		{
			for( x=0; x<(int)biWidth; x++)
			{
				cur = y*biAlign+3*x;
				tempB=lpData[cur];
				tempG=lpData[cur+1];
				tempR=lpData[cur+2];
				spOriginData[biHeight-1-y][x]=(short)(0.3*tempR+0.59*tempG+0.11*tempB);  //no problem
			}
		}
		
		//���ͼ�������С���任
		pTrans->DWT_TriLayers(spOriginData,spTransData0,spTransData1,biHeight,biHeight/2,biWidth,biWidth/2,3,1.414);
		
		MaxPixVal=spTransData1[0][0];
		MinPixVal=spTransData1[0][0];
		//�õ�С��ϵ���ļ���ֵ�ͼ�Сֵ
		for( y=0; y<(int)biHeight; y++)
		{
			for( x=0; x<(int)biWidth; x++)
			{
				if(MaxPixVal<spTransData1[y][x])
					MaxPixVal=spTransData1[y][x];
				if(MinPixVal>spTransData1[y][x])
					MinPixVal=spTransData1[y][x];
				m_WvltCoeff[y][x] = spTransData1[y][x];
			}
		}
		//�����С��ϵ���ļ�ֵ��
		Diff=MaxPixVal-MinPixVal;
		//��ͼ���С�����ݴ���������ʾ������
		for(y=0; y<(int)biHeight; y++)
		{
			for(x=0; x<(int)biWidth; x++)
			{
				//��ΪС���任���С��ϵ���п��ܳ���255�������࣬��ô�ͽ�
				//С��ϵ���ķ�Χӳ�䵽0~255�����ڣ��Ժ�������ƵĴ���Ŀ�Ķ���һ����
				fTempBufforDisp=spTransData1[biHeight-1-y][x];
				fTempBufforDisp-=MinPixVal;
				fTempBufforDisp*=255;
				fTempBufforDisp/=(float)Diff;
				cur= y*biAlign+3*x;		//current pixel	
				m_pTransferedALL[cur]	= (unsigned char)fTempBufforDisp;
				m_pTransferedALL[cur+1]= (unsigned char)fTempBufforDisp;
				m_pTransferedALL[cur+2]= (unsigned char)fTempBufforDisp;
			}
		}	
		/**********��Ҫд���ļ�������ֵ���ж�С��ϵ���Ĵ���******/
		double avg=0,sd=0; //��ֵ����׼��
		for( y=0; y<(int)biHeight; y++)
		{
			for( x=0; x<(int)biWidth; x++)
			{
				avg += m_WvltCoeff[y][x] ;			
			}
		}
		avg=avg/(biHeight*biWidth);
		
		for( y=0; y<(int)biHeight; y++)
		{
			for( x=0; x<(int)biWidth; x++)
			{
				double tmp=pow((double)fabs(m_WvltCoeff[y][x])-avg,2);	
				sd+=tmp;
			}
		}
		sd=sd/(biHeight*biWidth);
		sd=(double)sqrt(sd);
		
		featureAll[pic][0]=avg;
		featureAll[pic][1]=sd;
		pic++;
	
	}else{
		
		//��ȡ����ͼ����ļ�ͷ����ȡͼ������Բ���
		LPBITMAPINFOHEADER lpBitmapInfoHeader = (LPBITMAPINFOHEADER)(m_pBitmap+14);
		LPBITMAPFILEHEADER lpBitmapFileHeader = (LPBITMAPFILEHEADER)m_pBitmap;
		unsigned char *lpData = m_pBitmap + lpBitmapFileHeader->bfOffBits;
		unsigned long biHeight = lpBitmapInfoHeader->biHeight;
		unsigned long biWidth = lpBitmapInfoHeader->biWidth;
		unsigned long biAlign = (biWidth*3+3)/4 *4;
		unsigned long bmSize = biHeight * biAlign;
		if(m_pTransfered==NULL)
			m_pTransfered=(unsigned char*) malloc (bmSize);
		if(m_pTransfered==NULL)
			return;
		//ͼ�������������������
		int x,y,cur;
		unsigned char tempR, tempG, tempB;
		float fTempBufforDisp;
		short MaxPixVal,MinPixVal,Diff;
		short **spOriginData, **spTransData0, **spTransData1;  //ָ��ԭʼͼ�����ݣ�С���任ϵ����ˮƽ�任�󣩣�С���任ϵ������ֱ�任��
		//����ͼ��С���任�������ڴ�ռ�
		spOriginData = new short* [biHeight];
		spTransData0 = new short* [biHeight];
		spTransData1 = new short* [biHeight];
		m_WvltCoeff	= new short * [biHeight];
		for(int i = 0; i < biHeight; i ++)
		{
			spOriginData[i] = new short [biWidth];
			spTransData0[i] = new short [biWidth];
			spTransData1[i] = new short [biWidth];
			m_WvltCoeff[i] = new short [biWidth];
		}
		//����ͼ��С����
		CWvltTrans *pTrans;
		//���豸��ͼ�񻺴��л�ȡԭʼͼ�������
		for(y=0; y<(int)biHeight; y++)
		{
			for( x=0; x<(int)biWidth; x++)
			{
				cur = y*biAlign+3*x;
				tempB=lpData[cur];
				tempG=lpData[cur+1];
				tempR=lpData[cur+2];
				spOriginData[biHeight-1-y][x]=(short)(0.3*tempR+0.59*tempG+0.11*tempB);  //no problem
			}
		}
		
		//���ͼ�������С���任
		pTrans->DWT_TriLayers(spOriginData,spTransData0,spTransData1,biHeight,biHeight/2,biWidth,biWidth/2,3,1.414);
		
		// 	//����ͼ��Ա������־
		// 	m_bTribl = TRUE;
		// 	m_bTribl = m_bTribl & ~m_bTwice & ~m_bOnce & ~m_bFilter;
		MaxPixVal=spTransData1[0][0];
		MinPixVal=spTransData1[0][0];
		//�õ�С��ϵ���ļ���ֵ�ͼ�Сֵ
		for( y=0; y<(int)biHeight; y++)
		{
			for( x=0; x<(int)biWidth; x++)
			{
				if(MaxPixVal<spTransData1[y][x])
					MaxPixVal=spTransData1[y][x];
				if(MinPixVal>spTransData1[y][x])
					MinPixVal=spTransData1[y][x];
				m_WvltCoeff[y][x] = spTransData1[y][x];
			}
		}
		//�����С��ϵ���ļ�ֵ��
		Diff=MaxPixVal-MinPixVal;
		//��ͼ���С�����ݴ���������ʾ������
		for(y=0; y<(int)biHeight; y++)
		{
			for(x=0; x<(int)biWidth; x++)
			{
				//��ΪС���任���С��ϵ���п��ܳ���255�������࣬��ô�ͽ�
				//С��ϵ���ķ�Χӳ�䵽0~255�����ڣ��Ժ�������ƵĴ���Ŀ�Ķ���һ����
				fTempBufforDisp=spTransData1[biHeight-1-y][x];
				fTempBufforDisp-=MinPixVal;
				fTempBufforDisp*=255;
				fTempBufforDisp/=(float)Diff;
				cur= y*biAlign+3*x;		//current pixel	
				m_pTransfered[cur]	= (unsigned char)fTempBufforDisp;
				m_pTransfered[cur+1]= (unsigned char)fTempBufforDisp;
				m_pTransfered[cur+2]= (unsigned char)fTempBufforDisp;
			}
		}	

		/**********��Ҫ��ʶ��ͼ����ж�С��ϵ���Ĵ���******/
		double avg=0,sd=0; //��ֵ����׼��
		for( y=0; y<(int)biHeight; y++)
		{
			for( x=0; x<(int)biWidth; x++)
			{
				avg += m_WvltCoeff[y][x] ;			
			}
		}
		avg=avg/(biHeight*biWidth);
		
		for( y=0; y<(int)biHeight; y++)
		{
			for( x=0; x<(int)biWidth; x++)
			{
				double tmp=pow((double)fabs(m_WvltCoeff[y][x])-avg,2);	
				sd+=tmp;
			}
		}
		sd=sd/(biHeight*biWidth);
		sd=(double)sqrt(sd);
		
		Iavg=avg;
		Isd=sd;

		//ɾ����ʱ�����ݿռ�
		delete spOriginData;
		delete spTransData0;
		delete spTransData1;
	}

}


void CFruitClassificationDlg::Qsort(double a[], int low, int high)
{
	if(low >= high)
    {
        return;
    }
    int first = low;
    int last = high;
    double key = a[first];/*���ֱ�ĵ�һ����¼��Ϊ����*/
	
    while(first < last)
    {
        while(first < last && a[last] >= key)
        {
            --last;
        }
		
        a[first] = a[last];/*���ȵ�һ��С���Ƶ��Ͷ�*/
		
        while(first < last && a[first] <= key)
        {
            ++first;
        }
		
        a[last] = a[first];    
		/*���ȵ�һ������Ƶ��߶�*/
    }
    a[first] = key;/*�����¼��λ*/
    Qsort(a, low, first-1);
    Qsort(a, first+1, high);
}

void CFruitClassificationDlg::OnFeatures()     //������ɫ������
{
	// TODO: Add your control notification handler code here
	m_RcgResult="";
	int p=0;
	for (p=0;p<900;p++)
	{
		edAll[p]=0;            //��ʼ��
	}
	
	double tmpED=0;
	for (p=0;p<fileNum;p++)
	{
		tmpED+=pow(featureAll[p][0]-Iavg,2);
		tmpED+=pow(featureAll[p][1]-Isd,2);
		edAll[p]=sqrt(tmpED);                           //���EDALL
		tmpED=0;
	}
	
	for (p=0;p<fileNum;p++)
	{
		featureMerge[p][1]=edAll[p];            //С���������︳ֵ�Ͳ����ٶ��ļ���
		featureMerge[p][0]=tmpF1[p];
	}
	/*****************��ȡ�ļ� �õ�����������ŷ�Ͼ��룩**************************/  //�ļ�����Ϊ˫б��
 	int i=0;
	//����С���任��ŷ�Ͼ����Ѿ���ǰ�渳ֵ�����Բ����ٶ�ȡ�ļ���

	/********************�����ں�********************************/
	const int M=2; //2���������ںϣ���ɫֱ��ͼ С����

	for(i=0;i<fileNum;i++){
		f[i]=0;
	}

	double min1=featureMerge[i][0],min2=featureMerge[i][1],max1=featureMerge[i][0],max2=featureMerge[i][1];
	for (i=1;i<fileNum;i++)
	{
		if (featureMerge[i][0]<min1)
		{
			min1=featureMerge[i][0];
		}
		if (featureMerge[i][0]>max1)
		{
			max1=featureMerge[i][0];
		}
		if (featureMerge[i][1]<min2)
		{
			min2=featureMerge[i][1];
		}
		if (featureMerge[i][1]>max2)
		{
			max2=featureMerge[i][1];
		}
	}//Ѱ�������Сֵ Ϊ��һ����׼��

	for (i=0;i<fileNum;i++)
	{
		double f0=0,f1=0;
		/**********ֻ��С��**********/
//		f[i]=featureMerge[i][1];      //Ч������
		           

		//��һ������
	/*	y=(x-MinValue)/(MaxValue-MinValue)
    ˵����x��y�ֱ�Ϊת��ǰ�����ֵ��MaxValue��MinValue�ֱ�Ϊ���������ֵ����Сֵ��*/
		f0=(featureMerge[i][0]-min1)/(max1-min1);
		f1=(featureMerge[i][1]-min2)/(max2-min2);
		f[i]=f0+f1;// 
	}

	//ȡǰ5�� ��ȡ�ļ����� �����б�ÿ��������һ��   �����⣺��β���ԭ�ȵ�ͼƬ��-----������
	double tmpF[900];
	for (i=0;i<fileNum;i++)
	{
		tmpF[i]=f[i];      //��δ����֮ǰ�ĸ�ֵ  ���ں����ж����ĸ�ͼ��
	}
	Qsort(f,0,fileNum-1);  //��f[i]��������
	string pipei[5];        //��¼ǰ5��ƥ�������
	bool chongfu[900];      //���ܶ��ƥ��ͬһ��ͼƬ
	for (i=0;i<fileNum;i++)
	{
		chongfu[i]=false;
	}
	//�����б�ǰ5��������
	for (i=0;i<fileNum;i++)
	{
		if (f[0]==tmpF[i] && chongfu[i]!=true)
		{
			pipei[0]=filenameALL[i];             //�õ���1��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}
	for (i=0;i<fileNum;i++)
	{
		if (f[1]==tmpF[i] && chongfu[i]!=true)
		{
			pipei[1]=filenameALL[i];             //�õ���2��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}
	for (i=0;i<fileNum;i++)
	{
		if (f[2]==tmpF[i] && chongfu[i]!=true)
		{
			pipei[2]=filenameALL[i];             //�õ���3��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}
	for (i=0;i<fileNum;i++)
	{
		if (f[3]==tmpF[i] && chongfu[i]!=true)
		{
			pipei[3]=filenameALL[i];             //�õ���4��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}
	for (i=0;i<fileNum;i++)
	{
		if (f[4]==tmpF[i] && chongfu[i]!=true)
		{
			pipei[4]=filenameALL[i];             //�õ���5��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}

	const int kinds=15;
	int fruitCog[kinds];           //ÿ��ˮ�����ʶ��ˮ����ƥ����
	for (i=0;i<kinds;i++)
	{
		fruitCog[i]=0;
	}
	for (i=0;i<5;i++)
	{
		//ע��˴���·����Ҫȫ ��ͼ���·����Ҫ�Ķ�����������
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\a101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\a199.bmp")        ////////////////////////////////////////////////ͼ����е�ͼ�񣨿���Ҫ�޸ģ�
		{
			fruitCog[0]++; //��ݮ
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\b101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\b199.bmp")        
		{
			fruitCog[1]++;  //�㽶
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\c101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\c199.bmp")        
		{
			fruitCog[2]++;  //⨺���
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\d101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\d199.bmp")        
		{
			fruitCog[3]++;  //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\e101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\e199.bmp")        
		{
			fruitCog[4]++; //ˮ������
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\f101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\f199.bmp")        
		{
			fruitCog[5]++; //ƻ��
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\g101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\g199.bmp")        
		{
			fruitCog[6]++; //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\h101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\h199.bmp")        
		{
			fruitCog[7]++; //��
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\i101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\i199.bmp")        
		{
			fruitCog[8]++; //�����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\j101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\j199.bmp")        
		{
			fruitCog[9]++; //â��
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\k101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\k199.bmp")        
		{
			fruitCog[10]++; //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\l101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\l199.bmp")        
		{
			fruitCog[11]++; //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\m101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\m199.bmp")        
		{
			fruitCog[12]++; //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\n101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\n199.bmp")        
		{
			fruitCog[13]++; //������
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\o101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\o199.bmp")        
		{
			fruitCog[14]++; //ɽ��
		}
	}               //�õ�ǰ5����ƥ����
	//�ҵ�����һ�������±�
	int find=-1;
	for (i=0;i<kinds;i++)
	{
		if (fruitCog[i]>fruitCog[find])             //���Ǵ��ڵ���   �±�С�����ȼ���
		{
			find=i;
		}
	}
	UpdateData();
	switch (find)
	{
	case 0:
		m_RcgResult="��ݮ";
		break;
	case 1:
		m_RcgResult="�㽶";
		break;
	case 2:
		m_RcgResult="⨺���";
		break;
	case 3:
		m_RcgResult="����";
		break;
	case 4:
		m_RcgResult="ˮ������";
		break;
	case 5:
		m_RcgResult="ƻ��";
		break;
	case 6:
		m_RcgResult="����";
		break;
	case 7:
		m_RcgResult="��";
		break;
	case 8:
		m_RcgResult="�����";
		break;
	case 9:
		m_RcgResult="â��";
		break;
	case 10:
		m_RcgResult="����";
		break;
	case 11:
		m_RcgResult="����";
		break;
	case 12:
		m_RcgResult="����";
		break;
	case 13:
		m_RcgResult="������";
		break;
	case 14:
		m_RcgResult="ɽ��";
		break;
	default:
		m_RcgResult="failed";
		break;
	}
	UpdateData(false);
}


void CFruitClassificationDlg::OnColorHist()  //����ͼ���������ͼ�����ɫֱ��ͼ 
{
	// TODO: Add your control notification handler code here
	int i=0;
	for (i=0;i<fileNum;i++)
	{
		ImgAlldib.LoadFile(filenameALL[i]);
		CalHistogram(ImgAlldib);
		ofstream out("F://��ҵ���//codes//FruitClassification//RGBall.txt",ios::app);         //����׷�ӣ���������������
		for (int j=0;j<256;j++)
		{
			out<<ImgAlldib.R[j]<<" "<<ImgAlldib.G[j]<<" "<<ImgAlldib.B[j]<<" ";
		}	
		out.close();
	}
	MessageBox("�����ļ�������");
	
}

void CFruitClassificationDlg::OnColor()   //������ɫ����ʶ��
{
	// TODO: Add your control notification handler code here
	m_RcgResult="";
	
	ifstream read("F://��ҵ���//codes//FruitClassification//RGBall.txt");//��ͼ���������ͼ�����ɫֱ��ͼ �ļ�
	CalHistogram(dib);               //��ʶ���ͼ��

	int i=0;
	for (i=0;i<fileNum;i++)
	{
		double sum=0;
		double r,g,b;
		for (int j=0;j<256;j++)
		{
			read>>r>>g>>b;
			sum+=pow((dib.R[j]-r),2);
			sum+=pow((dib.G[j]-g),2);
		    sum+=pow((dib.B[j]-b),2);
		}
		EDALLcolor[i]=sqrt(sum);                         //��������е�ŷʽ����������������
	}
	read.close();	

	//�б�
		//ȡǰ5�� ��ȡ�ļ����� �����б�ÿ��������һ��   �����⣺��β���ԭ�ȵ�ͼƬ��-----������

	for (i=0;i<fileNum;i++)
	{
		tmpF1[i]=EDALLcolor[i];      //��δ����֮ǰ�ĸ�ֵ  ���ں����ж����ĸ�ͼ��
	}
	Qsort(EDALLcolor,0,fileNum-1);  //��EDALLcolor[i]��������
	string pipei[5];        //��¼ǰ5��ƥ�������
	bool chongfu[900];      //���ܶ��ƥ��ͬһ��ͼƬ
	for (i=0;i<fileNum;i++)
	{
		chongfu[i]=false;
	}
	//�����б�ǰ5��������
	for (i=0;i<fileNum;i++)
	{
		if (EDALLcolor[0]==tmpF1[i] && chongfu[i]!=true)
		{
			pipei[0]=filenameALL[i];             //�õ���1��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}
	for (i=0;i<fileNum;i++)
	{
		if (EDALLcolor[1]==tmpF1[i] && chongfu[i]!=true)
		{
			pipei[1]=filenameALL[i];             //�õ���2��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}
	for (i=0;i<fileNum;i++)
	{
		if (EDALLcolor[2]==tmpF1[i] && chongfu[i]!=true)
		{
			pipei[2]=filenameALL[i];             //�õ���3��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}
	for (i=0;i<fileNum;i++)
	{
		if (EDALLcolor[3]==tmpF1[i] && chongfu[i]!=true)
		{
			pipei[3]=filenameALL[i];             //�õ���4��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}
	for (i=0;i<fileNum;i++)
	{
		if (EDALLcolor[4]==tmpF1[i] && chongfu[i]!=true)
		{
			pipei[4]=filenameALL[i];             //�õ���5��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}

	const int kinds=15;
	int fruitCog[kinds];           //ÿ��ˮ�����ʶ��ˮ����ƥ����
	for (i=0;i<kinds;i++)
	{
		fruitCog[i]=0;
	}
	for (i=0;i<5;i++)
	{
		//ע��˴���·����Ҫȫ ��ͼ���·����Ҫ�Ķ�����������
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\a101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\a199.bmp")        ////////////////////////////////////////////////ͼ����е�ͼ�񣨿���Ҫ�޸ģ�
		{
			fruitCog[0]++; //��ݮ
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\b101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\b199.bmp")        
		{
			fruitCog[1]++;  //�㽶
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\c101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\c199.bmp")        
		{
			fruitCog[2]++;  //⨺���
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\d101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\d199.bmp")        
		{
			fruitCog[3]++;  //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\e101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\e199.bmp")        
		{
			fruitCog[4]++; //ˮ������
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\f101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\f199.bmp")        
		{
			fruitCog[5]++; //ƻ��
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\g101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\g199.bmp")        
		{
			fruitCog[6]++; //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\h101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\h199.bmp")        
		{
			fruitCog[7]++; //��
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\i101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\i199.bmp")        
		{
			fruitCog[8]++; //�����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\j101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\j199.bmp")        
		{
			fruitCog[9]++; //â��
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\k101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\k199.bmp")        
		{
			fruitCog[10]++; //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\l101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\l199.bmp")        
		{
			fruitCog[11]++; //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\m101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\m199.bmp")        
		{
			fruitCog[12]++; //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\n101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\n199.bmp")        
		{
			fruitCog[13]++; //������
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\o101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\o199.bmp")        
		{
			fruitCog[14]++; //ɽ��
		}
	}               //�õ�ǰ5����ƥ����
	//�ҵ�����һ�������±�
	int find=-1;
	for (i=0;i<kinds;i++)
	{
		if (fruitCog[i]>fruitCog[find])             //���Ǵ��ڵ���   �±�С�����ȼ���
		{
			find=i;
		}
	}
	UpdateData();
	switch (find)
	{
	case 0:
		m_RcgResult="��ݮ";
		break;
	case 1:
		m_RcgResult="�㽶";
		break;
	case 2:
		m_RcgResult="⨺���";
		break;
	case 3:
		m_RcgResult="����";
		break;
	case 4:
		m_RcgResult="ˮ������";
		break;
	case 5:
		m_RcgResult="ƻ��";
		break;
	case 6:
		m_RcgResult="����";
		break;
	case 7:
		m_RcgResult="��";
		break;
	case 8:
		m_RcgResult="�����";
		break;
	case 9:
		m_RcgResult="â��";
		break;
	case 10:
		m_RcgResult="����";
		break;
	case 11:
		m_RcgResult="����";
		break;
	case 12:
		m_RcgResult="����";
		break;
	case 13:
		m_RcgResult="������";
		break;
	case 14:
		m_RcgResult="ɽ��";
		break;
	default:
		m_RcgResult="failed";
		break;
	}
	UpdateData(false);
}

void CFruitClassificationDlg::CalHistogram(CDib &dib)
{
	// ѭ������
	int i;
	int j;
	// �Ҷȼ���
	int huiduR[256],huiduB[256],huiduG[256];    
	int wide,height;    //ԭͼ������
	// ������ʼ��
	memset(huiduR,0,sizeof(huiduR));
	memset(huiduG,0,sizeof(huiduR));
	memset(huiduB,0,sizeof(huiduR));
	wide=dib.GetDibWidthBytes();
	height=dib.GetHeight ();
	LPBYTE  temp1=new BYTE[wide*height];    //��ͼ�񻺳���
	//����ԭͼ�񵽻���ͼ��
	memcpy(temp1,dib.m_pData,wide*height );
	// �Ը����ؽ��лҶ�ͳ��
	for (i = 0; i < height; i ++)
	{
		for (j = 2; j <wide; j=j +3)
		{
			unsigned char  tempB = temp1[wide* i + j] ;
			// �Ҷ�ͳ�Ƽ���
			huiduB[tempB]++;		
		}
		
	}
	for (i = 0; i < height; i ++)
	{
		for (j = 1; j <wide; j=j +3)
		{
			unsigned char  tempG = temp1[wide* i + j] ;
			// �Ҷ�ͳ�Ƽ���
			huiduG[tempG]++;		
		}
	}
	for (i = 0; i < height; i ++)
	{
		for (j = 0; j <wide; j=j +3)
		{
			unsigned char  tempR = temp1[wide* i + j] ;
			// �Ҷ�ͳ�Ƽ���
			huiduR[tempR]++;		
		}
	}


	// ����Ҷȷֲ��ܶ�
	for(i=0;i<256;i++)
	{
		dib.R[i] = huiduR[i] / (height * wide *1.0f);
		dib.B[i] = huiduB[i] / (height * wide *1.0f);
		dib.G[i] = huiduG[i] / (height * wide *1.0f);
		
	}
	
}

double CFruitClassificationDlg::GetED(CDib &dib1, CDib &dib2)
{
	CalHistogram(dib1);  CalHistogram(dib2);
	double sum=0;
	for (int i=0;i<256;i++)
	{
		sum+=pow((dib1.R[i]-dib2.R[i]),2);
		sum+=pow((dib1.G[i]-dib2.G[i]),2);
		sum+=pow((dib1.B[i]-dib2.B[i]),2);
	}
	return (sqrt(sum));
}

void CFruitClassificationDlg::OnWave() 
{
	// TODO: Add your control notification handler code here
		m_RcgResult="";
	int p=0;
	for (p=0;p<900;p++)
	{
		edAll[p]=0;            //��ʼ��
	}
	
	double tmpED=0;
	for (p=0;p<fileNum;p++)
	{
		tmpED+=pow(featureAll[p][0]-Iavg,2);
		tmpED+=pow(featureAll[p][1]-Isd,2);
		edAll[p]=sqrt(tmpED);                           //���EDALL
		tmpED=0;
	}
	
	for (p=0;p<fileNum;p++)
	{
		featureMerge[p][1]=edAll[p];            //С���������︳ֵ�Ͳ����ٶ��ļ���
	}
	/*****************��ȡ�ļ� �õ�����������ŷ�Ͼ��룩**************************/  //�ļ�����Ϊ˫б��
 	int i=0;
	//����С���任��ŷ�Ͼ����Ѿ���ǰ�渳ֵ�����Բ����ٶ�ȡ�ļ���

	for(i=0;i<fileNum;i++){
		f[i]=0;
	}

	for (i=0;i<fileNum;i++)
	{
		/**********ֻ��С��**********/
		f[i]=featureMerge[i][1];      //Ч������
	}

	//ȡǰ5�� ��ȡ�ļ����� �����б�ÿ��������һ��   �����⣺��β���ԭ�ȵ�ͼƬ��-----������
	double tmpF[900];
	for (i=0;i<fileNum;i++)
	{
		tmpF[i]=f[i];      //��δ����֮ǰ�ĸ�ֵ  ���ں����ж����ĸ�ͼ��
	}
	Qsort(f,0,fileNum-1);  //��f[i]��������
	string pipei[5];        //��¼ǰ5��ƥ�������
	bool chongfu[900];      //���ܶ��ƥ��ͬһ��ͼƬ
	for (i=0;i<fileNum;i++)
	{
		chongfu[i]=false;
	}
	//�����б�ǰ5��������
	for (i=0;i<fileNum;i++)
	{
		if (f[0]==tmpF[i] && chongfu[i]!=true)
		{
			pipei[0]=filenameALL[i];             //�õ���1��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}
	for (i=0;i<fileNum;i++)
	{
		if (f[1]==tmpF[i] && chongfu[i]!=true)
		{
			pipei[1]=filenameALL[i];             //�õ���2��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}
	for (i=0;i<fileNum;i++)
	{
		if (f[2]==tmpF[i] && chongfu[i]!=true)
		{
			pipei[2]=filenameALL[i];             //�õ���3��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}
	for (i=0;i<fileNum;i++)
	{
		if (f[3]==tmpF[i] && chongfu[i]!=true)
		{
			pipei[3]=filenameALL[i];             //�õ���4��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}
	for (i=0;i<fileNum;i++)
	{
		if (f[4]==tmpF[i] && chongfu[i]!=true)
		{
			pipei[4]=filenameALL[i];             //�õ���5��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}

	const int kinds=15;
	int fruitCog[kinds];           //ÿ��ˮ�����ʶ��ˮ����ƥ����
	for (i=0;i<kinds;i++)
	{
		fruitCog[i]=0;
	}
	for (i=0;i<5;i++)
	{
		//ע��˴���·����Ҫȫ ��ͼ���·����Ҫ�Ķ�����������
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\a101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\a199.bmp")        ////////////////////////////////////////////////ͼ����е�ͼ�񣨿���Ҫ�޸ģ�
		{
			fruitCog[0]++; //��ݮ
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\b101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\b199.bmp")        
		{
			fruitCog[1]++;  //�㽶
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\c101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\c199.bmp")        
		{
			fruitCog[2]++;  //⨺���
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\d101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\d199.bmp")        
		{
			fruitCog[3]++;  //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\e101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\e199.bmp")        
		{
			fruitCog[4]++; //ˮ������
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\f101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\f199.bmp")        
		{
			fruitCog[5]++; //ƻ��
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\g101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\g199.bmp")        
		{
			fruitCog[6]++; //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\h101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\h199.bmp")        
		{
			fruitCog[7]++; //��
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\i101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\i199.bmp")        
		{
			fruitCog[8]++; //�����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\j101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\j199.bmp")        
		{
			fruitCog[9]++; //â��
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\k101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\k199.bmp")        
		{
			fruitCog[10]++; //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\l101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\l199.bmp")        
		{
			fruitCog[11]++; //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\m101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\m199.bmp")        
		{
			fruitCog[12]++; //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\n101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\n199.bmp")        
		{
			fruitCog[13]++; //������
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\o101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\o199.bmp")        
		{
			fruitCog[14]++; //ɽ��
		}
	}               //�õ�ǰ5����ƥ����
	//�ҵ�����һ�������±�
	int find=-1;
	for (i=0;i<kinds;i++)
	{
		if (fruitCog[i]>fruitCog[find])             //���Ǵ��ڵ���   �±�С�����ȼ���
		{
			find=i;
		}
	}
	UpdateData();
	switch (find)
	{
	case 0:
		m_RcgResult="��ݮ";
		break;
	case 1:
		m_RcgResult="�㽶";
		break;
	case 2:
		m_RcgResult="⨺���";
		break;
	case 3:
		m_RcgResult="����";
		break;
	case 4:
		m_RcgResult="ˮ������";
		break;
	case 5:
		m_RcgResult="ƻ��";
		break;
	case 6:
		m_RcgResult="����";
		break;
	case 7:
		m_RcgResult="��";
		break;
	case 8:
		m_RcgResult="�����";
		break;
	case 9:
		m_RcgResult="â��";
		break;
	case 10:
		m_RcgResult="����";
		break;
	case 11:
		m_RcgResult="����";
		break;
	case 12:
		m_RcgResult="����";
		break;
	case 13:
		m_RcgResult="������";
		break;
	case 14:
		m_RcgResult="ɽ��";
		break;
	default:
		m_RcgResult="failed";
		break;
	}
	UpdateData(false);
}

void CFruitClassificationDlg::OnColorCoOccur()           //����ɫ�ʹ�������ʶ��
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_DRAW);	// ���IDC_DRAW�ؼ�����ָ��
	pWnd->UpdateWindow();	  
	HWND hwnd=pWnd->GetSafeHwnd();// ȡ�ÿؼ��ľ��
	HDC pDC=::GetDC(hwnd);
	cooccur.RGBtoColorType(pDC,filename);
	cooccur.calCoOccurMatrix();
	cooccur.calMatrixFeature();
	int m=0;
	double featureOne[8];        //��Ŵ�ʶ��ͼ���8������
	for(int x=0;x<4;x++)
	{
		for(int y=0;y<2;y++)
		{
			featureOne[m++]=cooccur.feature[x][y];
		}
	}
	ifstream guiyi("F://��ҵ���//codes//FruitClassification//Guiyi16.txt");
	double tmpguiyi[16];
	for (m=0;m<16;m++)
	{
		guiyi>>tmpguiyi[m];
	}
	guiyi.close();
	//����ʶ��ͼ����������й�һ��
	for (m=0;m<8;m++)
	{
		featureOne[m]=(featureOne[m]-tmpguiyi[m*2])/(tmpguiyi[m*2+1]-tmpguiyi[m*2]);
	}

	ifstream read("F://��ҵ���//codes//FruitClassification//colorCO.txt");
	double ed[900],edtmp[900];
	double t[8];
	for (m=0;m<900;m++)
	{
		ed[m]=0;
		edtmp[m]=0;
	}
	for (m=0;m<fileNum;m++)
	{
		for (int k=0;k<8;k++)
		{
			read>>t[k];
			ed[m]+=pow((featureOne[k]-t[k]),2);
		}
		ed[m]=sqrt(ed[m]);
		edtmp[m]=ed[m];
	}
	Qsort(ed,0,fileNum-1);
	int i=0;
	string pipei[5];        //��¼ǰ5��ƥ�������
	bool chongfu[900];      //���ܶ��ƥ��ͬһ��ͼƬ
	for (i=0;i<fileNum;i++)
	{
		chongfu[i]=false;
	}
	//�����б�ǰ5��������
	for (i=0;i<fileNum;i++)
	{
		if (ed[0]==edtmp[i] && chongfu[i]!=true)
		{
			pipei[0]=filenameALL[i];             //�õ���1��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}
	for (i=0;i<fileNum;i++)
	{
		if (ed[1]==edtmp[i] && chongfu[i]!=true)
		{
			pipei[1]=filenameALL[i];             //�õ���2��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}
	for (i=0;i<fileNum;i++)
	{
		if (ed[2]==edtmp[i] && chongfu[i]!=true)
		{
			pipei[2]=filenameALL[i];             //�õ���3��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}
	for (i=0;i<fileNum;i++)
	{
		if (ed[3]==edtmp[i] && chongfu[i]!=true)
		{
			pipei[3]=filenameALL[i];             //�õ���4��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}
	for (i=0;i<fileNum;i++)
	{
		if (ed[4]==edtmp[i] && chongfu[i]!=true)
		{
			pipei[4]=filenameALL[i];             //�õ���5��ƥ�������
			chongfu[i]=true;
			break;
		}
		else
			continue;
	}

	const int kinds=15;
	int fruitCog[kinds];           //ÿ��ˮ�����ʶ��ˮ����ƥ����
	for (i=0;i<kinds;i++)
	{
		fruitCog[i]=0;
	}
	for (i=0;i<5;i++)
	{
		//ע��˴���·����Ҫȫ ��ͼ���·����Ҫ�Ķ�����������
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\a101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\a199.bmp")        ////////////////////////////////////////////////ͼ����е�ͼ�񣨿���Ҫ�޸ģ�
		{
			fruitCog[0]++; //��ݮ
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\b101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\b199.bmp")        
		{
			fruitCog[1]++;  //�㽶
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\c101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\c199.bmp")        
		{
			fruitCog[2]++;  //⨺���
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\d101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\d199.bmp")        
		{
			fruitCog[3]++;  //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\e101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\e199.bmp")        
		{
			fruitCog[4]++; //ˮ������
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\f101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\f199.bmp")        
		{
			fruitCog[5]++; //ƻ��
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\g101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\g199.bmp")        
		{
			fruitCog[6]++; //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\h101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\h199.bmp")        
		{
			fruitCog[7]++; //��
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\i101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\i199.bmp")        
		{
			fruitCog[8]++; //�����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\j101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\j199.bmp")        
		{
			fruitCog[9]++; //â��
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\k101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\k199.bmp")        
		{
			fruitCog[10]++; //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\l101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\l199.bmp")        
		{
			fruitCog[11]++; //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\m101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\m199.bmp")        
		{
			fruitCog[12]++; //����
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\n101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\n199.bmp")        
		{
			fruitCog[13]++; //������
		}
		if (pipei[i]>="F:\\��ҵ���\\codes\\fruit\\o101.bmp" && pipei[i]<="F:\\��ҵ���\\codes\\fruit\\o199.bmp")        
		{
			fruitCog[14]++; //ɽ��
		}
	}               //�õ�ǰ5����ƥ����
	//�ҵ�����һ�������±�
	int find=-1;
	for (i=0;i<kinds;i++)
	{
		if (fruitCog[i]>fruitCog[find])             //���Ǵ��ڵ���   �±�С�����ȼ���
		{
			find=i;
		}
	}
	UpdateData();
	switch (find)
	{
	case 0:
		m_RcgResult="��ݮ";
		break;
	case 1:
		m_RcgResult="�㽶";
		break;
	case 2:
		m_RcgResult="⨺���";
		break;
	case 3:
		m_RcgResult="����";
		break;
	case 4:
		m_RcgResult="ˮ������";
		break;
	case 5:
		m_RcgResult="ƻ��";
		break;
	case 6:
		m_RcgResult="����";
		break;
	case 7:
		m_RcgResult="��";
		break;
	case 8:
		m_RcgResult="�����";
		break;
	case 9:
		m_RcgResult="â��";
		break;
	case 10:
		m_RcgResult="����";
		break;
	case 11:
		m_RcgResult="����";
		break;
	case 12:
		m_RcgResult="����";
		break;
	case 13:
		m_RcgResult="������";
		break;
	case 14:
		m_RcgResult="ɽ��";
		break;
	default:
		m_RcgResult="failed";
		break;
	}
	UpdateData(false);
}

void CFruitClassificationDlg::OnCOcolorALL() 
{
	// TODO: Add your control notification handler code here
	double featureAll[900][8];               //ͼ�����ÿ��ͼ��� 4����������*2������ֵ ���
	int i=0,j=0;
	for (i=0;i<fileNum;i++)
	{
		cooccurFile.LoadFile(filenameALL[i]);
		CWnd* pWnd = GetDlgItem(IDC_DRAW);	// ���IDC_DRAW�ؼ�����ָ��
		pWnd->UpdateWindow();	  
		HWND hwnd=pWnd->GetSafeHwnd();// ȡ�ÿؼ��ľ��
		HDC pDC=::GetDC(hwnd);
		cooccurFile.RGBtoColorType(pDC,filenameALL[i]);	
		cooccurFile.calCoOccurMatrix();
		cooccurFile.calMatrixFeature();
		int m=0;
		for(int x=0;x<4;x++)
		{
			for(int y=0;y<2;y++)
			{
				featureAll[i][m++]=cooccurFile.feature[x][y];
			}
		}
	}

	//�������СֵΪ��һ����׼��
	double guiyi[16];   //�ȴ���С �ٴ����     ����ֵ ���� * 4����ͬ������
	//��0- 0\1    1-2\3  2-4\5  ***** ��
	for (i=0;i<8;i++)
	{
		guiyi[i*2]=featureAll[0][i];
		guiyi[i*2+1]=featureAll[0][i];
	}      //��ʼ��
	for (i=1;i<fileNum;i++)
	{
		for (j=0;j<8;j++)
		{
			if (featureAll[i][j]<guiyi[j*2])
			{
				guiyi[j*2]=featureAll[i][j];
			}
			if (featureAll[i][j]>guiyi[2*j+1])
			{
				guiyi[2*j+1]=featureAll[i][j];
			}
		}
	}
	//����һ����д���ļ� �Ա��ʶ��ͼ����й�һ��
	ofstream gui("F://��ҵ���//codes//FruitClassification//Guiyi16.txt");
	for (i=0;i<16;i++)
	{
		gui<<guiyi[i]<<endl;
	}
	gui.close();
	//��һ��
	ofstream cofile("F://��ҵ���//codes//FruitClassification//colorCO.txt");
	for (i=0;i<fileNum;i++)
	{
		for (j=0;j<8;j++)
		{
			featureAll[i][j]=(featureAll[i][j]-guiyi[j*2])/(guiyi[j*2+1]-guiyi[j*2]);
			cofile<<featureAll[i][j]<<" ";
		}
		cofile<<"\n";
	}
	cofile.close();
	MessageBox("�����ļ������ɡ�");
}
