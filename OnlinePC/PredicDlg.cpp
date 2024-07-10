// PredicDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include <windowsx.h>
#include "OnlinePC.h"
#include "OnlinePCDlg.h"
#include "PredicDlg.h"
#include ".\predicdlg.h"


///////////////////////////////////////////////////////////////////////////////
// definition
static CConfig			&cnf		= theApp.m_cnfBoexe;
static CConfig			&cnfD		= theApp.m_cnfBoexeD;
static InputPoint		*inputPoint	= theApp.m_cnfBoexe.inputPoint;
static ADChannel		*adChannel	= theApp.m_cnfBoexe.adChannel;
static TCCombination	*tcCombination	= theApp.m_cnfBoexe.tcCombination;
static TCCombination	*tcCombinationD= theApp.m_cnfBoexeD.tcCombination;
static CBolog			&lg			= theApp.m_log;
static CChartLib		&clib		= theApp.m_boexeChart;
static CBoColor			&bocol		= theApp.m_col;
static CNetConfig		&netcnf		= theApp.m_netcnf;
static BOOL				*pbTcExp	= theApp.m_bTcExp;
static int				*pnCutOffTimer = theApp.m_nCutOffTimer;
static TcName			*tcnm		= &theApp.m_stTcNm;

#define	AXIS_WIDTH		47
#define MAXTEMP			3000
#define MINTEMP			0
#define	ERR_FLK_TIME	10

#define LogDataPresent  1						// status: the logging data present
#define LogDataAbsent   2						// status: the logging data absent
#define ChangeLog       3						// status: the file changed
#define ChargeChange    4						// status: the charge changed

#define	MC_MSG	(LPSTR)(LPCTSTR)

///////////////////////////////////////////////////////////////////////////////
// old sample save function 2008/01
SampledData	sampleOld[OLD_SMPL_NUM];
int			numSampleOld = 0;
int			idxSampleOld = 0;
void SaveOldData(SampledData *data);

// auto TC cutoff function
SampledData	sampleTCcutoff[TCEXP_CHK_TIME_MAX];
int			numSampleTCcutoff = 0;
int			idxSampleTCcutoff = 0;
void SaveTCcutoffData(SampledData *data);
BOOL IsTCcutoff(int nChNo);

// CPredicDlg ダイアログ
IMPLEMENT_DYNAMIC(CPredicDlg, CDialog)
CPredicDlg::CPredicDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPredicDlg::IDD, pParent)
{
	// initialize member data.
	m_nTimerID = -1;
	m_nTimerID2 = -1;
	m_nTimerID3 = -1;
	m_nPtnNo = -1;
	m_nMoldSize = 0;
	m_nRecvSteel = -1;
	m_nRunWaitCnt = 0;
	m_blManuSteel = FALSE;
	m_blPreVcOn = FALSE;
	m_blPreChargeChange = FALSE;
	m_blPreCastStart = FALSE;
	m_blPreL1Err = FALSE;
	m_blPreAutoLVCtl = FALSE;
	m_blPulseBOFetched = FALSE;
	m_blPulseBopCondition = FALSE;
	m_blReqBoPosClear = FALSE;
	m_nAllResult = 0;
	m_bRunWait	  = TRUE;
	m_blOnSteel		= FALSE;
	m_clAlarm = RGB(205,0,0);
	//m_nCastLength	= 0;
	//m_nMdCWTmp		= 0;
	//m_nMdCWFRFix	= 0;
	//m_nMdCWFRLoose	= 0;
	//m_nMdCWFREast	= 0;
	//m_nMdCWFRWest	= 0;
	m_nActSendCnt	= 0;
	m_pDlgTest		= NULL;
	m_pDlgTcView	= NULL;
	m_isFinish		= FALSE;
	m_bRunOnce		= FALSE;
	m_nMoldSize		= 0;
	m_nHealthy		= 0;
	m_isGCMode		= TRUE;
	m_isManMode		= FALSE;
	for(int i = 0; i < NumInput; i++)
	{
		if( i < LastID-1)
		{
//			m_stTdView[i].fIndication = TRUE;
			m_stTdView[i].arg = i;
			m_stTdSel[i].fIndication = TRUE;
			m_stTdSel[i].arg = i;
		}
		else
		{
//			m_stTdView[i].fIndication = FALSE;
			m_stTdView[i].arg = -1;
			m_stTdSel[i].fIndication = FALSE;
			m_stTdSel[i].arg = -1;
		}
	}
	memset(&m_stBsInfo, 0, sizeof(m_stBsInfo));
	memset(m_nAveArea, 0, sizeof(m_nAveArea));
	memset(m_nAveAreaA, 0, sizeof(m_nAveAreaA));
	memset(m_aveAreac, 0, sizeof(m_aveAreac));
	memset(m_nErrBf, 0, sizeof(m_nErrBf));

	// get shared memory pointer and clear memory.
	m_pShMem = GetSharedMemory();
	memset(m_pShMem, 0, sizeof(SharedMemory));

	// create dialog.
	Create(CPredicDlg::IDD, pParent);				
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
}

CPredicDlg::~CPredicDlg()
{
}

void CPredicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Y_SCALE, m_textYScale);
	DDX_Control(pDX, IDC_CUR_TIME, m_textCurTime);
	DDX_Control(pDX, IDC_MOLD_SIZE, m_textMdSize);
	DDX_Control(pDX, IDC_LIST1, m_listData);
	DDX_Control(pDX, IDC_PTN_NO, m_textPtnNo);
	DDX_Control(pDX, IDC_FILTERSW, m_chkFilter);
	DDX_Control(pDX, IDC_ALARMRESET, m_rdoResetAlm);
	DDX_Control(pDX, IDC_FIX_FL, m_textFixCwFlow);
	DDX_Control(pDX, IDC_FIX_FL2, m_textFixCwFlow2);
	DDX_Control(pDX, IDC_FIX_FL3, m_textFixCwFlow3);
	DDX_Control(pDX, IDC_LOOSE_FL, m_textLooseCwFlow);
	DDX_Control(pDX, IDC_EAST_FL, m_textEastCwFlow);
	DDX_Control(pDX, IDC_WEST_FL, m_textWestCwFlow);
	DDX_Control(pDX, IDC_CAST_LEN, m_textCastLen);
	DDX_Control(pDX, IDC_TD_TEMP, m_textMdCwTemp);
	DDX_Control(pDX, IDC_FILTERFACTER, m_editTempFilter);
	DDX_Control(pDX, IDC_FILTERFACTERSN, m_editSnFilter);
	DDX_Control(pDX, IDC_FILTERFACTERLV, m_editLevelFilter);
	DDX_Control(pDX, IDC_FILTERFACTERPR, m_editVcFilter);
	DDX_Control(pDX, IDC_PRD_MOLD, m_mold);
	DDX_Control(pDX, IDC_VCON_RDO, m_rdoVcOnIn);
	DDX_Control(pDX, IDC_CHCHG_RDO, m_rdoChChgIn);
	DDX_Control(pDX, IDC_MDLVC_RDO, m_rdoMdLvcIn);
	DDX_Control(pDX, IDC_TDCHG_RDO, m_rdoTdChgIn);
	DDX_Control(pDX, IDC_BOALM_RDO, m_rdoBoAlmOut);
	DDX_Control(pDX, IDC_BOALMC_RDO, m_rdoBoAlmcOut);
	DDX_Control(pDX, IDC_STROBEBO, m_chkStrobe);
	DDX_Control(pDX, IDC_ENABAUTOLC, m_chkLvCtrl);
	DDX_Control(pDX, IDC_MODE, m_mode);
	DDX_Control(pDX, IDC_AUTO_MODE, m_automode);
	DDX_Control(pDX, IDC_EDIT_BOMSGF, m_textBoMsgFirst);
	DDX_Control(pDX, IDC_EDIT_BOMSGF2, m_textBoMsgFirstD);
	DDX_Control(pDX, IDC_CAST_VC, m_textCastVc);
	DDX_Control(pDX, IDC_ACT_LEVEL, m_textActLevel);
	DDX_Control(pDX, IDC_TD_WEIGHT, m_textTdWeight);
	DDX_Control(pDX, IDC_EMBR, m_textEMBr);
	DDX_Control(pDX, IDC_AUTOTCCUT, m_chkAutoCutTC);
	DDX_Control(pDX, IDC_BUTTON1, m_btnAutoChg);
	DDX_Control(pDX, IDC_BUTTON2, m_btnManChg);
}

BEGIN_MESSAGE_MAP(CPredicDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_WM_PAINT()
	ON_COMMAND(IDM_AXISSET, OnAxisset)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(IDM_TRENDCHG, OnTrendchg)
//	ON_COMMAND(IDM_TRENDVIEW, OnTrendview)
	ON_COMMAND(IDM_SELSTEELKIND, OnSelsteelkind)
	ON_COMMAND(IDM_INDIVIDUAL, OnIndividual)
	ON_UPDATE_COMMAND_UI(IDM_SELSTEELKIND, OnUpdateSelsteelkind)
	ON_BN_CLICKED(IDC_FILTERSW, OnClickedFilterSw)
	ON_BN_CLICKED(IDC_ALARMRESET, OnClickedResetAlarm)
	ON_WM_MOVE()
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(IDM_TCPOSITION, OnTcposition)
	ON_MESSAGE( WM_RESTART_BOP, OnRestartBOP )
	ON_MESSAGE( WM_MANU_TC_CUT, OnManualCutoffTC )
	ON_MESSAGE( WM_MESSAGE_OUT, OnMessageOut )
	ON_BN_CLICKED(IDC_BUTTON2, &CPredicDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CPredicDlg メッセージ ハンドラ

//It evades ending by Enter Key.
void CPredicDlg::OnOK(){}

//It evades ending by ESC Key.
void CPredicDlg::OnCancel(){}

void CPredicDlg::PostNcDestroy() 
{
	delete this;
//	CDialog::PostNcDestroy();
}

// WM_DESTROY proc.
void CPredicDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if( m_nTimerID != -1 )
		KillTimer(m_nTimerID);
	if( m_nTimerID2 != -1 )
		KillTimer(m_nTimerID2);
#ifdef USE_DEBUG_TIMER
	if( m_nTimerID3 != -1 )
		KillTimer(m_nTimerID3);
#endif

	m_rc.Uninitialize();

	// config data up date
	ConfDataUpDate();
}

// WM_PAINT proc.
void CPredicDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	DrawYScale(dc);
	//DrawFixMold(dc);
}

// WM_MOVE
void CPredicDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

#ifdef FIX_SCREEN
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
#endif
}

