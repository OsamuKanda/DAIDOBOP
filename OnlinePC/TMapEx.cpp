// TMapEx.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "TMapEx.h"


// CTMapEx ダイアログ
IMPLEMENT_DYNAMIC(CTMapEx, CDialog)

CTMapEx::CTMapEx(CDialog* pParent /*=NULL*/)
	: CDialog(CTMapEx::IDD, pParent)
{
	// create dialog.
	m_pParent = pParent;
	Create(CTMapEx::IDD, pParent);	
}

CTMapEx::~CTMapEx()
{
}

void CTMapEx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_scr);
	DDX_Control(pDX, IDC_CUR_TIME, m_txtCurTime);
}


BEGIN_MESSAGE_MAP(CTMapEx, CDialog)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CTMapEx::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTMapEx::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTMapEx::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTMapEx::OnBnClickedButton4)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CTMapEx メッセージ ハンドラ

//It evades ending by Enter Key.
void CTMapEx::OnOK(){}

//It evades ending by ESC Key.
void CTMapEx::OnCancel(){}

void CTMapEx::PostNcDestroy() 
{
	delete this;
//	CDialog::PostNcDestroy();
}

// initialize dialog.
BOOL CTMapEx::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}

// 閉じるボタン
void CTMapEx::OnBnClickedButton1()
{
	if( m_pParent != NULL )
		m_pParent->ShowWindow(SW_HIDE);
}

// 再生ボタン
void CTMapEx::OnBnClickedButton2()
{
	m_pParent->SendMessage(WM_RECSTART);
}

// 停止ボタン
void CTMapEx::OnBnClickedButton3()
{
	m_pParent->SendMessage(WM_RECSTOP);
}

// ﾊﾞｯﾁｼﾐｭﾚｰｼｮﾝ停止ボタン
void CTMapEx::OnBnClickedButton4()
{
	m_pParent->SendMessage(WM_BATCHSTOP);
}

// スクロール処理
void CTMapEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( &m_scr == pScrollBar )
	{
		m_pParent->SendMessage(WM_TMAP_SCR, nSBCode, nPos);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

// スクロールバーコントロールを取得
CScrollBar *GetScr(CDialog *pThis)
{
	CTMapEx	*pT = (CTMapEx *)pThis;
	return &pT->m_scr;
}

// テキストコントロールを取得
CStatic *GetText(CDialog *pThis)
{
	CTMapEx	*pT = (CTMapEx *)pThis;
	return &pT->m_txtCurTime;
}
