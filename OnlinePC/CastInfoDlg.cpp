// CastInfoDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "OnlinePCDlg.h"
#include "CastInfoDlg.h"

///////////////////////////////////////////////////////////////////////////////
// definition

static CConfig			&cnf		= theApp.m_cnfBoexe;

#define FixTapColumn	15
#define LooseTapColumn	15
#define N1TapColumn		1
#define N2TapColumn		1
#define	COLOR_NUM		15

int iFIXPos3D[TapRaw][FixTapColumn] = {	
						{ 3, 6, 9,12,15,18,21,24,27,30,33,36,39,42,45},
						{ 4, 7,10,13,16,19,22,25,28,31,34,37,40,43,46},
						{ 5, 8,11,14,17,20,23,26,29,32,35,38,41,44,47}};
int iLOOSEPos3D[TapRaw][LooseTapColumn] = {
						{48,51,54,57,60,63,66,69,72,75,78,81,84,87,90},
						{49,52,55,58,61,64,67,70,73,76,79,82,85,88,91},
						{50,53,56,59,62,65,68,71,74,77,80,83,86,89,92}};

COLORREF	g_colTone[COLOR_NUM] = 
{
	RGB( 55, 55,255),
	RGB( 55,155,255),
	RGB( 55,255,255),
	RGB( 55,255,155),
	RGB( 55,255, 55),
	RGB(155,255, 55),
	RGB(255,255, 55),
	RGB(255,230, 55),
	RGB(255,205, 55),
	RGB(255,180, 55),
	RGB(255,155, 55),
	RGB(255,130, 55),
	RGB(255,105, 55),
	RGB(255, 80, 55),
	RGB(255, 55, 55)
};

// reference data
static COLORREF			&MoldColor	= theApp.m_col.clMoldColor;

// CCastInfoDlg ダイアログ

IMPLEMENT_DYNAMIC(CCastInfoDlg, CDialog)
CCastInfoDlg::CCastInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCastInfoDlg::IDD, pParent)
{
	m_nAngleX = 60;
	m_nAngleY = 30;
	m_nTempMax = 300;
	m_nTempMin = 0;
	m_pDlgTcView = NULL;

	memset( m_cond, 0, sizeof(m_cond) );	
	m_numCond = 0;

	memset( m_info, 0, sizeof(m_info) );	
	m_numInfo = 0;
	m_bInfoMsg = FALSE;

	// create dialog.
	Create(CCastInfoDlg::IDD, pParent);				
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
}

CCastInfoDlg::~CCastInfoDlg()
{
}

void CCastInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COL_AXIS, m_lbColAxis);
	DDX_Control(pDX, IDC_ABCDEF, m_lbMold);
	DDX_Control(pDX, IDC_SPIN1, m_spnAngleX);
	DDX_Control(pDX, IDC_SPIN2, m_spnAngleY);
	DDX_Control(pDX, IDC_SPIN3, m_spnTempMax);
	DDX_Control(pDX, IDC_SPIN4, m_spnTempMin);
	DDX_Control(pDX, IDC_EDIT_ANGLEX, m_editAngleX);
	DDX_Control(pDX, IDC_EDIT_ANGLEY, m_editAngleY);
	DDX_Control(pDX, IDC_EDIT_TEMPMAX, m_editTempMax);
	DDX_Control(pDX, IDC_EDIT_TEMPMIN, m_editTempMin);
	DDX_Control(pDX, IDC_EDIT_A, m_editTempA);
	DDX_Control(pDX, IDC_EDIT_B, m_editTempB);
	DDX_Control(pDX, IDC_EDIT_C, m_editTempC);
	DDX_Control(pDX, IDC_EDIT_D, m_editTempD);
	DDX_Control(pDX, IDC_EDIT_E, m_editTempE);
	DDX_Control(pDX, IDC_EDIT_F, m_editTempF);
	DDX_Control(pDX, IDC_LIST_CONDSET, m_listCondSet);
	DDX_Control(pDX, IDC_LIST_CONDINFO, m_listInfoSet);
	DDX_Control(pDX, IDC_CAST_VC, m_textCastVc);
	DDX_Control(pDX, IDC_MD_WIDTH, m_textWidth);
	DDX_Control(pDX, IDC_ACT_LEVEL, m_textActLevel);
	DDX_Control(pDX, IDC_CAST_LEN, m_textCastLen);
	DDX_Control(pDX, IDC_EMBR, m_textEMBr);

	// ***CFX***
	// Do not use class wizard to generate these. It will cause the generation of unecesary classes
	DDX_Control(pDX, IDC_CI_CHARTFX1, m_ChartFX1);
	if (!pDX->m_bSaveAndValidate) // Link Chart FX pointer to control window
		m_pChartFX1 = m_ChartFX1.GetControlUnknown();
	DDX_Control(pDX, IDC_CI_CHARTFX2, m_ChartFX2);
	if (!pDX->m_bSaveAndValidate) // Link Chart FX pointer to control window
		m_pChartFX2 = m_ChartFX2.GetControlUnknown();
	DDX_Control(pDX, IDC_CI_CHARTFX3, m_ChartFX3);
	if (!pDX->m_bSaveAndValidate) // Link Chart FX pointer to control window
		m_pChartFX3 = m_ChartFX3.GetControlUnknown();
	// ***CFX***
}

