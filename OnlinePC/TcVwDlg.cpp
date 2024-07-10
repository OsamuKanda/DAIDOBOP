// TcVwDlg.cpp : TC View Screen source file
//
#include "stdafx.h"					// MFCヘッダ
#include "OnlinePC.h"
#include "TcVwDlg.h"				// クラスヘッダ

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// definition
static TcName			*tcnm		= &theApp.m_stTcNm;


// デプレッション表示有無
//#define	DISP_DEP

//----------------
// 定数
//----------------

//#define SermoHigh		370
//#define SermoWidth	280
#define FixTapColumn	3
#define LooseTapColumn	3
#define BTapColumn		3
#define DTapColumn		3
//#define SermoHigh		(370*2)
//#define SermoWidth		480
#define SermoHigh		(500*2)
#define SermoWidth		640
#define ATapNum			8
#define LooseTapColumn	3
#define N1TapColumn		3
#define DTapNum			6

#define TapRaw			3
#define SermoSize		40
#define FixWidth		( SermoSize * (FixTapColumn + 1) ) 
#define LooseWidth		( SermoSize * (LooseTapColumn + 1) ) 
#define BWidth			( SermoSize * (BTapColumn + 1) + 3 ) 
#define DWidth			( SermoSize * (DTapColumn + 1) + 3 )
#define FixHigh			( SermoSize * (TapRaw + 1) + 25 )
#define	SpaceWidth		10
#define LooseHigh		FixHigh
#define BHigh			FixHigh
#define DHigh			FixHigh
#define BaseX			30
#define BaseY			50
#define FixBaseX		(BBaseX+BWidth+SpaceWidth)//150
#define FixBaseY		50
#define BBaseX			30
#define BBaseY			130
#define LooseBaseX		(BBaseX+BWidth+SpaceWidth)//150
#define LooseBaseY		210
#define DBaseX			(FixBaseX+FixWidth+SpaceWidth)
#define DBaseY			130
#define DepOffset		350

#define PalleteColumn	4
#define PalleteRaw		4
#define PalleteBaseX	10
#define PalleteBaseY	35
#define PalleteWidth	PalleteColumn * SermoSize + 2
#define PalleteHigh		PalleteRaw * SermoSize + 2

// reference data
static CConfig			&cnf		= theApp.m_cnfBoexe;
static CConfig			&cnfD		= theApp.m_cnfBoexeD;
static COLORREF			&msgBase	= theApp.m_col.clMsgBase;
static COLORREF			&moldColor	= theApp.m_col.clMoldColor;
static COLORREF			Black		= RGB(0,0,0);
static COLORREF			White		= RGB(255,255,255);
static COLORREF			*colorTone	= theApp.m_col.clTones;

// TC位置記憶エリア
CPoint	g_pt[NumTCTap];

/////////////////////////////////////////////////////////////////////////////
// CTcVwDlgクラス

//-----------------------------------------------------------------------------
// 名前	: CTcVwDlg
// 内容	: 構築（コンストラクタ）
//		: １．AFX_DATA_INITのコードはMFCが自動生成に使用するので触らない。
//		: ２．メンバ変数の初期化をここで行う。
// 引数	: pParent	... 親画面ポインタ
// 戻値	: なし
//-----------------------------------------------------------------------------
CTcVwDlg::CTcVwDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTcVwDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTcVwDlg)
	//}}AFX_DATA_INIT

	// メンバの初期化
	m_bFirst = FALSE;

	// create dialog.
	Create(CTcVwDlg::IDD, pParent);				
}

//-----------------------------------------------------------------------------
// 名前	: DoDataExchange
// 内容	: 画面データの交換と有効性チェック
//		: MFCが自動でコード改変する為、ほとんど触らない。
// 引数	: …
// 戻値	: なし
//-----------------------------------------------------------------------------
void CTcVwDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTcVwDlg)
	//}}AFX_DATA_MAP
}

