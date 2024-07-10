// PassDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "PassDlg.h"
#include "PassWd.h"


// CPassDlg dialog
IMPLEMENT_DYNAMIC(CPassDlg, CDialog)
CPassDlg::CPassDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPassDlg::IDD, pParent)
{
}

CPassDlg::~CPassDlg()
{
}

void CPassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editPass);
}


BEGIN_MESSAGE_MAP(CPassDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnClickedNewPassword)
	ON_BN_CLICKED(IDCANCEL, OnClickedCancel)
END_MESSAGE_MAP()

// static value 2008/01
time_t	CPassDlg::tUnlock = 0;		// unlock elapse time
int		CPassDlg::nTernSec = 0;		// unlock term (sec)
BOOL	CPassDlg::isUnlock = FALSE;	// unlock status


///////////////////////////////////////////////////////////////////////////////
// CPassDlg message handler

// clicked new password button.
void CPassDlg::OnClickedNewPassword()
{
	CNewPass	dlg;

	dlg.DoModal();
}

// clicked cancel button.
void CPassDlg::OnClickedCancel()
{
	OnCancel();
}

// clicked OK button.
void CPassDlg::OnOK()
{
	TCHAR	str[256];
	TCHAR	path[_MAX_PATH];
	CPassWd	pw;

	_tcscpy(path, theApp.m_cnf.WorkRootDir);
	_tcscat(path, _T("/../Config/passwd"));
	pw.SetPassPath(path);
	m_editPass.GetWindowText(str, 255);
	if( pw.Compare((BYTE *)str, sizeof(TCHAR)*_tcslen(str)) )
	{
		// 2008/01 set unlock time
		SetUnlockTime(time(NULL));

		CDialog::OnOK();
		return;
	}

	AfxMessageBox(_T("パスワードが間違っています\nもう一度入力してください"));
	OnCancel();
}

BOOL CPassDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN && 
		pMsg->wParam == VK_RETURN )
	{
		OnOK();
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

// initialize dialog.
BOOL CPassDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_editPass.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
}

// setting pass valid time (sec)
void CPassDlg::SetPassTime(int nSec)
{
	nTernSec = nSec;
}

// setting unlock time
void CPassDlg::SetUnlockTime(time_t tETime)
{
	tUnlock = tETime;
	isUnlock = TRUE;
}

// password valid check
BOOL CPassDlg::CheckValidPass(void)
{
	time_t	ct = time(NULL);
	int		nPastTime;

	// if unlock?
	if( isUnlock )
	{
		// calc past time
		nPastTime = (int)(ct - tUnlock);

		// check time
		if( nPastTime > nTernSec )
			isUnlock = FALSE;
	}

	return isUnlock;
}

