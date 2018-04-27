// DirDialog.h: interface for the CDirDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRDIALOG_H__4CBB8C94_0C5A_485C_9585_D7C464DDC1AE__INCLUDED_)
#define AFX_DIRDIALOG_H__4CBB8C94_0C5A_485C_9585_D7C464DDC1AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDirDialog  
{
public:
	static int __stdcall BrowseCtrlCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	bool DoBrowse(CWnd *pwndParent);
//	BOOL DoBrowse(CWnd *pwndParent);
	CDirDialog();
	virtual ~CDirDialog();
	CString m_strWindowTitle;
    CString m_strPath;
    CString m_strInitDir;
    CString m_strSelDir;
    CString m_strTitle;
    int  m_iImageIndex;
    BOOL m_bStatus;
private:
//	static int __stdcall BrowseCtrlCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	virtual BOOL SelChanged(LPCSTR lpcsSelection, CString& csStatusText)
		{ 	return TRUE; };

};

#endif // !defined(AFX_DIRDIALOG_H__4CBB8C94_0C5A_485C_9585_D7C464DDC1AE__INCLUDED_)