// WM_INITDIALOG proc.
BOOL CPredicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// initialize the B.O. prediction condition.
	SetupBOCondition();

	//-------------------------------------------------------------------------
	// setup data view list
	//-------------------------------------------------------------------------
	CFont	newFont, *font;
	LOGFONT	lf;

	// 自動･手動切替ボタンの初期色設定
	m_btnAutoChg.SetBackColor(RGB(255,255,0));
	m_btnManChg.SetBackColor(RGB(200,200,200));

	// 幅１、高さ24のイメージを持つ
	m_image.Create(1, 24, ILC_COLOR, 0, 0);

	// 高さ調整用のイメージリストを設定する
	m_listData.SetImageList(&m_image, LVSIL_STATE);
	CHeaderCtrl	*ctl = m_listData.GetHeaderCtrl();
	if( ctl != NULL ) ctl->SetImageList(&m_image);

	// set list style
	m_listData.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_listData.DeleteAllItems();
	m_listData.InsertColumn(0, _T(""), LVCFMT_LEFT, 0);
	m_listData.InsertColumn(1, _T(""), LVCFMT_CENTER, 120);
	m_listData.InsertColumn(2, _T(""), LVCFMT_RIGHT, 65);
	m_listData.InsertColumn(3, _T(""), LVCFMT_CENTER, 120);
	m_listData.InsertColumn(4, _T(""), LVCFMT_RIGHT, 65);
//	m_listData.InsertColumn(5, _T(""), LVCFMT_RIGHT, 65);
//	m_listData.InsertColumn(6, _T(""), LVCFMT_RIGHT, 65);

	// change list font
	font = m_listData.GetFont();
	font->GetLogFont(&lf);
	lf.lfHeight = -14;
	lf.lfWidth = 0;
	newFont.CreateFontIndirect(&lf);
	m_listData.SetFont(&newFont);

	// setup data list
	SetupTdViewListItem();

	//-------------------------------------------------------------------------
	// create message window
	//-------------------------------------------------------------------------
	CRect	rc;
	CWnd	*wnd = GetDlgItem(IDC_BO_MSG);
	wnd->GetClientRect(&rc);
	wnd->MapWindowPoints(this, &rc);
	m_msg.m_nNumRow = 28;	// customize line
	m_msg.CreateEx(WS_EX_WINDOWEDGE, NULL, MSGWINDOW, WS_CHILDWINDOW|WS_VISIBLE|WS_BORDER,rc,this,IDC_BO_MSG);
	m_msg.ShowWindow(SW_SHOW);
	m_msg.SetLogDays(30);
	m_msg.SetLog(TRUE);

	//-------------------------------------------------------------------------
	// customize chart library
	//-------------------------------------------------------------------------
	clib.ChartMeshXCnt = 20;
	clib.ChartMeshYCnt = 15;

	//-------------------------------------------------------------------------
	// create graph window
	//-------------------------------------------------------------------------
	wnd = GetDlgItem(IDC_PRD_GRAPH_WIN);
	wnd->GetClientRect(&rc);
	wnd->MapWindowPoints(this, &rc);
	m_graph.CreateEx(WS_EX_WINDOWEDGE, NULL, _T("BoexeGraph"), WS_CHILDWINDOW|WS_VISIBLE|WS_BORDER,rc,this,IDC_PRD_GRAPH_WIN);
	m_graph.ShowWindow(SW_SHOW);

	// reset boexe graph
	ResetBoexeGraph();

	//-------------------------------------------------------------------------
	// setup mold
	//-------------------------------------------------------------------------
	// get mold area
//	m_mold.GetClientRect(&m_rcMold);
//	m_mold.MapWindowPoints(this, &m_rcMold);

	//-------------------------------------------------------------------------
	// setup log header
	//-------------------------------------------------------------------------
	DataFileHeader	header;
	int	ch, arg;

	// set the sampler version.
	header.header.version		= BOSystemVer;

	// set sample period.
	header.header.samplePeriod	= (short)SamplingPeriod;

	// set mold width.
//	header.header.moldWidth		= 0;

	// set sample remark
	memset( header.header.remark, 0, sizeof(header.header.remark) );

	// for each input argument,
	for (arg = 0; arg < NumInput; arg++) {
		// store the assigned A/D channel ID.
		header.header.adCh[arg]	= (short)inputPoint[arg].adCh;
	}

	// for each A/D channel, */
	for (ch = 0; ch < NumADChannel; ch++) {
		// store the configured input argument ID.
		header.header.arg[ch]		= (short)adChannel[ch].arg;
	}

	// B.O.パターン初期値
	header.header.nPtnNo = -1;

	// setup data header
	m_logging.SetupDataHeader(&header);

	//-------------------------------------------------------------------------
	// setup steel kind
	//-------------------------------------------------------------------------
	m_nRecvSteel = m_nPtnNo	= 0;

#if 0
	CPtnSel		dlg;
	int			nPtnNo;

	dlg.SetParam(cnf, m_nRecvSteel, m_nMoldSize);
	if( IDOK == dlg.DoModal() )
	{
		dlg.GetParam(&nPtnNo, &m_nMoldSize);
		m_nRecvSteel = m_nPtnNo = nPtnNo;
	}
#endif

	CString	str;
	cnf.ReadBosysInfo(m_nPtnNo, &m_stBsInfo);
	m_textPtnNo.SetWindowText(m_stBsInfo.szComment);
	str.Format(_T("%d"), m_nMoldSize);
	m_textMdSize.SetWindowText(str);

	//-------------------------------------------------------------------------
	// setup test dialog.
	//-------------------------------------------------------------------------
	m_pDlgTest = new CTestDlg(this);
	if( m_pDlgTest != NULL )
		m_pDlgTest->ShowWindow(SW_HIDE);

	//-------------------------------------------------------------------------
	// start bo.
	//-------------------------------------------------------------------------
	// start log
	m_logging.LogStart();

	// load network configuration
	netcnf.LoadConfig();

	// check auto cut tc
	m_chkAutoCutTC.SetCheck(1);

	// read tc exp data
//	cnf.ReadTcExpFile(pbTcExp);

	// initialize realtime.
	m_rc.Initialize(m_nPtnNo);

	// start realtime procedure
	int ret = SetupTickTimer();
	if( !ret )
	{
		AfxMessageBox(IDS_EXEC_MSG11);
	}

	// start internal timer.
	m_nTimerID = SetTimer(1, TIMER_FIRST_CYCLE, NULL);
	m_nTimerID2 = SetTimer(2, TIMER_SLOW_CYCLE, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL CPredicDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (GET_WM_COMMAND_ID(wParam, lParam)) 
	{
	case IDC_STROBEBO: 
	case IDC_ENABAUTOLC:  
	case IDC_FILTERSW:
	case IDC_FILTERFACTER:
	case IDC_FILTERFACTERSN:
	case IDC_FILTERFACTERLV:
	case IDC_FILTERFACTERPR:
	case IDC_FILTERFACTERWD:
	case IDC_AUTOTCCUT:
		// update the execution condition.
		BoConditionManage(GET_WM_COMMAND_ID(wParam, lParam), -1);
		break;
	}

	return CDialog::OnCommand(wParam, lParam);
}

