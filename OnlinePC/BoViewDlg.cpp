// BoViewDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "BoViewDlg.h"
#include ".\boviewdlg.h"

///////////////////////////////////////////////////////////////////////////////
// definition
static CConfig		&cnf	= theApp.m_cnfBoexe;
static CConfig		&cnfD	= theApp.m_cnfBoexeD;
static CChartLib	&clib1	= theApp.m_boviewChartFz;
static CChartLib	&clib2	= theApp.m_boviewChart5;
static CChartLib	&clib3	= theApp.m_boviewChart15;
static CChartLib	&clib4	= theApp.m_boviewChart30;
static CChartLib	&clib5	= theApp.m_boviewChart60;

#define	AXIS_WIDTH		40
#define	SCOOTER_WIDTH	16

// CBoViewDlg dialog
IMPLEMENT_DYNAMIC(CBoViewDlg, CDialog)
CBoViewDlg::CBoViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBoViewDlg::IDD, pParent)
{
	// initialize member data.
	m_nScPosX = 0;
	m_bScDrag = FALSE;
	m_pDlgTcView = NULL;
	m_nCurView = 1;

	memset(&m_ps, 0, sizeof(m_ps));

	int i;
	for (i = 0; i < TC_NO_MAX; i++) {
		m_tcg[i] = new CTcGraph(i);
	}

	//test
	extern COLORREF	colBack[];
	for( i = 0; i < PEN_NUM; i++ )
		m_ps.bEnbPen[i] = TRUE;

	for( i = 0; i < PEN_SET_NUM; i++ )
		m_ps.colPen[i] = colBack[i];
	m_ps.dMax[0] = 5;
	m_ps.dMax[1] = 100.0;
	m_ps.dMax[2] = 100.0;
	m_ps.dMax[3] = 150.0;
	m_ps.dMax[4] = 150.0;
	m_ps.dMin[0] = 0.0;
	m_ps.dMin[1] = 0.0;
	m_ps.dMin[2] = 0.0;
	m_ps.dMin[3] = 50.0;
	m_ps.dMin[4] = 50.0;
	m_ps.dUnit[0] = 0.01;
	m_ps.dUnit[1] = 1.0;
	m_ps.dUnit[2] = 0.1;
	m_ps.dUnit[3] = 0.01;
	m_ps.dUnit[4] = 0.1;
	_tcscpy(m_ps.szFmt[0], _T("%.2lf"));
	_tcscpy(m_ps.szFmt[1], _T("%.0lf"));
	_tcscpy(m_ps.szFmt[2], _T("%.1lf"));
	_tcscpy(m_ps.szFmt[3], _T("%.2lf"));
	_tcscpy(m_ps.szFmt[4], _T("%.1lf"));

	// create dialog.
	Create(CBoViewDlg::IDD, pParent);	
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
}

CBoViewDlg::~CBoViewDlg()
{
	int i;
	for (i = 0; i < TC_NO_MAX; i++)
		delete m_tcg[i];
}

void CBoViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listActData);
	DDX_Control(pDX, IDC_Y_SCALE, m_textYScale);
	DDX_Control(pDX, IDC_BUTTON2, m_btnFrz);
	DDX_Control(pDX, IDC_BUTTON3, m_btnMinute5);
	DDX_Control(pDX, IDC_BUTTON4, m_btnMinute15);
	DDX_Control(pDX, IDC_BUTTON5, m_btnMinute30);
	DDX_Control(pDX, IDC_BUTTON6, m_btnMinute60);
	//DDX_Control(pDX, IDC_TCY_SCALE1, m_textTcScale1);
	DDX_Control(pDX, IDC_TCY_SCALE2, m_textTcScale2);
	//DDX_Control(pDX, IDC_TCY_SCALE3, m_textTcScale3);
	//DDX_Control(pDX, IDC_TCY_SCALE4, m_textTcScale4);
	DDX_Control(pDX, IDC_TCY_SCALE5, m_textTcScale5);
	//DDX_Control(pDX, IDC_TCY_SCALE6, m_textTcScale6);
	DDX_Control(pDX, IDC_EDIT31, m_editScVal[0]);
	DDX_Control(pDX, IDC_EDIT32, m_editScVal[1]);
	DDX_Control(pDX, IDC_EDIT33, m_editScVal[2]);
	DDX_Control(pDX, IDC_EDIT34, m_editScVal[3]);
	DDX_Control(pDX, IDC_EDIT35, m_editScVal[4]);
	DDX_Control(pDX, IDC_EDIT36, m_editScVal[5]);
	DDX_Control(pDX, IDC_EDIT37, m_editScVal[6]);
//	DDX_Control(pDX, IDC_EDIT38, m_editScVal[7]);
//	DDX_Control(pDX, IDC_EDIT39, m_editScVal[8]);
	DDX_Control(pDX, IDC_STATIC_HIGH, m_stHigh);
	DDX_Control(pDX, IDC_STATIC_MIDDLE, m_stMiddle);
	DDX_Control(pDX, IDC_STATIC_LOW, m_stLow);
	DDX_Control(pDX, IDC_BUTTON25, m_btnOFF[0]);
	DDX_Control(pDX, IDC_BUTTON24, m_btnOFF[1]);
	DDX_Control(pDX, IDC_BUTTON23, m_btnOFF[2]);
	DDX_Control(pDX, IDC_BUTTON22, m_btnOFF[3]);
	DDX_Control(pDX, IDC_BUTTON21, m_btnOFF[4]);
	DDX_Control(pDX, IDC_BUTTON20, m_btnOFF[5]);
	DDX_Control(pDX, IDC_BUTTON19, m_btnOFF[6]);
	DDX_Control(pDX, IDC_BUTTON18, m_btnOFF[7]);
	DDX_Control(pDX, IDC_BUTTON17, m_btnOFF[8]);
	DDX_Control(pDX, IDC_BUTTON16, m_btnOFF[9]);
	DDX_Control(pDX, IDC_BUTTON15, m_btnOFF[10]);
	DDX_Control(pDX, IDC_BUTTON14, m_btnOFF[11]);
}

