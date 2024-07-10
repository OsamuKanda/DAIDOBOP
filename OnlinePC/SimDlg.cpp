// SimDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include <windowsx.h>
#include "OnlinePC.h"
#include "OnlinePCDlg.h"
#include "SimDlg.h"
#include "../include/chartlib.h"
#include ".\simdlg.h"


///////////////////////////////////////////////////////////////////////////////
// definition
static CConfig		&cnf = theApp.m_cnf;
static CBolog		&bolog = theApp.m_log;
static CChartLib	&clib = theApp.m_simChart;
static TcName		*tcnm	= &theApp.m_stTcNm;

#define DispCount	8
#define	Y_WIDTH		6
#define Y_WIDTH1	4
#define	AXIS_WIDTH	45

#define	WM_APP_SIM_CHG		(WM_APP + 0x10)
#define	WM_APP_BPNET_CHG	(WM_APP + 0x11)

// CSimDlg ダイアログ

IMPLEMENT_DYNAMIC(CSimDlg, CDialog)
CSimDlg::CSimDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimDlg::IDD, pParent)
{
	// initialize member data.
	m_nNetIdx = -1;
	m_nCombiIdx = -1;
	m_nSelectIdx = 0;
	m_isSelFile = FALSE;
	m_pDlgCalc = NULL;
	m_pDlgTcView = NULL;

	// create dialog.
	Create(CSimDlg::IDD, pParent);				
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
}

CSimDlg::~CSimDlg()
{
}

void CSimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILENAME2, m_textFileName);
	DDX_Control(pDX, IDC_REMARK, m_textRemark);
	DDX_Control(pDX, IDC_Y_SCALE, m_textYScale);
	DDX_Control(pDX, IDC_CONF_NAME, m_textConfName);
	DDX_Control(pDX, IDC_DATA_LIST, m_textDataList);
	DDX_Control(pDX, IDC_FILTERSW, m_chkFilter);
	DDX_Control(pDX, IDC_FILTERFACTER, m_editTempFilter);
}


BEGIN_MESSAGE_MAP(CSimDlg, CDialog)
	ON_COMMAND(IDM_DATA_SELECT, OnDataSelect)
	ON_COMMAND(IDM_NET_SELECT, OnNetSelect)
	ON_COMMAND(IDM_DO_DISPLAY, OnDoDisplay)
	ON_COMMAND(IDM_AXIS_CONFIG2, OnAxisConfig2)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(IDM_BACK_PROP, OnBackProp)
	ON_WM_TIMER()
	ON_COMMAND(IDM_CHART_SELECT, OnChartSelect)
	ON_COMMAND(IDM_SELECT_ALARM, OnSelectAlarm)
	ON_COMMAND(IDM_SELECT_ORDINARY, OnSelectOrdinary)
	ON_WM_MOVE()
	ON_MESSAGE( WM_APP_SIM_CHG, OnSimChg )
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_COMMAND(IDM_TCPOSITION, OnTcposition)
	ON_BN_CLICKED(IDC_FILTERSW, OnClickedFilterSw)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedDisplay)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CSimDlg message handler

//It evades ending by Enter Key.
void CSimDlg::OnOK(){}

//It evades ending by ESC Key.
void CSimDlg::OnCancel(){}

void CSimDlg::PostNcDestroy()
{
	if( m_pDlgCalc != NULL )
		delete m_pDlgCalc;

	delete this;
//	CDialog::PostNcDestroy();
}

// WM_MOVE
void CSimDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

#ifdef FIX_SCREEN
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
#endif
}