// WM_TIMER proc.
void CPredicDlg::OnTimer(UINT nIDEvent)
{
	DataFileHeader	header;
	StSampledData	sampleData;
	CString	str;
	WORD	wData[10];
	int		pData[NumADChannel];
	int		nStatus, arg, ch;

	// fast timer
	if( nIDEvent == m_nTimerID )
	{
////////////////////////////////////////////////////////////////////////////////////////////
// 
		// When receiving the steel species which is different in 
		// the condition which isn't manually chosen.
		if( m_blOnSteel )
		{
			m_blOnSteel = FALSE;
			SYSTEMTIME		st;
			CString			timeStr;

			// change time log
			if( m_nPtnNo != m_nRecvSteel )
			{
				// current time message
				GetLocalTime(&st);
				timeStr.Format(_T("\'%2d.%2d.%2d %02d:%02d:%02d"),
					st.wYear % 100, st.wMonth, st.wDay,
					st.wHour, st.wMinute, st.wSecond);
				str.LoadString(IDS_EXEC_MSG13);
				m_msg.Append((LPSTR)(LPCTSTR)str, m_nPtnNo+1, m_nRecvSteel+1, timeStr);
			}
			m_nPtnNo = m_nRecvSteel;

			// get current bosys information for display.
			cnf.ReadBosysInfo(m_nPtnNo, &m_stBsInfo);
			SetPtnName(m_stBsInfo.szComment);

			// stop realtime.
			m_rc.StopBOP();

			// wait realtime.
			Sleep(500);

			// 学習一時ファイル操作同期区間開始------------
			EnterCriticalSection(&theApp.m_lnLock);

			// reset boexe info.
			m_rc.ResetBopExe(m_nPtnNo);

			// 学習一時ファイル操作同期区間終了------------
			LeaveCriticalSection(&theApp.m_lnLock);

			// save adchannel
			SaveExtTC(m_nPtnNo);

			// for each input argument,
			for (arg = 0; arg < NumInput; arg++) {
				// store the assigned A/D channel ID.
				header.header.adCh[arg]	= (short)inputPoint[arg].adCh;
			}

			// for each A/D channel, */
			for (ch = 0; ch < NumADChannel; ch++) {
				// store the configured input argument ID.
				header.header.arg[ch]		= (short)adChannel[ch].arg;
			}

			// setup data header
			if( m_pShMem != NULL && m_pShMem->isVcOn )
				m_logging.SaveInputInfo(&header);

			// チャンネル変更（グラフ）
			ChangeChArgTm();

			// start realtime.
			m_rc.StartBOP();
		}

		// while the sampled data are in the logging data buffer,
		for ( ; ; ) 
		{
			// run wait
			if( m_nRunWaitCnt > 3 )
			{
				if( m_bRunWait )
					m_bRunOnce = TRUE;
				m_bRunWait = FALSE;
			}
			else
				m_nRunWaitCnt++;

			memset(m_blResultData, 0, sizeof(m_blResultData));
			memset(m_blResultDataD, 0, sizeof(m_blResultDataD));
			m_pResultMap[0] = m_blResultData;
			m_pResultMap[1] = m_blResultDataD;

			// get the logging data from buffer.
			nStatus = m_logging.GetLogData(&sampleData, m_blResultData, m_blResultDataD, &m_nAllResult);
			if( nStatus == LogDataAbsent || m_bRunWait )
				break;
			// if the status was the request for a new logging file, */
			else if(nStatus == ChangeLog) 
			{
				// create a new logging data file.
/* 2008/01		m_logging.CreateLogFile();
				if (!m_blPreVcOn && m_pShMem->isVcOn)
					// clear chart plane.
					m_graph.PlotReset();
*/
				if( !m_blPreVcOn && m_pShMem->isVcOn )
				{
					// create a new logging data file and save old data.
					m_logging.CreateLogFile(sampleOld, numSampleOld, idxSampleOld);
					// clear chart plane.
//					m_graph.PlotReset();
					// clear file name
					m_logtc.ClearFileName();
				}
				else
				{
					// 現在のパターンNo情報を書き込む
					m_logging.SaveHeatInfo(m_nPtnNo);

					// create a new logging data file.
					m_logging.CreateLogFile();
				}

				// update the execution condition.
//				if (m_blPreVcOn)
//					BoConditionManage(IDC_CHARGEONOFF, FALSE);

				m_blOnSteel = TRUE;
			}
			// otherwise,
			else {
				// save old data 2008/01
				SaveOldData(&sampleData.sd_raw);

				// save TC cutoff Data
				SaveTCcutoffData(&sampleData.sd);

				// write the data to the logging data file.
				if( m_pShMem->isVcOn )
					m_logging.WriteData(&sampleData.sd_raw);

				// check and create file name
				if( m_pShMem->isVcOn )
					m_logtc.CheckAndCreateFileName();

				// vc offでTC除外情報保存
				if( m_blPreVcOn && !m_pShMem->isVcOn )
				{
					// 現在のパターンNo情報を書き込む
					m_logging.SaveHeatInfo(m_nPtnNo);

					// 除外TC情報保存
					SaveExtTC(m_nPtnNo);
				}
				// auto cut-off check of exception tc.
				CheckAutoCutOffTC(&sampleData.sd);

				// limit chart data
				LimitSampleData(&sampleData.sd);

				// draw chart plane.
				m_graph.AppendData(&sampleData.sd);
				m_graph.Invalidate(FALSE);

				// draw Trend View.
				SetTdViewListData(&sampleData.sd);

				// calc average of temperature data
				SetAverageData(&sampleData.sd, pData);

				// limit temperature data for 3D and map graph.
				LimitTempData(pData);

				// 鋼種自動切替えの場合は
				if( m_isGCMode )
				{
					if( sampleData.vd.nPtnNo >= 0 && sampleData.vd.nPtnNo < NumSteelKind )
					{
						// 起動時初回は強制鋼種切り替え
						if( m_bRunOnce )
							m_blOnSteel = TRUE;
						m_bRunOnce = FALSE;
						m_nRecvSteel = sampleData.vd.nPtnNo;

						// パターンが変更されたなら鋼種切り替え'11/04/08
						if( m_nPtnNo != m_nRecvSteel )
							m_blOnSteel = TRUE;
					}
				}
				else
				{
					sampleData.vd.nPtnNo = m_nRecvSteel;
				}

				// B.O.メイン監視画面へのデータ送信
				COnlinePCDlg *dlg = (COnlinePCDlg *)AfxGetMainWnd();
				if( dlg->m_pDlgBoView != NULL ){
					dlg->m_pDlgBoView->SendMessage(WM_SMPLDATA, (WPARAM)m_pResultMap, (LPARAM)&sampleData);
				}
				if( dlg->m_pDlgChart3d != NULL ){
					dlg->m_pDlgChart3d->SendMessage(WM_CHARTDATA, 0, (LPARAM)pData);
				}
				if( dlg->m_pDlgTMap != NULL ){
					dlg->m_pDlgTMap->SendMessage(WM_MAPDATA, 0, (LPARAM)pData);
				}

				// 疑似信号ありなら
				if( m_pShMem->isEnabSimSignal )
				{
					// PLC通信処理にデータを送信
					wData[0] = (WORD)dlg->m_plccomm.m_stNetInfo.dwAddrW;
					wData[1] = (((m_pShMem->outBOFetched & 0x01) == 0x01) ||
								m_pShMem->simBocOut );		// B.O.警報
					wData[2] = (((m_pShMem->outBOFetched & 0x08) == 0x08) ||
								m_pShMem->simBoOut );		// ﾃﾞﾌﾟﾚｯｼｮﾝ警報
					wData[3] = (m_pShMem->BopCondition ||
								m_pShMem->simBoRun );		// B.O.予知装置稼働中
					wData[4] = m_pShMem->outPlcErrReset;	// PLC異常ﾘｾｯﾄ
				}
				else
				{
					// PLC通信処理にデータを送信
					wData[0] = (WORD)dlg->m_plccomm.m_stNetInfo.dwAddrW;
					wData[1] = ((m_pShMem->outBOFetched & 0x01) == 0x01);	// B.O.警報
					wData[2] = ((m_pShMem->outBOFetched & 0x08) == 0x08);	// ﾃﾞﾌﾟﾚｯｼｮﾝ警報
					wData[3] = m_pShMem->BopCondition;						// B.O.予知装置稼働中
					wData[4] = m_pShMem->outPlcErrReset;					// PLC異常ﾘｾｯﾄ
				}
				m_pShMem->outPlcErrReset = 0;	// 異常ﾘｾｯﾄ要求OFF
				SendPlc(idCmdSend, wData, sizeof(WORD)*5);

				// 通信ハートビート更新
				m_nHealthy = (m_nHealthy + 1) % 30000;

				// get value
				m_info.dTdWeight	= sampleData.vd.nTdWeidht;
				m_info.dLen			= sampleData.vd.nCastLengthC;
				m_info.dCoolWtFlow	= sampleData.vd.nMdCoolWtA;
				m_info.dCoolWtFlow2	= sampleData.vd.nMdCoolWtC;
				m_info.dCoolWtFlow3	= sampleData.vd.nMdCoolWtD;
				m_info.dCoolWtTemp	= sampleData.vd.nMdCoolWtTempD;

				// 実績データ表示
				DispActData();

				// vc offでLogTC用ファイル名をクリア
				if( m_blPreVcOn && !m_pShMem->isVcOn )
				{
					// clear file name
					m_logtc.ClearFileName();
				}

				// test the neural networks output.
				TestBOStatus(m_blResultData, m_blResultDataD, m_nAllResult, &sampleData.ft);
			}

			// test if B.O. occured,
			if (m_pShMem->isBOFetched) 
			{
				if( m_nAllResult != 0 )
				{
					m_graph.SetFetchColor(RGB(205,0,0));

					// display occurring B.O.
					m_graph.Fetch();
				}
				// clear the status.
				m_pShMem->isBOFetched = FALSE;
			}
		}// for

		// test the neural networks output.
		TestBOStatus(NULL, NULL, m_nAllResult);

////////////////////////////////////////////////////////////////////////////////////////////
// 
	}

	// slow timer
	if( nIDEvent == m_nTimerID2 )
	{
		// set current time.
		SetCurrentTime();

		// draw current time.
		DispActData();

		// set on/off button state.
		ButtomOnOff();

		// check off reset alarm button.
		CheckAlarm();

		SetMoldSize();

		// display mode
		if( m_pShMem->isEnabSimSignal == 0 )
			m_mode.ShowWindow(SW_HIDE);
		else
			m_mode.ShowWindow(SW_SHOW);

		// display mode
		if( m_isGCMode )
		{
			m_automode.SetWindowTextA(_T("自動"));
			m_automode.SetBackColor(RGB(0,255,0));
			m_automode.SetTextColor(RGB(0,0,0));
		}
		else
		{
			m_automode.SetWindowTextA(_T("手動"));
			m_automode.SetBackColor(RGB(255,255,0));
			m_automode.SetTextColor(RGB(0,0,0));
		}
	}
///////////////////////////////////////////////////////////////////////////////
#ifdef USE_DEBUG_TIMER
	// real timer for debug
	if( nIDEvent == m_nTimerID3 )
	{
		m_rc.TimeCallBack(0, 0, (DWORD)(SharedMemory *)m_pShMem, 0, 0);
	}
#endif

	CDialog::OnTimer(nIDEvent);
}

// selected axis config of menu.
void CPredicDlg::OnAxisset()
{
	CBoexeAxis		dlg;
	StGraphParam	prm;

	if( IDOK == dlg.DoModal() )
	{
		CRect	rc;
		StChartAxisParam	cp;

		cp.timeAxis		= cnf.m_stTimeAxis;
		cp.tempAxis		= cnf.m_stTempAxis;
		cp.levelAxis	= cnf.m_stLevelAxis;
		cp.vcAxis		= cnf.m_stVcAxis;
		cp.snAxis		= cnf.m_stSNAxis;
//		cp.widthAxis	= cnf.m_stWidthAxis;
		memcpy( &prm.timeAxis,	&cp.timeAxis,	sizeof(prm.timeAxis) );
 		memcpy( &prm.tempAxis,	&cp.tempAxis,	sizeof(prm.tempAxis) );
 		memcpy( &prm.levelAxis,	&cp.levelAxis,	sizeof(prm.levelAxis) );
 		memcpy( &prm.vcAxis,	&cp.vcAxis,		sizeof(prm.vcAxis) );
		memcpy( &prm.snAxis,	&cp.snAxis,		sizeof(prm.snAxis) );
 //		memcpy( &prm.widthAxis,	&cp.widthAxis,	sizeof(prm.widthAxis) );
		m_graph.ChangeAxis(&prm);
		m_textYScale.GetClientRect(&rc);
		m_textYScale.MapWindowPoints(this, &rc);
		InvalidateRect(&rc);
	}
}

// selected trend change of menu.
void CPredicDlg::OnTrendchg()
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

//// selected trend view of menu.
//void CPredicDlg::OnTrendview()
//{
//	CTrendView	dlg;
//
//	dlg.SetTrendSelect(m_stTdView);
//	if( IDOK == dlg.DoModal() )
//	{
//		dlg.GetTrendSelect(m_stTdView);
//		SetupTdViewListItem();
//	}
//}

void CPredicDlg::OnUpdateSelsteelkind(CCmdUI *pCmdUI)
{
	//if( m_pShMem->isEnabSimSignal == 0 )
	//	pCmdUI->Enable(FALSE);
	//else
		pCmdUI->Enable(TRUE);
}

void CPredicDlg::OnSelsteelkind()
{
	CPtnSel99	dlg;
	CString		str;
	CRect		rc;
	int			nPtnNo;

	if( m_isGCMode == TRUE )
		return;

	dlg.SetParam(cnf, m_nRecvSteel);
	if( IDOK == dlg.DoModal() )
	{
		dlg.GetParam(&nPtnNo);
		m_nRecvSteel = nPtnNo;

		// パターンが変更されたなら鋼種切り替え'11/04/08
		if( m_nPtnNo != m_nRecvSteel && m_isGCMode == FALSE )
			m_blOnSteel = TRUE;
	}
}

void CPredicDlg::SetPtnName(LPCTSTR pszName)
{
	CRect	rc;

	m_textPtnNo.SetWindowText(pszName);
	m_textPtnNo.GetClientRect(&rc);
	m_textPtnNo.MapWindowPoints(this, &rc);
	InvalidateRect(&rc, TRUE);
}

// pushed 'individual' of menu
void CPredicDlg::OnIndividual()
{
	if( m_pDlgTest != NULL )
		m_pDlgTest->ShowWindow(SW_SHOW);
}