BEGIN_MESSAGE_MAP(CBoViewDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDC_BUTTON1, OnClickedButton1)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON2, OnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnClickedButton6)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//ON_BN_CLICKED(IDC_BUTTON11, OnClickedOffButton1)
	//ON_BN_CLICKED(IDC_BUTTON12, OnClickedOffButton2)
	//ON_BN_CLICKED(IDC_BUTTON13, OnClickedOffButton3)
	//ON_BN_CLICKED(IDC_BUTTON14, OnClickedOffButton4)
	//ON_BN_CLICKED(IDC_BUTTON15, OnClickedOffButton5)
	//ON_BN_CLICKED(IDC_BUTTON16, OnClickedOffButton6)
	//ON_BN_CLICKED(IDC_BUTTON17, OnClickedOffButton7)
	//ON_BN_CLICKED(IDC_BUTTON18, OnClickedOffButton8)
	//ON_BN_CLICKED(IDC_BUTTON19, OnClickedOffButton9)
	//ON_BN_CLICKED(IDC_BUTTON20, OnClickedOffButton10)
	//ON_BN_CLICKED(IDC_BUTTON21, OnClickedOffButton11)
	//ON_BN_CLICKED(IDC_BUTTON22, OnClickedOffButton12)
	//ON_BN_CLICKED(IDC_BUTTON23, OnClickedOffButton13)
	//ON_BN_CLICKED(IDC_BUTTON24, OnClickedOffButton14)
	//ON_BN_CLICKED(IDC_BUTTON25, OnClickedOffButton15)
	//ON_BN_CLICKED(IDC_BUTTON26, OnClickedOffButton16)
	//ON_BN_CLICKED(IDC_BUTTON27, OnClickedOffButton17)
	//ON_BN_CLICKED(IDC_BUTTON28, OnClickedOffButton18)
	//ON_BN_CLICKED(IDC_BUTTON29, OnClickedOffButton19)
	//ON_BN_CLICKED(IDC_BUTTON30, OnClickedOffButton20)
	//ON_BN_CLICKED(IDC_BUTTON31, OnClickedOffButton21)
	//ON_BN_CLICKED(IDC_BUTTON32, OnClickedOffButton22)
	//ON_BN_CLICKED(IDC_BUTTON33, OnClickedOffButton23)
	//ON_BN_CLICKED(IDC_BUTTON34, OnClickedOffButton24)
	//ON_BN_CLICKED(IDC_BUTTON35, OnClickedOffButton25)
	//ON_BN_CLICKED(IDC_BUTTON36, OnClickedOffButton26)
	//ON_BN_CLICKED(IDC_BUTTON37, OnClickedOffButton27)
	//ON_BN_CLICKED(IDC_BUTTON38, OnClickedOffButton28)
	//ON_BN_CLICKED(IDC_BUTTON39, OnClickedOffButton29)
	//ON_BN_CLICKED(IDC_BUTTON40, OnClickedOffButton30)
	//ON_BN_CLICKED(IDC_BUTTON41, OnClickedOffButton31)
	//ON_BN_CLICKED(IDC_BUTTON42, OnClickedOffButton32)
	ON_WM_MOVE()
	ON_MESSAGE(WM_SMPLDATA, OnAddone)
	ON_COMMAND(IDM_AXISSET, OnAxisset)
	ON_COMMAND(IDM_TCPOSITION, OnTcposition)
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CBoViewDlg message handler

//It evades ending by Enter Key.
void CBoViewDlg::OnOK(){}

//It evades ending by ESC Key.
void CBoViewDlg::OnCancel(){}

void CBoViewDlg::PostNcDestroy() 
{
	delete this;
//	CDialog::PostNcDestroy();
}

// WM_MOVE
void CBoViewDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

#ifdef FIX_SCREEN
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
#endif
}

