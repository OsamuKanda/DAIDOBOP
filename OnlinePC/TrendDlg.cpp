// TrendDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include <windowsx.h>
#include "OnlinePC.h"
#include "TrendDlg.h"
#include "../include/ChartLib.h"
#include ".\trenddlg.h"


///////////////////////////////////////////////////////////////////////////////
// definition
static CConfig		&cnf = theApp.m_cnf;
static CBolog		&lg = theApp.m_log;
static CChartLib	&clib = theApp.m_trendChart;
static TcName		*tcnm	= &theApp.m_stTcNm;


#define DispCount	4
#define LabelWidth	70
#define PenWidth	200
#define TimeWidth	200
#define	AXIS_WIDTH	45


// CTrendDlg dialog
IMPLEMENT_DYNAMIC(CTrendDlg, CDialog)
CTrendDlg::CTrendDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrendDlg::IDD, pParent)
{
	// initialize member data.
	m_isSelFile = FALSE;
	m_nSelectIdx = 0;
	m_pDlgTcView = NULL;
	m_tScooter	 = 0;

	// create dialog.
	Create(CTrendDlg::IDD, pParent);				
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
}

CTrendDlg::~CTrendDlg()
{
}

void CTrendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILENAME, m_textFileName);
	DDX_Control(pDX, IDC_REMARK, m_textRemark);
	DDX_Control(pDX, IDC_PEN_LIST, m_textPenList);
	DDX_Control(pDX, IDC_Y_SCALE, m_textYScale);
	DDX_Control(pDX, IDC_SCOOTOR, m_scrScooter);
	DDX_Control(pDX, IDC_FILTERSW, m_chkFilter);
	DDX_Control(pDX, IDC_FILTERFACTER, m_editTempFilter);
	DDX_Control(pDX, IDC_FILTERFACTERSN, m_editSnFilter);
	DDX_Control(pDX, IDC_FILTERFACTERLV, m_editLevelFilter);
	DDX_Control(pDX, IDC_FILTERFACTERPR, m_editVcFilter);
	DDX_Control(pDX, IDC_SC_DATE, m_textScDate);
	DDX_Control(pDX, IDC_SC_TIME, m_textScTime);
}


BEGIN_MESSAGE_MAP(CTrendDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_COMMAND(IDM_LOGGED_DATA, OnLoggedData)
	ON_COMMAND(IDM_AXIS_CONFIG, OnAxisset)
	ON_COMMAND(IDM_PEN_SELECT, OnPenSelect)
	ON_WM_PAINT()
	ON_WM_MOVE()
	ON_COMMAND(IDM_DRIVE, OnDrive)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_WM_HSCROLL()
	ON_COMMAND(IDM_TCPOSITION, OnTcPosition)
	ON_BN_CLICKED(IDC_FILTERSW, OnClickedFilterSw)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedDisplay)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CTrendDlg message handler

// WM_MOVE
void CTrendDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

#ifdef FIX_SCREEN
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
#endif
}