// clicked filter sw button.
void CPredicDlg::OnClickedFilterSw()
{
	m_pShMem->filterSwitch = !m_pShMem->filterSwitch;
	m_chkFilter.SetCheck(m_pShMem->filterSwitch);
}

// clicked reset alarm button.
void CPredicDlg::OnClickedResetAlarm()
{
	if (m_pShMem->outBOFetched) {
		m_rdoResetAlm.SetCheck(BST_CHECKED);
		m_pShMem->inAlarmReset = 1;
	}
}

// selected 'TC Position' menu.
void CPredicDlg::OnTcposition()
{
	if( m_pDlgTcView == NULL )
	{
		m_pDlgTcView = new CTcVwDlg(this);
	}
	m_pDlgTcView->ShowWindow(SW_SHOW);
}

BOOL CPredicDlg::PreTranslateMessage(MSG* pMsg)
{
	return CDialog::PreTranslateMessage(pMsg);
}

// restart BOP
LRESULT CPredicDlg::OnRestartBOP(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR			pStr = (LPCTSTR)lParam;
	DataFileHeader	header;
	int				ch, arg;

	if( pStr == NULL )
		return 0;

	// get current bosys information for display.
//	if( _tcscmp(pStr, cnf.steel[m_nPtnNo].confName) == 0 )
	if( _tcscmp(pStr, m_stBsInfo.szConfName) == 0 )
	{
		// stop realtime.
		m_rc.StopBOP();

		// wait realtime.
		Sleep(500);

		// reset boexe info.
		m_rc.ResetBopExe(m_nPtnNo);

		// save adchannel
		SaveExtTC(m_nPtnNo);

		// for each input argument,
		for (arg = 0; arg < NumInput; arg++) {
			// store the assigned A/D channel ID.
			header.header.adCh[arg]	= (short)inputPoint[arg].adCh;
		}

		// for each A/D channel, */
		for (ch = 0; ch < NumADChannel; ch++) {
			// store the configured input argument ID.
			header.header.arg[ch]		= (short)adChannel[ch].arg;
		}

		// setup data header
		if( m_pShMem != NULL && m_pShMem->isVcOn )
			m_logging.SaveInputInfo(&header);

		// チャンネル変更（グラフ）
		ChangeChArgTm();

		// start realtime.
		m_rc.StartBOP();
	}

	return 0;
}

// manual cutoff TC message
LRESULT CPredicDlg::OnManualCutoffTC(WPARAM wParam, LPARAM lParam)
{
	int		nTcNo	= (int)wParam;
	BOOL	bCut	= (BOOL)lParam;
	CString	timeStr;
	int		nKind;

	if( nTcNo < 0 || nTcNo >= NumTCTap )
		return 0;

	// Is kind?
	if( bCut )
		nKind = 1;
	else
		nKind = 2;

	// get current time string
	timeStr = GetTimeStr();

	// write logTC file
	m_logtc.WriteLogFile(nKind, nTcNo, nTcNo, timeStr);

	// screen message
	switch(nKind)
	{
	case 1:
		m_msg.Append( _T(" %s %s, 手動カットオフ\n"), timeStr, tcnm->name[nTcNo] );
		break;
	case 2:
		m_msg.Append( _T(" %s %s, 手動復旧\n"), timeStr, tcnm->name[nTcNo] );
		break;
	}

	return 0;
}

//// message out
LRESULT CPredicDlg::OnMessageOut(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR		pMsg = (LPCTSTR)lParam;
	CString		timeStr;

	timeStr = GetTimeStr();
	m_msg.Append( _T(" %s %s\n"), timeStr, pMsg );

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// CBoexeSimDlg member function

// reset batch graph
void CPredicDlg::ResetBoexeGraph(void)
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

// draw Y scale
void CPredicDlg::DrawYScale(CDC &dc)
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
	int idn[] = {IDS_DEF_TEMP, IDS_DEF_SN, IDS_DEF_LEVEL, IDS_DEF_VC};
	int idu[] = {IDS_DEF_TEMP_UNIT, IDS_DEF_SN_UNIT, IDS_DEF_LEVEL_UNIT, IDS_DEF_VC_UNIT};
	CString strFmt[] = {_T("%3.1f"), _T("%3.0f"), _T("%3.1f"), _T("%3.2f")};
	ChartAxis	*cas[] = {&cnf.m_stTempAxis, &cnf.m_stSNAxis, &cnf.m_stLevelAxis, &cnf.m_stVcAxis}; 
	for( i = 0; i < 4; i++ )
	{
		strName.LoadString(idn[i]);
		strTmp.LoadString(idu[i]);
		strUnit.Format(_T("[%s]"), strTmp); 
		clib.DrawYAxis(dc, strName, strUnit,strFmt[i], cas[i], &newFont, clib.m_clText, 
			rcYScale.left + AXIS_WIDTH*i , rcGraph.top, clib.ChartHeight );
	}
}

// set current time.
void CPredicDlg::SetCurrentTime(void)
{
	static int		nStepCnt = 0;
	static time_t	preTime;
	time_t			t;
//	_TCHAR			buf[64];
	CString			str;

	// if the request is redrawing window,
	if(nStepCnt == 0)
	{
		// get current time.
		t = time(NULL);
		if (t != preTime) {
			// make up the display string.
//			_tcscpy(buf, _tctime(&t)),	buf[_tcslen(buf) - 1] = _T('\0');

			// clear displaying area.
			m_textCurTime.SetWindowText(GetTimeStr(t));

			// update current time.
			preTime	= t;
		}
	}
	nStepCnt = (nStepCnt + 1) % 5;
}

// set mold size.
void CPredicDlg::SetMoldSize(void)
{
	CString	strMoldSize;
	CRect	rc;

	strMoldSize.Format("%d", m_pShMem->moldSize);
	m_textMdSize.SetWindowText(strMoldSize);
	m_textMdSize.GetClientRect(&rc);
	m_textMdSize.MapWindowPoints(this, &rc);
	InvalidateRect(&rc, TRUE);
}

// 右下の数値表示タイトル表示
void CPredicDlg::SetupTdViewListItem(void)
{
	CString	str;
	int		i, j;

	m_listData.DeleteAllItems();
	for( i = 0, j = 0; i < LastID-1; i++ )
	{
//		if( m_stTdView[i].fIndication == TRUE )
//		{
			if( j % 2 == 0 )
				m_listData.InsertItem(j/2, _T(""));
			switch(m_stTdView[i].arg)
			{
			case SNID:		str.Format(_T("SN開度"));break;
			case LevelID:	str.Format(_T("レベル"));break;
			case VcID:		str.Format(_T("鋳込速度"));break;
			//case WidthID:	str.Format(_T("MD幅"));break;
//			case WidthLID:	str.Format(_T("WDL"));break;
//			case WidthRID:	str.Format(_T("WDR"));break;
			case LengthID:	str.Format(_T("鋳込長"));break;
			default:
				if( m_stTdView[i].arg >= 0 && m_stTdView[i].arg < NumTCTap )
					str.Format(_T("%s"), tcnm->name[m_stTdView[i].arg]);
				else
					str = _T("");
				break;
			}
			m_listData.SetItemText(j/2, (j%2)*2+1, str);
			j++;
//		}
	}
}

// 右下の数値表示
void CPredicDlg::SetTdViewListData(SampledData *pDat)
{
	CString	str;
	int		i, j, ch;

	for( i = 0, j = 0; i < LastID-1; i++ )
	{
//		if( m_stTdView[i].fIndication == TRUE )
//		{
			ch	= cnf.inputPoint[i].adCh;
			if( ch != -1 )
			{
				switch(m_stTdView[i].arg)
				{
				case SNID:		str.Format(_T("%.0lf"), pDat->val[ch] * PhisicalSNUnit);break;
				case LevelID:	str.Format(_T("%.1lf"), pDat->val[ch] * PhisicalLevelUnit);break;
				case VcID:		str.Format(_T("%.2lf"), pDat->val[ch] * PhisicalVcUnit);break;
				//case WidthID:	str.Format(_T("%.0lf"), pDat->val[ch] * PhisicalWidthUnit);break;
	//			case WidthLID:	str.Format(_T("%.0lf"), pDat->val[ch] * PhisicalWidthUnit);break;
	//			case WidthRID:	str.Format(_T("%.0lf"), pDat->val[ch] * PhisicalWidthUnit);break;
				case LengthID:	str.Format(_T("%.2lf"), pDat->val[ch] * PhisicalLengthUnit);break;
				default:		str.Format(_T("%.1lf"), pDat->val[ch] * PhisicalTempUnit);break;
				}
			}
			else
				str.Format(_T(""));
			m_listData.SetItemText(j/2, (j%2)*2+2, str);
			j++;
//		}
	}
}

// 表示データのリミットチェック
void CPredicDlg::LimitSampleData(SampledData *pDat)
{
	ADChannel	*ap;
	double		dMax, dMin;
	int			i;

	// for each A/D channel,
	for(i = 0, ap = adChannel; i < NumADChannel; i++, ap++)
	{
		// if this channel is not configured, do nothing.
		if(ap->arg < 0 || ap->arg >= LastID)	continue;

		dMax = ap->max;
		dMin = ap->min;
		switch(ap->arg)
		{
		case SNID:		dMax /= PhisicalSNUnit;		dMin /= PhisicalSNUnit;		break;
		case LevelID:	dMax /= PhisicalLevelUnit;	dMin /= PhisicalLevelUnit;	break;
		case VcID:		dMax /= PhisicalVcUnit;		dMin /= PhisicalVcUnit;		break;
		//case WidthID:	dMax /= PhisicalWidthUnit;	dMin /= PhisicalWidthUnit;	break;
//		case WidthLID:	dMax /= PhisicalWidthUnit;	dMin /= PhisicalWidthUnit;	break;
//		case WidthRID:	dMax /= PhisicalWidthUnit;	dMin /= PhisicalWidthUnit;	break;
//		case LengthID:	dMax /= PhisicalLengthUnit;	dMin /= PhisicalLengthUnit;	break;
		case LengthID:	break; //何もしない
		default:		dMax /= PhisicalTempUnit;	dMin /= PhisicalTempUnit;	break;
		}
		if( (double)pDat->val[i] > dMax )
			pDat->val[i] = (short)dMax;
		if( (double)pDat->val[i] < dMin )
			pDat->val[i] = (short)dMin;
	}
}

// 実績データの表示
void CPredicDlg::DispActData(void)
{
	CString	str;

	// disp active data
	str.Format(_T("%.2lf"), m_info.dTdWeight * 0.01 );
	m_textTdWeight.SetWindowText(str);
	str.Format(_T("%.2lf"), m_info.dLen/100.0 );
	m_textCastLen.SetWindowText(str);
	str.Format(_T("%.0lf"), m_info.dCoolWtFlow );
	m_textFixCwFlow.SetWindowText(str);
	str.Format(_T("%.0lf"), m_info.dCoolWtFlow2 );
	m_textFixCwFlow2.SetWindowText(str);
	str.Format(_T("%.0lf"), m_info.dCoolWtFlow3 );
	m_textFixCwFlow3.SetWindowText(str);
	str.Format(_T("%.0f"), m_info.dCoolWtTemp );
	m_textMdCwTemp.SetWindowText(str);
}