BOOL CBoViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString	str;
//	int	idName[] = {IDS_BOVIEW_LIST24,IDS_BOVIEW_LIST00,/*IDS_BOVIEW_LIST01,*/IDS_BOVIEW_LIST02,
//					IDS_BOVIEW_LIST03,IDS_BOVIEW_LIST04,IDS_BOVIEW_LIST05,IDS_BOVIEW_LIST06,
//					IDS_BOVIEW_LIST07,IDS_BOVIEW_LIST08,IDS_BOVIEW_LIST09,/*IDS_BOVIEW_LIST10,*/
//					IDS_BOVIEW_LIST11,/*IDS_BOVIEW_LIST12,IDS_BOVIEW_LIST13,*/IDS_BOVIEW_LIST14,
//					IDS_BOVIEW_LIST15,IDS_BOVIEW_LIST16,/*IDS_BOVIEW_LIST17,IDS_BOVIEW_LIST18,*/
//					/*IDS_BOVIEW_LIST19,*/IDS_BOVIEW_LIST21,/*IDS_BOVIEW_LIST22,*/IDS_BOVIEW_LIST23,
//					IDS_BOVIEW_LIST25,IDS_BOVIEW_LIST26,IDS_BOVIEW_LIST27,IDS_BOVIEW_LIST28,
//					NULL
//	};
	int	idName[] = {IDS_BOVIEW_LIST24,IDS_BOVIEW_LIST02,IDS_BOVIEW_LIST00,IDS_BOVIEW_LIST10,IDS_BOVIEW_LIST06,
					IDS_BOVIEW_LIST11,IDS_BOVIEW_LIST21,IDS_BOVIEW_LIST04,IDS_BOVIEW_LIST16,
					IDS_BOVIEW_LIST14,IDS_BOVIEW_LIST25,IDS_BOVIEW_LIST26,IDS_BOVIEW_LIST27,
					IDS_BOVIEW_LIST28,IDS_BOVIEW_LIST01,NULL
	};

	// Boview config data set up
	ConfDataSetUp();

	// 幅１、高さ24のイメージを持つ
	m_image.Create(1, 24, ILC_COLOR, 0, 0);

	// 高さ調整用のイメージリストを設定する
	m_listActData.SetImageList(&m_image, LVSIL_STATE);
	CHeaderCtrl	*ctl = m_listActData.GetHeaderCtrl();
	if( ctl != NULL ) ctl->SetImageList(&m_image);

	// set list style.
	m_listActData.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	CFont	newFont, *font;
	LOGFONT	lf;
	double	dR, dG, dB;
	int		i;

	// change font
	font = m_listActData.GetFont();
	font->GetLogFont(&lf);
	lf.lfHeight = -12;
	lf.lfWidth = 0;
	newFont.CreateFontIndirect(&lf);
	m_listActData.SetFont(&newFont);

	// setup list
	m_listActData.DeleteAllItems();
	m_listActData.InsertColumn(0, _T(""), LVCFMT_LEFT, 0);
	m_listActData.InsertColumn(1, _T(""), LVCFMT_LEFT, 240);
	m_listActData.InsertColumn(2, _T(""), LVCFMT_LEFT, 250);
	for( i = 0; idName[i] != NULL; i++ )
	{
		m_listActData.InsertItem(i, _T(""));
		str.LoadString(idName[i]);
		m_listActData.SetItemText(i, 1, str);
	}

	// get boview graph axis.
	cnf.ReadAxis2File(&m_stTempAxis2);

	// customize chart library
	clib1.ChartMeshXCnt = 20;
	clib1.ChartMeshYCnt = 15;
	clib2.ChartMeshXCnt = 20;
	clib2.ChartMeshYCnt = 15;
	clib3.ChartMeshXCnt = 20;
	clib3.ChartMeshYCnt = 15;
	clib4.ChartMeshXCnt = 20;
	clib4.ChartMeshYCnt = 15;
	clib5.ChartMeshXCnt = 20;
	clib5.ChartMeshYCnt = 15;

	CRect	rc;
	CWnd	*wnd;

	// create graph window
	wnd = GetDlgItem(IDC_BOVIEW_GRAPH);
	wnd->GetClientRect(&rc);
	wnd->MapWindowPoints(this, &rc);
	m_graph.CreateEx(WS_EX_WINDOWEDGE, NULL, _T("BoviewGraph"), WS_CHILDWINDOW|WS_VISIBLE|WS_BORDER,rc,this,IDC_BOVIEW_GRAPH);
	m_graph.ShowWindow(SW_SHOW);

	// get graph area.
	m_graph.GetClientRect(&m_rcGraph);
	m_graph.MapWindowPoints(this, &m_rcGraph);
	m_nScPosX = m_rcGraph.right;

	// tc graph setting
	for( i = 0; i < TC_NO_MAX; i++ )
	{
		m_tcg[i]->m_pLib->m_bChScaleDisp = TRUE;
		m_tcg[i]->m_pLib->m_nTmCh2LineWd = 1;
		m_tcg[i]->m_pLib->ChartMeshXCnt = 3;
		m_tcg[i]->m_pLib->ChartMeshYCnt = 10;
		m_tcg[i]->m_pLib2->m_clClear = RGB(0,255,0);
		m_tcg[i]->m_pLib2->m_bChScaleDisp = TRUE;
		m_tcg[i]->m_pLib2->m_nTmCh2LineWd = 1;
		m_tcg[i]->m_pLib2->ChartMeshXCnt = 3;
		m_tcg[i]->m_pLib2->ChartMeshYCnt = 10;

		m_btnOFF[i].SetWindowText(_T("0.0"));
	}

	// create tc graph window
	int		id[TC_NO_MAX] = {
					IDC_TC_GRAPH15, IDC_TC_GRAPH14, IDC_TC_GRAPH13, IDC_TC_GRAPH12,
					IDC_TC_GRAPH11, IDC_TC_GRAPH10, IDC_TC_GRAPH21, IDC_TC_GRAPH20, 
					IDC_TC_GRAPH19, IDC_TC_GRAPH18, IDC_TC_GRAPH17, IDC_TC_GRAPH16
	};
	for( i = 0; i < TC_NO_MAX; i++ )
	{
		wnd = GetDlgItem(id[i]);
		wnd->GetClientRect(&rc);
		wnd->MapWindowPoints(this, &rc);
		m_tcg[i]->CreateEx(WS_EX_WINDOWEDGE, NULL, _T("TcGraph"), WS_CHILDWINDOW|WS_VISIBLE|WS_BORDER,rc,this,id[i]);
		m_tcg[i]->ShowWindow(SW_SHOW);
	}

	// reset tc graph
	ResetTcGraph();

	// disp label
	m_stHigh.SetTextColor(RGB(0,200,0));
	m_stMiddle.SetTextColor(RGB(0,0,255));
	m_stLow.SetTextColor(RGB(255,0,0));

	// get scale area
	m_textYScale.GetClientRect(&m_rcYScale);
	m_textYScale.MapWindowPoints(this, &m_rcYScale);

	// set edit color
	for( i = 0; i < PEN_SET_NUM; i++ ){
		m_editScVal[i].SetFontInfo(16, TRUE);
		m_editScVal[i].SetBackColor(m_ps.colPen[i]);
		dR = GetRValue(m_ps.colPen[i]);
		dG = GetGValue(m_ps.colPen[i]);
		dB = GetBValue(m_ps.colPen[i]);
		dR *= 0.299;
		dG *= 0.587;
		dB *= 0.114;
		if( (int)(dR+dG+dB) >= 127 )
			m_editScVal[i].SetTextColor(RGB(0,0,0));
		else
			m_editScVal[i].SetTextColor(RGB(255,255,255));
	}

	// reset boview graph
	ResetBoviewGraph(TRUE);

	// button default selection.
	SetButtonLamp(1);

	// Set timer
	SetTimer(1, 200, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
}