BEGIN_MESSAGE_MAP(CCastInfoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_CHARTDATA2, OnAddone)
	ON_MESSAGE(WM_ACTINFO2, OnActInfo)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnSpinAngleX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnSpinAngleY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, OnSpinTempMax)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, OnSpinTempMin)
	ON_UPDATE_COMMAND_UI(IDM_LOAD_DATA, OnUpdateLoadData)
	ON_UPDATE_COMMAND_UI(IDM_SAVE_DATA, OnUpdateSaveData)
	ON_UPDATE_COMMAND_UI(IDM_RESTART, OnUpdateRestart)
	ON_UPDATE_COMMAND_UI(IDM_TCPOSITION, OnUpdateTcposition)
	ON_COMMAND(IDM_LOAD_DATA, OnLoadData)
	ON_COMMAND(IDM_SAVE_DATA, OnSaveData)
	ON_COMMAND(IDM_RESTART, OnRestart)
	ON_COMMAND(IDM_INFORMATION, OnInfoOut)
	ON_COMMAND(IDM_TCPOSITION, OnTcposition)
	ON_BN_CLICKED(IDC_COND_ADD, OnClickedCondAdd)
	ON_BN_CLICKED(IDC_COND_UPD, OnClickedCondUpdate)
	ON_BN_CLICKED(IDC_COND_DEL, OnClickedCondDelete)
	ON_BN_CLICKED(IDC_INFO_ADD, OnClickedInfoAdd)
	ON_BN_CLICKED(IDC_INFO_UPD, OnClickedInfoUpdate)
	ON_BN_CLICKED(IDC_INFO_DEL, OnClickedInfoDelete)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CCastInfoDlg メッセージ ハンドラ

//It evades ending by Enter Key.
void CCastInfoDlg::OnOK(){}

//It evades ending by ESC Key.
void CCastInfoDlg::OnCancel(){}

void CCastInfoDlg::PostNcDestroy() 
{
	delete this;
//	CDialog::PostNcDestroy();
}

// WM_MOVE
void CCastInfoDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

#ifdef FIX_SCREEN
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
#endif
}

// Initialize dialog
BOOL CCastInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString	str;

	// read condition.
	ReadConfigCondition();
	ReadConfigInformation();

	// spin setting
	m_spnAngleX.SetRange(0, 90);
	m_spnAngleX.SetPos(m_nAngleX);
	m_spnAngleY.SetRange(0, 180);
	m_spnAngleY.SetPos(m_nAngleY);
	m_spnTempMax.SetRange(0, 300);
	m_spnTempMax.SetPos(m_nTempMax);
	m_spnTempMin.SetRange(0, 300);
	m_spnTempMin.SetPos(m_nTempMin);

	// edit setting
	str.Format(_T("%d"), m_nAngleX);
	m_editAngleX.SetWindowText(str);
	str.Format(_T("%d"), m_nAngleY);
	m_editAngleY.SetWindowText(str);
	str.Format(_T("%d"), m_nTempMax);
	m_editTempMax.SetWindowText(str);
	str.Format(_T("%d"), m_nTempMin);
	m_editTempMin.SetWindowText(str);

	// list setting
	m_listCondSet.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_listCondSet.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_listCondSet.DeleteAllItems();
	m_listCondSet.InsertColumn(0, _T(""), LVCFMT_LEFT, 0);
	m_listCondSet.InsertColumn(1, _T("No."), LVCFMT_CENTER, 50);
	m_listCondSet.InsertColumn(2, _T("ID"), LVCFMT_CENTER, 50);
	m_listCondSet.InsertColumn(3, _T("Condition"), LVCFMT_LEFT, 325);
	m_listCondSet.InsertColumn(4, _T("Time(sec)"), LVCFMT_CENTER, 90);
	m_listCondSet.InsertColumn(5, _T("Remark"), LVCFMT_LEFT, 325);

	m_listInfoSet.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_listInfoSet.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_listInfoSet.DeleteAllItems();
	m_listInfoSet.InsertColumn(0, _T(""), LVCFMT_LEFT, 0);
	m_listInfoSet.InsertColumn(1, _T("No."), LVCFMT_CENTER, 50);
	m_listInfoSet.InsertColumn(2, _T("ID Combination"), LVCFMT_LEFT, 395);
	m_listInfoSet.InsertColumn(3, _T("Information"), LVCFMT_LEFT, 395);

	// chart setting
	SetupChartFX(m_pChartFX1);
	SetupChartFX(m_pChartFX2);
	SetupChartFX(m_pChartFX3);
	
	return TRUE;
}