// WM_INITDIALOG
BOOL CSimDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// customize color
	clib.m_clTones[0] = RGB(255,0,0);
	clib.m_clTones[1] = RGB(0,192,0);
	clib.m_clTones[2] = RGB(0,0,205);
	clib.m_clTones[3] = RGB(128,128,0);

	// regist subclass
	CRect	rc;
	CWnd	*wnd = GetDlgItem(IDC_SIM_GRAPH_WIN);
	wnd->GetClientRect(&rc);
	wnd->MapWindowPoints(this, &rc);
	m_graph.CreateEx(WS_EX_WINDOWEDGE, NULL, _T("SimGraph"), WS_CHILDWINDOW|WS_VISIBLE|WS_BORDER,rc,this,IDC_SIM_GRAPH_WIN);
	m_graph.ShowWindow(SW_SHOW);

	// set trend.out path
	clib.m_strTrendOutFileName.Format(_T("%s/../trendf.out"), bolog.LogRootDir);

	// set filter item.
	SharedMemory	*pShMem = GetSharedMemory();
	CString			strVal;
	m_bFilterSwitch = pShMem->filterSwitch;
	m_chkFilter.SetCheck(m_bFilterSwitch);
	m_fFilterFacter = pShMem->filterFacter;
	strVal.Format(_T("%.2g"), m_fFilterFacter);
	m_editTempFilter.SetWindowText(strVal);

	// open log file of old selection
	CLogSel2	dlg;
	if( dlg.NoDialogSetup(m_nSelectIdx) )
	{
		_TCHAR	sLogName[MAX_PATH];
		int		i;

		if( cnf.LoadLogSelFileName(_T("SimLogSel.txt"), sLogName) )
		{
			for( i = 0; i < dlg.m_nTrendNum; i++ )
			{
				if( _tcscmp(sLogName, dlg.trendDir[i].name) == 0 )
					break;
			}
			if( i < dlg.m_nTrendNum )
				m_nSelectIdx = i;
		}
		else
			m_nSelectIdx = 0;

		dlg.m_nSelectNum = m_nSelectIdx;
		DispLoggedData(dlg);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}

// show window
void CSimDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if( bShow )
	{
		m_textConfName.SetWindowText(cnf.GetWorkDirName());
	}
	else
	{
		// hide 
		if( m_pDlgTcView != NULL )
			m_pDlgTcView->ShowWindow(SW_HIDE);
	}
}

// paint
void CSimDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( m_isSelFile )
	{
		// draw Y scale
		DrawYScale(dc);

		// draw pen list
		DrawNetList(dc);
	}
}

// selected data of menu
void CSimDlg::OnDataSelect()
{
	CLogSel2	dlg;

	if( IDOK == dlg.DoModal() )
	{
		m_nSelectIdx = dlg.m_nSelectNum;
		DispLoggedData(dlg);
	}
}

// clicked 'Prev' Button.
void CSimDlg::OnBnClickedButton4()
{
	CLogSel2	dlg;

	if( dlg.NoDialogSetup(m_nSelectIdx-1) )
	{
		m_nSelectIdx = dlg.m_nSelectNum;
		DispLoggedData(dlg);
	}
}

// clicked 'Next' Button.
void CSimDlg::OnBnClickedButton5()
{
	CLogSel2	dlg;

	if( dlg.NoDialogSetup(m_nSelectIdx+1) )
	{
		m_nSelectIdx = dlg.m_nSelectNum;
		DispLoggedData(dlg);
	}
}

// selected network of menu
void CSimDlg::OnNetSelect()
{
	CNetSel	dlg;

	// set current value
	dlg.m_nCombinationIdx	= m_nCombiIdx;
	dlg.m_nNeuralnetIdx		= m_nNetIdx;

	// opne network select dialog
	if( IDOK == dlg.DoModal() )
	{
		int		nTrendArg[AllTrendArea];

		// get setting value
		m_nNetIdx = dlg.m_nNeuralnetIdx;
		m_nCombiIdx = dlg.m_nCombinationIdx;

		// if log file open,
		if( m_isSelFile )
		{
			ChgFilterSet();

			// load trend
			memset(nTrendArg, -1, sizeof(nTrendArg));
			if( m_nCombiIdx != -1 )
			{
				nTrendArg[0] = cnf.tcCombination[m_nCombiIdx].upper;
				nTrendArg[1] = cnf.tcCombination[m_nCombiIdx].lower;
			}
			m_graph.TrendLoad(nTrendArg);

			RefreshSimOut();
		}
	}
}