// WM_INITDIALOG
BOOL CTrendDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// create graph window
	CRect	rc;
	CWnd	*wnd = GetDlgItem(IDC_TRD_GRAPH_WIN);
	wnd->GetClientRect(&rc);
	wnd->MapWindowPoints(this, &rc);
	m_graph.CreateEx(WS_EX_WINDOWEDGE, NULL, _T("TrendGraph"), WS_CHILDWINDOW|WS_VISIBLE|WS_BORDER,rc,this,IDC_TRD_GRAPH_WIN);
	m_graph.ShowWindow(SW_SHOW);

	// set filter item.
	SharedMemory	*pShMem = GetSharedMemory();
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

	// set trend.out path
	clib.m_strTrendOutFileName.Format(_T("%s/../trendf.out"), lg.LogRootDir);

	// open log file of old selection
	CLogSel2	dlg;
	if( dlg.NoDialogSetup(m_nSelectIdx) )
	{
		_TCHAR	sLogName[MAX_PATH];
		int		i;

		if( cnf.LoadLogSelFileName(_T("TrendLogSel.txt"), sLogName) )
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

		// initialize scooter
		clib.m_nScooterPos = 0;
		clib.m_isScooter = TRUE;
		m_scrScooter.SetScrollRange(0, clib.ChartWidth);
		m_scrScooter.SetScrollPos(clib.m_nScooterPos);

		// get value of scooter position.
		GetScooterData(0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CTrendDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( m_isSelFile )
	{
		// draw Y scale
		DrawYScale(dc);

		// draw pen list
		DrawPenList(dc);
	}
}

//It evades ending by Enter Key.
void CTrendDlg::OnOk(){}

//It evades ending by ESC Key.
void CTrendDlg::OnCancel(){}

void CTrendDlg::PostNcDestroy()
{
	delete this;
//	CDialog::PostNcDestroy();
}

// selected Logged data of menu
void CTrendDlg::OnLoggedData()
{
	CLogSel2	dlg;

	if( IDOK == dlg.DoModal() )
	{
		m_nSelectIdx = dlg.m_nSelectNum;
		DispLoggedData(dlg);

		// initialize scooter
		clib.m_nScooterPos = 0;
		clib.m_isScooter = TRUE;
		m_scrScooter.SetScrollRange(0, clib.ChartWidth);
		m_scrScooter.SetScrollPos(clib.m_nScooterPos);

		// get value of scooter position.
		GetScooterData(0);
	}
}

// clicked 'Prev' Button.
void CTrendDlg::OnBnClickedButton4()
{
	CLogSel2	dlg;

	if( dlg.NoDialogSetup(m_nSelectIdx-1) )
	{
		m_nSelectIdx = dlg.m_nSelectNum;
		DispLoggedData(dlg);

		// initialize scooter
		clib.m_nScooterPos = 0;
		clib.m_isScooter = TRUE;
		m_scrScooter.SetScrollRange(0, clib.ChartWidth);
		m_scrScooter.SetScrollPos(clib.m_nScooterPos);

		// get value of scooter position.
		GetScooterData(0);
	}
}

// clicked 'Next' Button.
void CTrendDlg::OnBnClickedButton5()
{
	CLogSel2	dlg;

	if( dlg.NoDialogSetup(m_nSelectIdx+1) )
	{
		m_nSelectIdx = dlg.m_nSelectNum;
		DispLoggedData(dlg);

		// initialize scooter
		clib.m_nScooterPos = 0;
		clib.m_isScooter = TRUE;
		m_scrScooter.SetScrollRange(0, clib.ChartWidth);
		m_scrScooter.SetScrollPos(clib.m_nScooterPos);

		// get value of scooter position.
		GetScooterData(0);
	}
}

// selected pen of menu
void CTrendDlg::OnPenSelect()
{
	CTrendPen	dlg;

	if( IDOK == dlg.DoModal() )
	{
		CRect	rc;
		m_textPenList.GetClientRect(&rc);
		m_textPenList.MapWindowPoints(this, &rc);

		// load trend
		m_graph.TrendLoad(cnf.m_nTrendArg);
		InvalidateRect(&rc);

		// get scooter data
		GetScooterData(m_scrScooter.GetScrollPos());
	}
}

// selected Axis data of menu
void CTrendDlg::OnAxisset()
{
	CTrendAxis	dlg;

	if( IDOK == dlg.DoModal() )
	{
		CRect	rc;
		StChartAxisParam	cp;

		cp.timeAxis = cnf.m_stTimeAxis;
		cp.tempAxis = cnf.m_stTempAxis;
		cp.levelAxis = cnf.m_stLevelAxis;
		cp.vcAxis = cnf.m_stVcAxis;
		cp.snAxis = cnf.m_stSNAxis;
//		cp.widthAxis = cnf.m_stWidthAxis;
		
		m_graph.TrendLoad(&cp);
		m_textYScale.GetClientRect(&rc);
		m_textYScale.MapWindowPoints(this, &rc);
		InvalidateRect(&rc);

		// get scooter data
		GetScooterData(m_scrScooter.GetScrollPos());
	}
}

// open drive dialog.
void CTrendDlg::OnDrive()
{
	CDrvAcc	dlg(this);

	dlg.DoModal();
}

// selected 'TC Position' menu.
void CTrendDlg::OnTcPosition()
{
	if( m_pDlgTcView == NULL )
	{
		m_pDlgTcView = new CTcVwDlg(this);
	}
	m_pDlgTcView->ShowWindow(SW_SHOW);
}

// clicked filter sw button.
void CTrendDlg::OnClickedFilterSw()
{
	m_bFilterSwitch = !m_bFilterSwitch;
	m_chkFilter.SetCheck(m_bFilterSwitch);
}

// clicked 'Display' button.
void CTrendDlg::OnBnClickedDisplay()
{
	CString	strVal;

	if( m_chkFilter.GetCheck() == 1 )
		m_bFilterSwitch = TRUE;
	else
		m_bFilterSwitch = FALSE;
	clib.m_bFilterSwitch = m_bFilterSwitch;
	clib.m_fFilterTemp = m_fFilterFacter;
	clib.m_fFilterSn = m_fFilterFacterSn;
	clib.m_fFilterLv = m_fFilterFacterLv;
	clib.m_fFilterPr = m_fFilterFacterPr;
	strVal.Format(_T("%.2g"), m_fFilterFacter);
	m_editTempFilter.SetWindowText(strVal);
	strVal.Format(_T("%.2g"), m_fFilterFacterSn);
	m_editSnFilter.SetWindowText(strVal);
	strVal.Format(_T("%.2g"), m_fFilterFacterLv);
	m_editLevelFilter.SetWindowText(strVal);
	strVal.Format(_T("%.2g"), m_fFilterFacterPr);
	m_editVcFilter.SetWindowText(strVal);

	// load trend
	m_graph.TrendLoad();

	// get scooter data
	GetScooterData(m_scrScooter.GetScrollPos());
}

BOOL CTrendDlg::OnCommand(WPARAM wParam, LPARAM lParam)
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
// CTrendDlg member function

// draw Y scale
void CTrendDlg::DrawYScale(CDC &dc)
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

// draw Axis Data
void CTrendDlg::DrawPenList(CDC &dc)
{
	CString	str;
	CFont	newFont, *font;
	CRect	rc;
	LOGFONT	lf;
	int MapModePrevious;
	int i ,count, iLbX, iLbY, iDtX, iDtY;
	double	dData = 0.0;

	// get current font
	font = GetFont();
	font->GetLogFont(&lf);
	lf.lfWeight = 700;
	newFont.CreateFontIndirect(&lf);

	// mapping mode set
	MapModePrevious = SetMapMode(dc, MM_ANISOTROPIC);

	// clear display area.
	m_textPenList.GetClientRect(&rc);
	m_textPenList.MapWindowPoints(this, &rc);
	dc.FillSolidRect(&rc, GetSysColor(COLOR_BTNFACE));
	iLbX = rc.left, iLbY = rc.top;
	// write the configration.
	dc.SetTextAlign(TA_UPDATECP);

	for (i = 0, count = 0; i < NumTrendChannel; i++) 
	{
		if (cnf.m_nTrendArg[i] >= 0) 
		{
			// calc move pos.
			iLbX = rc.left + (count % DispCount) * PenWidth;
			iDtX = iLbX + LabelWidth;
			iLbY = (int)(rc.top - count / DispCount * (lf.lfHeight * 1.2));
			iDtY = iLbY;

			// print label
			dc.MoveTo(iLbX, iLbY);
			if (cnf.m_nTrendArg[i] < LevelID) {
				PrintText(dc, &newFont, clib.m_clTones[i], 0, 0, _T("%s"),
														tcnm->name[cnf.m_nTrendArg[i]]);
				dData = m_nPenData[i] * PhisicalTempUnit;
				str.Format( _T("%7.1lf,") , dData);
			}
			else if (cnf.m_nTrendArg[i] == LevelID){
				str.LoadString(IDS_DEF_LEVEL);
				PrintText(dc, &newFont, clib.m_clTones[i], 0, 0, str);
				dData = m_nPenData[i] * PhisicalLevelUnit;
				str.Format( _T("%7.1lf,") , dData);
			}
			else if (cnf.m_nTrendArg[i] == VcID){
				str.LoadString(IDS_DEF_VC);
				PrintText(dc, &newFont, clib.m_clTones[i], 0, 0, str);
				dData = m_nPenData[i] * PhisicalVcUnit;
				str.Format( _T("%7.2lf,") , dData);
			}
			else if (cnf.m_nTrendArg[i] == SNID){
				str.LoadString(IDS_DEF_SN);
				PrintText(dc, &newFont, clib.m_clTones[i], 0, 0, str);
				dData = m_nPenData[i] * PhisicalSNUnit;
				str.Format( _T("%7.0lf,") , dData);
			}
			//else if (cnf.m_nTrendArg[i] == WidthID){
			//	str.LoadString(IDS_DEF_WIDTH);
			//	PrintText(dc, &newFont, clib.m_clTones[i], 0, 0, str);
			//	dData = m_nPenData[i] * PhisicalWidthUnit;
			//	str.Format( _T("%7.0lf,") , dData);
			//}
			//else if (cnf.m_nTrendArg[i] == WidthLID){
			//	str.LoadString(IDS_DEF_WIDTH);
			//	str +=_T("L");
			//	PrintText(dc, &newFont, clib.m_clTones[i], 0, 0, str);
			//	dData = m_nPenData[i] * PhisicalWidthUnit;
			//}
			//else if (cnf.m_nTrendArg[i] == WidthRID){
			//	str.LoadString(IDS_DEF_WIDTH);
			//	str +=_T("R");
			//	PrintText(dc, &newFont, clib.m_clTones[i], 0, 0, str);
			//	dData = m_nPenData[i] * PhisicalWidthUnit;
			//}
			
			// print data
			dc.MoveTo(iDtX, iDtY);
//			str.Format( _T("%7.3lf,") , dData);
			PrintText(dc, &newFont, clib.m_clTones[i], 0, 0, str);

			// plus count
			count++;
		}
	}

	dc.SetTextAlign(TA_NOUPDATECP);

	// mapping mode reset
	dc.SetMapMode(MapModePrevious);
}

// 
void CTrendDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	CRect	rc;
	StChartAxisParam	cp;

	cp.timeAxis		= cnf.m_stTimeAxis;
	cp.tempAxis		= cnf.m_stTempAxis;
	cp.levelAxis	= cnf.m_stLevelAxis;
	cp.vcAxis		= cnf.m_stVcAxis;
	cp.snAxis		= cnf.m_stSNAxis;

	// reload axis
	cnf.ReadAxisFile();

	if( 0 != memcmp( &cp.timeAxis,	&cnf.m_stTimeAxis,	sizeof(cnf.m_stTimeAxis)) ||
		0 != memcmp( &cp.tempAxis,	&cnf.m_stTempAxis,	sizeof(cnf.m_stTempAxis)) ||
		0 != memcmp( &cp.levelAxis, &cnf.m_stLevelAxis, sizeof(cnf.m_stLevelAxis)) ||
		0 != memcmp( &cp.vcAxis,	&cnf.m_stVcAxis,	sizeof(cnf.m_stVcAxis)) ||
		0 != memcmp( &cp.snAxis,	&cnf.m_stSNAxis,	sizeof(cnf.m_stSNAxis)) )
	{
		cp.timeAxis = cnf.m_stTimeAxis;
		cp.tempAxis = cnf.m_stTempAxis;
		cp.levelAxis = cnf.m_stLevelAxis;
		cp.vcAxis = cnf.m_stVcAxis;
		cp.snAxis = cnf.m_stSNAxis;
//		cp.widthAxis = cnf.m_stWidthAxis;
		
		m_graph.TrendLoad(&cp);
		m_textYScale.GetClientRect(&rc);
		m_textYScale.MapWindowPoints(this, &rc);
		InvalidateRect(&rc);
	}

	if( !bShow )
	{
		if( m_pDlgTcView != NULL )
			m_pDlgTcView->ShowWindow(SW_HIDE);
	}
}