// selected pens button.
void CBoViewDlg::OnClickedButton1()
{
	CPenSetting		dlg;
	int				i;
	double			dR, dG, dB;

	if( m_nCurView == 0 )
	{
		AfxMessageBox("停止中はペン設定できません", MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	dlg.SetParam(&m_ps);
	if( IDOK == dlg.DoModal() )
	{
		dlg.GetParam(&m_ps);

		// set edit color
		for( i = 0; i < PEN_SET_NUM; i++ ){
			m_editScVal[i].SetBackColor(m_ps.colPen[i]);
			dR = GetRValue(m_ps.colPen[i]);
			dG = GetGValue(m_ps.colPen[i]);
			dB = GetBValue(m_ps.colPen[i]);
			dR *= 0.299;
			dG *= 0.587;
			dB *= 0.114;
			if( (int)(dR+dG+dB) >= 127 )
				m_editScVal[i].SetTextColor(RGB(0,0,0));
			else
				m_editScVal[i].SetTextColor(RGB(255,255,255));
		}

		// reset graph
		ResetBoviewGraph(FALSE);
//		ResetTcGraph();
		Invalidate(FALSE);
	}
}

// clicked freeze button.
void CBoViewDlg::OnClickedButton2()
{
	m_nCurView = 0;
	m_graph.ChangeView(0);
	SetButtonLamp(0);
}

// clicked 5 minutes button.
void CBoViewDlg::OnClickedButton3()
{
	m_nCurView = 1;
	m_graph.ChangeView(1);
	SetButtonLamp(1);
}

// clicked 15 minutes button.
void CBoViewDlg::OnClickedButton4()
{
	m_nCurView = 2;
	m_graph.ChangeView(2);
	SetButtonLamp(2);
}

// clicked 30 minutes button.
void CBoViewDlg::OnClickedButton5()
{
	m_nCurView = 3;
	m_graph.ChangeView(3);
	SetButtonLamp(3);
}

// clicked 60 minutes button.
void CBoViewDlg::OnClickedButton6()
{
	m_nCurView = 4;
	m_graph.ChangeView(4);
	SetButtonLamp(4);
}

// 描画時
void CBoViewDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

//	DrawTcScale1(dc, m_textTcScale1);
	DrawTcScale1(dc, m_textTcScale2);
//	DrawTcScale1(dc, m_textTcScale3);
//	DrawTcScale2(dc, m_textTcScale4);
	DrawTcScale2(dc, m_textTcScale5);
//	DrawTcScale2(dc, m_textTcScale6);
	DrawYScale(dc);
	DrawScooter(dc);
}

// タイマー処理
void CBoViewDlg::OnTimer(UINT nIDEvent)
{
	CRect	rcSc;

	// redraw scooter
	rcSc.SetRect(m_rcGraph.left - 10, m_rcGraph.top - 21, m_rcGraph.right + 10, m_rcGraph.top);
	InvalidateRect(&rcSc, FALSE);
	m_graph.Invalidate();

	CDialog::OnTimer(nIDEvent);
}

// ﾊﾞｯｸｸﾞﾗｳﾝﾄﾞ処理からのデータ受信時
LRESULT CBoViewDlg::OnAddone(WPARAM wParam, LPARAM lParam) 
{
	static BOOL		bFirst	= TRUE;
	BOOL			*pRsltMap	= (BOOL *)wParam;
	BOOL			*pRslt	= (BOOL *)pRsltMap[0];
	BOOL			*pRsltD	= (BOOL *)pRsltMap[1];
	StSampledData	*pSmpl	= (StSampledData *)lParam;
	SampledData		sd		= pSmpl->sd;
	ViewData		vd		= pSmpl->vd;
	SampledData		data;
	CString			str;
	_TCHAR			buf[32];
	BOOL			bChgHeat = pSmpl->isChgHeat;
	BOOL			bAlarm[TC_NO_MAX] = {0};
	double			dDeffT;
	int				i, ch, idx, dat, line;

// tcno->列No変換テーブル
int tc2line[28] = 
{
	0,	0,
	1,	1,
	2,	2,
	3,	3,	3,
	4,	4,
	5,	5,	5,
	6,	6,
	7,	7,
	8,	8,
	9,	9,	9,
	10,	10,
	11,	11,	11
};

// 変換Table
int	point[36] = 
{ 0,  1, -1, 
  2,  3, -1, 
  4,  5, -1, 
  6,  7,  8, 
  9, 10, -1, 
 11, 12, 13, 
 14, 15, -1, 
 16, 17, -1, 
 18, 19, -1, 
 20, 21, 22, 
 23, 24, -1, 
 25, 26, 27
};

	//-------------------------------------------------------------------------
	// set tc graph back color.
	for( i = 0; i < cnf.numCombination; i++ )
	{
		if( pRslt[i] )
		{
			if( cnf.tcCombination[i].lower != -1 )
			{
				line = tc2line[cnf.tcCombination[i].lower];
				bAlarm[line] = TRUE;
			}
			if( cnf.tcCombination[i].upper != -1 )
			{
				line = tc2line[cnf.tcCombination[i].upper];
				bAlarm[line] = TRUE;
			}
		}
	}
	for( i = 0; i < cnfD.numCombination; i++ )
	{
		if( pRsltD[i] )
		{
			if( cnfD.tcCombination[i].lower != -1 )
			{
				line = tc2line[cnfD.tcCombination[i].lower];
				bAlarm[line] = TRUE;
			}
			if( cnfD.tcCombination[i].upper != -1 )
			{
				line = tc2line[cnfD.tcCombination[i].upper];
				bAlarm[line] = TRUE;
			}
		}
	}

	//-------------------------------------------------------------------------
	// setting graph data.

	// draw tc graph
	for( i = 0; i < TC_NO_MAX; i++ )
	{
		if( point[i*3] >= 0 )
		{
			ch = cnf.inputPoint[point[i*3]].adCh;
			if( ch != -1 )
				data.val[0] = sd.val[ch];
			else
				data.val[0] = 0;
		}
		else
			data.val[0] = 0;

		if( point[i*3+1] >= 0 )
		{
			ch = cnf.inputPoint[point[i*3+1]].adCh;
			if( ch != -1 )
				data.val[1] = sd.val[ch];
			else
				data.val[1] = 0;
		}
		else
			data.val[1] = 0;

		if( point[i*3+2] >= 0 )
		{
			ch = cnf.inputPoint[point[i*3+2]].adCh;
			if( ch != -1 )
				data.val[2] = sd.val[ch];
			else
				data.val[2] = 0;
		}
		else
			data.val[2] = 0;

		m_tcg[i]->m_bAlarm = bAlarm[i];
		m_tcg[i]->AppendData(&data);
		m_tcg[i]->Invalidate(FALSE);

		// 温度差表示
		if( point[i*3+2] >= 0 )	// ３段目が存在
		{
			dDeffT = (data.val[0]-data.val[2])*PhisicalTempUnit;
		}
		else					// ３段目が無い
		{
			dDeffT = (data.val[0]-data.val[1])*PhisicalTempUnit;
		}
		str.Format(_T("%.1lf"), dDeffT);
		if( dDeffT < 0.0 )
			m_btnOFF[i].SetTextColor(RGB(255,0,0));
		else
			m_btnOFF[i].SetTextColor(RGB(0,0,0));
		m_btnOFF[i].SetWindowText(str);
	}

	//-------------------------------------------------------------------------
	// setting graph data.

	// create data.
	for( i = 0; i < (PEN_NUM - 1 + NumTCTap + 8); i++ )
	{
		switch(i)
		{
		case 0:
			ch = cnf.inputPoint[VcID].adCh;
			if( ch != -1 )
				data.val[i] = (short)sd.val[ch];
			else
				data.val[i] = 0;
			break;
		case 1:
			ch = cnf.inputPoint[SNID].adCh;
			if( ch != -1 )
				data.val[i] = (short)sd.val[ch];
			else
				data.val[i] = 0;
//			data.val[i] = (short)vd.nSnOpen;	
			break;
		case 2:
			ch = cnf.inputPoint[LevelID].adCh;
			if( ch != -1 )
				data.val[i] = (short)sd.val[ch];
			else
				data.val[i] = 0;
//			data.val[i] = (short)vd.nMdLevel;
			break;
		case 3:
			data.val[i] = (short)vd.nTdWeidht;
			break;
		default:
			if( point[i-PEN_NUM+1] >= 0 )
			{
				ch = cnf.inputPoint[point[i-PEN_NUM+1]].adCh;
				if( ch != -1 )
					data.val[i] = (short)sd.val[ch];
				else
					data.val[i] = 0;
				break;
			}
			else
				data.val[i] = 0;
		}
	}

	// set data and redraw.
	m_graph.AppendData(&data);
	m_graph.Invalidate();

	//-------------------------------------------------------------------------
	// setting list data
	int nPt = 0;

	// 鋼種グループNo
	str.Format(_T("%d"), vd.nPtnNo+1);
	m_listActData.SetItemText(nPt++, 2, str);

	// 溶解鋼種名
	memset(buf, 0, sizeof(buf));
	memcpy(buf, vd.cGradeCode, sizeof(vd.cGradeCode)-1);
	m_listActData.SetItemText(nPt++, 2, buf);

	// 溶番
	memset(buf, 0, sizeof(buf));
	memcpy(buf, vd.cHeatNo, sizeof(vd.cHeatNo)-1);
	m_listActData.SetItemText(nPt++, 2, buf);

	// 標準No
	memset(buf, 0, sizeof(buf));
	memcpy(buf, vd.cStandardNo, sizeof(vd.cStandardNo)-1);
	m_listActData.SetItemText(nPt++, 2, buf);

	// 湯面レベル
	ch = cnf.inputPoint[LevelID].adCh;
	if( ch != -1 )
		dat = sd.val[ch];
	else
		dat = 0;
	str.Format(_T("%.1lf"), dat*0.1);
	m_listActData.SetItemText(nPt++, 2, str);

	// 鋳込速度
	ch = cnf.inputPoint[VcID].adCh;
	if( ch != -1 )
		dat = sd.val[ch];
	else
		dat = 0;
	str.Format(_T("%.2lf"), dat*0.01);
	m_listActData.SetItemText(nPt++, 2, str);

	// SN 開度PV
	ch = cnf.inputPoint[SNID].adCh;
	if( ch != -1 )
		dat = sd.val[ch];
	else
		dat = 0;
	str.Format(_T("%.0lf"), dat*1.0);
	m_listActData.SetItemText(nPt++, 2, str);

	//// モールド幅
	//str.Format(_T("%d"), vd.nMdWidth);
	//m_listActData.SetItemText(nPt++, 2, str);

	// モールド厚
	//str.Format(_T("%d"), vd.nMdThick);
	//m_listActData.SetItemText(nPt++, 2, str);

	// シリーズ鋳込長
	str.Format(_T("%.0lf"), (vd.nCastLengthS)*1.0);
	m_listActData.SetItemText(nPt++, 2, str);

	// 鋳込長（チャージリセット）
	str.Format(_T("%.2lf"), (vd.nCastLengthC)*0.01);
	m_listActData.SetItemText(nPt++, 2, str);

	// TD重量
	str.Format(_T("%.2lf"), (vd.nTdWeidht)*0.01);
	m_listActData.SetItemText(nPt++, 2, str);

	// MD冷却水流量A面
	str.Format(_T("%.0lf"), (vd.nMdCoolWtA)*1.0);
	m_listActData.SetItemText(nPt++, 2, str);

	// MD冷却水流量C面
	str.Format(_T("%.0lf"), (vd.nMdCoolWtC)*1.0);
	m_listActData.SetItemText(nPt++, 2, str);

	// MD冷却水流量D面
	str.Format(_T("%.0lf"), (vd.nMdCoolWtD)*1.0);
	m_listActData.SetItemText(nPt++, 2, str);

	// MD冷却水温度D面
	str.Format(_T("%.0lf"), (vd.nMdCoolWtTempD)*1.0);
	m_listActData.SetItemText(nPt++, 2, str);

	// LD開
	if( vd.isLdOn == 1 )
		str = _T("開");
	else
		str = _T("閉");
	m_listActData.SetItemText(nPt++, 2, str);


	// redraw axis
	double	val,min,max;
 
	CClientDC	dc(this);
	CPoint		pt[3];
	double		scale, zero;
	for( i = 0; i < PEN_SET_NUM; i++ )
	{
		if( i < PEN_NUM )
		{
			if( i < PEN_NUM - 1 )
				val = data.val[i] * m_ps.dUnit[i];
			else
				val = data.val[i + TapRaw * m_ps.nCombiNo] * m_ps.dUnit[i];
			if( m_ps.dMin[i] <= m_ps.dMax[i] )
			{
				min = m_ps.dMin[i];
				max = m_ps.dMax[i];
			}
			else
			{
				min = m_ps.dMax[i];
				max = m_ps.dMin[i];
			}
			if( val < min )
				val = min;
			if( val > max )
				val = max;
			scale = (m_ps.dMax[i] - m_ps.dMin[i]) / clib1.ChartHeight;
			zero = m_ps.dMin[i];
			pt[0].x = m_rcYScale.left + AXIS_WIDTH*i + SCOOTER_WIDTH;
			pt[0].y = m_rcGraph.top + clib1.ChartHeight - (int)((val - zero) / scale);
		}
		else
		{
			val = data.val[i + TapRaw * m_ps.nCombiNo] * m_ps.dUnit[PEN_NUM-1];
			if( m_ps.dMin[PEN_NUM-1] <= m_ps.dMax[PEN_NUM-1] )
			{
				min = m_ps.dMin[PEN_NUM-1];
				max = m_ps.dMax[PEN_NUM-1];
			}
			else
			{
				min = m_ps.dMax[PEN_NUM-1];
				max = m_ps.dMin[PEN_NUM-1];
			}
			if( val < min )
				val = min;
			if( val > max )
				val = max;
			scale = (m_ps.dMax[PEN_NUM-1] - m_ps.dMin[PEN_NUM-1]) / clib1.ChartHeight;
			zero = m_ps.dMin[PEN_NUM-1];
			pt[0].x = m_rcYScale.left + AXIS_WIDTH*(PEN_NUM-1) + SCOOTER_WIDTH;
			pt[0].y = m_rcGraph.top + clib1.ChartHeight - (int)((val - zero) / scale);
		}
		pt[1].x = pt[0].x - SCOOTER_WIDTH;
		pt[1].y = pt[0].y - SCOOTER_WIDTH/2;
		pt[2].x = pt[0].x - SCOOTER_WIDTH;
		pt[2].y = pt[0].y + SCOOTER_WIDTH/2;

		if( i < PEN_NUM )
		{
			dc.FillSolidRect(
				m_rcYScale.left + AXIS_WIDTH*i, 
				m_rcGraph.top, 
				SCOOTER_WIDTH + 1, 
				clib1.ChartHeight,
				::GetSysColor(COLOR_BTNFACE));
		}
		CBrush br, *oldBr;
		br.CreateSolidBrush(m_ps.colPen[i]);
		oldBr = dc.SelectObject(&br);
		CRgn	rgn;
		if( i < PEN_NUM )
		{
			rgn.CreateRectRgn(
					m_rcYScale.left + AXIS_WIDTH*i, 
					m_rcGraph.top, 
					m_rcYScale.left + AXIS_WIDTH*i + SCOOTER_WIDTH + 1, 
					m_rcGraph.top + clib1.ChartHeight);
		}
		else
		{
			rgn.CreateRectRgn(
					m_rcYScale.left + AXIS_WIDTH*(PEN_NUM-1), 
					m_rcGraph.top, 
					m_rcYScale.left + AXIS_WIDTH*(PEN_NUM-1) + SCOOTER_WIDTH + 1, 
					m_rcGraph.top + clib1.ChartHeight);
		}
		//idx = i + TapRaw * (m_ps.nCombiNo);
		//if( idx < (NumTCTap + (PEN_NUM-1)) )
		//{
		dc.SelectClipRgn(&rgn);
		dc.Polygon(pt, 3);
		dc.SelectClipRgn(NULL);
		//}
		rgn.DeleteObject();
		dc.SelectObject(oldBr);
		br.DeleteObject();
	}

	// set scooter val
	SampledData	sd2;
	BOOL		bRet;
	bRet = m_graph.GetScooterVal(&sd2);
	for( i = 0; i < PEN_SET_NUM; i++ )
	{ 
		if( bRet )
		{
			if( i < (PEN_NUM-1))
			{
				idx = i;
				str.Format(m_ps.szFmt[i], sd2.val[idx] * m_ps.dUnit[i]);
			}
			else
			{
				idx = i + TapRaw * (m_ps.nCombiNo);
				str.Format(m_ps.szFmt[PEN_NUM-1], sd2.val[idx] * m_ps.dUnit[PEN_NUM-1]);
			}
		}
		else
		{
			str = _T("-");
		}
		m_editScVal[i].SetWindowText(str);
	}

	return 0;
}

// axis setting
void CBoViewDlg::OnAxisset()
{
	CBovwAxis		dlg;

	dlg.SetAxis(m_stTempAxis2);
	if( IDOK == dlg.DoModal() )
	{
		// decide temp axis.
		dlg.GetAxis(m_stTempAxis2);
		cnf.WriteAxis2File(&m_stTempAxis2);

		// reset TC graph
		ResetTcGraph();
	}
}

// selected 'TC Position' menu.
void CBoViewDlg::OnTcposition()
{
	if( m_pDlgTcView == NULL )
	{
		m_pDlgTcView = new CTcVwDlg(this);
	}
	m_pDlgTcView->ShowWindow(SW_SHOW);
}

///////////////////////////////////////////////////////////////////////////////
// member function

// reset batch graph
void CBoViewDlg::ResetBoviewGraph(BOOL bFirst)
{
	StGraphParam	gp;
	CRect			rc;
	int				i;

	gp.numChannel		= PEN_SET_NUM;
	gp.neuralChannel	= 0;
	gp.isNeuralCal		= FALSE;
	gp.dispStartTime	= 0;
	gp.samplePeriod		= 500;
	gp.nCombiNo			= m_ps.nCombiNo;
    memset(gp.nTrendChArg, 0, sizeof(gp.nTrendChArg));
    memset(gp.nTrendChArgTm, 0, sizeof(gp.nTrendChArgTm));

	gp.timeAxis.scale = 0.25;
	gp.timeAxis.zero = 0;
	for( i = 0; i < (PEN_NUM + 2); i++ )
	{
		if( i < PEN_NUM )
		{
			if( m_ps.bEnbPen[i] )
				gp.nTrendChArgTm[i] = i;
			else
				gp.nTrendChArgTm[i] = -1;
		}
		else
		{
			if( m_ps.bEnbPen[PEN_NUM-1] )
				gp.nTrendChArgTm[i] = i;
			else
				gp.nTrendChArgTm[i] = -1;
		}
		clib1.m_clTonesTm[i] = m_ps.colPen[i];
		clib2.m_clTonesTm[i] = m_ps.colPen[i];
		clib3.m_clTonesTm[i] = m_ps.colPen[i];
		clib4.m_clTonesTm[i] = m_ps.colPen[i];
		clib5.m_clTonesTm[i] = m_ps.colPen[i];

		if( i < PEN_NUM )
		{
			gp.stAxisTm2[i].scale = (m_ps.dMax[i] - m_ps.dMin[i]) / clib1.ChartMeshYCnt;
			gp.stAxisTm2[i].zero = m_ps.dMin[i];
			gp.dUnitTm[i] = m_ps.dUnit[i];
		}
		else
		{
			gp.stAxisTm2[i].scale = (m_ps.dMax[PEN_NUM-1] - m_ps.dMin[PEN_NUM-1]) / clib1.ChartMeshYCnt;
			gp.stAxisTm2[i].zero = m_ps.dMin[PEN_NUM-1];
			gp.dUnitTm[i] = m_ps.dUnit[PEN_NUM-1];
		}
	}

	// load trend
	if( bFirst )
		m_graph.BoviewLoad(NULL, &gp);
	else
		m_graph.BoviewReload(NULL, &gp);

	// update graph
	m_graph.SetScooterX(m_nScPosX - m_rcGraph.left);
	m_graph.GetClientRect(&rc);
	m_graph.MapWindowPoints(this, &rc);
	InvalidateRect(&rc, FALSE);
}

// reset batch graph
void CBoViewDlg::ResetTcGraph(void)
{
	COLORREF		cols[3] = {RGB(0,200,0),RGB(0,0,255),RGB(255,0,0)};	
//	CStatic			*stc[] = {&m_textTcScale1,&m_textTcScale2,&m_textTcScale3,&m_textTcScale4,&m_textTcScale5,&m_textTcScale6};
	CStatic			*stc[] = {&m_textTcScale2,&m_textTcScale5};
	StGraphParam	gp;
	CRect			rc, rcYScale;
	int				i, j;

	gp.numChannel		= 3;
	gp.neuralChannel	= 0;
	gp.isNeuralCal		= FALSE;
	gp.dispStartTime	= 0;
	gp.samplePeriod		= 500;
    memset(gp.nTrendChArg, 0, sizeof(gp.nTrendChArg));
    memset(gp.nTrendChArgTm, 0, sizeof(gp.nTrendChArgTm));

	gp.timeAxis.scale = 0.25;
	gp.timeAxis.zero = 0;
	for( i = 0; i < 3; i++ )
	{
		gp.nTrendChArgTm[i] = i;
		for( j = 0; j < TC_NO_MAX; j++ )
			m_tcg[j]->m_pLib->m_clTonesTm[i] = cols[i];
		gp.stAxisTm2[i].scale = m_stTempAxis2.scale;
		gp.stAxisTm2[i].zero = m_stTempAxis2.zero;
		gp.dUnitTm[i] = 0.1;
	}

	// load trend
	for( i = 0; i < TC_NO_MAX; i++ )
	{
		m_tcg[i]->TcLoad(NULL, &gp);

		// update graph
		m_tcg[i]->GetClientRect(&rc);
		m_tcg[i]->MapWindowPoints(this, &rc);
		InvalidateRect(&rc, FALSE);
	}

	for( i = 0; i < 2; i++ )
	{
		stc[i]->GetClientRect(&rcYScale);
		rcYScale.DeflateRect(-5,-5);
		stc[i]->MapWindowPoints(this, &rcYScale);
		InvalidateRect(&rcYScale);
	}
}

// draw Y scale
void CBoViewDlg::DrawYScale(CDC &dc)
{
	CFont	newFont, *font;
	CString	strName, strUnit, strTmp;
	LOGFONT	lf;
	int		i, cnt;

	// clear Y scale.
	m_textYScale.GetClientRect(&m_rcYScale);
	m_textYScale.MapWindowPoints(this, &m_rcYScale);
	dc.FillSolidRect(&m_rcYScale, GetSysColor(COLOR_BTNFACE));

	// change font
	font = GetFont();
	font->GetLogFont(&lf);
	lf.lfHeight = -11;
	lf.lfWeight |= FW_BOLD;
	newFont.CreateFontIndirect(&lf);

	// draw
	ChartAxis	stAxis[PEN_NUM];
	for( i = 0, cnt = 0; i < PEN_NUM; i++ )
	{
		stAxis[i].scale = (m_ps.dMax[i] - m_ps.dMin[i]) / clib1.ChartMeshYCnt;
		stAxis[i].zero = m_ps.dMin[i];
		clib1.DrawYAxis2(dc, m_ps.szFmt[i], &stAxis[i], &newFont, m_ps.colPen[i], 
			m_rcYScale.left + AXIS_WIDTH*cnt , m_rcGraph.top, clib1.ChartHeight );
		cnt++;
	}
}

// draw scooter
void CBoViewDlg::DrawScooter(CDC &dc)
{
	static	CPoint	oldPtSc[3];
	CPoint	ptSc[3];
	CPoint	ptBase;
	CBrush	br, *pOldBr;

	// make scooter point
	MakeScooterPoints(m_nScPosX, ptSc);

	// draw sctooter
	if( ptSc[0].x != oldPtSc[0].x && oldPtSc[0].x != 0 )
	{
		CRect	rcClear;
		br.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
		rcClear.SetRect(oldPtSc[2].x-1, oldPtSc[2].y-1, oldPtSc[1].x+1, oldPtSc[0].y+1);
		dc.FillRect(&rcClear, &br);
		br.DeleteObject();
	}
	// draw new scooter
	if( m_bScDrag )
		br.CreateSolidBrush(RGB(0,255,0));
	else
		br.CreateSolidBrush(RGB(0,0,255));
	pOldBr = dc.SelectObject(&br);
	dc.Polygon(ptSc, 3);
	dc.SelectObject(pOldBr);
	br.DeleteObject();

	// save old Point
	memcpy(oldPtSc, ptSc, sizeof(oldPtSc));
}

// 
void CBoViewDlg::SetButtonLamp(int no)
{
	if( no == 0 )
		m_btnFrz.SetBackColor(RGB(0,255,0));
	else
		m_btnFrz.SetBackColor(::GetSysColor(COLOR_BTNFACE));
	if( no == 1 )
		m_btnMinute5.SetBackColor(RGB(0,255,0));
	else
		m_btnMinute5.SetBackColor(::GetSysColor(COLOR_BTNFACE));
	if( no == 2 )
		m_btnMinute15.SetBackColor(RGB(0,255,0));
	else
		m_btnMinute15.SetBackColor(::GetSysColor(COLOR_BTNFACE));
	if( no == 3 )
		m_btnMinute30.SetBackColor(RGB(0,255,0));
	else
		m_btnMinute30.SetBackColor(::GetSysColor(COLOR_BTNFACE));
	if( no == 4 )
		m_btnMinute60.SetBackColor(RGB(0,255,0));
	else
		m_btnMinute60.SetBackColor(::GetSysColor(COLOR_BTNFACE));
}

void CBoViewDlg::MakeScooterPoints(int &x, POINT *pt)
{
	CPoint		ptBase;
	const int	height = 20;

	// get scooter pos.
	if( x < m_rcGraph.left )
		x = m_rcGraph.left;
	if( x > m_rcGraph.right )
		x = m_rcGraph.right;
	ptBase.x = m_nScPosX;
	ptBase.y = m_rcGraph.top;

	// calc scooter point.
	pt[0].x = ptBase.x;
	pt[0].y = ptBase.y;
	pt[1].x = (LONG)(ptBase.x + height*0.5);
	pt[1].y = ptBase.y - height;
	pt[2].x = (LONG)(ptBase.x - height*0.5);
	pt[2].y = ptBase.y - height;
}

void CBoViewDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPoint	ptSc[3];
	CRgn	rgnSc;

	// make scooter point.
	MakeScooterPoints(m_nScPosX, ptSc);

	// create region
	rgnSc.CreatePolygonRgn(ptSc, 3, ALTERNATE);

	if( rgnSc.PtInRegion(point) )
	{
		SetCapture();
		m_bScDrag = TRUE;
	}

	rgnSc.DeleteObject();

	CDialog::OnLButtonDown(nFlags, point);
}


void CBoViewDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_bScDrag = FALSE;

	CDialog::OnLButtonUp(nFlags, point);
}

void CBoViewDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	int		x;

	if( m_bScDrag )
	{
		x = point.x;

		// get scooter pos.
		if( x < m_rcGraph.left )
			x = m_rcGraph.left;
		if( x > m_rcGraph.right )
			x = m_rcGraph.right;

		m_nScPosX = x;
		m_graph.SetScooterX(m_nScPosX - m_rcGraph.left);

		if( !(nFlags & MK_LBUTTON) ){
			ReleaseCapture();
			m_bScDrag = FALSE;
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

//void CBoViewDlg::OnClickedOffButton1(){	DoOffButton(0); }
//void CBoViewDlg::OnClickedOffButton2(){	DoOffButton(1); }
//void CBoViewDlg::OnClickedOffButton3(){	DoOffButton(2); }
//void CBoViewDlg::OnClickedOffButton4(){	DoOffButton(3); }
//void CBoViewDlg::OnClickedOffButton5(){	DoOffButton(4); }
//void CBoViewDlg::OnClickedOffButton6(){	DoOffButton(5); }
//void CBoViewDlg::OnClickedOffButton7(){	DoOffButton(6); }
//void CBoViewDlg::OnClickedOffButton8(){	DoOffButton(7); }
//void CBoViewDlg::OnClickedOffButton9(){	DoOffButton(8); }
//void CBoViewDlg::OnClickedOffButton10(){DoOffButton(9); }
//void CBoViewDlg::OnClickedOffButton11(){DoOffButton(10); }
//void CBoViewDlg::OnClickedOffButton12(){DoOffButton(11); }
//void CBoViewDlg::OnClickedOffButton13(){DoOffButton(12); }
//void CBoViewDlg::OnClickedOffButton14(){DoOffButton(13); }
//void CBoViewDlg::OnClickedOffButton15(){DoOffButton(14); }
//void CBoViewDlg::OnClickedOffButton16(){DoOffButton(15); }
//void CBoViewDlg::OnClickedOffButton17(){DoOffButton(16); }
//void CBoViewDlg::OnClickedOffButton18(){DoOffButton(17); }
//void CBoViewDlg::OnClickedOffButton19(){DoOffButton(18); }
//void CBoViewDlg::OnClickedOffButton20(){DoOffButton(19); }
//void CBoViewDlg::OnClickedOffButton21(){DoOffButton(20); }
//void CBoViewDlg::OnClickedOffButton22(){DoOffButton(21); }
//void CBoViewDlg::OnClickedOffButton23(){DoOffButton(22); }
//void CBoViewDlg::OnClickedOffButton24(){DoOffButton(23); }
//void CBoViewDlg::OnClickedOffButton25(){DoOffButton(24); }
//void CBoViewDlg::OnClickedOffButton26(){DoOffButton(25); }
//void CBoViewDlg::OnClickedOffButton27(){DoOffButton(26); }
//void CBoViewDlg::OnClickedOffButton28(){DoOffButton(27); }
//void CBoViewDlg::OnClickedOffButton29(){DoOffButton(28); }
//void CBoViewDlg::OnClickedOffButton30(){DoOffButton(29); }
//void CBoViewDlg::OnClickedOffButton31(){DoOffButton(30); }
//void CBoViewDlg::OnClickedOffButton32(){DoOffButton(31); }

void CBoViewDlg::DoOffButton(int no)
{
//	CExeConf	dlg;

//	dlg.DoModal();
}

// draw Y scale
void CBoViewDlg::DrawTcScale1(CDC &dc, CStatic &cs)
{
	CFont	newFont, *font;
	CString	strName, strUnit, strTmp;
	LOGFONT	lf;
	CRect	rcYScale;

	// clear Y scale.
	cs.GetClientRect(&rcYScale);
	cs.MapWindowPoints(this, &rcYScale);
	dc.FillSolidRect(&rcYScale, GetSysColor(COLOR_BTNFACE));

	// change font
	font = GetFont();
	font->GetLogFont(&lf);
	lf.lfHeight = -11;
	lf.lfWeight |= FW_BOLD;
	newFont.CreateFontIndirect(&lf);

	// draw
	theApp.m_tcChart[0].DrawYAxis3(dc, _T("%3.0f"), &m_stTempAxis2, &newFont, theApp.m_tcChart[0].m_clText, 
		rcYScale.left, rcYScale.top, theApp.m_tcChart[0].ChartHeight );
}

void CBoViewDlg::DrawTcScale2(CDC &dc, CStatic &cs)
{
	CFont	newFont, *font;
	CString	strName, strUnit, strTmp;
	LOGFONT	lf;
	CRect	rcYScale;

	// clear Y scale.
	cs.GetClientRect(&rcYScale);
	cs.MapWindowPoints(this, &rcYScale);
	dc.FillSolidRect(&rcYScale, GetSysColor(COLOR_BTNFACE));

	// change font
	font = GetFont();
	font->GetLogFont(&lf);
	lf.lfHeight = -11;
	lf.lfWeight |= FW_BOLD;
	newFont.CreateFontIndirect(&lf);

	// draw
	theApp.m_tcChart[0].DrawYAxis4(dc, _T("%3.0f"), &m_stTempAxis2, &newFont, theApp.m_tcChart[0].m_clText, 
		rcYScale.right, rcYScale.top, theApp.m_tcChart[0].ChartHeight );
}

void CBoViewDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if( !bShow )
	{
		if( m_pDlgTcView != NULL )
			m_pDlgTcView->ShowWindow(SW_HIDE);
	}
}

// boexec config file data setup
BOOL CBoViewDlg::ConfDataSetUp(void)
{
	FILE	*fp;
	TCHAR	name[MAX_PATH];

	// make up boexec config file name
	_tcscat( _tcscpy( name, cnf.WorkRootDir ), BOViewConfFile );

	// open the boexec config file
	if((fp = _tfopen(name, _T("rb"))) == NULL) 
	{
		// create the boexec config file
		if((fp = _tfopen(name, _T("wb+"))) != NULL) 
		{
			// config data write
			fwrite(&m_ps, sizeof(m_ps), 1, fp);
		}
	}
	else 
	{
		// config data read
		fread(&m_ps, sizeof(m_ps), 1, fp);
	}

	// fix setting
	m_ps.dUnit[0] = 0.01;
	m_ps.dUnit[1] = 1.0;
	m_ps.dUnit[2] = 0.1;
	m_ps.dUnit[3] = 0.01;
	m_ps.dUnit[4] = 0.1;
	_tcscpy(m_ps.szFmt[0], _T("%.2lf"));
	_tcscpy(m_ps.szFmt[1], _T("%.0lf"));
	_tcscpy(m_ps.szFmt[2], _T("%.1lf"));
	_tcscpy(m_ps.szFmt[3], _T("%.2lf"));
	_tcscpy(m_ps.szFmt[4], _T("%.1lf"));

	// file close
	fclose(fp);

	return TRUE;
}

// boexec config file up date
BOOL CBoViewDlg::ConfDataUpDate(void)
{
	FILE	*fp;
	TCHAR	name[MAX_PATH];

	// make up boexec config file name
	_tcscat( _tcscpy( name, cnf.WorkRootDir ), BOViewConfFile );

	// open the boexec config file
	if((fp = _tfopen(name, _T("wb+"))) == NULL) 
	{
		// file error
		return FALSE;
	}
	else 
	{
		fwrite(&m_ps, sizeof(m_ps), 1, fp);
	}

	// file close
	fclose(fp);

	return TRUE;
}

// 画面破棄時
void CBoViewDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// config data up date
	ConfDataUpDate();
}