// selected do display of menu
void CSimDlg::OnDoDisplay()
{
	CString	strVal;
	int		nTimer, ret;

	if( m_isSelFile )
	{
		// set log filter
		m_exe.SetLogFilter(m_bFilterSwitch, m_fFilterFacter);
		strVal.Format(_T("%.2g"), m_fFilterFacter);
		m_editTempFilter.SetWindowText(strVal);

		// start simulate
		ret = m_exe.SimulateBOPredictStart(m_graph.m_szFilePath, m_nNetIdx, m_nCombiIdx);
		if( ret != CSimExe::NORMAL ){
			DispSimExeErrMsg(ret);
			return;
		}

		nTimer = SetTimer(100, 1, NULL);

		if( m_pDlgCalc != NULL )
			delete m_pDlgCalc;
		m_pDlgCalc = new CNetCalc(this);
		
		m_pDlgCalc->SetRange(0, clib.m_nLoadFileRecordCount);
		ret = m_pDlgCalc->DoModal();

		m_exe.SimulateBOPredictEnd();
		KillTimer(nTimer);

		ChgFilterSet();

		// load trend
		_TCHAR	dirName[MAX_PATH];
		_tcscat( _tcscpy( dirName, cnf.WorkRootDir ), BOSimulateFile );
		m_graph.TrendLoad(((ret==IDOK)?TRUE:FALSE), dirName);
		Invalidate();
	}
}

// selected do display of menu
void CSimDlg::OnAxisConfig2()
{
	CSimAxis	dlg;

	if( IDOK == dlg.DoModal() )
	{
		CRect	rc;
		StChartAxisParam	cp;

		cp.timeAxis = cnf.m_stTimeAxis;
		cp.tempAxis = cnf.m_stTempAxis;
		
		ChgFilterSet();

		m_graph.TrendLoad(&cp);
		m_textYScale.GetClientRect(&rc);
		m_textYScale.MapWindowPoints(this, &rc);
		InvalidateRect(&rc);
	}
}

// selected back propagation of menu
void CSimDlg::OnBackProp()
{
	COnlinePCDlg *dlg = (COnlinePCDlg *)AfxGetMainWnd();
	
	if( dlg != NULL ){
		dlg->m_pDlgBpnet->OpenBpnetSecurityPass();
		dlg->m_pDlgBpnet->SendMessage(WM_APP_BPNET_CHG, m_nNetIdx); 
		dlg->m_pDlgBpnet->ShowWindow(SW_SHOW);
		dlg->m_pDlgActive = dlg->m_pDlgBpnet;

		RefreshSimOut();
	}
	ShowWindow(SW_HIDE);
}

void CSimDlg::OnTimer(UINT nIDEvent)
{
	int		nloop;

	// while succeed getting trend data,
	for(nloop = 0; nloop < SIM_LOOP_CNT; nloop++) {
		if (m_exe.GetSampledData()) {
			m_exe.SimulateBOPredictLoop(m_nCombiIdx);
		}
		else {
			// calc. end  dlg clear
			if( m_pDlgCalc )
				m_pDlgCalc->End();
			// Timer end 
			break;
		}
	}

	// % check 
	if( m_pDlgCalc )
	{
		m_pDlgCalc->SetPos(m_exe.m_nInputIdx);
		m_pDlgCalc->Notify();
	}

	CDialog::OnTimer(nIDEvent);
}

// selected chart select of menu
void CSimDlg::OnChartSelect()
{
	if( m_nCombiIdx == -1 )
	{
		DispSimExeErrMsg(CSimExe::ERR_NOT_TC);
		return;
	}

	if( m_isSelFile )
	{
		clib.m_nCutAreaLen = m_exe.GetInputSize(m_nNetIdx);
		clib.SelectStart();
	}
	else
		AfxMessageBox(IDS_SIM_DLG07);
}