void CTrendDlg::DispLoggedData(CLogSel2& dlg)
{
	CString			strVal;
	DataFileHeader	header;
	FILE			*fp;
	StGraphParam	gp;
	_TCHAR	buf[MAX_PATH];
	int		sel = dlg.m_nSelectNum;

	if( sel < 0 || sel >= dlg.m_nTrendNum )
		return;

	gp.numChannel = AllTrendArea;
	gp.neuralChannel = 0;
	gp.isNeuralCal = FALSE;
	gp.dispStartTime	= dlg.trendDir[sel].startTime;
	gp.samplePeriod		= dlg.m_header.header.samplePeriod;
    memcpy(gp.nTrendChArg, cnf.m_nTrendArg, sizeof(gp.nTrendChArg));
	memcpy(&gp.timeAxis, &cnf.m_stTimeAxis, sizeof(gp.timeAxis));
	memcpy(&gp.tempAxis, &cnf.m_stTempAxis, sizeof(gp.tempAxis));
	memcpy(&gp.levelAxis, &cnf.m_stLevelAxis, sizeof(gp.levelAxis));
	memcpy(&gp.vcAxis, &cnf.m_stVcAxis, sizeof(gp.vcAxis));
	memcpy(&gp.snAxis, &cnf.m_stSNAxis, sizeof(gp.snAxis));
//		memcpy(&gp.widthAxis, &cnf.m_stWidthAxis, sizeof(gp.widthAxis));
	_tcscat( _tcscat( _tcscpy( buf, lg.LogRootDir ), _T("\\") ), dlg.trendDir[sel].name );

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
	clib.m_fFilterSn = m_fFilterFacterSn;
	clib.m_fFilterLv = m_fFilterFacterLv;
	clib.m_fFilterPr = m_fFilterFacterPr;
	strVal.Format(_T("%.2g"), m_fFilterFacter);
	m_editTempFilter.SetWindowText(strVal);
	strVal.Format(_T("%.2g"), m_fFilterFacterSn);
	m_editSnFilter.SetWindowText(strVal);
	strVal.Format(_T("%.2g"), m_fFilterFacterLv);
	m_editLevelFilter.SetWindowText(strVal);
	strVal.Format(_T("%.2g"), m_fFilterFacterPr);
	m_editVcFilter.SetWindowText(strVal);

	// load trend
	m_graph.TrendLoad(buf, &gp);

	// set file name & remark text
	m_textFileName.SetWindowText(dlg.trendDir[sel].date);
	m_textRemark.SetWindowText(header.header.remark);

	m_isSelFile = TRUE;
	Invalidate();

	// save now selection
	cnf.SaveLogSelFileName(_T("TrendLogSel.txt"), dlg.trendDir[sel].name);
}

void CTrendDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar != NULL )
	{
		int t, max;
		t = pScrollBar->GetScrollPos();
		max = clib.ChartWidth;
		switch(nSBCode)
		{
			case SB_LINELEFT:	// 左スクロールの場合
								t += -1;
								if(t < 0) t = 0;
								break;
			case SB_LINERIGHT:	// 右スクロールの場合
								t += 1;
								if(t > max) t = max;
								break;
			case SB_PAGELEFT:	// 左PAGEの場合
								t += -max/30;
								if(t < 0) t = 0;
								break;
			case SB_PAGERIGHT:	// 右PAGEの場合
								t += max/30;
								if(t > max) t = max;
								break;
			case SB_THUMBTRACK:	// ドラッグの場合
								t = nPos;
								break;
			default:			break;
		}

		// スクロールボックスの位置設定
		if(pScrollBar == &m_scrScooter)
		{
			clib.m_nScooterPos = t;
			m_graph.Invalidate(FALSE);
			GetScooterData(t);
		}
		pScrollBar->SetScrollPos(t);
		nPos = t;
	}
	else
	{
		// graph window scroll
		m_graph.Invalidate(FALSE);
		GetScooterData(m_scrScooter.GetScrollPos());
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

// get value of scooter positon.
void CTrendDlg::GetScooterData(int nPos)
{
	CString		str;
	SampledData	data;
	CRect		rc;
	time_t		tScooter;
	struct tm	*startT;
	int			i, arg;

	if( clib.GetChartData(nPos, &data) )
	{
		for (i = 0; i < NumTrendChannel; i++) 
		{
			arg = cnf.m_nTrendArg[i];
			if (arg >= 0) 
			{
				m_nPenData[i] = data.val[i];
			}
		}
	}

	if( clib.GetChartTime(nPos, &tScooter) )
	{
		m_tScooter = tScooter;
		startT	= localtime(&m_tScooter);
		str.Format(_T("%02d.%2d.%02d"), startT->tm_year % 100, startT->tm_mon + 1, startT->tm_mday);
		m_textScDate.SetWindowText(str);
		str.Format(_T("%02d:%02d:%02d"), startT->tm_hour, startT->tm_min, startT->tm_sec);
		m_textScTime.SetWindowText(str);
	}

	m_textPenList.GetWindowRect(rc);
	ScreenToClient(rc);
	InvalidateRect(rc, TRUE);
}

// update the execution condition.
void CTrendDlg::BoConditionManage(WPARAM wParam, int cmd)
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