// update the execution condition.
void CPredicDlg::BoConditionManage(WPARAM wParam, int cmd)
{
	static char dmyBuf[128];
	static int	textSize;
	static float dmyFacter;
//	float		dmyFlowRate;
	CString		msg, timeStr;

	switch(wParam) {
	case IDC_STROBEBO:
		if (cmd < 0)
			m_pShMem->isStrobeBO			= !m_pShMem->isStrobeBO;
		break;
	case IDC_ENABAUTOLC:
		if (cmd < 0)
			m_pShMem->isEnabLevelCtl		= !m_pShMem->isEnabLevelCtl;
		break;
	case IDC_FILTERFACTER:
		m_editTempFilter.GetWindowText(dmyBuf, 128);
		textSize = _tcslen(dmyBuf);
		dmyBuf[textSize] = '\0';
		dmyFacter = (float)atof(dmyBuf);
		if (0.001f <= dmyFacter && dmyFacter <= 1.000f)
			m_pShMem->filterFacter = dmyFacter;
		break;

	case IDC_FILTERFACTERSN:
		m_editSnFilter.GetWindowText(dmyBuf, 128);
		textSize = _tcslen(dmyBuf);
		dmyBuf[textSize] = '\0';
		dmyFacter = (float)atof(dmyBuf);
		if (0.001f <= dmyFacter && dmyFacter <= 1.000f)
			m_pShMem->filterFacterSn = dmyFacter;
		break;

	case IDC_FILTERFACTERLV:
		m_editLevelFilter.GetWindowText(dmyBuf, 128);
		textSize = _tcslen(dmyBuf);
		dmyBuf[textSize] = '\0';
		dmyFacter = (float)atof(dmyBuf);
		if (0.001f <= dmyFacter && dmyFacter <= 1.000f)
			m_pShMem->filterFacterLv = dmyFacter;
		break;

	case IDC_FILTERFACTERPR:
		m_editVcFilter.GetWindowText(dmyBuf, 128);
		textSize = _tcslen(dmyBuf);
		dmyBuf[textSize] = '\0';
		dmyFacter = (float)atof(dmyBuf);
		if (0.001f <= dmyFacter && dmyFacter <= 1.000f)
			m_pShMem->filterFacterPr = dmyFacter;
		break;

	case IDC_AUTOTCCUT:
		timeStr = GetTimeStr();
		if( m_chkAutoCutTC.GetCheck() )
		{
			msg.LoadString(IDS_EXEC_MSG16);
			m_msg.Append(MC_MSG msg, timeStr);
		}
		else
		{
			msg.LoadString(IDS_EXEC_MSG17);
			m_msg.Append(MC_MSG msg, timeStr);
		}
		break;

/*
	case IDC_FILTERFACTERWD:
		m_editWidthFilter.GetWindowText(dmyBuf, 128);
		textSize = _tcslen(dmyBuf);
		dmyBuf[textSize] = '\0';
		dmyFacter = (float)atof(dmyBuf);
		if (0.001f <= dmyFacter && dmyFacter <= 1.000f)
			m_pShMem->filterFacterWd = dmyFacter;
		break;
*/
/*	case IDC_WATERFLOWRATE:
		m_editFlowFacter.GetWindowText(dmyBuf, 128);
		textSize = _tcslen(dmyBuf);
		dmyBuf[textSize] = '\0';
		dmyFlowRate = (float)atof(dmyBuf);
		if (0.001f <= dmyFlowRate && dmyFlowRate <= 1.000f)
			m_pShMem->waterFlowRate = dmyFlowRate;
		break;
	*/
	}
}

// test the B.O. status.
void CPredicDlg::TestBOStatus(BOOL *status, BOOL *statusD, int allRes, FILETIME *pFt/*=NULL*/)
{
	TCCombination	*cp;
	SYSTEMTIME		st;
	static SYSTEMTIME	oldSt = {0};
	int				nMsgColor, nMsgColorD, i;
	CString			msg;
	CString			strMsg, strMsgD, str;
	BOOL			bFirst = FALSE;
	BOOL			bFirstD = FALSE;
	int				nMsgCount = 0;;

	if( pFt != NULL )
		FileTimeToSystemTime(pFt, &st);
	else
		GetLocalTime(&st);

	if (oldSt.wDay != st.wDay || 
		oldSt.wHour != st.wHour || 
		oldSt.wMinute != st.wMinute || 
		oldSt.wSecond != st.wSecond ) 
	{
		m_timeStr.Format(_T("\'%2d.%2d.%2d %02d:%02d:%02d"),
			st.wYear % 100, st.wMonth, st.wDay,
			st.wHour, st.wMinute, st.wSecond);
		oldSt = st;
	}

	// test the raising edge of 'Vc On' signal.
	if(!m_blPreVcOn && m_pShMem->isVcOn) 
	{
		msg.LoadString(IDS_EXEC_MSG01);
		m_msg.Append(MC_MSG msg, m_timeStr);
		if (!m_pShMem->isEnabLevelCtl || m_pShMem->isAutoLVCtl) 
		{
			msg.LoadString(IDS_EXEC_MSG02);
			m_msg.Append(MC_MSG msg, m_timeStr);
		}
		else 
		{
			msg.LoadString(IDS_EXEC_MSG03);
			m_msg.Append(MC_MSG msg, m_timeStr);
		}

		// BO位置表示クリア要求ON
		m_blReqBoPosClear = TRUE;
	}
	// test the falling edge of 'Vc On' signal.
	else if(m_blPreVcOn && !m_pShMem->isVcOn)
	{
		msg.LoadString(IDS_EXEC_MSG04);
		m_msg.Append(MC_MSG msg, m_timeStr);

		// BO位置表示クリア要求OFF
		m_blReqBoPosClear = FALSE;
	}

	// test the raising edge of 'Charge Change' signal.
	if(!m_blPreChargeChange && m_pShMem->isChargeChange)
	{
		msg.LoadString(IDS_EXEC_MSG05);
		m_msg.Append(MC_MSG msg, m_timeStr);
	}

	// test the raising edge of 'Auto Level Control' signal.
	if(!m_blPreAutoLVCtl && m_pShMem->isAutoLVCtl) 
	{
		msg.LoadString(IDS_EXEC_MSG06);
		m_msg.Append(MC_MSG msg, m_timeStr);
		if(m_pShMem->isEnabLevelCtl && m_pShMem->isVcOn) 
		{
			msg.LoadString(IDS_EXEC_MSG07);
			m_msg.Append(MC_MSG msg, m_timeStr);
		}
	}
	// test the falling edge of 'Auto Level Control' signal.
	else if(m_blPreAutoLVCtl && !m_pShMem->isAutoLVCtl) 
	{
		msg.LoadString(IDS_EXEC_MSG08);
		m_msg.Append(MC_MSG msg, m_timeStr);
	}

	// test the raising edge of 'L/D Open' signal.
	if(!m_blPreCastStart && m_pShMem->isCastStart)
	{
		msg.LoadString(IDS_EXEC_MSG14);
		m_msg.Append(MC_MSG msg, m_timeStr);
	}
	// test the raising edge of 'L/D Open' signal.
	if(m_blPreCastStart && !m_pShMem->isCastStart)
	{
		msg.LoadString(IDS_EXEC_MSG15);
		m_msg.Append(MC_MSG msg, m_timeStr);
	}

	// test the raising edge of 'L1 Err' signal.
	if(!m_blPreL1Err && m_pShMem->isL1Err)
	{
		msg.LoadString(IDS_EXEC_MSG18);
		m_msg.Append(MC_MSG msg, m_timeStr);
	}
	// test the raising edge of 'L1 Err' signal.
	if(m_blPreL1Err && !m_pShMem->isL1Err)
	{
		msg.LoadString(IDS_EXEC_MSG19);
		m_msg.Append(MC_MSG msg, m_timeStr);
	}

	// 上位異常なら鋼種モードをで手動に切替え
	if( m_pShMem->isL1Err == 1 )
	{
		// 自動->手動切替え時に現在の鋼種Noを手動Noにセット
		if( m_isGCMode )
		{
			m_nRecvSteel = m_nPtnNo;
		}
		m_isGCMode = FALSE;
	}
	// 上位正常なら
	else if( m_pShMem->isL1Err == 0 )
	{
		// 強制手動OFFなら自動に復帰
		if( m_isManMode == FALSE )
		{
			m_isGCMode = TRUE;
		}
	}

	// BO位置クリア要求ありでかつ検出開始の立ち上がりなら
	if( m_blReqBoPosClear && !m_blPulseBopCondition && m_pShMem->BopCondition )
	{
		// BO位置表示クリア
		m_textBoMsgFirst.SetWindowText(_T(""));
		m_textBoMsgFirstD.SetWindowText(_T(""));

		// BO位置表示クリア要求OFF
		m_blReqBoPosClear = FALSE;
	}

	// if the result is valid,
	if( status && statusD ) 
	{
		if( ((m_pShMem->outBOFetched & 0x01) == 0x01) )
			nMsgColor = RGB(205,0,0);
		else
			nMsgColor = RGB(0,0,0);

		if( ((m_pShMem->outBOFetched & 0x08) == 0x08) )
			nMsgColorD = RGB(205,0,0);
		else
			nMsgColorD = RGB(0,0,0);

		if( ((m_pShMem->outBOFetched & 0x01) == 0x01) && ((m_blPulseBOFetched & 0x01) == 0x00) )
		{
			strMsg = m_timeStr;
			strMsg += _T(" "); 
			bFirst = TRUE;
		}

		if( ((m_pShMem->outBOFetched & 0x08) == 0x08) && ((m_blPulseBOFetched & 0x08) == 0x00) )
		{
			strMsgD = m_timeStr;
			strMsgD += _T(" "); 
			bFirstD = TRUE;
		}

		// for each output of combination.
		for(i = 0, cp = tcCombination; i < cnf.numCombination; i++, cp++)
		{
			// test the output value of neural networks. and B.O. detected,
			if(status[i])
			{
				if( cp->upper >= 0 && cp->upper < NumTCTap && cp->lower >= 0 && cp->lower < NumTCTap )
				{
					// display the B.O. message.
					msg.LoadString(IDS_EXEC_MSG09);
					m_msg.Append(nMsgColor, MC_MSG msg, m_timeStr, tcnm->name[cp->upper], tcnm->name[cp->lower], (int)(m_info.dLen+0.5));
					// BEEP音追加
					MessageBeep(-1);

					if( bFirst )
					{
						str.Format(_T("%s,%s(B.O.側) 鋳込長:%5dcm"), tcnm->name[cp->upper], tcnm->name[cp->lower], (int)(m_info.dLen+0.5));
						strMsg += str;
						m_textBoMsgFirst.SetTextColor(nMsgColor);
						m_textBoMsgFirst.SetWindowText(strMsg);
						bFirst = FALSE;
					}
				}
			}
		}

		// for each output of combination.(for dep.)
		for(i = 0, cp = tcCombinationD; i < cnfD.numCombination; i++, cp++)
		{
			// test the output value of neural networks. and B.O. detected,
			if(statusD[i])
			{
				if( cp->upper >= 0 && cp->upper < NumTCTap && cp->lower >= 0 && cp->lower < NumTCTap )
				{
					// display the B.O. message.
					msg.LoadString(IDS_EXEC_MSG20);
					m_msg.Append(nMsgColorD, MC_MSG msg, m_timeStr, tcnm->name[cp->upper], tcnm->name[cp->lower], (int)(m_info.dLen+0.5));
					// BEEP音追加
					MessageBeep(-1);

					if( bFirstD )
					{
						str.Format(_T("%s,%s(ﾃﾞﾌﾟﾚ側) 鋳込長:%5dcm"), tcnm->name[cp->upper], tcnm->name[cp->lower], (int)(m_info.dLen+0.5));
						strMsgD += str;
						m_textBoMsgFirstD.SetTextColor(nMsgColorD);
						m_textBoMsgFirstD.SetWindowText(strMsgD);
						bFirstD = FALSE;
					}
				}
			}
		}
	}

	// B.O.警報リセット時にメッセージ表示
	if (m_blPulseBOFetched && !m_pShMem->outBOFetched) 
	{
		msg.LoadString(IDS_EXEC_MSG10);
		m_msg.Append(MC_MSG msg, m_timeStr);
	}

	// update the status of input signals.
	m_blPreVcOn				= m_pShMem->isVcOn;
	m_blPreChargeChange		= m_pShMem->isChargeChange;
	m_blPreCastStart		= m_pShMem->isCastStart;
	m_blPreL1Err			= m_pShMem->isL1Err;
	m_blPreAutoLVCtl		= m_pShMem->isAutoLVCtl;
	m_blPulseBOFetched		= m_pShMem->outBOFetched;
	m_blPulseBopCondition	= m_pShMem->BopCondition;
}