// selected alarm of menu
void CSimDlg::OnSelectAlarm()
{
	if (clib.m_isCutAreaSelect == TRUE) {
		if (AfxMessageBox(IDS_SIM_DLG05,MB_OKCANCEL) == IDOK) 
		{
			if( m_bFilterSwitch )
			{
				// save alarm data
				AlarmSave(clib.m_strSelFileName, clib.m_nStartDataNo, clib.m_nDataCount, clib.m_fFilterTemp);
			}
			else
			{
				// save alarm data
				AlarmSave(clib.m_strSelFileName, clib.m_nStartDataNo, clib.m_nDataCount, 1.0);
			}
			// clear select area
			clib.GraphSelectAreaClear(this);

			// 異常Logファイルを予備フォルダに保存
			bolog.SaveLogFileToTemp(clib.m_strSelFileName);
		}
	}
	else {
		// nonselect cut area
		AfxMessageBox(IDS_SIM_DLG06);
	}
}

// selected ordinary of menu
void CSimDlg::OnSelectOrdinary()
{
	BOOL	ret = FALSE;

	if (clib.m_isCutAreaSelect == TRUE) 
	{
		if( m_bFilterSwitch )
		{
			// save ordinary data 
			ret = OrdinarySave(clib.m_strSelFileName, clib.m_nStartDataNo, clib.m_nDataCount, clib.m_fFilterTemp);
		}
		else
		{
			// save ordinary data 
			ret = OrdinarySave(clib.m_strSelFileName, clib.m_nStartDataNo, clib.m_nDataCount, 1.0);
		}
		// clear select area 
		clib.GraphSelectAreaClear(this);

		// 正常Logファイルを予備フォルダに保存
		bolog.SaveLogFileToTemp(clib.m_strSelFileName);
	}
	else {
		// nonselect cut area
		AfxMessageBox(IDS_SIM_DLG06);
	}
}

// selected 'TC Position' menu.
void CSimDlg::OnTcposition()
{
	if( m_pDlgTcView == NULL )
	{
		m_pDlgTcView = new CTcVwDlg(this);
	}
	m_pDlgTcView->ShowWindow(SW_SHOW);
}

// clicked filter sw button.
void CSimDlg::OnClickedFilterSw()
{
	m_bFilterSwitch = !m_bFilterSwitch;
	m_chkFilter.SetCheck(m_bFilterSwitch);
}

// clicked 'Display' button.
void CSimDlg::OnBnClickedDisplay()
{
	ChgFilterSet();

	// load trend
	m_graph.TrendLoad();

	RefreshSimOut();
}