//-----------------------------------------------------------------------------
// 名前	: メッセージマップ
// 内容	: メッセージハンドラの登録
//		: MFCが自動でコード改変する為、ほとんど触らない。
//-----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CTcVwDlg, CDialog)
	//{{AFX_MSG_MAP(CTcVwDlg)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTcVwDlgクラス メッセージ ハンドラ

//-----------------------------------------------------------------------------
// 名前	: OnOK、OnCancel、OnClose
// 内容	: モードレス画面定型コード
//		: OnOK		... リターンキーで画面が閉じるのを無効にする
//		: OnCancel	... ＥＳＣキーで画面が閉じるのを無効にする
//		: OnClose	... ×ボタン、Alt+F4で画面が閉じるのを無効にする
// 引数	: なし
// 戻値	: なし
//-----------------------------------------------------------------------------
void CTcVwDlg::OnOK() {}
void CTcVwDlg::OnCancel() {}
void CTcVwDlg::OnClose() 
{
	// 画面非表示
	ShowWindow(SW_HIDE);
}

//-----------------------------------------------------------------------------
// 名前	: PostNcDestroy
// 内容	: 画面が破棄された後に呼ばれる。明示的に破棄しない場合は、
//		: 親が破棄された時に自動で破棄される。
//		: １．生成したオブジェクト等の削除
//		: ２．本クラスはnewで生成されている為、deleteで破棄する。
//		:	　因みに破棄するのは画面ではなくクラスの実体（インスタンス）
// 引数	: なし
// 戻値	: なし
//-----------------------------------------------------------------------------
void CTcVwDlg::PostNcDestroy() 
{
	// インスタンス開放
	delete this;
}

//-----------------------------------------------------------------------------
// 名前	: OnInitDialog
// 内容	: 画面が破棄された後に呼ばれる。明示的に破棄しない場合は、
//		: 親が破棄された時に自動で破棄される。
//		: １．本クラスはnewで生成されている為、deleteで破棄する。
//		:	　因みに破棄するのは画面ではなくクラスの実体（インスタンス）
// 引数	: なし
// 戻値	: 正常：TRUE、異常：FALSE
//-----------------------------------------------------------------------------
BOOL CTcVwDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TC表示位置の設定
	g_pt[0]		= CPoint(DBaseX + 0 * SermoSize,	DBaseY + 0 * SermoSize);
	g_pt[1]		= CPoint(DBaseX + 0 * SermoSize,	DBaseY + 1 * SermoSize);
	g_pt[2]		= CPoint(DBaseX + 1 * SermoSize,	DBaseY + 0 * SermoSize);
	g_pt[3]		= CPoint(DBaseX + 1 * SermoSize,	DBaseY + 1 * SermoSize);
	g_pt[4]		= CPoint(DBaseX + 2 * SermoSize,	DBaseY + 0 * SermoSize);
	g_pt[5]		= CPoint(DBaseX + 2 * SermoSize,	DBaseY + 1 * SermoSize);

	g_pt[6]		= CPoint(FixBaseX + 0 * SermoSize,	FixBaseY + 0 * SermoSize);
	g_pt[7]		= CPoint(FixBaseX + 0 * SermoSize,	FixBaseY + 1 * SermoSize);
	g_pt[8]		= CPoint(FixBaseX + 0 * SermoSize,	FixBaseY + 2 * SermoSize);
	g_pt[9]		= CPoint(FixBaseX + 1 * SermoSize,	FixBaseY + 0 * SermoSize);
	g_pt[10]	= CPoint(FixBaseX + 1 * SermoSize,	FixBaseY + 1 * SermoSize);
	g_pt[11]	= CPoint(FixBaseX + 2 * SermoSize,	FixBaseY + 0 * SermoSize);
	g_pt[12]	= CPoint(FixBaseX + 2 * SermoSize,	FixBaseY + 1 * SermoSize);
	g_pt[13]	= CPoint(FixBaseX + 2 * SermoSize,	FixBaseY + 2 * SermoSize);

	g_pt[14]	= CPoint(BBaseX + 0 * SermoSize,	BBaseY + 0 * SermoSize);
	g_pt[15]	= CPoint(BBaseX + 0 * SermoSize,	BBaseY + 1 * SermoSize);
	g_pt[16]	= CPoint(BBaseX + 1 * SermoSize,	BBaseY + 0 * SermoSize);
	g_pt[17]	= CPoint(BBaseX + 1 * SermoSize,	BBaseY + 1 * SermoSize);
	g_pt[18]	= CPoint(BBaseX + 2 * SermoSize,	BBaseY + 0 * SermoSize);
	g_pt[19]	= CPoint(BBaseX + 2 * SermoSize,	BBaseY + 1 * SermoSize);

	g_pt[20]	= CPoint(LooseBaseX + 0 * SermoSize,	LooseBaseY + 0 * SermoSize);
	g_pt[21]	= CPoint(LooseBaseX + 0 * SermoSize,	LooseBaseY + 1 * SermoSize);
	g_pt[22]	= CPoint(LooseBaseX + 0 * SermoSize,	LooseBaseY + 2 * SermoSize);
	g_pt[23]	= CPoint(LooseBaseX + 1 * SermoSize,	LooseBaseY + 0 * SermoSize);
	g_pt[24]	= CPoint(LooseBaseX + 1 * SermoSize,	LooseBaseY + 1 * SermoSize);
	g_pt[25]	= CPoint(LooseBaseX + 2 * SermoSize,	LooseBaseY + 0 * SermoSize);
	g_pt[26]	= CPoint(LooseBaseX + 2 * SermoSize,	LooseBaseY + 1 * SermoSize);
	g_pt[27]	= CPoint(LooseBaseX + 2 * SermoSize,	LooseBaseY + 2 * SermoSize);

	return TRUE;
}

