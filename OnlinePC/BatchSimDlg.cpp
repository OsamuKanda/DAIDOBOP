// BatchSimDlg.cpp : 実装ファイル
//
#include "stdafx.h"
#include <windowsx.h>
#include "OnlinePC.h"
#include "OnlinePCDlg.h"
#include "BatchSimDlg.h"
#include ".\batchsimdlg.h"

#define NO_PASSWD
//////////////////////////////////////////////////////////////////////////////
// definition
static CConfig		&cnf	= theApp.m_cnfBatch;
static CConfig		&cnfD	= theApp.m_cnfBatchD;
static CBolog		&lg		= theApp.m_log;
static CChartLib	&clib	= theApp.m_batchChart;
static TcName		*tcnm	= &theApp.m_stTcNm;

#define	AXIS_WIDTH	47

//@ 追加機能
FILETIME
UnixTimeToFileTime(time_t t){
    FILETIME ft;
    LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000;
    ft.dwLowDateTime = (DWORD)ll;
    ft.dwHighDateTime = (DWORD)(ll >> 32);
    return ft;
}

//    time_t -> SYSTEMTIME 変換

SYSTEMTIME
UnixTimeToSystemTime(time_t t){
    SYSTEMTIME st;
    FILETIME ft = UnixTimeToFileTime(t);
    ::FileTimeToSystemTime(&ft, &st);
    return st;
}

extern FILETIME FT_Add(FILETIME ft1, FILETIME ft2);


// CBatchSimDlg dialog
IMPLEMENT_DYNAMIC(CBatchSimDlg, CDialog)
CBatchSimDlg::CBatchSimDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBatchSimDlg::IDD, pParent)
{
	// initialize member data.
	m_pDlgTMapForBatch		= NULL;
	m_pDlgChart3dForBatch	= NULL;
	m_pDlgTcView			= NULL;
	m_blOutBOFetched		= FALSE;
	m_blOutBOFetchedD		= FALSE;
	m_blPulseOutBOFetched	= FALSE;
	m_blPulseOutBOFetchedD	= FALSE;
	m_blStopFlg				= TRUE;
	m_nSelectInfo			= -1;
	m_nSelFile				= 0;
	m_nTimerID				= -1;
	m_nStartTime			= 0;
	m_nTickCount			= 0;
	m_nMdSize				= 0;
	m_isAutoMoldWidth		= 1;
	m_isEnabLevelCtrl		= 0;
	m_simAutoLvCtrl			= 0; 
	m_nTimer				= 100;
	m_nTrendNum				= 0;
	m_bFirst				= TRUE;
	m_bFirstBO				= FALSE;
	m_blPause				= FALSE;

	m_bPass					= FALSE;

	for(int i = 0; i < NumInput; i++)
	{
		if( i < LastID-1)
		{
			m_stTdSel[i].fIndication = TRUE;
			m_stTdSel[i].arg = i;
		}
		else
		{
			m_stTdSel[i].fIndication = FALSE;
			m_stTdSel[i].arg = -1;
		}
	}

	// create dialog.
	Create(CBatchSimDlg::IDD, pParent);				
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
}

CBatchSimDlg::~CBatchSimDlg()
{
}

void CBatchSimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Y_SCALE, m_textYScale);
	//DDX_Control(pDX, IDC_COMBO1, m_cmbLog);
	DDX_Control(pDX, IDC_LOG_REMARK, m_editRemark);
	DDX_Control(pDX, IDC_EDIT2, m_editMdSize);
	DDX_Control(pDX, IDC_EDIT1, m_editStTime);
	DDX_Control(pDX, IDC_CUR_TIME, m_textCurTime);
	DDX_Control(pDX, IDC_BUTTON35, m_btnPass);
	DDX_Control(pDX, IDC_STATIC1, m_textSG);
	DDX_Control(pDX, IDC_CHECK1, m_chkAuto);
	DDX_Control(pDX, IDC_CHECK2, m_chkHighSpeed);
	DDX_Control(pDX, IDC_CHECK3, m_btnALvlCtl);
	DDX_Control(pDX, IDC_CHECK4, m_btnExeALvlCtl);
	DDX_Control(pDX, IDC_BUTTON4, m_btnLogPrev);
	DDX_Control(pDX, IDC_BUTTON5, m_btnLogNext);
	DDX_Control(pDX, IDC_EDIT_BOMSGF, m_textBoMsgFirst);
	DDX_Control(pDX, IDC_EDIT_BOMSGF2, m_textBoMsgFirstD);
	DDX_Control(pDX, IDC_FILTERSW, m_chkFilter);
	DDX_Control(pDX, IDC_FILTERFACTER, m_editTempFilter);
	DDX_Control(pDX, IDC_FILTERFACTERSN, m_editSnFilter);
	DDX_Control(pDX, IDC_FILTERFACTERLV, m_editLevelFilter);
	DDX_Control(pDX, IDC_FILTERFACTERPR, m_editVcFilter);
	DDX_Control(pDX, IDC_CONFNAME, m_txtConfName);
	DDX_Control(pDX, IDC_CONFNAME2, m_txtConfNameD);
	DDX_Control(pDX, IDC_CHECK5, m_chkCsvOut);
	DDX_Control(pDX, IDC_BUTTON1, m_btnStartSim);
	DDX_Control(pDX, IDC_FILENAME2, m_labLogName);
	DDX_Control(pDX, IDC_BUTTON6, m_btnLogList);
}