BOOL CSimDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (GET_WM_COMMAND_ID(wParam, lParam)) 
	{
	case IDC_FILTERFACTER:
		// update the execution condition.
		BoConditionManage(GET_WM_COMMAND_ID(wParam, lParam), -1);
		break;
	}

	return CDialog::OnCommand(wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////
// CTrendDlg member function

// draw Y scale
void CSimDlg::DrawYScale(CDC &dc)
{
	CRect	rcYScale, rcGraph;
	CString	strName, strUnit, strTmp;
	CFont	newFont, *font;
	LOGFONT	lf;
	ChartAxis	netAxis;
	int		height;

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

	// draw Temp
	strName.LoadString(IDS_DEF_TEMP);
	strTmp.LoadString(IDS_DEF_TEMP_UNIT);
	strUnit.Format(_T("[%s]"), strTmp); 
	clib.DrawYAxis(dc, strName, strUnit, _T("%3.1f"), &cnf.m_stTempAxis, &newFont, clib.m_clText, 
		rcYScale.left , rcGraph.top, clib.ChartHeight );

	// draw neural
	strName.LoadString(IDS_DEF_NET1);
	strUnit.LoadString(IDS_DEF_NET2);
	netAxis.zero = 0.0;
	netAxis.scale = 0.1;
	height = clib.ChartMeshHeight;
	clib.DrawYAxis(dc, strName, strUnit, _T("%.1lf"), &netAxis, &newFont, clib.m_clText, 
		rcYScale.left + AXIS_WIDTH*1 , rcGraph.top + height * (clib.ChartMeshYCnt-10), height * 10 );
}

void CSimDlg::DrawNetList(CDC &dc)
{
	CFont	*font;
	int		MapModePrevious, upper, lower;
	CRect	rc;
	CString	str;
	LOGFONT	lf;

	// get current font
	font = GetFont();
	font->GetLogFont(&lf);

	// mapping mode set
	MapModePrevious = dc.SetMapMode(MM_ANISOTROPIC);

	// get area.
	m_textDataList.GetClientRect(&rc);
	m_textDataList.MapWindowPoints(this, &rc);

	// clear display area.
	dc.FillSolidRect(&rc, GetSysColor(COLOR_BTNFACE));

	// set text align
	dc.SetTextAlign(TA_UPDATECP);

	// write network.
	dc.MoveTo(rc.left, rc.top);
	if( m_nNetIdx != -1 )
	{
		str.LoadString(IDS_SIM_DLG00);
		PrintText(dc, font, clib.m_clText,
			rc.left, rc.top, str, cnf.neuralnetConf[m_nNetIdx].remark);
	}

	// write the configration.
	dc.MoveTo(rc.left, rc.top - (lf.lfHeight-2));
	str.LoadString(IDS_SIM_DLG01);
	PrintText(dc, font, clib.m_clText, 0, 0, str);
	if (m_nCombiIdx != -1) {
		upper = cnf.tcCombination[m_nCombiIdx].upper;
		lower = cnf.tcCombination[m_nCombiIdx].lower;
		if( upper >= 0 && upper < NumTCTap && lower >= 0 && lower < NumTCTap )
		{
			PrintText(dc, font, clib.m_clTones[0], 0, 0, _T("%s"),
											tcnm->name[upper]);
			PrintText(dc, font, clib.m_clText, 0, 0, _T(", "));
			PrintText(dc, font, clib.m_clTones[1], 0, 0, _T("%s"),
											tcnm->name[lower]);
		}
	}

	// write the simulate out.
	dc.MoveTo(rc.left, rc.top - (lf.lfHeight-2) * 2);
	str.LoadString(IDS_SIM_DLG02);
	PrintText(dc, font, clib.m_clText, 0, 0, str);
	str.LoadString(IDS_SIM_DLG03);
	PrintText(dc, font, clib.m_clTones[2], 0, 0, str);
	PrintText(dc, font, clib.m_clText, 0, 0, _T(", "));
	str.LoadString(IDS_SIM_DLG04);
	PrintText(dc, font, clib.m_clTones[3], 0, 0, str);

	// set text align
	dc.SetTextAlign(TA_NOUPDATECP);

	// mapping mode reset
	dc.SetMapMode(MapModePrevious);
}

// save alarm position.
BOOL CSimDlg::AlarmSave(LPCTSTR dir, DWORD startNo, DWORD dataCnt, float fFilter)
{
	_TCHAR	buf[128], name[64];
	FILE	*fd;

	if( m_nCombiIdx < 0 || m_nCombiIdx >= cnf.numCombination )
		return FALSE;

	// make up trend data file name start pointer, data count and combination index 
	_stprintf(buf, _T("%s\r\n%lu\r\n%lu\r\n%d,%d\r\n%f\r\n"), dir, startNo, dataCnt, cnf.tcCombination[m_nCombiIdx].upper, cnf.tcCombination[m_nCombiIdx].lower, fFilter);
	// make up alarm data file name
	_stprintf(name, _T("%s\\%s") BPAlarmDataFile, cnf.WorkRootDir, cnf.GetWorkDirName(), m_nNetIdx );

	// open and write the alarm data
	if ((fd = _tfopen(name, _T("w+b"))) == NULL) {
		return FALSE;
	}
	// dir startpoint datacount write
	fwrite(buf, strlen(buf), 1, fd);

	// file close
	fclose(fd);

	return TRUE;
}

// save ordinary position.
BOOL CSimDlg::OrdinarySave(LPCTSTR dir, DWORD startNo, DWORD dataCnt, float fFilter)
{
	_TCHAR	buf[128], name[64];
	FILE	*fd;

	if( m_nCombiIdx < 0 || m_nCombiIdx >= cnf.numCombination )
		return FALSE;

	// make up trend data file name start pointer, data count and combination index 
	_stprintf(buf, _T("%s\r\n%lu\r\n%lu\r\n%d,%d\r\n%f\r\n"), dir, startNo, dataCnt, cnf.tcCombination[m_nCombiIdx].upper, cnf.tcCombination[m_nCombiIdx].lower, fFilter);
	// make up alarm data file name
	_stprintf(name, _T("%s\\%s") BPOrdinaryDataFile, cnf.WorkRootDir, cnf.GetWorkDirName(), m_nNetIdx );

	// open and write the alarm data
	if ((fd = _tfopen(name, _T("w+b"))) == NULL) {
		return FALSE;
	}
	// dir startpoint datacount write
	fwrite(buf, strlen(buf), 1, fd);

	// file close
	fclose(fd);

	return TRUE;
}

// display error message
int CSimDlg::DispSimExeErrMsg(int ret)
{
	CString	str;
	UINT	nType;

	switch(ret)
	{
	case CSimExe::ERR_NO_NET:		str.LoadString(IDS_SIM_EXE01);nType = MB_OK;break;
	case CSimExe::ERR_LOAD_NN:		str.LoadString(IDS_SIM_EXE02);nType = MB_OK;break;
	case CSimExe::ERR_MEM_ALLOC:	str.LoadString(IDS_SIM_EXE03);nType = MB_OK;break;
	case CSimExe::ERR_NOT_TC:		str.LoadString(IDS_SIM_EXE04);nType = MB_OK;break;
	case CSimExe::ERR_NO_DATA:		str.LoadString(IDS_SIM_EXE05);nType = MB_OK;break;
	case CSimExe::ERR_OPEN_DATA:	str.LoadString(IDS_SIM_EXE06);nType = MB_OK;break;
	case CSimExe::ERR_NO_UPCH:		str.LoadString(IDS_SIM_EXE07);nType = MB_OK;break;
	case CSimExe::ERR_NO_LOCH:		str.LoadString(IDS_SIM_EXE08);nType = MB_OK;break;
	case CSimExe::ERR_OPEN_OUT:		str.LoadString(IDS_SIM_EXE09);nType = MB_OK;break;
	case CSimExe::ERR_SIM_OUT:		str.LoadString(IDS_SIM_EXE10);nType = MB_OK;break;
	default:						str.LoadString(IDS_SIM_EXE00);nType = MB_OK;break;
	}
	return AfxMessageBox(str, nType);
}

LRESULT CSimDlg::OnSimChg(WPARAM wParam, LPARAM lParam)
{
	int		nTrendArg[AllTrendArea];
	int		i, nArg1, nArg2;

	if( m_nNetIdx != wParam )
	{
		// get setting value
		m_nNetIdx = wParam;
		nArg1		= (lParam>>16);
		nArg2		= lParam & 0xffff;
		for( i = 0; i < cnf.numCombination; i++ )
		{
			if( cnf.tcCombination[i].upper == nArg1 &&
				cnf.tcCombination[i].lower == nArg2 )
			{
				break;
			}
		}
		if( i != cnf.numCombination )
			m_nCombiIdx = i;
		else
			m_nCombiIdx = -1;
	}

	// if log file open,
	if( m_isSelFile )
	{
		RefreshSimOut();

		ChgFilterSet();

		// load trend
		memset(nTrendArg, -1, sizeof(nTrendArg));
		if( m_nCombiIdx != -1 )
		{
			nTrendArg[0] = cnf.tcCombination[m_nCombiIdx].upper;
			nTrendArg[1] = cnf.tcCombination[m_nCombiIdx].lower;
		}
		m_graph.TrendLoad(nTrendArg);
		Invalidate();
	}

	return 0;
}

void CSimDlg::RefreshSimOut( void )
{
	// if log file open,
	if( m_isSelFile )
	{
		_TCHAR	dirName[MAX_PATH];
		_tcscat( _tcscpy( dirName, cnf.WorkRootDir ), BOSimulateFile );
		m_graph.TrendLoad(FALSE, dirName);
		Invalidate();
	}
}

void CSimDlg::DispLoggedData(CLogSel2& dlg)
{
	CString			strVal;
	DataFileHeader	header;
	FILE			*fp;
	StGraphParam	gp;
	int		nTrendArg[AllTrendArea];
	_TCHAR	buf[MAX_PATH];
	int		sel = dlg.m_nSelectNum;

	if( sel < 0 || sel >= dlg.m_nTrendNum )
		return;

	gp.numChannel = 2;
	gp.neuralChannel = 0;
	gp.isNeuralCal = FALSE;
	gp.dispStartTime	= dlg.trendDir[sel].startTime;
	gp.samplePeriod		= dlg.m_header.header.samplePeriod;
	memset(nTrendArg, -1, sizeof(gp.nTrendChArg));
	if( m_nCombiIdx != -1 )
	{
		nTrendArg[0] = cnf.tcCombination[m_nCombiIdx].upper;
		nTrendArg[1] = cnf.tcCombination[m_nCombiIdx].lower;
	}
	memcpy(gp.nTrendChArg, nTrendArg, sizeof(gp.nTrendChArg));
	memcpy(&gp.timeAxis, &cnf.m_stTimeAxis, sizeof(gp.timeAxis));
	memcpy(&gp.tempAxis, &cnf.m_stTempAxis, sizeof(gp.tempAxis));
	memcpy(&gp.levelAxis, &cnf.m_stLevelAxis, sizeof(gp.levelAxis));
	memcpy(&gp.vcAxis, &cnf.m_stVcAxis, sizeof(gp.vcAxis));
	memcpy(&gp.snAxis, &cnf.m_stSNAxis, sizeof(gp.snAxis));
//	memcpy(&gp.widthAxis, &cnf.m_stWidthAxis, sizeof(gp.widthAxis));
	_tcscat( _tcscat( _tcscpy( buf, bolog.LogRootDir ), _T("\\") ), dlg.trendDir[sel].name );

	// header read
	m_bFilterSwitch = FALSE;
	if( (fp = _tfopen( buf, _T("rb")) ) != NULL )
	{
		if( fread(&header, sizeof(header), 1, fp) == 1 )
		{
			// set filter sw
			if( header.header.version >= 620 )
				m_bFilterSwitch = TRUE;
		}
		fclose(fp);
	}

	// set filter sw
	m_chkFilter.SetCheck(m_bFilterSwitch);
	clib.m_bFilterSwitch = m_bFilterSwitch;
	clib.m_fFilterTemp = m_fFilterFacter;
	strVal.Format(_T("%.2g"), m_fFilterFacter);
	m_editTempFilter.SetWindowText(strVal);

	// load trend
	m_graph.TrendLoad(buf, &gp);

	// set remark text
	m_textFileName.SetWindowText(dlg.trendDir[sel].date);
	m_textRemark.SetWindowText(header.header.remark);

	m_isSelFile = TRUE;
	Invalidate();

	// save now selection
	cnf.SaveLogSelFileName(_T("SimLogSel.txt"), dlg.trendDir[sel].name);
}

// update the execution condition.
void CSimDlg::BoConditionManage(WPARAM wParam, int cmd)
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
	}
}

// change filter setting.
void CSimDlg::ChgFilterSet(void)
{
	CString	strVal;

	if( m_chkFilter.GetCheck() == 1 )
		m_bFilterSwitch = TRUE;
	else
		m_bFilterSwitch = FALSE;
	clib.m_bFilterSwitch = m_bFilterSwitch;
	clib.m_fFilterTemp = m_fFilterFacter;
	strVal.Format(_T("%.2g"), m_fFilterFacter);
	m_editTempFilter.SetWindowText(strVal);
}

