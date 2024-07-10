// CloseDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "CloseDlg.h"


// CCloseDlg ダイアログ
IMPLEMENT_DYNAMIC(CCloseDlg, CDialog)

CCloseDlg::CCloseDlg(CDialog* pParent /*=NULL*/)
	: CDialog(CCloseDlg::IDD, pParent)
{
	// create dialog.
	m_pParent = pParent;
	Create(CCloseDlg::IDD, pParent);	
}

CCloseDlg::~CCloseDlg()
{
}

void CCloseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCloseDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CCloseDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CCloseDlg メッセージ ハンドラ

//It evades ending by Enter Key.
void CCloseDlg::OnOK(){}

//It evades ending by ESC Key.
void CCloseDlg::OnCancel(){}

void CCloseDlg::PostNcDestroy() 
{
	delete this;
//	CDialog::PostNcDestroy();
}

// initialize dialog.
BOOL CCloseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}

// 閉じるボタン
void CCloseDlg::OnBnClickedButton1()
{
	if( m_pParent != NULL )
		m_pParent->ShowWindow(SW_HIDE);
}