BEGIN_MESSAGE_MAP(CBatchSimDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnClickedButton2)
//	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON3, OnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON11, OnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON35, OnClickedPasswordButton)
	ON_COMMAND(IDM_SELSTEELKIND, OnSelsteelkind)
	ON_UPDATE_COMMAND_UI(IDM_SELSTEELKIND, OnUpdateSelsteelkind)
	ON_WM_MOVE()
	ON_COMMAND(IDM_TRENDCHG, OnTrendchg)
	ON_BN_CLICKED(IDC_CHECK3, OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
//	ON_COMMAND(IDM_TCPOSITION, OnTcposition)
	ON_BN_CLICKED(IDC_FILTERSW, OnClickedFilterSw)
	ON_BN_CLICKED(IDC_BUTTON6, &CBatchSimDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CBatchSimDlg message handler

//It evades ending by Enter Key.
void CBatchSimDlg::OnOK(){}

//It evades ending by ESC Key.
void CBatchSimDlg::OnCancel(){}

void CBatchSimDlg::PostNcDestroy() 
{
	delete this;
//	CDialog::PostNcDestroy();
}

// WM_MOVE
void CBatchSimDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

#ifdef FIX_SCREEN
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
#endif
}

// initialize dialog.
BOOL CBatchSimDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// set radio button.
	SharedMemory	*pShMem = GetSharedMemory();
	m_simAutoLvCtrl = pShMem->isAutoLVCtl;
	m_btnALvlCtl.SetCheck(m_simAutoLvCtrl);

	// set steel kind name
	m_textSG.SetWindowText(_T(""));

	// customize chart library
	clib.ChartMeshXCnt = 20;
	clib.ChartMeshYCnt = 15;

	//// create thermal mapping window
	StLogInfo	stInfo;
	if( m_pDlgTMapForBatch == NULL )
	{
	//	m_pDlgTMapForBatch = new CTMapDlgEx(this);
	//	if( !m_pDlgTMapForBatch )
	//		return FALSE;
	//	m_pDlgTMapForBatch->ShowWindow(SW_HIDE);
		m_pDlgTMapForBatch = GetTMapDlg(this, TRUE);
		if( !m_pDlgTMapForBatch )
			return FALSE;
		stInfo.pBatch = &m_batch;
		stInfo.nStartTime = m_nStartTime;
		stInfo.nOffsetTime = m_nOffsetTime;
		SetLogInfo(m_pDlgTMapForBatch, &stInfo);
		m_pDlgTMapForBatch->ShowWindow(SW_HIDE);
	}

	// create 3d chart window
	if( m_pDlgChart3dForBatch == NULL )
	{
		m_pDlgChart3dForBatch = GetChart3DDlg(this, TRUE);
		if( !m_pDlgChart3dForBatch )
			return FALSE;
		m_pDlgChart3dForBatch->ShowWindow(SW_HIDE);
	}

	// set filter item.
	CString			strVal;
	m_bFilterSwitch = pShMem->filterSwitch;
	m_chkFilter.SetCheck(m_bFilterSwitch);
	m_fFilterFacter = pShMem->filterFacter;
	strVal.Format(_T("%.2g"), m_fFilterFacter);
	m_editTempFilter.SetWindowText(strVal);
	m_fFilterFacterSn = pShMem->filterFacterSn;
	strVal.Format(_T("%.2g"), m_fFilterFacterSn);
	m_editSnFilter.SetWindowText(strVal);
	m_fFilterFacterLv = pShMem->filterFacterLv;
	strVal.Format(_T("%.2g"), m_fFilterFacterLv);
	m_editLevelFilter.SetWindowText(strVal);
	m_fFilterFacterPr = pShMem->filterFacterPr;
	strVal.Format(_T("%.2g"), m_fFilterFacterPr);
	m_editVcFilter.SetWindowText(strVal);

	// create message window
	CRect	rc;
	CWnd	*wnd = GetDlgItem(IDC_BO_MSG);
	wnd->GetClientRect(&rc);
	wnd->MapWindowPoints(this, &rc);
	m_msg.m_nNumRow = 40;	// customize line
	m_msg.CreateEx(WS_EX_WINDOWEDGE, NULL, MSGWINDOW, WS_CHILDWINDOW|WS_VISIBLE|WS_BORDER,rc,this,IDC_BO_MSG);
	m_msg.ShowWindow(SW_SHOW);

	// create graph window
	wnd = GetDlgItem(IDC_BAT_GRAPH_WIN);
	wnd->GetClientRect(&rc);
	wnd->MapWindowPoints(this, &rc);
	m_graph.CreateEx(WS_EX_WINDOWEDGE, NULL, _T("BatchGraph"), WS_CHILDWINDOW|WS_VISIBLE|WS_BORDER,rc,this,IDC_BAT_GRAPH_WIN);
	m_graph.ShowWindow(SW_SHOW);

	// reset batch graph
	ResetBatchGraph();

	// setup batchsim dialog
//	SetupBatchsimDlg();

	// select log
//	m_cmbLog.SetCurSel(m_nSelFile);
//	SetSelectedTrendEntry(m_nSelFile);

//	m_chkAuto.SetCheck(1);
	m_chkHighSpeed.SetCheck(1);

	// read default steel kind
	BoSysInfo	stBsInfo;
	if( cnf.ReadBosysInfo(0, &stBsInfo) )
	{
		m_nSelectInfo = 0;
		m_textSG.SetWindowText(stBsInfo.szComment);
		m_txtConfName.SetWindowText(stBsInfo.szConfName);
		m_txtConfNameD.SetWindowText(stBsInfo.szConfNameD);
	}
	else
	{
		m_nSelectInfo = -1;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}

// WM_SHOWWINDOW proc.
void CBatchSimDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	BoSysInfo	stBsInfo;

	// show? 
	if( bShow )
	{
		// if stop flag is on,
		if( m_blStopFlg )
		{
			// setup batchsim dialog
			SetupBatchsimDlg();

			if( m_bFirst )
			{
				// open log file of old selection
				_TCHAR	sLogName[MAX_PATH];
				int		i;

				if( cnf.LoadLogSelFileName(_T("BatchLogSel.txt"), sLogName) )
				{
					for( i = 0; i < m_nTrendNum; i++ )
					{
						if( _tcscmp(sLogName, trendDir[i].name) == 0 )
							break;
					}
					if( i < m_nTrendNum )
						m_nSelFile = i;
				}
				else
					m_nSelFile = 0;

				m_bFirst = FALSE;
			}

			// select current log
			//m_cmbLog.SetCurSel(m_nSelFile);
			SetSelectedTrendEntry(m_nSelFile);

			// reload axis
			cnf.ReadAxisFile();
			cnfD.ReadAxisFile();

			//@ read steel config file
			cnf.ReadBosysInfo(m_nSelectInfo, &stBsInfo);
			m_textSG.SetWindowText(stBsInfo.szComment);
			m_txtConfName.SetWindowText(stBsInfo.szConfName);
			m_txtConfNameD.SetWindowText(stBsInfo.szConfNameD);

			// ボタン文字設定
			m_btnStartSim.SetWindowText(_T("シミュレーション開始"));

			// set color for password button.
			#ifdef NO_PASSWD
			m_bPass = TRUE;
			#else
			m_bPass = FALSE;
			#endif
			EnableControl(m_bPass);
		}
	}
	// hide?
	else
	{
		// stop batch simulation.
		// StopBatchSimulate();

		// create 3d chart window
		if( m_pDlgChart3dForBatch != NULL )
		{
			m_pDlgChart3dForBatch->ShowWindow(SW_HIDE);
		}
		// create thermal mapping window
		if( m_pDlgTMapForBatch != NULL )
		{
			m_pDlgTMapForBatch->ShowWindow(SW_HIDE);
		}
		// hide 
		if( m_pDlgTcView != NULL )
			m_pDlgTcView->ShowWindow(SW_HIDE);
	}
}