// add data
LRESULT CCastInfoDlg::OnAddone(WPARAM, LPARAM lParam) 
{
	StCondInput	stInput;
	CString		str;
	int			nData[NumADChannel];
    int			i;

	memcpy(nData, (int *)lParam, sizeof(nData));
	SetAverageData(nData);
	for( i = 0; i < NumTCTap; i++ )
		m_iData[i] = nData[i];

	UpdateChartAll();

	// average ABCDEF
	SetAverageTemp(stInput);

	// display ABCDEF
	str.Format(_T("%.1lf"), stInput.dTempAve[ID_TempA]);
	m_editTempA.SetWindowText(str);
	str.Format(_T("%.1lf"), stInput.dTempAve[ID_TempB]);
	m_editTempB.SetWindowText(str);
	str.Format(_T("%.1lf"), stInput.dTempAve[ID_TempC]);
	m_editTempC.SetWindowText(str);
	str.Format(_T("%.1lf"), stInput.dTempAve[ID_TempD]);
	m_editTempD.SetWindowText(str);
	str.Format(_T("%.1lf"), stInput.dTempAve[ID_TempE]);
	m_editTempE.SetWindowText(str);
	str.Format(_T("%.1lf"), stInput.dTempAve[ID_TempF]);
	m_editTempF.SetWindowText(str);

	// reset?
	if( GetRestartRequest() )
	{
		// reset condition & information
		ResetCastCond();
		ResetCastInfo();
	}

	// check condition & information
	CheckCondition(&stInput);
	CheckInformation();

	// check information message
	m_bInfoMsg = IsInfoOutNum();

	// send result information.
	COnlinePCDlg *dlg = (COnlinePCDlg *)AfxGetMainWnd();
	if( dlg->m_pInfoOut != NULL ){
		dlg->m_pInfoOut->SendMessage(WM_INFODATA, (WPARAM)GetInformation(), (LPARAM)GetInformationResult());
	}

	return 0;
}

LRESULT CCastInfoDlg::OnActInfo(WPARAM, LPARAM lParam) 
{
	StActiveInfo	info = *((StActiveInfo *)lParam);
	CString			str;

	str.Format(_T("%.2lf"), info.dVc );
	m_textCastVc.SetWindowText(str);
	str.Format(_T("%.0lf"), info.dWidthR+info.dWidthL );
	m_textWidth.SetWindowText(str);
	str.Format(_T("%.1lf"), info.dLevel );
	m_textActLevel.SetWindowText(str);
	str.Format(_T("%.0lf"), info.dLen );
	m_textCastLen.SetWindowText(str);
	str.Format(_T("%.0lf"), info.dEMBr );
	m_textEMBr.SetWindowText(str);

	return 0;
}

void CCastInfoDlg::UpdateChartAll(void)
{
	UpdateChart( m_pChartFX1, 0 );
	UpdateChart( m_pChartFX2, 1 );
	UpdateChart( m_pChartFX3, 2 );
}

// paint screen
void CCastInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	DrawColorAxis(dc);
	DrawMold(dc);
}

void CCastInfoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if( bShow )
	{
		if( ReadCastCond(m_cond, &m_numCond) )
		{
			SetupCondition();
		}
		if( ReadCastInfo(m_info, &m_numInfo) )
		{
			SetupInformation();
		}
	}
	else
	{
		// hide 
		if( m_pDlgTcView != NULL )
			m_pDlgTcView->ShowWindow(SW_HIDE);
	}
}

void CCastInfoDlg::OnSpinAngleX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	CString	str;
	
	m_nAngleX += pNMUpDown->iDelta;
	if( m_nAngleX > 90 )
		m_nAngleX = 90;
	if( m_nAngleX < 0 )
		m_nAngleX = 0;
	m_spnAngleX.SetPos(m_nAngleX);
	str.Format(_T("%d"), m_nAngleX);
	m_editAngleX.SetWindowText(str);
	UpdateChartAll();

	*pResult = 0;
}

void CCastInfoDlg::OnSpinAngleY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString	str;
	
	m_nAngleY += pNMUpDown->iDelta;
	if( m_nAngleY > 180 )
		m_nAngleY = 180;
	if( m_nAngleY < 0 )
		m_nAngleY = 0;
	m_spnAngleY.SetPos(m_nAngleY);
	str.Format(_T("%d"), m_nAngleY);
	m_editAngleY.SetWindowText(str);
	UpdateChartAll();

	*pResult = 0;
}

void CCastInfoDlg::OnSpinTempMax(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString	str;
	
	m_nTempMax += pNMUpDown->iDelta;
	if( m_nTempMax > 300 )
		m_nTempMax = 300;
	if( m_nTempMax <= m_nTempMin )
	{
		m_nTempMax++;
	}
	m_spnTempMax.SetPos(m_nTempMax);
	str.Format(_T("%d"), m_nTempMax);
	m_editTempMax.SetWindowText(str);
	UpdateChartAll();

	CRect	rc;
	m_lbColAxis.GetClientRect(rc);
	m_lbColAxis.MapWindowPoints(this, rc);
	InvalidateRect(rc, TRUE);

	*pResult = 0;
}

void CCastInfoDlg::OnSpinTempMin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString	str;
	
	m_nTempMin += pNMUpDown->iDelta;
	if( m_nTempMin < 0 )
		m_nTempMin = 0;
	if( m_nTempMax <= m_nTempMin )
	{
		m_nTempMin--;
	}
	m_spnTempMin.SetPos(m_nTempMin);
	str.Format(_T("%d"), m_nTempMin);
	m_editTempMin.SetWindowText(str);
	UpdateChartAll();

	CRect	rc;
	m_lbColAxis.GetClientRect(rc);
	m_lbColAxis.MapWindowPoints(this, rc);
	InvalidateRect(rc, TRUE);

	*pResult = 0;
}