void CTcVwDlg::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	CRect	rect;

	if( !m_bFirst )
	{
		createTcViewFrame(dc);
		m_bFirst = TRUE;
	}
	
	CBitmap	*old;
	old = (CBitmap *)m_mdc.SelectObject(&m_bitmap);

	drawSermo(m_mdc);

	CRect	rc;
	GetClientRect(&rc);
	dc.BitBlt(0,0,rc.Width(),rc.Height(),&m_mdc,0,0,SRCCOPY);
	m_mdc.SelectObject(old);
}

///////////////////////////////////////////////////////////////////////////////
// ユーザー追加関数

// Tc View表示フレームを生成する。
void CTcVwDlg::createTcViewFrame(CDC &dc)
{
	CRect	rc;
	// clear memory plane.
	GetClientRect(rc);

	// create font for Point Info
	m_fontPointInfo.CreateFont(12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS,	_T("MS Shell Dlg") );

	// create bitmap
	m_mdc.CreateCompatibleDC(&dc);
	m_bitmap.CreateCompatibleBitmap(&dc, SermoWidth, SermoHigh);
}

// 
void CTcVwDlg::drawSermo(CDC& dc)
{
	int	getTempColor(int);
	SharedMemory *pShMem = GetSharedMemory();
	int	x, y, ch, nNumber;
	int	i;
	CFont *pFont;
	CRect rc;

	// erase all entry for BO.
	boxFillSolid(dc, msgBase, 0, 0, SermoWidth+50, SermoHigh+50);
#ifdef DISP_DEP
	PrintText(dc, 0, RGB(0,0,0), 10, 10, "Ｂ．Ｏ．側");
#endif
	boxFillSolid(dc, moldColor, BBaseX - 15, BBaseY - 30, BWidth, BHigh);
	PrintText(dc, 0, RGB(0,0,0), BBaseX + (BWidth / 2) - 20, BBaseY - 24, "B");
	boxFillSolid(dc, moldColor, FixBaseX - 15, FixBaseY - 30, FixWidth, FixHigh);
	PrintText(dc, 0, RGB(0,0,0), FixBaseX + (FixWidth / 2) - 20, FixBaseY - 24, "C");
	boxFillSolid(dc, moldColor, DBaseX - 15, DBaseY - 30, DWidth, DHigh);
	PrintText(dc, 0, RGB(0,0,0), DBaseX + (DWidth / 2) - 20, DBaseY - 24, "D");
	boxFillSolid(dc, moldColor, LooseBaseX - 15, LooseBaseY -30, LooseWidth, LooseHigh);
	PrintText(dc, 0, RGB(0,0,0), LooseBaseX + (LooseWidth / 2) - 20, LooseBaseY -30 + 5, "A");

#ifdef DISP_DEP
	// erase all entry for DEP.
	boxFillSolid(dc, msgBase, 0, DepOffset, SermoWidth+50, SermoHigh+50);
	PrintText(dc, 0, RGB(0,0,0), 10, DepOffset + 10, "デプレッション側");
	boxFillSolid(dc, moldColor, BBaseX - 15, DepOffset + BBaseY - 30, BWidth, BHigh);
	PrintText(dc, 0, RGB(0,0,0), BBaseX + (BWidth / 2) - 20, DepOffset + BBaseY - 24, "B");
	boxFillSolid(dc, moldColor, FixBaseX - 15, DepOffset + FixBaseY - 30, FixWidth, FixHigh);
	PrintText(dc, 0, RGB(0,0,0), FixBaseX + (FixWidth / 2) - 20, DepOffset + FixBaseY - 24, "C");
	boxFillSolid(dc, moldColor, DBaseX - 15, DepOffset + DBaseY - 30, DWidth, DHigh);
	PrintText(dc, 0, RGB(0,0,0), DBaseX + (DWidth / 2) - 20, DepOffset + DBaseY - 24, "D");
	boxFillSolid(dc, moldColor, LooseBaseX - 15, DepOffset + LooseBaseY -30, LooseWidth, LooseHigh);
	PrintText(dc, 0, RGB(0,0,0), LooseBaseX + (LooseWidth / 2) - 20, DepOffset + LooseBaseY -30 + 5, "A");
#endif

	nNumber = 1;

	// B.O.側　
	pFont = (CFont *)dc.SelectObject(&m_fontPointInfo);
	for(i = 0;i < NumTCTap;i++)
	{
		x = g_pt[nNumber-1].x;
		y = g_pt[nNumber-1].y;
		ch = cnf.inputPoint[nNumber - 1].adCh;
		if( ch > -1 )
			boxFillSolid(dc, colorTone[ch], x, y, SermoSize, SermoSize);
		else
			boxFillSolid(dc, White, x, y, SermoSize, SermoSize);
		box(dc, PS_SOLID, 1, Black, R2_COPYPEN, x, y, SermoSize, SermoSize);
//		PrintText(dc, &m_fontPointInfo, Black, x + 3, y + 6, "%s", tcnm->name[nNumber-1]);
		rc.left		= x;
		rc.top		= y;
		rc.right	= x + SermoSize;
		rc.bottom	= y + SermoSize;
		dc.DrawText(tcnm->name[nNumber-1], TCNM_MAX_LEN, &rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		nNumber++;
	}
	dc.SelectObject(pFont);

#ifdef DISP_DEP
	nNumber = 1;

	// ﾃﾞﾌﾟﾚｯｼｮﾝ側　
	for(i = 0;i < NumTCTap;i++)
	{
		x = g_pt[nNumber-1].x;
		y = g_pt[nNumber-1].y + DepOffset;
		ch = cnfD.inputPoint[nNumber - 1].adCh;
		if( ch > -1 )
			boxFillSolid(dc, colorTone[ch], x, y, SermoSize, SermoSize);
		else
			boxFillSolid(dc, White, x, y, SermoSize, SermoSize);
		box(dc, PS_SOLID, 1, Black, R2_COPYPEN, x, y, SermoSize, SermoSize);
		PrintText(dc, 0, Black, x + 3, y + 6, "#%2d", nNumber);
		nNumber++;
	}
#endif
}