// WM_DESTROY proc.
void CBatchSimDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// stop timer.
	StopTimer();
}

// WM_PAINT proc.
void CBatchSimDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// draw Y scale.
    DrawYScale(dc);
}

// WM_TIMER proc.
void CBatchSimDlg::OnTimer(UINT nIDEvent)
{
	BOOL	bStopReq = FALSE;

	if( GetBatchStopReq(m_pDlgTMapForBatch) )
		bStopReq = TRUE;

	// if stop flag is false,
	if(!m_blStopFlg && !bStopReq) 
	{
		// 一時停止中なら処理しない
		if( !m_blPause )
		{
			// step batch simulation.
			StepBatchSimulate();

			m_graph.Invalidate(FALSE);
		}
	}
	else
	{
		// stop batch simulation.
		StopBatchSimulate();
		StopTimer();
	}

	CDialog::OnTimer(nIDEvent);
}

// click 'Start Simulation' button.
void CBatchSimDlg::OnClickedButton1()
{
	CString	strVal;

	if( !m_bPass )
		return;

	// 一時停止中なら
	if( m_blPause )
	{
		// ボタン文字設定
		m_btnStartSim.SetWindowText(_T("一時停止"));

		// 一時停止OFF
		m_blPause = FALSE;
		return;
	}
	// 動作中にStart（一時停止）が押されたら
	else if( !m_blStopFlg )
	{
		// ボタン文字設定
		m_btnStartSim.SetWindowText(_T("再開"));

		// 一時停止ON
		m_blPause = TRUE;
		return;
	}

	// set log filter
	m_batch.SetLogFilter(m_bFilterSwitch, m_fFilterFacter, m_fFilterFacterSn, m_fFilterFacterLv, m_fFilterFacterPr);
	strVal.Format(_T("%.1g"), m_fFilterFacter);
	m_editTempFilter.SetWindowText(strVal);
	strVal.Format(_T("%.1g"), m_fFilterFacterSn);
	m_editSnFilter.SetWindowText(strVal);
	strVal.Format(_T("%.1g"), m_fFilterFacterLv);
	m_editLevelFilter.SetWindowText(strVal);
	strVal.Format(_T("%.1g"), m_fFilterFacterPr);
	m_editVcFilter.SetWindowText(strVal);

	// reset first bo flag
	m_bFirstBO = FALSE;

	// reset output fetch
	m_blPulseOutBOFetched = FALSE;
	m_blPulseOutBOFetchedD = FALSE;

	// clear BO Msg First box
	m_textBoMsgFirst.SetWindowText(_T(""));
	m_textBoMsgFirstD.SetWindowText(_T(""));

	// reset message list
	m_msg.ResetContent();

	// stop batchsim stop flag off
	SetBatchStopReq(m_pDlgTMapForBatch, FALSE);

	// start batch simulation.
	StartBatchSimulate();

	// reset batch graph
	ResetBatchGraph();

	// start timer
	StartTimer(m_nTimer);
}

// click 'Stop Simulation' button.
void CBatchSimDlg::OnClickedButton2()
{
	if( !m_bPass )
		return;

	// stop batch simulation.
	StopBatchSimulate();
	StopTimer();
}

//// select change log combobox.
//void CBatchSimDlg::OnSelchangeCombo1()
//{
//	int sel = m_cmbLog.GetCurSel();
//	SetSelectedTrendEntry(sel);
//
//	for(int i = 0; i < NumInput; i++)
//	{
//		if( i < LastID-1)
//		{
//			m_stTdSel[i].fIndication = TRUE;
//			m_stTdSel[i].arg = i;
//		}
//		else
//		{
//			m_stTdSel[i].fIndication = FALSE;
//			m_stTdSel[i].arg = -1;
//		}
//	}
//}

// clicked 3d chart button.
void CBatchSimDlg::OnClickedButton3()
{
	// create 3d chart window
	if( m_pDlgChart3dForBatch != NULL )
	{
		m_pDlgChart3dForBatch->ShowWindow(SW_SHOW);
	}
}

// clicked thermal mapping button.
void CBatchSimDlg::OnClickedButton11()
{
	// create thermal mapping window
	if( m_pDlgTMapForBatch != NULL )
	{
		m_pDlgTMapForBatch->ShowWindow(SW_SHOW);
	}
}

void CBatchSimDlg::OnClickedPasswordButton()
{
	CPassDlg	dlg;

	if( m_bPass == FALSE && IDOK == dlg.DoModal() )
	{
		m_bPass = TRUE;
		EnableControl(m_bPass);
	}
}

