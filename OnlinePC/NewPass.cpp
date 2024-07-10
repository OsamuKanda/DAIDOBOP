// NewPass.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "NewPass.h"
#include "PassWd.h"


// CNewPass dialog

IMPLEMENT_DYNAMIC(CNewPass, CDialog)
CNewPass::CNewPass(CWnd* pParent /*=NULL*/)
	: CDialog(CNewPass::IDD, pParent)
{
}

CNewPass::~CNewPass()
{
}

void CNewPass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editPass);
	DDX_Control(pDX, IDC_EDIT2, m_editNewPass);
	DDX_Control(pDX, IDC_EDIT3, m_editNewPassAgain);
}


BEGIN_MESSAGE_MAP(CNewPass, CDialog)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CNewPass message handler

void CNewPass::OnOK()
{
	TCHAR	str1[256], str2[256] = {0}, str3[256] = {0};
	TCHAR	path[_MAX_PATH];
	CPassWd	pw;

	_tcscpy(path, theApp.m_cnf.WorkRootDir);
	_tcscat(path, _T("/../Config/passwd"));
	pw.SetPassPath(path);
	m_editPass.GetWindowText(str1, 255);
	m_editNewPass.GetWindowText(str2, 255);
	m_editNewPassAgain.GetWindowText(str3, 255);
	if( pw.Compare((BYTE *)str1, sizeof(TCHAR)*_tcslen(str1)) &&
		memcmp(str2, str3, sizeof(str2)) == 0 )
	{
		pw.SaveNew((BYTE *)str2, sizeof(TCHAR)*_tcslen(str2));
		CDialog::OnOK();
		return;
	}

	AfxMessageBox(_T("パスワードが間違っています\nもう一度入力してください"));
	OnCancel();
}

BOOL CNewPass::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN && 
		pMsg->wParam == VK_RETURN )
	{
		OnOK();
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