// start the tick timer.
BOOL CPredicDlg::SetupTickTimer(void)
{
#ifdef USE_DEBUG_TIMER
	m_nTimerID3 = SetTimer(3, SamplingPeriod, NULL);
	return TRUE;
#else
	// start timer.
	return m_rt.SetTimer(SamplingPeriod, CRealTime::TimeCallBack, (LPVOID)(SharedMemory *)m_pShMem);
#endif
}

LPCTSTR CPredicDlg::GetTimeStr(void)
{
	time_t			t;
	struct			tm	*tp;
	static _TCHAR	timeStr[32];
	
	// get simulation time.
	t	= time(NULL);
	tp	= localtime(&t);
	_stprintf(timeStr, _T("\'%2d.%2d.%2d %02d:%02d:%02d"),
			tp->tm_year % 100, tp->tm_mon + 1, tp->tm_mday,
			tp->tm_hour, tp->tm_min, tp->tm_sec);
	return timeStr;
}

LPCTSTR CPredicDlg::GetTimeStr(time_t tm)
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

// request end procedure
void CPredicDlg::EndRequest(void)
{
	// save message log
	m_msg.ForceSave();
	m_isFinish = TRUE;
}

// finish end procedure?
BOOL CPredicDlg::IsFinish(void)
{
	return m_isFinish;
}

void CPredicDlg::ButtomOnOff(void)
{
	static int sMemTstVcOn 			= FALSE;
	static int sMemInChargeChange 	= FALSE;
	static int sMemtstAutoLVCtl 	= FALSE;
	static int sMemtstCastStart		= FALSE;
	static int sMemBoArarmHard 		= FALSE;
	static int sMemBoArarmSoft 		= FALSE;

	if (sMemTstVcOn == FALSE && m_pShMem->tstVcOn == TRUE) {
		sMemTstVcOn = TRUE;
		m_rdoVcOnIn.SetCheck(BST_CHECKED);
	}
	else if (sMemTstVcOn == TRUE && m_pShMem->tstVcOn == FALSE) {
		sMemTstVcOn = FALSE;
		m_rdoVcOnIn.SetCheck(BST_UNCHECKED);
	}
	if (sMemInChargeChange == FALSE && m_pShMem->tstChargeChange == TRUE) {
		sMemInChargeChange = TRUE;
		m_rdoChChgIn.SetCheck(BST_CHECKED);
	}
	else if (sMemInChargeChange == TRUE && m_pShMem->tstChargeChange == FALSE) {
		sMemInChargeChange = FALSE;
		m_rdoChChgIn.SetCheck(BST_UNCHECKED);
	}

	if (sMemtstAutoLVCtl == FALSE && m_pShMem->tstAutoLVCtl == TRUE) {
		sMemtstAutoLVCtl = TRUE;
		m_rdoMdLvcIn.SetCheck(BST_CHECKED);
	}
	else if (sMemtstAutoLVCtl == TRUE && m_pShMem->tstAutoLVCtl == FALSE) {
		sMemtstAutoLVCtl = FALSE;
		m_rdoMdLvcIn.SetCheck(BST_UNCHECKED);
	}
	if (sMemtstCastStart == FALSE && m_pShMem->tstCastStart == TRUE) {
		sMemtstCastStart = TRUE;
		m_rdoTdChgIn.SetCheck(BST_CHECKED);
	}
	else if (sMemtstCastStart == TRUE && m_pShMem->tstCastStart == FALSE) {
		sMemtstCastStart = FALSE;
		m_rdoTdChgIn.SetCheck(BST_UNCHECKED);
	}
	if (sMemBoArarmSoft == FALSE && (m_pShMem->outBOFetched & 0x01)) {
		sMemBoArarmSoft = TRUE;
		m_rdoBoAlmOut.SetCheck(BST_CHECKED);
	}
	else if (sMemBoArarmSoft == TRUE && !(m_pShMem->outBOFetched & 0x01)) {
		sMemBoArarmSoft = FALSE;
		m_rdoBoAlmOut.SetCheck(BST_UNCHECKED);
	}
	if (sMemBoArarmHard == FALSE && (m_pShMem->outBOFetched & 0x08)) {
		sMemBoArarmHard = TRUE;
		m_rdoBoAlmcOut.SetCheck(BST_CHECKED);
	}
	else if (sMemBoArarmHard == TRUE && !(m_pShMem->outBOFetched & 0x08)) {
		sMemBoArarmHard = FALSE;
		m_rdoBoAlmcOut.SetCheck(BST_UNCHECKED);
	}
}

// initialize the B.O. prediction condition.
void CPredicDlg::SetupBOCondition(void)
{
	CString	strVal;

	// boexec config data set up
	ConfDataSetUp();

	m_chkStrobe.SetCheck(m_pShMem->isStrobeBO);
	m_chkLvCtrl.SetCheck(m_pShMem->isEnabLevelCtl);
	m_chkFilter.SetCheck(m_pShMem->filterSwitch);
	strVal.Format(_T("%.2g"), m_pShMem->filterFacter);
	m_editTempFilter.SetWindowText(strVal);
	strVal.Format(_T("%.2g"), m_pShMem->filterFacterSn);
	m_editSnFilter.SetWindowText(strVal);
	strVal.Format(_T("%.2g"), m_pShMem->filterFacterLv);
	m_editLevelFilter.SetWindowText(strVal);
	strVal.Format(_T("%.2g"), m_pShMem->filterFacterPr);
	m_editVcFilter.SetWindowText(strVal);

	// setup individual color
	m_mode.SetBackColor(RGB(0,255,0));
	m_mode.SetTextColor(RGB(0,0,0));

	// setup auto mode color
	m_automode.SetBackColor(RGB(0,255,0));
	m_automode.SetTextColor(RGB(0,0,0));
}

// boexec config file data setup
BOOL CPredicDlg::ConfDataSetUp(void)
{
	FILE	*fp;
	TCHAR	name[MAX_PATH];

	// make up boexec config file name
	_tcscat( _tcscpy( name, cnf.WorkRootDir ), BOExecConfFile );

	// open the boexec config file
	if((fp = _tfopen(name, _T("rb"))) == NULL) 
	{
		// create the boexec config file
		if((fp = _tfopen(name, _T("wb+"))) != NULL) 
		{
			// set up default condition.
			m_pShMem->isStrobeBO	  = TRUE;

			// config data write
			fwrite(m_pShMem, sizeof(SharedMemory), 1, fp);
		}
	}
	else 
	{
		// config data read
		fread(m_pShMem, sizeof(SharedMemory), 1, fp);
	}
	m_pShMem->isEnabSimSignal	= FALSE;
	m_pShMem->inAlarmReset		= FALSE;
	m_pShMem->simVcOn			= FALSE;
	m_pShMem->simChargeChange	= FALSE;
	m_pShMem->simAutoLVCtl		= FALSE;
	m_pShMem->simCastStart		= FALSE;
	m_pShMem->simL1Err			= FALSE;

	// file close
	fclose(fp);

	return TRUE;
}

// boexec config file up date
BOOL CPredicDlg::ConfDataUpDate(void)
{
	FILE	*fp;
	TCHAR	name[MAX_PATH];

	// make up boexec config file name
	_tcscat( _tcscpy( name, cnf.WorkRootDir ), BOExecConfFile );

	// open the boexec config file
	if((fp = _tfopen(name, _T("wb+"))) == NULL) 
	{
		// file error
		return FALSE;
	}
	else 
	{
		fwrite(m_pShMem, sizeof(SharedMemory), 1, fp);
	}

	// file close
	fclose(fp);

	return TRUE;
}

// check alarm.
void CPredicDlg::CheckAlarm(void)
{
	static int almcnt = 0;
	static int almsts = 0;

	switch(almsts)
	{
	// check alarm reset start.
	case 0:
		if( m_pShMem->inAlarmReset == 1 || m_rdoResetAlm.GetCheck() == BST_CHECKED )
			almsts = 1;
		break;
	// wait alarm.
	case 1:
		almcnt++;
		if( almcnt > 5 )
		{
			m_pShMem->inAlarmReset = 0;
			almsts = 2;
		}
		break;
		// check alarm reset end.
	case 2:
		if( m_pShMem->inAlarmReset == 0 )
		{
			m_rdoResetAlm.SetCheck(BST_UNCHECKED);
			almcnt = 0;
			almsts = 0;
		}
		else
			m_pShMem->inAlarmReset = 0;
		break;
	}
}