void CBatchSimDlg::OnSelsteelkind()
{
	CPtnSel99	dlg;
	CString		str;
	BoSysInfo	stBsInfo;

	dlg.SetParam(cnf, m_nSelectInfo);
	if( IDOK == dlg.DoModal() )
	{
		dlg.GetParam(&m_nSelectInfo);
		cnf.ReadBosysInfo(m_nSelectInfo, &stBsInfo);
		m_textSG.SetWindowText(stBsInfo.szComment);
		m_txtConfName.SetWindowText(stBsInfo.szConfName);
		m_txtConfNameD.SetWindowText(stBsInfo.szConfNameD);
	}
}

// clicked filter sw button.
void CBatchSimDlg::OnClickedFilterSw()
{
	m_bFilterSwitch = !m_bFilterSwitch;
	m_chkFilter.SetCheck(m_bFilterSwitch);
}

void CBatchSimDlg::OnUpdateSelsteelkind(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bPass);
}

// selected 'TC Position' menu.
//void CBatchSimDlg::OnTcposition()
//{
//	if( m_pDlgTcView == NULL )
//	{
//		m_pDlgTcView = new CTcVwDlg(this);
//	}
//	m_pDlgTcView->ShowWindow(SW_SHOW);
//}

BOOL CBatchSimDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (GET_WM_COMMAND_ID(wParam, lParam)) 
	{
	case IDC_FILTERFACTER:
	case IDC_FILTERFACTERSN:
	case IDC_FILTERFACTERLV:
	case IDC_FILTERFACTERPR:
		// update the execution condition.
		BoConditionManage(GET_WM_COMMAND_ID(wParam, lParam), -1);
		break;
	}

	return CDialog::OnCommand(wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////
// CBatchSimDlg member function

// reset batch graph
void CBatchSimDlg::ResetBatchGraph(void)
{
	StGraphParam	gp;
	CRect			rc;
	int				i, ch, arg;

	gp.numChannel		= NumADChannel;
	gp.neuralChannel	= 0;
	gp.isNeuralCal		= FALSE;
	gp.dispStartTime	= 0;
	gp.samplePeriod		= 500;
    memset(gp.nTrendChArg, 0, sizeof(gp.nTrendChArg));
    memset(gp.nTrendChArgTm, 0, sizeof(gp.nTrendChArgTm));

	memcpy(&gp.timeAxis, &cnf.m_stTimeAxis, sizeof(gp.timeAxis));
	memcpy(&gp.tempAxis, &cnf.m_stTempAxis, sizeof(gp.tempAxis));
	memcpy(&gp.levelAxis, &cnf.m_stLevelAxis, sizeof(gp.levelAxis));
	memcpy(&gp.vcAxis, &cnf.m_stVcAxis, sizeof(gp.vcAxis));
	memcpy(&gp.snAxis, &cnf.m_stSNAxis, sizeof(gp.snAxis));
//	memcpy(&gp.widthAxis, &cnf.m_stWidthAxis, sizeof(gp.widthAxis));

	// トレンド描画ｃｈ設定
	for(i = 0; i < NumADChannel; i++)
	{
		arg = cnf.adChannel[i].arg;
		if( arg >= 0 && arg < LastID-1)
			gp.nTrendChArgTm[i] = arg;
		else
			gp.nTrendChArgTm[i] = -1;
	}

	// チャンネル表示設定値初期化
	memset(gp.isTrendIndicate, 0, sizeof(gp.isTrendIndicate));

	// 表示入力ポイントに対して
	for( i = 0; i < NumInput; i++ )
	{
		// 表示／非表示を設定
		ch = cnf.inputPoint[i].adCh;
		if( ch != -1 )
			gp.isTrendIndicate[ch] = m_stTdSel[i].fIndication;
	}

	// load trend
	m_graph.BatchLoad(NULL, &gp);

	// update graph
	m_graph.GetClientRect(&rc);
	m_graph.MapWindowPoints(this, &rc);
	InvalidateRect(&rc, FALSE);
}

// setup batchsim dialog
void CBatchSimDlg::SetupBatchsimDlg(void)
{
	// set start time
	m_editStTime.SetWindowText(_T("0"));

	// setup log combo box
	SetupLogList();
}

void CBatchSimDlg::SetupLogList(void)
{
//	TrendDirectory	*p;
	int				numTrendFile;
//	int				i;

	// get trend file entry in the trend file directory.
	numTrendFile = lg.GetTrendDirectory(trendDir);
	m_nTrendNum = numTrendFile;

	//// reset trend file list.
	//m_cmbLog.SetRedraw(FALSE);
	//m_cmbLog.ResetContent();

	//// for each entry,
	//for (i = 0, p = trendDir; i < numTrendFile; i++, p++) 
	//{
	//	// append trend file entry.
	//	m_cmbLog.AddString(lg.MakeupTrendEntString(p));
	//}

	//// terminate trend file entry.
	//m_cmbLog.SetRedraw(TRUE);
}

// draw Y scale
void CBatchSimDlg::DrawYScale(CDC &dc)
{
	CRect	rcYScale, rcGraph;
	CFont	newFont, *font;
	CString	strName, strUnit, strTmp;
	LOGFONT	lf;
	int		i;

	// get graph area
	m_graph.GetClientRect(&rcGraph);
	m_graph.MapWindowPoints(this, &rcGraph);

	// clear Y scale.
	m_textYScale.GetClientRect(&rcYScale);
	m_textYScale.MapWindowPoints(this, &rcYScale);
	dc.FillSolidRect(&rcYScale, GetSysColor(COLOR_BTNFACE));

	// change font
	font = GetFont();
	font->GetLogFont(&lf);
	lf.lfHeight = -12;
	newFont.CreateFontIndirect(&lf);

	// draw SN-Level-Vc-Width
//	int idn[] = {IDS_DEF_TEMP, IDS_DEF_SN, IDS_DEF_LEVEL, IDS_DEF_VC, IDS_DEF_WIDTH};
//	int idu[] = {IDS_DEF_TEMP_UNIT, IDS_DEF_SN_UNIT, IDS_DEF_LEVEL_UNIT, IDS_DEF_VC_UNIT, IDS_DEF_WIDTH_UNIT};
//	ChartAxis	*cas[] = {&cnf.m_stTempAxis, &cnf.m_stSNAxis, &cnf.m_stLevelAxis, &cnf.m_stVcAxis, &cnf.m_stWidthAxis}; 
//	for( i = 0; i < 5; i++ )
	int idn[] = {IDS_DEF_TEMP, IDS_DEF_SN, IDS_DEF_LEVEL, IDS_DEF_VC};
	int idu[] = {IDS_DEF_TEMP_UNIT, IDS_DEF_SN_UNIT, IDS_DEF_LEVEL_UNIT, IDS_DEF_VC_UNIT};
	CString strFmt[] = {_T("%3.1f"), _T("%3.0f"), _T("%3.1f"), _T("%3.2f")};
	ChartAxis	*cas[] = {&cnf.m_stTempAxis, &cnf.m_stSNAxis, &cnf.m_stLevelAxis, &cnf.m_stVcAxis}; 
	for( i = 0; i < 4; i++ )
	{
		strName.LoadString(idn[i]);
		strTmp.LoadString(idu[i]);
		strUnit.Format(_T("[%s]"), strTmp); 
		clib.DrawYAxis(dc, strName, strUnit, strFmt[i], cas[i], &newFont, clib.m_clText, 
			rcYScale.left + AXIS_WIDTH*i , rcGraph.top, clib.ChartHeight );
	}
}

// set now selected entry information.
void CBatchSimDlg::SetSelectedTrendEntry(int sel)
{
	FILE	*fp;
	CString	str, str2;
	_TCHAR	dirName[MAX_PATH];

	// trend num zero check
	if( m_nTrendNum <= 0 )
		return;

	// not sel check
	if (sel < 0) return;
	m_nSelFile = sel;

	// make up trend data file name.
	_tcscat( _tcscat( _tcscpy( dirName, lg.LogRootDir ), _T("\\") ), trendDir[sel].name );

	// open and read the trend file header.
	if ((fp = _tfopen( dirName, _T("rb") )) == NULL)
	{
		// if file not found, display null string.
		m_editMdSize.SetWindowText(_T(""));
		m_editRemark.SetWindowText(_T(""));
		return;
	}
	fread(&m_header, sizeof(m_header), 1, fp);
	// close the trend file.
	fclose(fp);

	// get start time.
	m_nStartTime = trendDir[sel].startTime;

//#ifdef Compatibility
//	// display mold size of this entry.
//	if( m_header.header.version <= 500 )
//		_stprintf(buf, _T("%d"), m_header.header.arg[NumADChannelV5]);
//	else
//		_stprintf(buf, _T("%d"), m_header.header.moldWidth);
//#else
//	// display mold size of this entry.
//	_stprintf(buf, _T("%d"), m_header.header.moldWidth);//M
//#endif
//	m_editMdSize.SetWindowText(buf);

	// ファイル名表示
	str = lg.MakeupTrendEntString(&trendDir[sel]);
	if( m_header.header.version >= 801 && m_header.header.nPtnNo >= 0 && m_header.header.nPtnNo < NumSteelKind )
	{
		str2.Format(_T("(%d)"), m_header.header.nPtnNo+1);
		str += str2;
	}
	m_labLogName.SetWindowText(str);

	// display remark of this entry.
	m_editRemark.SetWindowText(m_header.header.remark);

	// header read
	if( m_header.header.version >= 620 )
		m_bFilterSwitch = TRUE;
	else
		m_bFilterSwitch = FALSE;
	m_chkFilter.SetCheck(m_bFilterSwitch);

	// save now selection
	cnf.SaveLogSelFileName(_T("BatchLogSel.txt"), trendDir[sel].name);
}

// set now selected entry information.
void CBatchSimDlg::StartBatchSimulate(void)
{
	_TCHAR	buf[256];
	_TCHAR	*p;
	int		dumyStTime, sel, totalMinute, nChkSpd;

	// check select of steel kind.
	if(m_nSelectInfo < 0)
	{
		AfxMessageBox("B.O.パターンが選択されていません", MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	// get high speed state.
	nChkSpd = m_chkHighSpeed.GetCheck();
	if( nChkSpd == BST_CHECKED )
		m_nTimer = 10;
	else
		m_nTimer = 100;

	//// get auto check state.
	//m_isAutoMoldWidth = m_chkAuto.GetCheck();

	//// store Mold Size.
	//if( !m_isAutoMoldWidth )
	//{
	//	m_editMdSize.GetWindowText(buf, 32);
	//	if( _stscanf(buf, _T("%d"), &dumyMdSize) != 1 ||
	//		dumyMdSize < 0 || dumyMdSize > 2500) 
	//	{
	//		AfxMessageBox("モールド幅が不正です", MB_ICONEXCLAMATION | MB_OK);
	//		m_editMdSize.SetWindowText(_T("0"));
	//		return;
	//	}
	//}
	//else
	//	dumyMdSize = 2500;

	// Establishment value is acquired.
	m_editStTime.GetWindowText(buf, 32);
	if( _stscanf(buf, _T("%d"), &dumyStTime) != 1 ) 
	{
		m_editStTime.SetWindowText(_T("0"));
		AfxMessageBox("オフセット時間が不正です", MB_ICONEXCLAMATION | MB_OK);
	}
	//sel = m_cmbLog.GetCurSel();
	//m_cmbLog.GetLBText(sel, buf);
	sel = m_nSelFile;
	m_labLogName.GetWindowText(buf, 255);
	p = _tcsstr(buf, _T("["));
	if( p != NULL && _stscanf(++p, _T("%d"), &totalMinute) != 1 ||
		dumyStTime < 0 || dumyStTime > totalMinute)
	{
		m_editStTime.SetWindowText(_T("0"));
		AfxMessageBox("オフセット時間が範囲外です", MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	_stprintf(buf, _T("%d"), dumyStTime);
	m_nOffsetTime = dumyStTime * 60 * 1000 / m_header.header.samplePeriod;
	m_editStTime.SetWindowText(buf);

	//@ 追加機能　有効／無効
	m_batch.SetCsvOut(m_chkCsvOut.GetCheck() == BST_CHECKED);
	m_batch.SetLogName(trendDir[sel].name);

	// reset BO prediction information.
	m_batch.ResetBOBatchInfo(m_nSelectInfo, 0);
	m_nMdSize = 0;

	// tickCount initialize.
	m_nTickCount = m_nOffsetTime;

	// discard the logging file.
	m_batch.DiscardLogFile();

	// setup log
	m_batch.SetupLogFile(trendDir[sel].name, m_nTickCount);

	// set fetch color
//	m_graph.SetFetchColor(m_batch.m_clBoLineColor);

	// disable control
	EnableControl(FALSE);

	// set log info.
	StLogInfo	stInfo;
	stInfo.pBatch = &m_batch;
	stInfo.nStartTime = m_nStartTime;
	stInfo.nOffsetTime = m_nOffsetTime;
	SetLogInfo(m_pDlgTMapForBatch, &stInfo);

	// ボタン文字設定
	m_btnStartSim.SetWindowText(_T("一時停止"));

	// stop flag OFF
	m_blStopFlg = FALSE;
}

// set now selected entry information.
void CBatchSimDlg::StepBatchSimulate(void)
{
	CString		str;
	SampledData	data;
	ResultData	result;
	int			nData[NumADChannel];
	int			i, ret, nMoldSizeL, nMoldSizeR;

	//@ 追加機能
	FILETIME		ft1, ft2;
	LARGE_INTEGER	li;
	ft1 = UnixTimeToFileTime(m_nStartTime);
	li.QuadPart = (LONGLONG)(m_nTickCount * m_header.header.samplePeriod * 10000.0);
	ft2.dwLowDateTime = li.LowPart;
	ft2.dwHighDateTime = li.HighPart;
	m_batch.m_ftTime = FT_Add(ft1, ft2);

	// get the sampled data. if successed,
	ret = m_batch.GetSampledData(&data);
	if( ret == FALSE ){
		StopBatchSimulate();
		return;
	}

	// get the sampled data. if successed,
	m_graph.AppendData(&data);

	//
	m_batch.SetAverageData(&data, nData);
	m_batch.LimitTempData(nData);

	COnlinePCDlg *dlg = (COnlinePCDlg *)AfxGetMainWnd();
	if( m_pDlgChart3dForBatch != NULL ){
		m_pDlgChart3dForBatch->SendMessage(WM_CHARTDATA, 0, (LPARAM)nData);
	}
	if( m_pDlgTMapForBatch != NULL ){
		m_pDlgTMapForBatch->SendMessage(WM_MAPDATA, (WPARAM)m_batch.GetCurrentRecord(), (LPARAM)nData);
	}

	//if( m_isAutoMoldWidth )
	//{
	//	//// search width left
	//	//for(i = 0; i < NumADChannel; i++)
	//	//	if( cnf.adChannel[i].arg == WidthLID )
	//	//		break;
	//	//if( i == NumADChannel )
	//	//	nMoldSizeL = 0;
	//	//else
	//	//	nMoldSizeL = data.val[i];

	//	//// search width right
	//	//for(i = 0; i < NumADChannel; i++)
	//	//	if( cnf.adChannel[i].arg == WidthRID )
	//	//		break;
	//	//if( i == NumADChannel )
	//	//	nMoldSizeR = 0;
	//	//else
	//	//	nMoldSizeR = data.val[i];

	//	for(i = 0; i < NumADChannel; i++)
	//		if( cnf.adChannel[i].arg == WidthID )
	//			break;
	//	if( i == NumADChannel )
	//		nMoldSizeL = nMoldSizeR = 0;
	//	else
	//		nMoldSizeL = nMoldSizeR = data.val[i]/2;
	//}
	//else
	//{
	//	nMoldSizeL = (int)(m_nMdSize * 0.5);
	//	nMoldSizeR = (int)(m_nMdSize * 0.5);
	//}

	// recalc group.
	m_batch.RecalcGroup();

	//// disp mold width.
	//str.Format( _T("%ld"), (nMoldSizeL+nMoldSizeR));//M
	//m_editMdSize.SetWindowText(str);

	// clear the result data of B.O. prediction.
	memset(&result, 0, sizeof(result));

	// test the B.O. prediction mode, and if enabled,
	if( !m_isEnabLevelCtrl || m_simAutoLvCtrl ) 
	{
		// execute B.O. prediction. if B.O. detected,
		m_blOutBOFetched = m_batch.DoBOBatch(&data, &result);
		m_blOutBOFetchedD = m_batch.DoBOBatchD(&data, &result);

		if( m_blOutBOFetched != 0 || m_blOutBOFetchedD != 0 ) 
		{
			// display occurring B.O.
			m_graph.SetFetchColor(RGB(255,0,0));
			m_graph.Fetch();
		}
	}
	// test the B.O. prediction mode, and if disabled,
	else
	{
		m_blOutBOFetched = 0;
		m_blOutBOFetchedD = 0;
		// clear the status table.
		m_batch.ClearStatusTbl();
		// un-occurrence is registered.
		m_batch.ClearOccureResult();
		// clear B.O. sampled data buffer.
		m_batch.ClearSampledData();
	}

	// test the neural networks output.
	TestBOStatus();

	// draw simulation time.
	SetSimulationTime();

	// increase the tick counter.
	m_nTickCount++;
}

// set now selected entry information.
void CBatchSimDlg::StopBatchSimulate(void)
{
	if( m_blStopFlg == TRUE )
		return;

	// stop flag ON
	m_blStopFlg = TRUE;

	// ボタン文字設定
	m_btnStartSim.SetWindowText(_T("シミュレーション開始"));

	// 一時停止OFF
	m_blPause = FALSE;

	// clear BO batch info.
	m_batch.ClearBOBatchInfo();

	// disable control
	EnableControl(TRUE);
}

void CBatchSimDlg::EnableControl(BOOL blEnable)
{
	if( m_bPass )
	{
		m_btnPass.SetTextColor(RGB(0,0,0));
		m_btnPass.SetBackColor(RGB(0,255,0));
	}
	else
	{
		m_btnPass.SetTextColor(RGB(255,255,255));
		m_btnPass.SetBackColor(RGB(255,0,0));
	}
	m_textSG.EnableWindow(blEnable);
	//m_cmbLog.EnableWindow(blEnable);
	m_btnLogList.EnableWindow(blEnable);
	m_editMdSize.EnableWindow(blEnable);
	m_editStTime.EnableWindow(blEnable);
	m_chkAuto.EnableWindow(blEnable);
	m_chkHighSpeed.EnableWindow(blEnable);
	m_btnLogPrev.EnableWindow(blEnable);
	m_btnLogNext.EnableWindow(blEnable);
	m_chkFilter.EnableWindow(blEnable);
	m_editTempFilter.EnableWindow(blEnable);
	m_editSnFilter.EnableWindow(blEnable);
	m_editLevelFilter.EnableWindow(blEnable);
	m_editVcFilter.EnableWindow(blEnable);
	m_chkCsvOut.EnableWindow(blEnable);
}

// set current time.
void CBatchSimDlg::SetSimulationTime(void)
{
	static time_t	preTime;
	time_t			t;
	CString			str;

	// get current time.
	t = (time_t)(m_nStartTime + m_nTickCount * m_header.header.samplePeriod * 0.001);
	if (t != preTime) 
	{
		// clear displaying area.
		m_textCurTime.SetWindowText(GetTimeStr(t));

		// update current time.
		preTime	= t;
	}
}

// test the B.O. status.
void CBatchSimDlg::TestBOStatus(void)
{
	TCCombination	*cp;
	struct tm		*tp;
	time_t			t;
	_TCHAR			timeStr[32];
	CString			strMsg, strMsgD, str;
	int				nMsgColor;
	int				nMsgColorD;
	int				i;

	// get simulation time.
	t	= (time_t)(m_nStartTime + m_nTickCount * m_header.header.samplePeriod * 0.001);
	tp	= localtime(&t);
	_stprintf(timeStr, _T("\'%2d.%2d.%2d %02d:%02d:%02d"),
			tp->tm_year % 100, tp->tm_mon + 1, tp->tm_mday,
			tp->tm_hour, tp->tm_min, tp->tm_sec);

//	if(!m_blPulseOutBOFetched && m_blOutBOFetched)
//		nMsgColor = RGB(255,0,0);
//	else
//		nMsgColor = RGB(0,0,0);

	// メッセージ色
	if(m_blOutBOFetched != 0)
	{
		// for alarm.
		nMsgColor = RGB(255,0,0);
	}
	else
	{
		// for normal.
		nMsgColor = RGB(0,0,0);
	}

	// メッセージ色 for dep.
	if(m_blOutBOFetchedD != 0)
	{
		// for alarm.
		nMsgColorD = RGB(255,0,0);
	}
	else
	{
		// for normal.
		nMsgColorD = RGB(0,0,0);
	}

	// B.O発生立上り判定
	if( (m_blOutBOFetched != 0) && (m_blPulseOutBOFetched == 0) )
	{
		strMsg = timeStr;
		strMsg += _T(" "); 
		m_bFirstBO = TRUE;
	}

	// B.O発生立上り判定 for dep.
	if( (m_blOutBOFetchedD != 0) && (m_blPulseOutBOFetchedD == 0) )
	{
		strMsgD = timeStr;
		strMsgD += _T(" "); 
		m_bFirstBOD = TRUE;
	}

	// for each output of combination.
	for (i = 0, cp = cnf.tcCombination; i < cnf.numCombination; i++, cp++) 
	{
		// test the output value of neural networks. and B.O. detected,
		if (m_batch.m_blStatusTbl[i]) 
		{
			if( cp->upper >= 0 && cp->upper < NumTCTap && cp->lower >= 0 && cp->lower < NumTCTap )
			{
				// display the B.O. message.
				m_msg.Append(nMsgColor, _T(" %s' , %s, %s (B.O.側)"), timeStr,
					tcnm->name[cp->upper], tcnm->name[cp->lower]);
				if( m_bFirstBO )
				{
					str.Format(_T("%s, %s (B.O.側) "), tcnm->name[cp->upper], tcnm->name[cp->lower]);
					strMsg += str;
					m_textBoMsgFirst.SetTextColor(nMsgColor);
					m_textBoMsgFirst.SetWindowText(strMsg);
					m_bFirstBO = FALSE;
				}
			}
		}
		m_batch.m_blStatusTbl[i] = FALSE;

	}

	// for each output of combination.(デプレッション側)
	for (i = 0, cp = cnfD.tcCombination; i < cnfD.numCombination; i++, cp++) 
	{
		// test the output value of neural networks. and B.O. detected,
		if (m_batch.m_blStatusTblD[i]) 
		{
			if( cp->upper >= 0 && cp->upper < NumTCTap && cp->lower >= 0 && cp->lower < NumTCTap )
			{
				// display the B.O. message.
				m_msg.Append(nMsgColorD, _T(" %s' , %s, %s (ﾃﾞﾌﾟﾚ側)"), timeStr,
					tcnm->name[cp->upper], tcnm->name[cp->lower]);
				if( m_bFirstBOD )
				{
					str.Format(_T("%s, %s (ﾃﾞﾌﾟﾚ側) "), tcnm->name[cp->upper], tcnm->name[cp->lower]);
					strMsgD += str;
					m_textBoMsgFirstD.SetTextColor(nMsgColorD);
					m_textBoMsgFirstD.SetWindowText(strMsgD);
					m_bFirstBOD = FALSE;
				}
			}
		}
		m_batch.m_blStatusTblD[i] = FALSE;

	}

	m_blPulseOutBOFetched = m_blOutBOFetched;
	m_blPulseOutBOFetchedD = m_blOutBOFetchedD;
}

void CBatchSimDlg::StartTimer(DWORD dwTime)
{
	StopTimer();

	// start timer
	m_nTimerID = SetTimer(1, dwTime, NULL);
}

void CBatchSimDlg::StopTimer(void)
{
	if( m_nTimerID != -1 )
	{
		KillTimer(m_nTimerID);
		m_nTimerID = -1;
	}
}

void CBatchSimDlg::OnTrendchg()
{
	CTrendSel	dlg;
	CRect		rc;
	int			chArgs[NumADChannel];
	int			i, ch;

	dlg.SetTrendSelect(m_stTdSel);
	if( IDOK == dlg.DoModal() )
	{
		// 表示選択情報を取得
		dlg.GetTrendSelect(m_stTdSel);

		// チャンネル表示設定値初期化
		memset(chArgs, 0, sizeof(chArgs));

		// 表示入力ポイントに対して
		for( i = 0; i < NumInput; i++ )
		{
			// 表示／非表示を設定
			ch = cnf.inputPoint[i].adCh;
			if( ch != -1 )
				chArgs[ch] = m_stTdSel[i].fIndication;
		}

		// グラフに設定値を渡し再描画
		m_graph.ChangeArg(chArgs);
		m_graph.GetClientRect(&rc);
		m_graph.MapWindowPoints(this, &rc);
		InvalidateRect(&rc, FALSE);
	}
}

// clicked Auto level ctl radio button.
void CBatchSimDlg::OnBnClickedCheck3()
{
	m_simAutoLvCtrl = m_btnALvlCtl.GetCheck();
}

// clicked execute while automatic level control.
void CBatchSimDlg::OnBnClickedCheck4()
{
	m_isEnabLevelCtrl = m_btnExeALvlCtl.GetCheck();
}

// clicked 'Prev' Button
void CBatchSimDlg::OnBnClickedButton4()
{
	if( m_nTrendNum <= 0 )
		return;

	if( m_nSelFile > 0 )
	{
		m_nSelFile = m_nSelFile - 1;
		//m_cmbLog.SetCurSel(m_nSelFile);
		SetSelectedTrendEntry(m_nSelFile);
	}
}

// clicked 'Next' Button
void CBatchSimDlg::OnBnClickedButton5()
{
	if( m_nTrendNum <= 0 )
		return;

	if( (m_nSelFile+1) < m_nTrendNum )
	{
		m_nSelFile = m_nSelFile + 1;
		//m_cmbLog.SetCurSel(m_nSelFile);
		SetSelectedTrendEntry(m_nSelFile);
	}
}

LPCTSTR CBatchSimDlg::GetTimeStr(time_t tm)
{
	struct			tm	*tp;
	static _TCHAR	timeStr[32];
	
	// get simulation time.
	tp	= localtime(&tm);
	_stprintf(timeStr, _T("\'%2d.%2d.%2d %02d:%02d:%02d"),
			tp->tm_year % 100, tp->tm_mon + 1, tp->tm_mday,
			tp->tm_hour, tp->tm_min, tp->tm_sec);
	return timeStr;
}

// update the execution condition.
void CBatchSimDlg::BoConditionManage(WPARAM wParam, int cmd)
{
	static char dmyBuf[128];
	static int	textSize;
	static float dmyFacter;

	switch(wParam) 
	{
	case IDC_FILTERFACTER:
		m_editTempFilter.GetWindowText(dmyBuf, 128);
		textSize = _tcslen(dmyBuf);
		dmyBuf[textSize] = '\0';
		dmyFacter = (float)atof(dmyBuf);
		if (0.001f <= dmyFacter && dmyFacter <= 1.000f)
			m_fFilterFacter = dmyFacter;
		break;

	case IDC_FILTERFACTERSN:
		m_editSnFilter.GetWindowText(dmyBuf, 128);
		textSize = _tcslen(dmyBuf);
		dmyBuf[textSize] = '\0';
		dmyFacter = (float)atof(dmyBuf);
		if (0.001f <= dmyFacter && dmyFacter <= 1.000f)
			m_fFilterFacterSn = dmyFacter;
		break;

	case IDC_FILTERFACTERLV:
		m_editLevelFilter.GetWindowText(dmyBuf, 128);
		textSize = _tcslen(dmyBuf);
		dmyBuf[textSize] = '\0';
		dmyFacter = (float)atof(dmyBuf);
		if (0.001f <= dmyFacter && dmyFacter <= 1.000f)
			m_fFilterFacterLv = dmyFacter;
		break;

	case IDC_FILTERFACTERPR:
		m_editVcFilter.GetWindowText(dmyBuf, 128);
		textSize = _tcslen(dmyBuf);
		dmyBuf[textSize] = '\0';
		dmyFacter = (float)atof(dmyBuf);
		if (0.001f <= dmyFacter && dmyFacter <= 1.000f)
			m_fFilterFacterPr = dmyFacter;
		break;
	}
}

// ▼ボタンでログファイルを開く
void CBatchSimDlg::OnBnClickedButton6()
{
	CLogSel2	dlg;
	int			i;

	// 表示が選択された場合
	if( IDOK == dlg.DoModal() )
	{
		m_nSelFile = dlg.m_nSelectNum;

		// ログリストを再セットアップ
		SetupLogList();
		SetSelectedTrendEntry(m_nSelFile);

		for(i = 0; i < NumInput; i++)
		{
			if( i < LastID-1)
			{
				m_stTdSel[i].fIndication = TRUE;
				m_stTdSel[i].arg = i;
			}
			else
			{
				m_stTdSel[i].fIndication = FALSE;
				m_stTdSel[i].arg = -1;
			}
		}
	}
	// 閉じるが選択された場合も再セットアップする
	else
	{
		// ログリストを再セットアップ
		SetupLogList();

		// 選択Noがトレンド数以上になってたら最後の項目を選択
		if( m_nSelFile >= m_nTrendNum )
			m_nSelFile = m_nTrendNum - 1;

		SetSelectedTrendEntry(m_nSelFile);
	}
}
