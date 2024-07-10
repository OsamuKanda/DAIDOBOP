// Chart3DDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "OnlinePCDlg.h"
#include "CloseDlg.h"
#include "Chart3D2.h"
#include "Chart3D2If.h"

static CConfig		&cnf	= theApp.m_cnfBoexe;

// CChart3DDlg ダイアログ

IMPLEMENT_DYNAMIC(CChart3DDlg, CWinFormsDialog<Chart3D::Chart3DControl>)

CChart3DDlg::CChart3DDlg(CWnd* pParent, BOOL isCloseBtn)
{
	m_isCloseBtn = isCloseBtn;
	m_pDlgTcView = NULL;
	m_pDlgClose = NULL;
	memset(m_iData, 0, sizeof(m_iData));

	// create dialog.
	Create(CChart3DDlg::IDD, pParent);				
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
}

CChart3DDlg::~CChart3DDlg()
{
}

void CChart3DDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChart3DDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_MESSAGE(WM_CHARTDATA, OnAddone)
	ON_COMMAND(IDM_TCPOSITION, OnTcposition)
	ON_WM_SHOWWINDOW()
	ON_WM_MOVE()
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CChart3dDlg メッセージ ハンドラ

//It evades ending by Enter Key.
void CChart3DDlg::OnOk(){}

//It evades ending by ESC Key.
void CChart3DDlg::OnCancel(){}

void CChart3DDlg::PostNcDestroy() 
{
	delete this;
//	CDialog::PostNcDestroy();
}

// WM_MOVE
void CChart3DDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

#ifdef FIX_SCREEN
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
#endif
}

// ダイアログの初期化
BOOL CChart3DDlg::OnInitDialog() 
{
	CWinFormsDialog<Chart3D::Chart3DControl>::OnInitDialog();
	
	// SYSTEMメニュー削除
	::SetWindowLong(this->GetSafeHwnd(), GWL_STYLE, WS_POPUP);
	ModifyStyle(WS_SYSMENU, 0, 0);

	// メニュー追加
    m_menu.LoadMenu( IDR_CHART3D_MENU );
    SetMenu( &m_menu );

	// 閉じるボタン表示
	if( m_isCloseBtn )
	{
		m_pDlgClose = new CCloseDlg(this);
		m_pDlgClose->SetWindowPos( &wndTop, 1000, 900, 0, 0, SWP_NOSIZE );
		m_pDlgClose->ShowWindow(SW_HIDE);
	}

	return TRUE;
}

// WM_SHOWWINDOW proc.
void CChart3DDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// show? 
	if( bShow )
	{
		// グラフコントロールにアクセス
		Chart3D::Chart3DControl	^cc = (Chart3D::Chart3DControl	^)GetControl();

		// TcNameの再読み込み
		cc->chgTcName();

		// 閉じるボタン表示
		if( m_isCloseBtn )
			m_pDlgClose->ShowWindow(SW_SHOW);
	}
	// hide?
	else
	{
		// 閉じるボタン表示
		if( m_isCloseBtn )
			m_pDlgClose->ShowWindow(SW_HIDE);
	}
}

// selected 'TC Position' menu.
void CChart3DDlg::OnTcposition()
{
	if( m_pDlgTcView == NULL )
	{
		m_pDlgTcView = new CTcVwDlg(this);
	}
	m_pDlgTcView->ShowWindow(SW_SHOW);
}

//
CDialog *GetChart3DDlg(CDialog *parent, BOOL isCloseBtn)
{
	return new CChart3DDlg(parent, isCloseBtn);
}

// tcno->XYNo変換テーブル
static int tc2xy[28][2] = 
{
	{0,	0},
	{0, 1},
	{1,	0},
	{1, 1},
	{2,	0},
	{2, 1},
	{3,	0},
	{3,	1},
	{3, 2},
	{4,	0},
	{4, 1},
	{5,	0},
	{5,	1},
	{5, 2},
	{6,	0},
	{6, 1},
	{7,	0},
	{7, 1},
	{8,	0},
	{8, 1},
	{9,	0},
	{9,	1},
	{9, 2},
	{10,0},	
	{10,1},
	{11,0},	
	{11,1},	
	{11,2}
};

LRESULT CChart3DDlg::OnAddone(WPARAM, LPARAM lParam) 
{
	int		nData[NumADChannel];
	int		nDataEx[4];
	int		i, ch;

	// データ取得
	memcpy(nData, (int *)lParam, sizeof(nData));

	// データシフト
	MoveMemory( &m_iData[0][1][0], &m_iData[0][0][0], sizeof(m_iData[0]) - sizeof(m_iData[0][0]));
	MoveMemory( &m_iData[1][1][0], &m_iData[1][0][0], sizeof(m_iData[1]) - sizeof(m_iData[1][0]));
	MoveMemory( &m_iData[2][1][0], &m_iData[2][0][0], sizeof(m_iData[2]) - sizeof(m_iData[2][0]));

	// 今回データ取得
	for( i = 0; i < 28; i++ )
	{
		m_iData[tc2xy[i][1]][0][tc2xy[i][0]] = nData[i];
	}

	// データの無いところの平均処理
	m_iData[2][0][10] = (m_iData[2][0][11]+m_iData[2][0][9])/2;
	m_iData[2][0][8] = 3*(m_iData[2][0][9]-m_iData[2][0][5])/4+m_iData[2][0][5];
	m_iData[2][0][7] = 2*(m_iData[2][0][9]-m_iData[2][0][5])/4+m_iData[2][0][5];
	m_iData[2][0][6] = 1*(m_iData[2][0][9]-m_iData[2][0][5])/4+m_iData[2][0][5];
	m_iData[2][0][4] = (m_iData[2][0][5]+m_iData[2][0][3])/2;
	m_iData[2][0][2]  = 3*(m_iData[2][0][3]-m_iData[2][0][11])/4+m_iData[2][0][11];
	m_iData[2][0][1] = 2*(m_iData[2][0][3]-m_iData[2][0][11])/4+m_iData[2][0][11];
	m_iData[2][0][0] = 1*(m_iData[2][0][3]-m_iData[2][0][11])/4+m_iData[2][0][11];

	// グラフコントロールにアクセス
	Chart3D::Chart3DControl	^cc = (Chart3D::Chart3DControl	^)GetControl();

	// 表示ラインのデータをセット
	cc->setData1((int*)m_iData[0]);
	cc->setData2((int*)m_iData[1]);
	cc->setData3((int*)m_iData[2]);

	// 温度以外のデータセット
	ch = cnf.inputPoint[LevelID].adCh;
	if( ch != -1 )
		nDataEx[0] = nData[ch];
	else
		nDataEx[0] = 0;
	ch = cnf.inputPoint[VcID].adCh;
	if( ch != -1 )
		nDataEx[1] = nData[ch];
	else
		nDataEx[1] = 0;
	ch = cnf.inputPoint[SNID].adCh;
	if( ch != -1 )
		nDataEx[2] = nData[ch];
	else
		nDataEx[2] = 0;
	ch = cnf.inputPoint[LengthID].adCh;
	if( ch != -1 )
		nDataEx[3] = nData[ch];
	else
		nDataEx[3] = 0;
	cc->setDataEx((int*)nDataEx);

	return 0;
}