// recover auto check of exception TC.
void CPredicDlg::CheckAutoRecoverTC(SampledData *pDat)
{
	/*
	double	dVal = 0.0;
	BOOL	bChg;
	int		i;

	for( i = 0; i < NumADChannel; i++ )
	{
		if( cnf.adChannel[i].arg < 0 )
			continue;

		switch(cnf.adChannel[i].arg)
		{
		case SNID:		
		case LevelID:	
		case VcID:		
		case WidthLID:	
		case WidthRID:	
		case LengthID:	
			continue;
		default:
			dVal = pDat->val[i] * PhisicalTempUnit;
			break;
		}

		if( dVal <= cnf.adChannel[i].max && dVal >= cnf.adChannel[i].min )
			break;
	}
	// all except?
	if( i == NumADChannel )
	{
		for( i = 0, bChg = FALSE; i < NumInput; i++ )
		{
			if( cnf.bTcExp[i] )
				bChg = TRUE;
			cnf.bTcExp[i] = FALSE;
		}
		if( bChg )
			cnf.WriteTcExpFile();
	}
	*/
}

// recover auto check of exception TC.
void CPredicDlg::CheckAutoCutOffTC(SampledData *pDat)
{
	CString	timeStr;
	double	dVal = 0.0;
	BOOL	bChg, bChgSave = FALSE;
	int		i, j, arg;

	// 自動TCカット機能OFFなら
	if( m_chkAutoCutTC.GetCheck() == 0 )
	{
		// 全てのデータについて
		for( i = 0; i < NumADChannel; i++ )
		{
			// 除外関係の変数初期化
			pbTcExp[i] = FALSE;
			pnCutOffTimer[i] = 0;
		}
		return;
	}

	// 自動TCカット機能ONなら
	// 鋳込中なら
	if( m_pShMem->isVcOn )
	{
		// 全てのデータについて
		for( i = 0; i < NumADChannel; i++ )
		{
			// 割り付けなしなら何もしない
			if( cnf.adChannel[i].arg < 0 )
				continue;

			// データ種別が温度なら
			if( cnf.adChannel[i].arg < NumTCTap ) 
			{
				// 温度なら
				bChg = FALSE;
				dVal = pDat->val[i] * PhisicalTempUnit;

				// 温度範囲外をチェック（条件１）
				// 設定時間前の温度と差をチェックする（条件２）
				if( (dVal <= cnf.m_stTcExpSet.dTempMin || dVal >= cnf.m_stTcExpSet.dTempMax) ||
					(IsTCcutoff(i) == TRUE) )
				{
					// 設定時間を超えたら
					if( pnCutOffTimer[i] >= (cnf.m_stTcExpSet.nWatchTime*2) )
					{
						pnCutOffTimer[i] = (cnf.m_stTcExpSet.nWatchTime*2);
						bChg = TRUE;	// 除外フラグON
					}
					else
						pnCutOffTimer[i]++;
				}
				// 範囲内ならタイマーリセット
				else
				{
					pnCutOffTimer[i] = 0;
				}

				// 条件１と条件２のどちらかが成立したら
				if( bChg )
				{
					// 該当温度の熱電対を除外
					pbTcExp[i] = TRUE;

					// 割り付けありなら
					arg = cnf.adChannel[i].arg;
					if( arg != -1 )
					{
						// 全コンビネーションについて
						for (j = 0; j < cnf.numCombination; j++) 
						{
							// 上下のどちらかと一致したら
							if( (tcCombination[j].lower == arg ||
								 tcCombination[j].upper == arg) &&
								 tcCombination[j].netID != -1 )
							{
								// コンビネーションを削除する
								timeStr = GetTimeStr();
								m_logtc.WriteLogFile(0, tcCombination[j].upper, tcCombination[j].lower, timeStr);
								m_msg.Append( _T(" %s %s,%s(B.O.側) 自動カットオフ\n"), timeStr, 
									tcnm->name[tcCombination[j].upper], tcnm->name[tcCombination[j].lower]);
								tcCombination[j].netID = -1;
//								DeleteCombination(tcCombination[j].upper, tcCombination[j].lower);
							}
						}

						// 全コンビネーションについて(for dep.)
						for (j = 0; j < cnfD.numCombination; j++) 
						{
							// 上下のどちらかと一致したら
							if( (tcCombinationD[j].lower == arg ||
								 tcCombinationD[j].upper == arg) &&
								 tcCombinationD[j].netID != -1)
							{
								// コンビネーションを削除する
								timeStr = GetTimeStr();
								m_logtc.WriteLogFile(0, tcCombinationD[j].upper, tcCombinationD[j].lower, timeStr);
								m_msg.Append( _T(" %s %s,%s(ﾃﾞﾌﾟﾚ側) 自動カットオフ\n"), timeStr, 
									tcnm->name[tcCombination[j].upper], tcnm->name[tcCombination[j].lower]);
								tcCombinationD[j].netID = -1;
//								DeleteCombinationD(tcCombinationD[j].upper, tcCombinationD[j].lower);
							}
						}
					}
				}
			}
		}
	}
	else
	{
		for( i = 0; i < NumADChannel; i++ )
		{
			pbTcExp[i] = FALSE;
		}
	}
}

// 鋼種切替え　自動切替え
void CPredicDlg::OnBnClickedButton1()
{
	// 上位が正常なら自動に復帰
	if( m_pShMem->isL1Err == 0 )
	{
		m_isGCMode = TRUE;
	}

	// 強制手動をOFF
	m_isManMode = FALSE;

	// 自動･手動切替ボタンの色設定
	m_btnAutoChg.SetBackColor(RGB(255,255,0));
	m_btnManChg.SetBackColor(RGB(200,200,200));
}

// 手動切り替えボタン
void CPredicDlg::OnBnClickedButton2()
{
	// 自動->手動切替え時に現在の鋼種Noを手動Noにセット
	if( m_isGCMode )
	{
		m_nRecvSteel = m_nPtnNo;
	}
	m_isGCMode = FALSE;

	// 強制手動をON
	m_isManMode = TRUE;

	// 自動･手動切替ボタンの色設定
	m_btnAutoChg.SetBackColor(RGB(200,200,200));
	m_btnManChg.SetBackColor(RGB(255,255,0));
}

// 
void CPredicDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	CRect	rc;
	StGraphParam		prm;

	prm.timeAxis	= cnf.m_stTimeAxis;
	prm.tempAxis	= cnf.m_stTempAxis;
	prm.levelAxis	= cnf.m_stLevelAxis;
	prm.vcAxis		= cnf.m_stVcAxis;
	prm.snAxis		= cnf.m_stSNAxis;

	// reload axis
	cnf.ReadAxisFile();

	if( 0 != memcmp( &prm.timeAxis,	&cnf.m_stTimeAxis,	sizeof(cnf.m_stTimeAxis)) ||
		0 != memcmp( &prm.tempAxis,	&cnf.m_stTempAxis,	sizeof(cnf.m_stTempAxis)) ||
		0 != memcmp( &prm.levelAxis, &cnf.m_stLevelAxis, sizeof(cnf.m_stLevelAxis)) ||
		0 != memcmp( &prm.vcAxis,	&cnf.m_stVcAxis,	sizeof(cnf.m_stVcAxis)) ||
		0 != memcmp( &prm.snAxis,	&cnf.m_stSNAxis,	sizeof(cnf.m_stSNAxis)) )
	{
		prm.timeAxis	= cnf.m_stTimeAxis;
		prm.tempAxis	= cnf.m_stTempAxis;
		prm.levelAxis	= cnf.m_stLevelAxis;
		prm.vcAxis		= cnf.m_stVcAxis;
		prm.snAxis		= cnf.m_stSNAxis;
		m_graph.ChangeAxis(&prm);
		m_textYScale.GetClientRect(&rc);
		m_textYScale.MapWindowPoints(this, &rc);
		InvalidateRect(&rc);
	}

	if( !bShow )
	{
		if( m_pDlgTcView != NULL )
			m_pDlgTcView->ShowWindow(SW_HIDE);
	}
	else
	{
		// setup data list
		SetupTdViewListItem();
	}
}

// 除外TCの情報をファイルに適用する
BOOL CPredicDlg::SaveExtTC(int nPtnNo)
{
	BOOL	bChg = FALSE;
	int		i, j, arg;

	//// 全チャンネル情報について
	//for( i = 0; i < NumADChannel; i++ )
	//{
	//	// 除外フラグがONなら
	//	if( pbTcExp[i] == TRUE )
	//	{
	//		// データ割付されてるなら
	//		if( cnf.adChannel[i].arg != -1 )
	//		{
	//			// 入力ポイントをOFFする
	//			cnf.inputPoint[cnf.adChannel[i].arg].adCh = -1;
	//			cnf.adChannel[i].arg = -1;
	//			bChg = TRUE;
	//		}
	//	}
	//}

	// 全チャンネル情報について
	for( i = 0; i < NumADChannel; i++ )
	{
		// 除外フラグがONでかつ割り付けありなら
		arg = cnf.adChannel[i].arg;
		if( pbTcExp[i] == TRUE && arg != -1 )
		{
			bChg = TRUE;
			// 全コンビネーションについて
			for (j = 0; j < cnf.numCombination; j++) 
			{
				// 上下のどちらかと一致したら
				if( tcCombination[j].lower == arg ||
					tcCombination[j].upper == arg )
				{
					// コンビネーションを削除する
					tcCombination[j].netID = -1;
//					DeleteCombination(tcCombination[j].upper, tcCombination[j].lower);
				}
			}
			// 全コンビネーションについて(for dep.)
			for (j = 0; j < cnfD.numCombination; j++) 
			{
				// 上下のどちらかと一致したら
				if( tcCombinationD[j].lower == arg ||
					tcCombinationD[j].upper == arg )
				{
					// コンビネーションを削除する
					tcCombinationD[j].netID = -1;
//					DeleteCombinationD(tcCombinationD[j].upper, tcCombinationD[j].lower);
				}
			}
		}
	}

	// コンフィグの変更があったなら
	if( bChg )
	{
		// コンフィグ情報（コンビネーション情報）を保存する
//		cnf.WriteADConfigFile(m_stBsInfo.szConfName);
		cnf.WriteCombination(m_stBsInfo.szConfName);
		cnfD.WriteCombination(m_stBsInfo.szConfNameD);

		// コンフィグ画面に自動読み込み要求を通知
		COnlinePCDlg *dlg = (COnlinePCDlg *)AfxGetMainWnd();
		if( dlg->m_pDlgConfig != NULL ){
			dlg->m_pDlgConfig->SetAutoLoad(nPtnNo, m_stBsInfo.szConfName);
			dlg->m_pDlgConfig->SetAutoLoadD(nPtnNo, m_stBsInfo.szConfNameD);
		}
	}

	return bChg;
}