// enable change load of menu.
void CCastInfoDlg::OnUpdateLoadData(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

// enable change save of menu.
void CCastInfoDlg::OnUpdateSaveData(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

// enable change save of menu.
void CCastInfoDlg::OnUpdateRestart(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

// enable change save of menu.
void CCastInfoDlg::OnUpdateTcposition(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

// load casting setting
void CCastInfoDlg::OnLoadData()
{
	if( ReadCastCond(m_cond, &m_numCond) )
	{
		SetupCondition();
	}
	if( ReadCastInfo(m_info, &m_numInfo) )
	{
		SetupInformation();
	}
}

// save casting setting
void CCastInfoDlg::OnSaveData()
{
	if(IDNO == AfxMessageBox(_T("May I save establishment value?"), MB_ICONEXCLAMATION | MB_YESNO))
		return;

	// save casting condition.
	WriteCastCond(m_cond, m_numCond);

	// save casting informatin.
	WriteCastInfo(m_info, m_numInfo);

	if(IDYES == AfxMessageBox(_T("A setting value isn't reflected when not restarting.\r\nMay I restart?"), MB_ICONEXCLAMATION | MB_YESNO))
	{
		// set restart request
		SetRestartRequest();
	}
}

// restart condition & information
void CCastInfoDlg::OnRestart()
{
	if(IDNO == AfxMessageBox(_T("May I restart?"), MB_ICONEXCLAMATION | MB_YESNO))
		return;

	// set restart request
	SetRestartRequest();
}

// display condition & information
void CCastInfoDlg::OnInfoOut()
{
	// send result information.
	COnlinePCDlg *dlg = (COnlinePCDlg *)AfxGetMainWnd();
	if( dlg->m_pInfoOut != NULL ){
		dlg->m_pInfoOut->ShowWindow(SW_SHOW);
		dlg->m_pInfoOut->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}

// selected 'TC Position' menu.
void CCastInfoDlg::OnTcposition()
{
	if( m_pDlgTcView == NULL )
	{
		m_pDlgTcView = new CTcVwDlg(this);
	}
	m_pDlgTcView->ShowWindow(SW_SHOW);
}

// clicked 'Add' button for condition setting.
void CCastInfoDlg::OnClickedCondAdd()
{
	CCondSet	dlg;
	StCondItem	stItem = {0};

	if( m_numCond >= COND_ITEM_MAX )
	{
		AfxMessageBox(_T("Can't Add!! (Max50)"));
		return;
	}

	dlg.SetData(m_listCondSet.GetItemCount()+1, stItem);
	if( IDOK == dlg.DoModal() )
	{
		dlg.GetData( stItem );

		CreateCondID( stItem );

		AddCondList( stItem );

		m_cond[m_numCond] = stItem;
		m_numCond++;
	}
}

// clicked 'Update' button for condition setting.
void CCastInfoDlg::OnClickedCondUpdate()
{
	CCondSet	dlg;
	StCondItem	stItem;
	int		nSel;

	nSel = GetListCurSel(m_listCondSet);
	if( nSel == -1 )
		return;

	stItem = m_cond[nSel];
	dlg.SetData(nSel+1, stItem);
	if( IDOK == dlg.DoModal() )
	{
		dlg.GetData( stItem );
		UpdateCondList( nSel, stItem );
		m_cond[nSel] = stItem;
	}
}

// clicked 'Delete' button for condition setting.
void CCastInfoDlg::OnClickedCondDelete()
{
	POSITION	pos = m_listCondSet.GetFirstSelectedItemPosition();
	int			cnt, sel;

	// check selection count.
	cnt = 0;
	while(pos != NULL)
	{
		sel = m_listCondSet.GetNextSelectedItem(pos);
		if( sel == -1 )
			break;
		
		// check cond ID
		if( CheckInfoUsed(m_cond[sel].nID) )
		{
			AfxMessageBox(_T("Can't delete!! (ID already used)"));
			return;
		}

		cnt++;
	}
	if( cnt == 0 ) return;

	if( cnt == 1 )
	{
		if( IDNO == AfxMessageBox(_T("Do you delete a item ?"), MB_YESNO) )
		{
			return;
		}
	}
	else
	{
		if( IDNO == AfxMessageBox(_T("Do you delete selection item ?"), MB_YESNO) )
		{
			return;
		}
	}

	// start delete
	pos = m_listCondSet.GetFirstSelectedItemPosition();
	while(pos != NULL)
	{
		sel = m_listCondSet.GetNextSelectedItem(pos);
		if( sel == -1 )
			break;

		// delete confirmation
		DeleteCondItem(sel);
	}

	RestructCondItem();
	SetupCondition();
}

// clicked 'Add' button for information setting.
void CCastInfoDlg::OnClickedInfoAdd()
{
	CInfoSet	dlg;
	StInfoItem	stItem = {0};
	int			i, id[COND_ITEM_MAX] = {0};

	if( m_numInfo >= INFO_ITEM_MAX )
	{
		AfxMessageBox(_T("Can't Add!! (Max50)"));
		return;
	}

	dlg.SetData(m_listInfoSet.GetItemCount()+1, stItem);
	for( i = 0; i < m_numCond; i++ )
	{
		id[i] = m_cond[i].nID;
	}
	dlg.SetIDList( id, m_numCond );
	if( IDOK == dlg.DoModal() )
	{
		dlg.GetData( stItem );

		AddInfoList( stItem );

		m_info[m_numInfo] = stItem;
		m_numInfo++;
	}
}

// clicked 'Update' button for information setting.
void CCastInfoDlg::OnClickedInfoUpdate()
{
	CInfoSet	dlg;
	StInfoItem	stItem;
	int		nSel;

	nSel = GetListCurSel(m_listInfoSet);
	if( nSel == -1 )
		return;

	stItem = m_info[nSel];
	dlg.SetData(nSel+1, stItem);
	if( IDOK == dlg.DoModal() )
	{
		dlg.GetData( stItem );
		UpdateInfoList( nSel, stItem );
		m_info[nSel] = stItem;
	}
}

// clicked 'Delete' button for information setting.
void CCastInfoDlg::OnClickedInfoDelete()
{
	POSITION	pos = m_listInfoSet.GetFirstSelectedItemPosition();
	int			cnt, sel;

	// check selection count.
	cnt = 0;
	while(pos != NULL)
	{
		sel = m_listInfoSet.GetNextSelectedItem(pos);
		if( sel == -1 )
			break;
		cnt++;
	}
	if( cnt == 0 ) return;

	if( cnt == 1 )
	{
		if( IDNO == AfxMessageBox(_T("Do you delete a item ?"), MB_YESNO) )
		{
			return;
		}
	}
	else
	{
		if( IDNO == AfxMessageBox(_T("Do you delete selection item ?"), MB_YESNO) )
		{
			return;
		}
	}

	// start delete
	pos = m_listInfoSet.GetFirstSelectedItemPosition();
	while(pos != NULL)
	{
		sel = m_listInfoSet.GetNextSelectedItem(pos);
		if( sel == -1 )
			break;

		// delete confirmation
		DeleteInfoItem(sel);
	}

	RestructInfoItem();
	SetupInformation();
}


///////////////////////////////////////////////////////////////////////////////
// member function

void CCastInfoDlg::SetupChartFX( IChartFXPtr pChartFX )
{
	pChartFX->MultipleColors = TRUE;
	pChartFX->Scheme = CHART_CSSOLID;
	pChartFX->PaletteBar = FALSE;
    pChartFX->ShowTips = FALSE;
    pChartFX->AllowDrag = FALSE;
	pChartFX->ContextMenus = FALSE;

	SetupPalleteChartFX( pChartFX );

	pChartFX->View3D = TRUE;
	pChartFX->AngleX = 75;
	pChartFX->AngleY = 5;
	pChartFX->SerLegBoxObj->BorderStyle = BBS_NONE;
	pChartFX->LeftGap = 0;
	pChartFX->RightGap = 0;
	pChartFX->TopGap = 0;
	pChartFX->BottomGap = 0;
	pChartFX->Gallery = SURFACE;// SURFACE 
	pChartFX->SerLegBoxObj->Docked = TGFP_RIGHT;
	pChartFX->SerLegBoxObj->Visible = FALSE;
	pChartFX->AxesStyle = (CfxAxesStyle) (pChartFX->AxesStyle | (CfxAxesStyle)32); // | AS_BREAKZERO

	ChangeContourStep( pChartFX, m_nTempMin, m_nTempMax );
}

void CCastInfoDlg::SetupPalleteChartFX( IChartFXPtr pChartFX )
{
	pChartFX->OpenDataEx(COD_COLORS, COLOR_NUM, 0);
	pChartFX->Color[ 0] = g_colTone[ 0];
	pChartFX->Color[ 1] = g_colTone[ 1];
	pChartFX->Color[ 2] = g_colTone[ 2];
	pChartFX->Color[ 3] = g_colTone[ 3];
	pChartFX->Color[ 4] = g_colTone[ 4];
	pChartFX->Color[ 5] = g_colTone[ 5];
	pChartFX->Color[ 6] = g_colTone[ 6];
	pChartFX->Color[ 7] = g_colTone[ 7];
	pChartFX->Color[ 8] = g_colTone[ 8];
	pChartFX->Color[ 9] = g_colTone[ 9];
	pChartFX->Color[10] = g_colTone[10];
	pChartFX->Color[11] = g_colTone[11];
	pChartFX->Color[12] = g_colTone[12];
	pChartFX->Color[13] = g_colTone[13];
	pChartFX->Color[14] = g_colTone[14];
	/*
	pChartFX->put_Color( 0,  RGB(  0,  0,255));//1 blue
	pChartFX->put_Color( 1,  RGB(  0, 25,225));
	pChartFX->put_Color( 2,  RGB(  0, 50,200));
	pChartFX->put_Color( 3,  RGB(  0, 75,175));
	pChartFX->put_Color( 4,  RGB(  0,100,150));
	pChartFX->put_Color( 5,  RGB(  0,125,125));
	pChartFX->put_Color( 6,  RGB(  0,150,100));
	pChartFX->put_Color( 7,  RGB(  0,175, 75));
	pChartFX->put_Color( 8,  RGB(  0,200, 50));
	pChartFX->put_Color( 9,  RGB(  0,225, 25));
	pChartFX->put_Color(10,  RGB(  0,255,  0));//2 green
	pChartFX->put_Color(11,  RGB( 25,255,  0));
	pChartFX->put_Color(12,  RGB( 50,255,  0));
	pChartFX->put_Color(13,  RGB( 75,255,  0));
	pChartFX->put_Color(14,  RGB(100,255,  0));
	pChartFX->put_Color(15,  RGB(125,255,  0));
	pChartFX->put_Color(16,  RGB(150,255,  0));
	pChartFX->put_Color(17,  RGB(175,255,  0));
	pChartFX->put_Color(18,  RGB(200,255,  0));
	pChartFX->put_Color(19,  RGB(225,250,  0));
	pChartFX->put_Color(20,  RGB(255,255,  0));//3 yellow
	pChartFX->put_Color(21,  RGB(255,225, 25));
	pChartFX->put_Color(22,  RGB(255,200, 50));
	pChartFX->put_Color(23,  RGB(255,175, 75));
	pChartFX->put_Color(24,  RGB(255,150,100));
	pChartFX->put_Color(25,  RGB(255,125,125));
	pChartFX->put_Color(26,  RGB(255,100,150));
	pChartFX->put_Color(27,  RGB(255, 75,175));
	pChartFX->put_Color(28,  RGB(255, 50,200));
	pChartFX->put_Color(29,  RGB(255, 25,225));
	pChartFX->put_Color(30,  RGB(255,  0,255));//4 perple
	pChartFX->put_Color(31,  RGB(255,  0,240));
	pChartFX->put_Color(32,  RGB(255,  0,225));
	pChartFX->put_Color(33,  RGB(255,  0,210));
	pChartFX->put_Color(34,  RGB(255,  0,195));
	pChartFX->put_Color(35,  RGB(255,  0,180));
	pChartFX->put_Color(36,  RGB(255,  0,165));
	pChartFX->put_Color(37,  RGB(255,  0,150));
	pChartFX->put_Color(38,  RGB(255,  0,135));
	pChartFX->put_Color(39,  RGB(255,  0,120));
	pChartFX->put_Color(40,  RGB(255,  0,105));//5 darkred
	pChartFX->put_Color(41,  RGB(255,  0, 90));
	pChartFX->put_Color(42,  RGB(255,  0, 75));
	pChartFX->put_Color(43,  RGB(255,  0, 60));
	pChartFX->put_Color(44,  RGB(255,  0, 45));
	pChartFX->put_Color(45,  RGB(255,  0, 30));
	pChartFX->put_Color(46,  RGB(255,  0, 15));
	pChartFX->put_Color(47,  RGB(255,  0,  0));
	pChartFX->put_Color(48,  RGB(255,  0,  0));
	pChartFX->put_Color(49,  RGB(255,  0,  0));//6 red
	*/
	pChartFX->CloseData( COD_COLORS );	// COD_COLORS
}

// update chart data
void CCastInfoDlg::UpdateChart( IChartFXPtr pChartFX, int iPos )
{
	ICfxSeriesPtr	pSerie;
	int				i;

	pChartFX->OpenDataEx( COD_VALUES, 2, 15 );

	// set data.
	for( i = 0; i < 15; i++ )
	{
		pSerie = pChartFX->Series->Item[(short)0];
		pSerie->Yvalue[i] = m_iData[iLOOSEPos3D[iPos][i]]*PhisicalTempUnit;
		pSerie = pChartFX->Series->Item[(short)1];
		pSerie->Yvalue[i] = m_iData[iFIXPos3D[iPos][i]]*PhisicalTempUnit;
	}

	// set angle
	if( pChartFX->AngleX != (short)m_nAngleX ||
		pChartFX->AngleY != (short)m_nAngleY )
	{
		pChartFX->AngleX = (short)m_nAngleX;
		pChartFX->AngleY = (short)m_nAngleY;
	}

	// set contour
	ChangeContourStep(pChartFX, m_nTempMin, m_nTempMax);

	pChartFX->SerLegBoxObj->Visible = FALSE;
	pChartFX->CloseData( (CfxCod)(COD_VALUES | COD_SMOOTH) );
}

// setAverageData.
void CCastInfoDlg::SetAverageData(int *data)
{
	int l[] = { 31, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 30};
	int	dmData[NumTCTap];
	int i, j;

	// 配列変数の初期化
	memcpy( dmData, data, sizeof(dmData) );

	for( i = 0; i < NumTCTap / TapRaw; i++ )
		for( j = 0; j < TapRaw; j++ )
			data[i * TapRaw + j] = dmData[l[i] * TapRaw + j];

}

void CCastInfoDlg::ChangeContourStep( IChartFXPtr pChartFX, int nMin, int nMax )
{
	if( pChartFX->Axis->Item[AXIS_Y]->Min != (double)nMin ||
		pChartFX->Axis->Item[AXIS_Y]->Max != (double)nMax )
	{
		pChartFX->Axis->Item[AXIS_Y]->STEP = (double)(nMax - nMin) / COLOR_NUM;		//AXIS_Y:0
		pChartFX->Axis->Item[AXIS_Y]->Min = (double)nMin;
		pChartFX->Axis->Item[AXIS_Y]->Max = (double)nMax;
		pChartFX->SetContourLabels(pChartFX->Axis->Item[AXIS_Y]->STEP);
	}
}

void CCastInfoDlg::DrawColorAxis(CDC &dc)
{
	COLORREF	bkCol;
	CBrush		brBox(RGB(0,0,0));
	CString		str;
	CRect		rc, rcBox, rcText;
	double		step;
	int			i, min, max;

	// ready
	m_lbColAxis.GetClientRect(rc);
	m_lbColAxis.MapWindowPoints(this, rc);
	rcBox = rc;
	rcText = rc;
	rcBox.right = rc.left + rcBox.Width() / 5;
	rcText.left = rc.left + rcText.Width() / 5;
	min = m_nTempMin;
	max = m_nTempMax;
	step = (double)(max - min) / COLOR_NUM;

	bkCol = dc.SetTextColor(RGB(0,0,0));
	for( i = 0; i < COLOR_NUM; i++ )
	{
		// box
		rcBox.top = rc.top + rc.Height() * i / COLOR_NUM;
		rcBox.bottom = rc.top + rc.Height() * (i+1) / COLOR_NUM;
		dc.FillSolidRect(rcBox, g_colTone[COLOR_NUM-i-1]);
		dc.FrameRect(rcBox, &brBox);

		// text
		rcText.top = rc.top + rc.Height() * i / COLOR_NUM;
		rcText.bottom = rc.top + rc.Height() * (i+1) / COLOR_NUM;
		dc.SetBkMode(TRANSPARENT);
		str.Format(_T("%3d-%3d"), (int)(max - step * (i+1)), (int)(max - step * i));
		dc.DrawText(str, rcText, DT_CENTER | DT_VCENTER);
	}
	dc.SetTextColor(bkCol);
}

void CCastInfoDlg::DrawMold(CDC &dc)
{
	CBrush		brBox(RGB(0,0,0));
	CRect		rc, rcBox, rcHeat;

	// ready
	m_lbMold.GetClientRect(rc);
	m_lbMold.MapWindowPoints(this, rc);
	rcBox = rc;
	rcHeat = rc;

	// box
	dc.FillSolidRect(rcBox, MoldColor);
	dc.FrameRect(rcBox, &brBox);

	// heat
	rcHeat.DeflateRect(15, 15);
	dc.FillSolidRect(rcHeat, RGB(255,0,0));
	dc.FrameRect(rcHeat, &brBox);
}

// add item to condition list.
void CCastInfoDlg::AddCondList( StCondItem &stItem )
{
	CString		strVal;
	int			nIdx;

	nIdx = m_listCondSet.GetItemCount();
	m_listCondSet.InsertItem(nIdx, _T(""));

	strVal.Format(_T("%02d"), nIdx + 1 );
	m_listCondSet.SetItemText(nIdx, 1, strVal);

	strVal.Format(_T("%d"), stItem.nID );
	m_listCondSet.SetItemText(nIdx, 2, strVal);

	strVal = CCondSet::GetCondString(stItem);
	m_listCondSet.SetItemText(nIdx, 3, strVal);

	strVal.Format(_T("%d"), stItem.nTime );
	m_listCondSet.SetItemText(nIdx, 4, strVal);

	strVal = stItem.cRemark;
	m_listCondSet.SetItemText(nIdx, 5, strVal);
}

// add item to condition list.
void CCastInfoDlg::UpdateCondList( int nSel, StCondItem &stItem )
{
	CString		strVal;

	strVal.Format(_T("%d"), stItem.nID );
	m_listCondSet.SetItemText(nSel, 2, strVal);

	strVal = CCondSet::GetCondString(stItem);
	m_listCondSet.SetItemText(nSel, 3, strVal);

	strVal.Format(_T("%d"), stItem.nTime );
	m_listCondSet.SetItemText(nSel, 4, strVal);

	strVal = stItem.cRemark;
	m_listCondSet.SetItemText(nSel, 5, strVal);
}

// delete condition item.
void CCastInfoDlg::DeleteCondItem(int nSel)
{
	m_cond[nSel].nItemNum = -1;
}

// restruct condition item.
void CCastInfoDlg::RestructCondItem(void)
{
	int		i;

	for( i = 0; i < m_numCond; i++ )
	{
		if( m_cond[i].nItemNum == -1 )
		{
			if( i < (m_numCond - 1) )
			{
				memmove(&m_cond[i], &m_cond[i+1], sizeof(StCondItem) * (m_numCond - i - 1));
				i--;
			}
			else
				memset(&m_cond[i], 0, sizeof(StCondItem));
			m_numCond--;
		}
	}
}

// setup condition
void CCastInfoDlg::SetupCondition(void)
{
	int		i;

	m_listCondSet.DeleteAllItems();

	for( i = 0; i < m_numCond; i++ )
	{
		AddCondList(m_cond[i]);
	}
}

// add item to information list.
void CCastInfoDlg::AddInfoList( StInfoItem &stItem )
{
	CString		strVal;
	int			nIdx;

	nIdx = m_listInfoSet.GetItemCount();
	m_listInfoSet.InsertItem(nIdx, _T(""));

	strVal.Format(_T("%02d"), nIdx + 1 );
	m_listInfoSet.SetItemText(nIdx, 1, strVal);

	strVal = CInfoSet::GetCondString(stItem);
	m_listInfoSet.SetItemText(nIdx, 2, strVal);

	strVal = stItem.cInfo;
	m_listInfoSet.SetItemText(nIdx, 3, strVal);
}

// add item to information list.
void CCastInfoDlg::UpdateInfoList( int nSel, StInfoItem &stItem )
{
	CString		strVal;

	strVal = CInfoSet::GetCondString(stItem);
	m_listInfoSet.SetItemText(nSel, 2, strVal);

	strVal = stItem.cInfo;
	m_listInfoSet.SetItemText(nSel, 3, strVal);
}

// delete information item.
void CCastInfoDlg::DeleteInfoItem(int nSel)
{
	m_info[nSel].nItemNum = -1;
}

// restruct information item.
void CCastInfoDlg::RestructInfoItem(void)
{
	int		i;

	for( i = 0; i < m_numInfo; i++ )
	{
		if( m_info[i].nItemNum == -1 )
		{
			if( i < (m_numInfo - 1) )
			{
				memmove(&m_info[i], &m_info[i+1], sizeof(StInfoItem) * (m_numInfo - i - 1));
				i--;
			}
			else
				memset(&m_info[i], 0, sizeof(StInfoItem));
			m_numInfo--;
		}
	}
}

// setup information
void CCastInfoDlg::SetupInformation(void)
{
	int		i;

	m_listInfoSet.DeleteAllItems();

	for( i = 0; i < m_numInfo; i++ )
	{
		AddInfoList(m_info[i]);
	}
}

// get list current selection.
int CCastInfoDlg::GetListCurSel(CListCtrl &list)
{
	POSITION pos = list.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return -1;
	else
	{
		return list.GetNextSelectedItem(pos);
	}
}

// create condition ID
void CCastInfoDlg::CreateCondID( StCondItem &stItem )
{
	int		i, j;

	for( i = 1; i <= COND_ITEM_MAX; i++ )
	{
		for( j = 0; j < m_numCond; j++ )
		{
			if( m_cond[j].nID == i )
				break;
		}
		if( j == m_numCond )
		{
			stItem.nID = i;
			break;
		}
	}
}

// check info used ID;
BOOL CCastInfoDlg::CheckInfoUsed( int nID )
{
	int		i, j;

	for( i = 0; i < m_numInfo; i++ )
	{
		for( j = 0; j < m_info[i].nItemNum; j++ )
		{
			if( m_info[i].data[j].nElem == nID )
				break;
		}
		if( j != m_info[i].nItemNum )
		{
			return TRUE;
		}
	}

	return FALSE;
}

// set average temperature.
void CCastInfoDlg::SetAverageTemp(StCondInput &stInput)
{
	SharedMemory	*pShm = GetSharedMemory();
	int				i, j, nSum[6], nCnt[6];

	memset(nSum, 0, sizeof(nSum));
	memset(nCnt, 0, sizeof(nCnt));
	for( i = 0; i < (NumFixTc + NumLooseTc); i++ )
	{
		if( (i % TapRaw) != 0 ) 
			continue;

		if( cnf.inputPoint[i].adCh == -1 )
			continue;

//		if( cnf.bTcExp[i] == TRUE )
//			continue;

		j = i / TapRaw;
		if( j >= 0 && j < 8 )
		{
			if( cnf.nTpPosData[j] > pShm->moldSizeR )
				continue;

			nSum[j/5] += m_iData[iFIXPos3D[0][14-j]];
			nCnt[j/5]++;
		}
		else if( j >= 8 && j < 15 )
		{
			if( cnf.nTpPosData[j] > pShm->moldSizeL )
				continue;

			nSum[j/5] += m_iData[iFIXPos3D[0][14-j]];
			nCnt[j/5]++;
		}
		else if( j >= 15 && j < 22 )
		{
			if( cnf.nTpPosData[j] > pShm->moldSizeR )
				continue;

			nSum[j/5] += m_iData[iLOOSEPos3D[0][29-j]];
			nCnt[j/5]++;
		}
		else if( j >= 22 && j < 30 )
		{
			if( cnf.nTpPosData[j] > pShm->moldSizeL )
				continue;

			nSum[j/5] += m_iData[iLOOSEPos3D[0][29-j]];
			nCnt[j/5]++;
		}
	}

	if( nCnt[0] == 0 ) nCnt[0] = 1;
	stInput.dTempAve[ID_TempC] = (double)(nSum[0] / nCnt[0] * PhisicalTempUnit);
	if( nCnt[1] == 0 ) nCnt[1] = 1;
	stInput.dTempAve[ID_TempB] = (double)(nSum[1] / nCnt[1] * PhisicalTempUnit);
	if( nCnt[2] == 0 ) nCnt[2] = 1;
	stInput.dTempAve[ID_TempA] = (double)(nSum[2] / nCnt[2] * PhisicalTempUnit);
	if( nCnt[3] == 0 ) nCnt[3] = 1;
	stInput.dTempAve[ID_TempF] = (double)(nSum[3] / nCnt[3] * PhisicalTempUnit);
	if( nCnt[4] == 0 ) nCnt[4] = 1;
	stInput.dTempAve[ID_TempE] = (double)(nSum[4] / nCnt[4] * PhisicalTempUnit);
	if( nCnt[5] == 0 ) nCnt[5] = 1;
	stInput.dTempAve[ID_TempD] = (double)(nSum[5] / nCnt[5] * PhisicalTempUnit);
}