// コンビネーション情報から指定の組合せを削除
void CPredicDlg::DeleteCombination(int upper, int lower)
{
	int		i;

	// search entry. 
	for (i = 0; i < cnf.numCombination; i++) {
		if (cnf.tcCombination[i].upper == upper && cnf.tcCombination[i].lower == lower)
			break;
	}
	// if found, 
	if (i < cnf.numCombination) {
		// delete the entry. 
		for ( ; i < cnf.numCombination - 1; i++)
			cnf.tcCombination[i] = cnf.tcCombination[i + 1];
		cnf.numCombination--;
	}
}

// コンビネーション情報から指定の組合せを削除
void CPredicDlg::DeleteCombinationD(int upper, int lower)
{
	int		i;

	// search entry. 
	for (i = 0; i < cnfD.numCombination; i++) {
		if (cnfD.tcCombination[i].upper == upper && cnfD.tcCombination[i].lower == lower)
			break;
	}
	// if found, 
	if (i < cnfD.numCombination) {
		// delete the entry. 
		for ( ; i < cnfD.numCombination - 1; i++)
			cnfD.tcCombination[i] = cnfD.tcCombination[i + 1];
		cnfD.numCombination--;
	}
}

// save old data
void SaveOldData(SampledData *data)
{
	sampleOld[idxSampleOld] = *data;
	if( ++idxSampleOld >= OLD_SMPL_NUM )
		idxSampleOld = 0;
	if( numSampleOld < OLD_SMPL_NUM )
		numSampleOld++;
}

// save tc cutoff data
void SaveTCcutoffData(SampledData *data)
{
	sampleTCcutoff[idxSampleTCcutoff] = *data;
	if( ++idxSampleTCcutoff >= TCEXP_CHK_TIME_MAX )
		idxSampleTCcutoff = 0;
	if( numSampleTCcutoff < TCEXP_CHK_TIME_MAX )
		numSampleTCcutoff++;
}

BOOL IsTCcutoff(int nChNo)
{
	double	dCurTemp, dOldTemp, dDiffTemp;
	int		idxCur, idxOld, cnt;

	cnt = cnf.m_stTcExpSet.nCheckTime * 2;

	if( numSampleTCcutoff < cnt )
		return FALSE;

	if( cnt > TCEXP_CHK_TIME_MAX )
		cnt = TCEXP_CHK_TIME_MAX;

	idxCur = idxSampleTCcutoff - 1;
	if( idxCur < 0 ) 
		idxCur = TCEXP_CHK_TIME_MAX + idxCur;

	idxOld = idxSampleTCcutoff - cnt - 1;
	if( idxOld < 0 ) 
		idxOld = TCEXP_CHK_TIME_MAX + idxOld;

	dCurTemp = sampleTCcutoff[idxCur].val[nChNo] * PhisicalTempUnit;
	dOldTemp = sampleTCcutoff[idxOld].val[nChNo] * PhisicalTempUnit;
	dDiffTemp = abs(dCurTemp - dOldTemp);
	if( dDiffTemp >= cnf.m_stTcExpSet.dDeltaT )
		return TRUE;

	return FALSE;
}

// Predic起動状態を返す
BOOL CPredicDlg::IsRun(void)
{
	return m_bRunWait;
}

// setAverageData.
void  CPredicDlg::SetAverageData(SampledData *data, int *pData)
{
	static int	left[28][11] = 
	{
		{26,24,21,19,17,15,12,10,7,5,3},//#1
		{27,25,22,20,18,16,13,11,8,6,4},//#2
		{1,26,24,21,19,17,15,12,10,7,5},//#3
		{2,27,25,22,20,18,16,13,11,8,6},//#4
		{3,1,26,24,21,19,17,15,12,10,7},//#5
		{4,2,27,25,22,20,18,16,13,11,8},//#6
		{5,3,1,26,24,21,19,17,15,12,10},//#7
		{6,4,2,27,25,22,20,18,16,13,11},//#8
		{28,23,14,-1,-1,-1,-1,-1,-1,-1,-1},//#9
		{7,5,3,1,26,24,21,19,17,15,12},//#10
		{8,6,4,2,27,25,22,20,18,16,13},//#11
		{10,7,5,3,1,26,24,21,19,17,15},//#12
		{11,8,6,4,2,27,25,22,20,18,16},//#13
		{9,28,23,-1,-1,-1,-1,-1,-1,-1,-1},//#14
		{12,10,7,5,3,1,26,24,21,19,17},//#15
		{13,11,8,6,4,2,27,25,22,20,18},//#16
		{15,12,10,7,5,3,1,26,24,21,19},//#17
		{16,13,11,8,6,4,2,27,25,22,20},//#18
		{19,15,12,10,7,5,3,1,26,24,21},//#19
		{20,16,13,11,8,6,4,2,27,25,22},//#20
		{21,19,15,12,10,7,5,3,1,26,24},//#21
		{22,20,16,13,11,8,6,4,2,27,25},//#22
		{14,9,28,-1,-1,-1,-1,-1,-1,-1,-1},//#23
		{24,21,19,15,12,10,7,5,3,1,26},//#24
		{25,22,20,16,13,11,8,6,4,2,27},//#25
		{26,24,21,19,15,12,10,7,5,3,1},//#26
		{27,25,22,20,16,13,11,8,6,4,2},//#27
		{23,14,9,-1,-1,-1,-1,-1,-1,-1,-1}//#28
	};
	static int	right[28][11] = 
	{
		{3,5,7,10,12,15,17,19,21,24,26},//#1
		{4,6,8,11,13,16,18,20,22,25,27},//#2
		{5,7,10,12,15,17,19,21,24,26,1},//#3
		{6,8,11,13,16,18,20,22,25,27,2},//#4
		{7,10,12,15,17,19,21,24,26,1,3},//#5
		{8,11,13,16,18,20,22,25,27,2,4},//#6
		{10,12,15,17,19,21,24,26,1,3,5},//#7
		{11,13,16,18,20,22,25,27,2,4,6},//#8
		{14,23,28,-1,-1,-1,-1,-1,-1,-1,-1},//#9
		{12,15,17,19,21,24,26,1,3,5,7},//#10
		{13,16,18,20,22,25,27,2,4,6,8},//#11
		{15,17,19,21,24,26,1,3,5,7,10},//#12
		{16,18,20,22,25,27,2,4,6,8,11},//#13
		{23,28,9,-1,-1,-1,-1,-1,-1,-1,-1},//#14
		{17,19,21,24,26,1,3,5,7,10,12},//#15
		{18,20,22,25,27,2,4,6,8,11,13},//#16
		{19,21,24,26,1,3,5,7,10,12,15},//#17
		{20,22,25,27,2,4,6,8,11,13,16},//#18
		{21,24,26,1,3,5,7,10,12,15,17},//#19
		{22,25,27,2,4,6,8,11,13,16,18},//#20
		{24,26,1,3,5,7,10,12,15,17,19},//#21
		{25,27,2,4,6,8,11,13,16,18,20},//#22
		{28,9,14,-1,-1,-1,-1,-1,-1,-1,-1},//#23
		{26,1,3,5,7,10,12,15,17,19,21},//#24
		{27,2,4,6,8,11,13,16,18,20,22},//#25
		{1,3,5,7,10,12,15,17,19,21,24},//#26
		{2,4,6,8,11,13,16,18,20,22,25},//#27
		{9,14,23,-1,-1,-1,-1,-1,-1,-1,-1}//#28
	};
	int		i, j, iL, iR;

	//配列変数の初期化
	memset( pData, 0, sizeof( pData ) );

	// 基本ロジック
	// 熱電対データが無い場合（又は切断）、左右の有効な熱電対温度から
	// 計算して求める

	// 全熱電対温度について熱電対順位並べ替え＆無いとこを計算
	for( i = 0; i < 28; i++ )
	{
		// データが無いなら計算する
		if( cnf.inputPoint[i].adCh == -1 )
		{
			pData[i] = 0;
			iL = -1;
			iR = -1;

			// 左方向の有効な熱電対Indexを探す
			for( j = 0; j < 11; j++ )
			{
				if( left[i][j] != -1 )
				{
					if( cnf.inputPoint[left[i][j]-1].adCh != -1 )
					{
						iL = j;
						break;
					}
				}
			}

			// 右方向の有効な熱電対Indexを探す
			for( j = 0; j < 11; j++ )
			{
				if( right[i][j] != -1 )
				{
					if( cnf.inputPoint[right[i][j]-1].adCh != -1 )
					{
						iR = j;
						break;
					}
				}
			}

			// 左右に有効な熱電対が見つかったら
			if( iL != -1 && iR != -1 )
			{
				// 左右のIndexが同じなら左側のデータをセット
				if( left[i][iL] == right[i][iR] )
				{
					pData[i] = data->val[cnf.inputPoint[left[i][j]-1].adCh];
				}
				// 違うなら割合計算
				else
				{
					int	div, oft;
					div = data->val[cnf.inputPoint[right[i][iR]-1].adCh] - 
						  data->val[cnf.inputPoint[left[i][iL]-1].adCh];
					oft = data->val[cnf.inputPoint[left[i][iL]-1].adCh];
					pData[i] = div * (iL+1) / (iL+iR+2) + oft;
				}
			}
		}
		// データがあるならそのままセット
		else
		{
			pData[i] = data->val[cnf.inputPoint[i].adCh];
		}
	}

	// 温度以外はそのままセット
	for( i = NumTCTap; i < NumADChannel; i++ )
		pData[i] = data->val[i];
}

void CPredicDlg::LimitTempData(int *pDat)
{
	int			i;

	for(i = 0; i < NumTCTap; i++)
	{
		if( (double)pDat[i] > MAXTEMP )
			pDat[i] = MAXTEMP;
		if( (double)pDat[i] < MINTEMP )
			pDat[i] = MINTEMP;
	}
}

// グラフのチャンネル割付変更
void CPredicDlg::ChangeChArgTm(void)
{
	CRect		rc;
	int			chArgs[NumADChannel];
	int			chArgsTm[NumADChannel];
	int			i, arg, ch;

	// チャンネル表示設定値初期化
	memset(chArgs, 0, sizeof(chArgs));
	memset(chArgsTm, 0, sizeof(chArgsTm));

	// 表示入力ポイントに対して
	for( i = 0; i < NumInput; i++ )
	{
		// 表示／非表示を設定
		ch = cnf.inputPoint[i].adCh;
		if( ch != -1 )
			chArgs[ch] = m_stTdSel[i].fIndication;
	}

	// トレンド描画ｃｈ設定
	for(i = 0; i < NumADChannel; i++)
	{
		arg = cnf.adChannel[i].arg;
		if( arg >= 0 && arg < LastID-1)
			chArgsTm[i] = arg;
		else
			chArgsTm[i] = -1;
	}

	// グラフに設定値を渡し再描画
	m_graph.ChangeArg(chArgs);
	m_graph.ChangeChArgTm(chArgsTm);
	m_graph.GetClientRect(&rc);
	m_graph.MapWindowPoints(this, &rc);
	InvalidateRect(&rc, FALSE);
}

