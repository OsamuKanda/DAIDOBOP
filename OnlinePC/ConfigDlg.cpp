// ConfigDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "OnlinePCDlg.h"
#include "ConfigDlg.h"
#include "../include/graphic.h"
#include ".\configdlg.h"

#define NO_PASSWD

///////////////////////////////////////////////////////////////////////////////
// definition
#define	TcTapSize		15
#define	NNLineSize		7
#define	TPPosMax		1250
#define	TPPosMin		-1250

// reference data
static CConfig			&cnf		= theApp.m_cnf;
static InputPoint		*inputPoint	= theApp.m_cnf.inputPoint;
//static int				*nTpPosData = theApp.m_cnf.nTpPosData;
static COLORREF			&MoldColor	= theApp.m_col.clMoldColor;
static COLORREF			Black		= RGB(0,0,0);
static COLORREF			White		= RGB(255,255,255);
static COLORREF			*ToneColor	= theApp.m_col.clTones;
static TCCombination	*tcCombi	= theApp.m_cnf.tcCombination;
static int				&numCombi	= theApp.m_cnf.numCombination;
static BOOL				*pbTcExp	= theApp.m_bTcExp;
static int				*pnCutOffTimer = theApp.m_nCutOffTimer;
static TcName			*tcnm		= &theApp.m_stTcNm;


///////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog
IMPLEMENT_DYNAMIC(CConfigDlg, CDialog)

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	// initialize member
	m_pDlgAdTable	= NULL;
	m_pDlgNnTable	= NULL;
	m_nAsnArg		= -1;
	m_bADAssign		= FALSE;
	m_bNetDef		= FALSE;
	m_nNetDefStep	= 0;
	m_bPass			= FALSE;
	m_bAutoLoad		= FALSE;
	int i;
	for( i = 0; i < NumSteelKind; i++ )
		m_strAutoLoadName[i] = _T("");
	for( i = 0; i < NumSteelKind; i++ )
		m_strAutoLoadNameD[i] = _T("");
	for( i = 0; i < NumInput; i++ )
		m_pTcButton[i] = NULL;

	// create dialog.
	Create(CConfigDlg::IDD, pParent);				
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FIX_FRAME, m_frameFIX);
	DDX_Control(pDX, IDC_LOOSE_FRAME, m_frameLOOSE);
	DDX_Control(pDX, IDC_WEST_FRAME, m_frameWEST);
	DDX_Control(pDX, IDC_EAST_FRAME, m_frameEAST);
	DDX_Control(pDX, IDC_FIX_NAME, m_textFIX);
	DDX_Control(pDX, IDC_LOOSE_NAME, m_textLOOSE);
	DDX_Control(pDX, IDC_WEST_NAME, m_textWEST);
	DDX_Control(pDX, IDC_EAST_NAME, m_textEAST);
	DDX_Control(pDX, IDC_FIX_NAME2, m_textFIX2);
	DDX_Control(pDX, IDC_LOOSE_NAME2, m_textLOOSE2);
	DDX_Control(pDX, IDC_WEST_NAME2, m_textWEST2);
	DDX_Control(pDX, IDC_EAST_NAME2, m_textEAST2);
	DDX_Control(pDX, IDC_MOLD_FRAME, m_frameMOLD);
	DDX_Control(pDX, IDC_MOLD_FIX, m_moldFIX);
	DDX_Control(pDX, IDC_MOLD_LOOSE, m_moldLOOSE);
	DDX_Control(pDX, IDC_MOLD_WEST, m_moldWEST);
	DDX_Control(pDX, IDC_MOLD_EAST, m_moldEAST);
	DDX_Control(pDX, IDC_MOLD_RTNO, m_moldRTNO);
	DDX_Control(pDX, IDC_MOLD_LTNO, m_moldLTNO);
	DDX_Control(pDX, IDC_MOLD_LBNO, m_moldLBNO);
	DDX_Control(pDX, IDC_MOLD_RBNO, m_moldRBNO);
	DDX_Control(pDX, IDC_MOLD_WTNO, m_moldWTNO);
	DDX_Control(pDX, IDC_MOLD_ETNO, m_moldETNO);
	DDX_Control(pDX, IDC_MOLD_EYE, m_moldEye);
	DDX_Control(pDX, IDC_RADIO1, m_tc01);
	DDX_Control(pDX, IDC_RADIO2, m_tc02);
	DDX_Control(pDX, IDC_RADIO3, m_tc03);
	DDX_Control(pDX, IDC_RADIO4, m_tc04);
	DDX_Control(pDX, IDC_RADIO5, m_tc05);
	DDX_Control(pDX, IDC_RADIO6, m_tc06);
	DDX_Control(pDX, IDC_RADIO7, m_tc07);
	DDX_Control(pDX, IDC_RADIO8, m_tc08);
	DDX_Control(pDX, IDC_RADIO9, m_tc09);
	DDX_Control(pDX, IDC_RADIO10, m_tc10);
	DDX_Control(pDX, IDC_RADIO11, m_tc11);
	DDX_Control(pDX, IDC_RADIO12, m_tc12);
	DDX_Control(pDX, IDC_RADIO13, m_tc13);
	DDX_Control(pDX, IDC_RADIO14, m_tc14);
	DDX_Control(pDX, IDC_RADIO15, m_tc15);
	DDX_Control(pDX, IDC_RADIO16, m_tc16);
	DDX_Control(pDX, IDC_RADIO17, m_tc17);
	DDX_Control(pDX, IDC_RADIO18, m_tc18);
	DDX_Control(pDX, IDC_RADIO19, m_tc19);
	DDX_Control(pDX, IDC_RADIO20, m_tc20);
	DDX_Control(pDX, IDC_RADIO21, m_tc21);
	DDX_Control(pDX, IDC_RADIO22, m_tc22);
	DDX_Control(pDX, IDC_RADIO23, m_tc23);
	DDX_Control(pDX, IDC_RADIO24, m_tc24);
	DDX_Control(pDX, IDC_RADIO25, m_tc25);
	DDX_Control(pDX, IDC_RADIO26, m_tc26);
	DDX_Control(pDX, IDC_RADIO27, m_tc27);
	DDX_Control(pDX, IDC_RADIO28, m_tc28);
	DDX_Control(pDX, IDC_RADIO97, m_sn);
	DDX_Control(pDX, IDC_RADIO98, m_level);
	DDX_Control(pDX, IDC_RADIO99, m_vc);
	DDX_Control(pDX, IDC_RADIO100, m_width);
//	DDX_Control(pDX, IDC_RADIO100, m_widthL);
//	DDX_Control(pDX, IDC_RADIO101, m_widthR);
	DDX_Control(pDX, IDC_RADIO102, m_length);
	DDX_Control(pDX, IDC_RADIO114, m_tdweight);
	DDX_Control(pDX, IDC_RADIO115, m_mdwtA);
	DDX_Control(pDX, IDC_RADIO116, m_mdwtC);
	DDX_Control(pDX, IDC_RADIO117, m_mdwtD);
	DDX_Control(pDX, IDC_RADIO118, m_mdtmpD);
	DDX_Control(pDX, IDC_CONF_NAME, m_textConfName);
	DDX_Control(pDX, IDC_EDIT1, m_tp01);
	DDX_Control(pDX, IDC_EDIT2, m_tp02);
	DDX_Control(pDX, IDC_EDIT3, m_tp03);
	DDX_Control(pDX, IDC_EDIT4, m_tp04);
	DDX_Control(pDX, IDC_EDIT5, m_tp05);
	DDX_Control(pDX, IDC_EDIT6, m_tp06);
	DDX_Control(pDX, IDC_EDIT7, m_tp07);
	DDX_Control(pDX, IDC_EDIT8, m_tp08);
	DDX_Control(pDX, IDC_EDIT9, m_tp09);
	DDX_Control(pDX, IDC_EDIT10, m_tp10);
	DDX_Control(pDX, IDC_EDIT11, m_tp11);
	DDX_Control(pDX, IDC_EDIT12, m_tp12);
	DDX_Control(pDX, IDC_EDIT13, m_tp13);
	DDX_Control(pDX, IDC_EDIT14, m_tp14);
	DDX_Control(pDX, IDC_EDIT15, m_tp15);
	DDX_Control(pDX, IDC_EDIT16, m_tp16);
	DDX_Control(pDX, IDC_EDIT17, m_tp17);
	DDX_Control(pDX, IDC_EDIT18, m_tp18);
	DDX_Control(pDX, IDC_EDIT19, m_tp19);
	DDX_Control(pDX, IDC_EDIT20, m_tp20);
	DDX_Control(pDX, IDC_EDIT21, m_tp21);
	DDX_Control(pDX, IDC_EDIT22, m_tp22);
	DDX_Control(pDX, IDC_EDIT23, m_tp23);
	DDX_Control(pDX, IDC_EDIT24, m_tp24);
	DDX_Control(pDX, IDC_EDIT25A, m_tp25);
	DDX_Control(pDX, IDC_EDIT26A, m_tp26);
	DDX_Control(pDX, IDC_EDIT27A, m_tp27);
	DDX_Control(pDX, IDC_EDIT28A, m_tp28);
	DDX_Control(pDX, IDC_EDIT29A, m_tp29);
	DDX_Control(pDX, IDC_EDIT30A, m_tp30);
	DDX_Control(pDX, IDC_EDIT31A, m_tp31);
	DDX_Control(pDX, IDC_EDIT32A, m_tp32);
	DDX_Control(pDX, IDC_CONF_NAME2, m_textSelConfName);
	DDX_Control(pDX, IDC_BUTTON35, m_btnPass);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(IDM_ADCH_ASSIGN, OnAdchAssign)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(IDM_NET_DEFINE, OnNetDefine)
	ON_COMMAND(IDM_CONFIG_NAME, OnConfigName)
	ON_COMMAND(IDM_CONFIG_LOAD, OnConfigLoad)
	ON_COMMAND(IDM_CONFIG_SAVE, OnConfigSave)
//	ON_COMMAND(IDM_RELATION, OnRelation)
	ON_UPDATE_COMMAND_UI(IDM_CONFIG_NAME, OnUpdateConfigName)
	ON_UPDATE_COMMAND_UI(IDM_CONFIG_LOAD, OnUpdateConfigLoad)
	ON_UPDATE_COMMAND_UI(IDM_CONFIG_SAVE, OnUpdateConfigSave)
	ON_UPDATE_COMMAND_UI(IDM_ADCH_ASSIGN, OnUpdateAdchAssign)
	ON_UPDATE_COMMAND_UI(IDM_NET_DEFINE, OnUpdateNetDefine)
//	ON_UPDATE_COMMAND_UI(IDM_RELATION, OnUpdateRelation)
	ON_BN_CLICKED(IDC_BUTTON35, OnClickedPasswordButton)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_RADIO4, &CConfigDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_BUTTON1, &CConfigDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CConfigDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CConfigDlg メッセージ ハンドラ

//It evades ending by Enter Key.
void CConfigDlg::OnOk(){}

//It evades ending by ESC Key.
void CConfigDlg::OnCancel(){}

void CConfigDlg::PostNcDestroy() 
{
	delete this;
//	CDialog::PostNcDestroy();
}

// paint screen
void CConfigDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	DrawCanvas(dc);
}

// WM_MOVE
void CConfigDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

#ifdef FIX_SCREEN
	SetWindowPos( &wndTop, 0, 69, 0, 0, SWP_NOSIZE );
#endif
}

// It is only 1 execute when the dialog opens.
BOOL CConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// setup input point for screen
	SetupInputPoints();

	//// setup TC pos for screen
	//SetupTCPos();

	// disp work directry name
	m_textConfName.SetWindowText(cnf.GetWorkDirName());
	m_textSelConfName.SetWindowText(cnf.GetWorkDirName());
	
	return TRUE;
}

// show window
void CConfigDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if( !bShow )
	{
		if( m_pDlgAdTable != NULL )
			m_pDlgAdTable->ShowWindow(SW_HIDE);
		if( m_pDlgNnTable != NULL )
			m_pDlgNnTable->ShowWindow(SW_HIDE);
	}
	else if( bShow )
	{
		// set color for password button.
		#ifdef NO_PASSWD
		m_bPass = TRUE;
		#else
		m_bPass = CPassDlg::CheckValidPass();
		#endif
		EnableControl(m_bPass);

		// auto load configuration
		CString	name;
		int		i;

		if( m_bAutoLoad )
		{
			m_textConfName.GetWindowText(name);
			for( i = 0; i < NumSteelKind; i++ )
			{
				if( m_strAutoLoadName[i].GetLength() > 0 &&
					name == m_strAutoLoadName[i] )
				{
					if( IDYES == AfxMessageBox(IDS_CNF_DLG04, MB_YESNO) )
						LoadConfig();
					for( i = 0; i < NumSteelKind; i++ )
						m_strAutoLoadName[i] = _T("");
					for( i = 0; i < NumSteelKind; i++ )
						m_strAutoLoadNameD[i] = _T("");
					break;
				}
				//for DEP.
				if( m_strAutoLoadNameD[i].GetLength() > 0 &&
					name == m_strAutoLoadNameD[i] )
				{
					if( IDYES == AfxMessageBox(IDS_CNF_DLG04, MB_YESNO) )
						LoadConfig();
					for( i = 0; i < NumSteelKind; i++ )
						m_strAutoLoadName[i] = _T("");
					for( i = 0; i < NumSteelKind; i++ )
						m_strAutoLoadNameD[i] = _T("");
					break;
				}
			}
			m_bAutoLoad = FALSE;
		}
	}
}

// clicked configuration name of menu.
void CConfigDlg::OnConfigName()
{
	CConfName	dlg;

	if( IDOK == dlg.DoModal() )
	{
		m_textConfName.SetWindowText(cnf.GetWorkDirName());
		m_textSelConfName.SetWindowText(dlg.m_szSelectName);
	}
}

// clicked load configuration of menu.
void CConfigDlg::OnConfigLoad()
{
	if( IDNO == AfxMessageBox(IDS_CNF_DLG02, MB_YESNO) )
		return;

	LoadConfig();
}
void CConfigDlg::LoadConfig()
{
	_TCHAR buf[256];

	m_textSelConfName.GetWindowText(buf, 256);

	// store word directory name.
	cnf.SetWorkDirName(buf);

	// work directory name copy.
	cnf.CopyWorkDirName();
	
	// load configration.
	cnf.LoadConfiguration(cnf.workDir);

	// setup input point for screen
	SetupInputPoints();

	// setup TC pos for screen
	//SetupTCPos();

	// redraw window
	if (m_bADAssign && m_pDlgAdTable != NULL)
		m_pDlgAdTable->Invalidate(TRUE);
	if (m_bNetDef && m_pDlgNnTable != NULL)
		m_pDlgNnTable->Invalidate(TRUE);

	// get work dir name
	m_textConfName.SetWindowText(cnf.GetWorkDirName());

	// neural config cnv. send
	ConfReset();
}

// clicked save configuration of menu.
void CConfigDlg::OnConfigSave()
{
	_TCHAR buf[256];

	if( IDNO == AfxMessageBox(IDS_CNF_DLG03, MB_YESNO) )
		return;

	m_textSelConfName.GetWindowText(buf, 256);

	// store word directory name.
	cnf.SetWorkDirName(buf);

	//// store tc position.
	//if( StoreTCPos() == FALSE )
	//{
	//	AfxMessageBox(IDS_CNF_DLG01, MB_ICONEXCLAMATION | MB_OK);
	//	return;
	//}

	// save configuration
	if( cnf.SaveConfiguration() )
	{
		// get work dir name
		m_textConfName.SetWindowText(cnf.GetWorkDirName());

		// reset BOP
		BopReset(buf);

		// work directory name copy.
		cnf.CopyWorkDirName();
		
		// neural config cnv. send
		ConfReset();
	}
	else
	{
		AfxMessageBox(IDS_CNF_DLG00, MB_ICONEXCLAMATION | MB_OK);
	}
}

// clicked A/D channel assign of menu.
void CConfigDlg::OnAdchAssign()
{
	if( m_bNetDef )
	{
		m_bNetDef = FALSE;
		if( m_pDlgNnTable != NULL )
			m_pDlgNnTable->ShowWindow(SW_HIDE);
	}
	if( m_pDlgAdTable == NULL )
	{
		m_pDlgAdTable = new CAdTable(this);
	}
	m_pDlgAdTable->ShowWindow(SW_SHOW);
	m_bADAssign = TRUE;
}

// clicked network definition of menu.
void CConfigDlg::OnNetDefine()
{
	if( m_bADAssign )
	{
		m_bADAssign = FALSE;
		if( m_pDlgAdTable != NULL )
			m_pDlgAdTable->ShowWindow(SW_HIDE);
	}
	if( m_pDlgNnTable == NULL )
	{
		m_pDlgNnTable = new CNnTable(this);
	}
	m_pDlgNnTable->ShowWindow(SW_SHOW);
	m_bNetDef = TRUE;
	m_nNetDefStep = 0;
}

//void CConfigDlg::OnRelation()
//{
//	CSkAssign	dlg;
//
//	dlg.DoModal();
//}

void CConfigDlg::OnUpdateConfigName(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bPass);
}

void CConfigDlg::OnUpdateConfigLoad(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bPass);
}

void CConfigDlg::OnUpdateConfigSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bPass);
}

void CConfigDlg::OnUpdateAdchAssign(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bPass);
}

void CConfigDlg::OnUpdateNetDefine(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bPass);
}

//void CConfigDlg::OnUpdateRelation(CCmdUI *pCmdUI)
//{
//	pCmdUI->Enable(m_bPass);
//}

// clicked double left mouse button.
void CConfigDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CClientDC	dc(this);
	DWORD	dwStyle;
	int		x, y, nAsnArg;

	if( !m_bPass )
		return;


	// get ad table window style
	if( m_pDlgAdTable != NULL )
		dwStyle = m_pDlgAdTable->GetStyle();
	else
		dwStyle = 0;

	// search TC name point and if found it, 
	if (GetTCNameAssignPoint(point.x, point.y,
							&nAsnArg, &x, &y) == TRUE) 
	{
		// pop up TC Name setting Dialog Box.
		CTcName	dlg(nAsnArg, this);
		if( IDOK == dlg.DoModal() )
		{
			// tc name 読み込み
			GetTcName();

			// update
			Invalidate(TRUE);
			if (m_bADAssign && ((dwStyle & WS_VISIBLE) != 0))
			{
				m_pDlgAdTable->Invalidate(TRUE);
			}
		}

		return;
	}

	// search A/D assign point and if found it, 
	if (m_bADAssign && ((dwStyle & WS_VISIBLE) != 0) &&
		GetADAssignPoint(point.x, point.y,
							&m_nAsnArg, &x, &y) == TRUE) 
	{
		// change color to white. 
		DrawADAssignPoint(dc, White, x, y, TcTapSize);

		// pop up A/D assignment Dialog Box.
		CAdChSign	dlg(m_nAsnArg, this);
		if( IDOK == dlg.DoModal() )
		{
			// send manual cutoff message
			ManualCutoffTC(m_nAsnArg, ((cnf.inputPoint[m_nAsnArg].adCh != -1) ? FALSE: TRUE));
		}
		m_nAsnArg = -1;
		Invalidate(TRUE);
		m_pDlgAdTable->Invalidate(TRUE);
	}
	else 
	{
		// Beep. 
		MessageBeep(0xFFFFFFFF);
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

// clicked left mouse button.
void CConfigDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	DWORD	dwStyle;
	int x, y;

	if( !m_bPass )
		return;

	// get ad table window style
	if( m_pDlgNnTable != NULL )
		dwStyle = m_pDlgNnTable->GetStyle();
	else
		dwStyle = 0;

	// when configrating neural network, 
	if (m_bNetDef && ((dwStyle & WS_VISIBLE) != 0) )
	{
		// if pointed geometry is valid, 
		if (GetADAssignPoint(point.x, point.y,
							&m_nAsnPoint[m_nNetDefStep], &x, &y) == TRUE && m_nAsnPoint[m_nNetDefStep] < NumTCTap) 
		{
			// if two points are specified, 
			if (++m_nNetDefStep == 2) 
			{
				// if valid specification, 
				if (m_nAsnPoint[0] != m_nAsnPoint[1]) 
				{
					// pop up combination assignment Dialog Box. 
					CCombiSign	dlg(m_nAsnPoint[0], m_nAsnPoint[1], this);
					dlg.DoModal();
					Invalidate(TRUE);
				}
				// if same point is specified twice, 
				else 
				{
					// Beep. 
					MessageBeep(0);
				}
				// initialize step. 
				m_nNetDefStep	= 0;
			}
		}
		// if pointed geometry is invalid, 
		else 
		{
			// initialize step and beep. 
			m_nNetDefStep	= 0;
			MessageBeep(0);
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CConfigDlg::OnClickedPasswordButton()
{
	CPassDlg	dlg;

	if( m_bPass == FALSE && IDOK == dlg.DoModal() )
	{
		m_bPass = TRUE;
		EnableControl(m_bPass);
	}
}

///////////////////////////////////////////////////////////////////////////////
// CConfigDlg user function

void CConfigDlg::DrawCanvas(CDC &dc)
{
	// draw mold surface. 
	DrawMold(dc);

	// draw mold model. 
	DrawMoldModel(dc);

	// draw thermo-couple combination. 
	DrawTCCombination(dc);

	// draw TC Taps. 
	DrawInputPoints(dc);
}

void CConfigDlg::DrawMold(CDC &dc)
{
	CStatic*	sf[] = {&m_frameFIX,&m_frameLOOSE,&m_frameWEST,&m_frameEAST,NULL};
	CStatic*	sn[] = {&m_textFIX, &m_textLOOSE, &m_textWEST, &m_textEAST, NULL};
	CStatic*	sn2[] = {&m_textFIX2, &m_textLOOSE2, &m_textWEST2, &m_textEAST2, NULL};
	CBrush		brBox(Black);
	CString		str;
	CRect		rc;
	int			i;

	i = 0;
	while( sf[i] != NULL && sn[i] != NULL )
	{
		// draw Mold surface
		sf[i]->GetClientRect(rc);
		sf[i]->MapWindowPoints(this, rc);
		dc.FillSolidRect(rc, MoldColor);
		dc.FrameRect(rc, &brBox);

		// draw 'Name'
		sn[i]->GetClientRect(rc);
		sn[i]->MapWindowPoints(this, rc);
		sn[i]->GetWindowText(str);
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(str, rc, DT_CENTER);

		// draw 'Name2'
		sn2[i]->GetClientRect(rc);
		sn2[i]->MapWindowPoints(this, rc);
		sn2[i]->GetWindowText(str);
		dc.DrawText(str, rc, DT_CENTER);

		i++;
	}
}

void CConfigDlg::DrawMoldModel(CDC &dc)
{
	CStatic*	sf[] = {&m_frameMOLD,NULL};
	CStatic*	sn[] = {&m_moldFIX, &m_moldLOOSE, &m_moldWEST, &m_moldEAST, 
						&m_moldEye, NULL};
	CStatic*	stc[] ={&m_moldRTNO, &m_moldLTNO, &m_moldLBNO, &m_moldRBNO, 
						&m_moldWTNO, &m_moldETNO, NULL};
	int			nNo[] ={          6,          11,          20,          25,
						          2,          16, NULL};

	CBrush		brBox(Black);
	CString		str;
	CRect		rc;
	CPoint		line;
	int			i;

	i = 0;
	while( sf[i] != NULL )
	{
		// draw Mold surface
		sf[i]->GetClientRect(rc);
		sf[i]->MapWindowPoints(this, rc);
		dc.FillSolidRect(rc, MoldColor);
		dc.FrameRect(rc, &brBox);
		i++;
	}

	i = 0;
	while( sn[i] != NULL )
	{
		// draw 'Name'
		sn[i]->GetClientRect(rc);
		sn[i]->MapWindowPoints(this, rc);
		sn[i]->GetWindowText(str);
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(str, rc, DT_CENTER);
		i++;
	}

	i = 0;
	while( stc[i] != NULL )
	{
		// draw 'Name'
		stc[i]->GetClientRect(rc);
		stc[i]->MapWindowPoints(this, rc);
		str.Format(_T("%s"), tcnm->name[nNo[i]]);
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText(str, rc, DT_CENTER);
		i++;
	}

	// draw eye line
	//m_moldEye.GetClientRect(rc);
	//m_moldEye.MapWindowPoints(this, rc);
	//line.x = rc.left + rc.Width()/2;
	//line.y = rc.top+20;
	//dc.MoveTo(line);
	//line.y += 60;	// arrow length
	//dc.LineTo(line);
	//dc.LineTo(line.x+5, line.y-5);
	//dc.LineTo(line);
	//dc.LineTo(line.x-5, line.y-5);
}

void CConfigDlg::DrawTCCombination(CDC &dc)
{
	int	i;

	// for each thermo couple combination, 
	for (i = 0; i < numCombi; i++) {
		// draw combination line. 
		DrawTCCombiLine(dc, &tcCombi[i]);
	}
}

// draw combination line.
void CConfigDlg::DrawTCCombiLine(CDC &dc, TCCombination *cp)
{
	int	color	= ToneColor[4 * cp->netID];
	int	sx		= inputPoint[cp->upper].gx;
	int	sy		= inputPoint[cp->upper].gy;
	int	ex		= inputPoint[cp->lower].gx;
	int	ey		= inputPoint[cp->lower].gy;

	// if link next of lower neighbor. 
	if (cp->lower == cp->upper + 2) 
	{
		DrawLine(dc, PS_SOLID, NNLineSize,   Black,	sx,   sy, sx-15, sy+5);
		DrawLine(dc, PS_SOLID, NNLineSize,   Black, sx-15, sy+5, ex-15, ey-5);
		DrawLine(dc, PS_SOLID, NNLineSize,	 Black, ex-15, ey-5,    ex,   ey);
		DrawLine(dc, PS_SOLID, NNLineSize-2, color,    sx,   sy, sx-15, sy+5);
		DrawLine(dc, PS_SOLID, NNLineSize-2, color, sx-15, sy+5, ex-15, ey-5);
		DrawLine(dc, PS_SOLID, NNLineSize-2, color, ex-15, ey-5,    ex,   ey);
	}
	// if link next of left neighbor. 
	else if (cp->lower == cp->upper - 6) 
	{
		DrawLine(dc, PS_SOLID, NNLineSize,   Black,   sx,    sy, sx-5, sy-15);
		DrawLine(dc, PS_SOLID, NNLineSize,   Black, sx-5, sy-15, ex+5, ey-15);
		DrawLine(dc, PS_SOLID, NNLineSize,   Black, ex+5, ey-15,   ex,    ey);
		DrawLine(dc, PS_SOLID, NNLineSize-2, color,   sx,    sy, sx-5, sy-15);
		DrawLine(dc, PS_SOLID, NNLineSize-2, color, sx-5, sy-15, ex+5, ey-15);
		DrawLine(dc, PS_SOLID, NNLineSize-2, color, ex+5, ey-15,   ex,    ey);
	}
	// the other case, 
	else 
	{
		DrawLine(dc, PS_SOLID, NNLineSize,   Black, sx, sy, ex, ey);
		DrawLine(dc, PS_SOLID, NNLineSize-2, color, sx, sy, ex, ey);
	}
}

void CConfigDlg::DrawLine(CDC &dc, int ps, int width, COLORREF col, int x1, int y1, int x2, int y2)
{
	CPen	pen(ps, width, col);
	CPen	*pPen;

	pPen = dc.SelectObject(&pen);
	dc.MoveTo(x1, y1);
	dc.LineTo(x2, y2);
	dc.SelectObject(pPen);
}

void CConfigDlg::DrawInputPoints(CDC &dc)
{
	COLORREF	color;
	CRect		rc;
	CString		str;
	TCHAR		*pBuf;
	int			i;

	// for each TC Taps.
	for (i = 0; i < NumInput; i++)
	{
		// skip if disable.
		if (inputPoint[i].gx < 0 || inputPoint[i].gy < 0) continue;

		// get display color.
		if (m_nAsnArg == i)
			color	= White;
		else if (inputPoint[i].adCh >= 0)
			color	= ToneColor[inputPoint[i].adCh];
		else
			color	= Black;

		// draw A/D assign point. 
		DrawADAssignPoint(dc, color, inputPoint[i].gx, inputPoint[i].gy, TcTapSize);

		// if now drawing TCTap
		if (i < NumTCTap) 
		{
			// draw TC Tap name.
			if( m_pTcButton[i] != NULL )
			{
//				m_pTcButton[i]->GetWindowText(str);
//				pBuf = str.GetBuffer(256);
//				PrintText(dc, NULL, Black,
//					inputPoint[i].gx + 3, inputPoint[i].gy - 25, pBuf);
//				str.ReleaseBuffer(256);
				PrintText(dc, NULL, Black,
					inputPoint[i].gx + 3, inputPoint[i].gy - 25, tcnm->name[i]);
			}
		}
		else if (i < (LastID+5) ) 
		{
			// draw TC Tap name.
			if( m_pTcButton[i] != NULL )
			{
				m_pTcButton[i]->GetWindowText(str);
				pBuf = str.GetBuffer(256);
				PrintText(dc, NULL, Black,
					inputPoint[i].gx + TcTapSize, inputPoint[i].gy - 10, pBuf);
				str.ReleaseBuffer(256);
			}
		}
	}
}

void CConfigDlg::DrawADAssignPoint(CDC &dc, COLORREF col, int x, int y, int size)
{
	CPen		pen, *ppen;
	CBrush		brush, *pbrush;

	pen.CreatePen(PS_SOLID, 1, Black);
	brush.CreateSolidBrush(col);
	ppen = dc.SelectObject(&pen);
	pbrush = dc.SelectObject(&brush);
	dc.Ellipse(	x-size/2, y-size/2, 
				x+size/2, y+size/2);
	dc.SelectObject(ppen);
	dc.SelectObject(pbrush);
	pen.DeleteObject();
	brush.DeleteObject();
}

// initialize input points for screen.
void CConfigDlg::SetupInputPoints(void)
{
	CButton*	bn[] = {
					&m_tc01,&m_tc02,&m_tc03,&m_tc04,&m_tc05,&m_tc06,&m_tc07,&m_tc08,&m_tc09,
					&m_tc10,&m_tc11,&m_tc12,&m_tc13,&m_tc14,&m_tc15,&m_tc16,&m_tc17,&m_tc18,
					&m_tc19,&m_tc20,&m_tc21,&m_tc22,&m_tc23,&m_tc24,&m_tc25,&m_tc26,&m_tc27,
					&m_tc28,&m_level,&m_vc,&m_sn,&m_length,&m_tdweight,&m_mdwtA,&m_mdwtC,&m_mdwtD,&m_mdtmpD,
//					&m_sn,&m_level,&m_vc,&m_widthL,&m_widthR,&m_length,
				NULL};
	CRect	rc;
	int i, idx;

	idx = 0;
	// input data narrow west tc
	for( i = 0; i < NumWestTc; i++ )
	{
		bn[idx]->GetClientRect(rc);
		bn[idx]->MapWindowPoints(this, rc);
		inputPoint[idx].side = NarrowWest;
		inputPoint[idx].gx = rc.left + TcTapSize/2;
		inputPoint[idx].gy = rc.top + TcTapSize/2;
		m_pTcButton[idx] = bn[idx];
		idx++;
	}
	// input data fix tc
	for( i = 0; i < NumFixTc; i++ )
	{
		bn[idx]->GetClientRect(rc);
		bn[idx]->MapWindowPoints(this, rc);
		inputPoint[idx].side = FixSide;
		inputPoint[idx].gx = rc.left + TcTapSize/2;
		inputPoint[idx].gy = rc.top + TcTapSize/2;
		m_pTcButton[idx] = bn[idx];
		idx++;
	}
	// input data narrow east tc
	for( i = 0; i < NumEastTc; i++ )
	{
		bn[idx]->GetClientRect(rc);
		bn[idx]->MapWindowPoints(this, rc);
		inputPoint[idx].side = NarrowEast;
		inputPoint[idx].gx = rc.left + TcTapSize/2;
		inputPoint[idx].gy = rc.top + TcTapSize/2;
		m_pTcButton[idx] = bn[idx];
		idx++;
	}
	// input data loose tc
	for( i = 0; i < NumLooseTc; i++ )
	{
		bn[idx]->GetClientRect(rc);
		bn[idx]->MapWindowPoints(this, rc);
		inputPoint[idx].side = LooseSide;
		inputPoint[idx].gx = rc.left + TcTapSize/2;
		inputPoint[idx].gy = rc.top + TcTapSize/2;
		m_pTcButton[idx] = bn[idx];
		idx++;
	}
	// input data other.
	for( i = 0; i < NumOtherTap; i++ )
	{
		bn[idx]->GetClientRect(rc);
		bn[idx]->MapWindowPoints(this, rc);
		inputPoint[idx].side = NonSide;
		inputPoint[idx].gx = rc.left + TcTapSize/2;
		inputPoint[idx].gy = rc.top + TcTapSize/2;
		m_pTcButton[idx] = bn[idx];
		idx++;
	}
}

//// initialize TC pos for screen.
//void CConfigDlg::SetupTCPos(void)
//{
//	CEdit*	ed[] = {
//					&m_tp01,&m_tp02,&m_tp03,&m_tp04,&m_tp05,&m_tp06,&m_tp07,&m_tp08,
//					&m_tp09,&m_tp10,&m_tp11,&m_tp12,&m_tp13,&m_tp14,&m_tp15,&m_tp16,
//					&m_tp17,&m_tp18,&m_tp19,&m_tp20,&m_tp21,&m_tp22,&m_tp23,&m_tp24,
//					&m_tp25,&m_tp26,&m_tp27,&m_tp28,&m_tp29,&m_tp30,&m_tp31,&m_tp32,
//				NULL};
//	CString	str;
//	int idx;
//
//	// input tc pos.
//	for( idx = 0; ed[idx] != NULL; idx++ )
//	{
//		str.Format("%d", nTpPosData[idx]);
//		ed[idx]->SetWindowText(str);
//	}
//}
//
//// store TC pos for screen.
//BOOL CConfigDlg::StoreTCPos(void)
//{
//	CEdit*	ed[] = {
//					&m_tp01,&m_tp02,&m_tp03,&m_tp04,&m_tp05,&m_tp06,&m_tp07,&m_tp08,
//					&m_tp09,&m_tp10,&m_tp11,&m_tp12,&m_tp13,&m_tp14,&m_tp15,&m_tp16,
//					&m_tp17,&m_tp18,&m_tp19,&m_tp20,&m_tp21,&m_tp22,&m_tp23,&m_tp24,
//					&m_tp25,&m_tp26,&m_tp27,&m_tp28,&m_tp29,&m_tp30,&m_tp31,&m_tp32,
//				NULL};
//	CString	str;
//	int idx, data;
//
//	// output tc pos.
//	for( idx = 0; ed[idx] != NULL; idx++ )
//	{
//		ed[idx]->GetWindowText(str);
//		data = _tstoi(str);
//		nTpPosData[idx] = data;
//		if ( TPPosMin > data || data > TPPosMax || _tcsspn( str, _T("0123456789-") ) != _tcslen( str ) )
//			return FALSE;
//	}
//
//	return TRUE;
//}

// search A/D assign point.
BOOL CConfigDlg::GetADAssignPoint(int x, int y, int *arg, int *xp, int *yp)
{
	register int	i;

	// for each input point, 
	for (i = 0; i < NumInput; i++) {
		// skip if disable. 
		if (inputPoint[i].gx < 0 || inputPoint[i].gy < 0)		continue;

		// if clicked point is in neighbor of A/D AssignPoint, 
		if (x >= inputPoint[i].gx - (TcTapSize/2) && x <= inputPoint[i].gx + (TcTapSize/2) &&
			y >= inputPoint[i].gy - (TcTapSize/2) && y <= inputPoint[i].gy + (TcTapSize/2) ) {
			// set argument. 
			*arg	= i;
			// set center position. 
			*xp		= inputPoint[i].gx,		*yp		= inputPoint[i].gy;
			// return OK. 
			return	TRUE;
		}
	}
	return	FALSE;
}

// search A/D assign point.
BOOL CConfigDlg::GetTCNameAssignPoint(int x, int y, int *arg, int *xp, int *yp)
{
	register int	i;

	// for each input point, 
	for (i = 0; i < NumInput; i++) {
		// skip if disable. 
		if (inputPoint[i].gx < 0 || inputPoint[i].gy < 0)		continue;

		// if clicked point is in neighbor of TCName AssignPoint, 
		if (x >= inputPoint[i].gx + TcTapSize/2 && x <= inputPoint[i].gx + TcTapSize*2 + TcTapSize/2 &&
			y >= inputPoint[i].gy - TcTapSize*2 && y <= inputPoint[i].gy - TcTapSize/2 ) {
			// set argument. 
			*arg	= i;
			// set center position. 
			*xp		= inputPoint[i].gx,		*yp		= inputPoint[i].gy;
			// return OK. 
			return	TRUE;
		}
	}
	return	FALSE;
}

// neural config cnv. send
BOOL CConfigDlg::ConfReset(void)
{
	Invalidate(TRUE);
	return TRUE;	
}

// reset BOP
void CConfigDlg::BopReset(LPCTSTR pBuf)
{
	// restart BOP
	COnlinePCDlg *dlg = (COnlinePCDlg *)AfxGetMainWnd();
	if( dlg->m_pDlgPredic != NULL ){
		dlg->m_pDlgPredic->SendMessage(WM_RESTART_BOP, (WPARAM)0, (LPARAM)pBuf);
	}
}

// send manual cutoff TC message
void CConfigDlg::ManualCutoffTC(int nTcNo, BOOL bCut)
{
	COnlinePCDlg *dlg = (COnlinePCDlg *)AfxGetMainWnd();
	if( dlg->m_pDlgPredic != NULL ){
		dlg->m_pDlgPredic->SendMessage(WM_MANU_TC_CUT, (WPARAM)nTcNo, (LPARAM)bCut);
	}
}

// enable control
void CConfigDlg::EnableControl(BOOL bEnable)
{
	SharedMemory	*pShMem = GetSharedMemory();
//	COLORREF		col;
//	int				nWidthL, nWidthR;

	if( bEnable )
	{
		m_btnPass.SetTextColor(RGB(0,0,0));
		m_btnPass.SetBackColor(RGB(0,255,0));
	}
	else
	{
		m_btnPass.SetTextColor(RGB(255,255,255));
		m_btnPass.SetBackColor(RGB(255,0,0));
	}
	m_tp01.EnableWindow(bEnable);
	m_tp02.EnableWindow(bEnable);
	m_tp03.EnableWindow(bEnable);
	m_tp04.EnableWindow(bEnable);
	m_tp05.EnableWindow(bEnable);
	m_tp06.EnableWindow(bEnable);
	m_tp07.EnableWindow(bEnable);
	m_tp08.EnableWindow(bEnable);
	m_tp09.EnableWindow(bEnable);
	m_tp10.EnableWindow(bEnable);
	m_tp11.EnableWindow(bEnable);
	m_tp12.EnableWindow(bEnable);
	m_tp13.EnableWindow(bEnable);
	m_tp14.EnableWindow(bEnable);
	m_tp15.EnableWindow(bEnable);
	m_tp16.EnableWindow(bEnable);
	m_tp17.EnableWindow(bEnable);
	m_tp18.EnableWindow(bEnable);
	m_tp19.EnableWindow(bEnable);
	m_tp20.EnableWindow(bEnable);
	m_tp21.EnableWindow(bEnable);
	m_tp22.EnableWindow(bEnable);
	m_tp23.EnableWindow(bEnable);
	m_tp24.EnableWindow(bEnable);
	m_tp25.EnableWindow(bEnable);
	m_tp26.EnableWindow(bEnable);
	m_tp27.EnableWindow(bEnable);
	m_tp28.EnableWindow(bEnable);
	m_tp29.EnableWindow(bEnable);
	m_tp30.EnableWindow(bEnable);
	m_tp31.EnableWindow(bEnable);
	m_tp32.EnableWindow(bEnable);

	m_tp01.SetBackColor(RGB(255,255,255));
	m_tp02.SetBackColor(RGB(255,255,255));
	m_tp03.SetBackColor(RGB(255,255,255));
	m_tp04.SetBackColor(RGB(255,255,255));
	m_tp05.SetBackColor(RGB(255,255,255));
	m_tp06.SetBackColor(RGB(255,255,255));
	m_tp07.SetBackColor(RGB(255,255,255));
	m_tp08.SetBackColor(RGB(255,255,255));
	m_tp09.SetBackColor(RGB(255,255,255));
	m_tp10.SetBackColor(RGB(255,255,255));
	m_tp11.SetBackColor(RGB(255,255,255));
	m_tp12.SetBackColor(RGB(255,255,255));
	m_tp13.SetBackColor(RGB(255,255,255));
	m_tp14.SetBackColor(RGB(255,255,255));
	m_tp15.SetBackColor(RGB(255,255,255));
	m_tp16.SetBackColor(RGB(255,255,255));
	m_tp17.SetBackColor(RGB(255,255,255));
	m_tp18.SetBackColor(RGB(255,255,255));
	m_tp19.SetBackColor(RGB(255,255,255));
	m_tp20.SetBackColor(RGB(255,255,255));
	m_tp21.SetBackColor(RGB(255,255,255));
	m_tp22.SetBackColor(RGB(255,255,255));
	m_tp23.SetBackColor(RGB(255,255,255));
	m_tp24.SetBackColor(RGB(255,255,255));
	m_tp25.SetBackColor(RGB(255,255,255));
	m_tp26.SetBackColor(RGB(255,255,255));
	m_tp27.SetBackColor(RGB(255,255,255));
	m_tp28.SetBackColor(RGB(255,255,255));
	m_tp29.SetBackColor(RGB(255,255,255));
	m_tp30.SetBackColor(RGB(255,255,255));
	m_tp31.SetBackColor(RGB(255,255,255));
	m_tp32.SetBackColor(RGB(255,255,255));

	//if( pShMem != NULL )
	//{
	//	nWidthR = pShMem->moldSizeR;
	//	nWidthL = pShMem->moldSizeL;
	//	col = RGB(80,80,80);
	//	if( cnf.nTpPosData[0] > nWidthR ) m_tp01.SetBackColor(col);
	//	if( cnf.nTpPosData[1] > nWidthR ) m_tp02.SetBackColor(col);
	//	if( cnf.nTpPosData[2] > nWidthR ) m_tp03.SetBackColor(col);
	//	if( cnf.nTpPosData[3] > nWidthR ) m_tp04.SetBackColor(col);
	//	if( cnf.nTpPosData[4] > nWidthR ) m_tp05.SetBackColor(col);
	//	if( cnf.nTpPosData[5] > nWidthR ) m_tp06.SetBackColor(col);
	//	if( cnf.nTpPosData[6] > nWidthR ) m_tp07.SetBackColor(col);
	//	if( cnf.nTpPosData[15] > nWidthR ) m_tp16.SetBackColor(col);
	//	if( cnf.nTpPosData[16] > nWidthR ) m_tp17.SetBackColor(col);
	//	if( cnf.nTpPosData[17] > nWidthR ) m_tp18.SetBackColor(col);
	//	if( cnf.nTpPosData[18] > nWidthR ) m_tp19.SetBackColor(col);
	//	if( cnf.nTpPosData[19] > nWidthR ) m_tp20.SetBackColor(col);
	//	if( cnf.nTpPosData[20] > nWidthR ) m_tp21.SetBackColor(col);
	//	if( cnf.nTpPosData[21] > nWidthR ) m_tp22.SetBackColor(col);
	//	if( cnf.nTpPosData[8] > nWidthL ) m_tp09.SetBackColor(col);
	//	if( cnf.nTpPosData[9] > nWidthL ) m_tp10.SetBackColor(col);
	//	if( cnf.nTpPosData[10] > nWidthL ) m_tp11.SetBackColor(col);
	//	if( cnf.nTpPosData[11] > nWidthL ) m_tp12.SetBackColor(col);
	//	if( cnf.nTpPosData[12] > nWidthL ) m_tp13.SetBackColor(col);
	//	if( cnf.nTpPosData[13] > nWidthL ) m_tp14.SetBackColor(col);
	//	if( cnf.nTpPosData[14] > nWidthL ) m_tp15.SetBackColor(col);
	//	if( cnf.nTpPosData[23] > nWidthL ) m_tp24.SetBackColor(col);
	//	if( cnf.nTpPosData[24] > nWidthL ) m_tp25.SetBackColor(col);
	//	if( cnf.nTpPosData[25] > nWidthL ) m_tp26.SetBackColor(col);
	//	if( cnf.nTpPosData[26] > nWidthL ) m_tp27.SetBackColor(col);
	//	if( cnf.nTpPosData[27] > nWidthL ) m_tp28.SetBackColor(col);
	//	if( cnf.nTpPosData[28] > nWidthL ) m_tp29.SetBackColor(col);
	//	if( cnf.nTpPosData[29] > nWidthL ) m_tp30.SetBackColor(col);
	//}
}

void CConfigDlg::SetAutoLoad(int nPtnNo, LPCTSTR name)
{
	if( nPtnNo < NumSteelKind )
	{
		m_strAutoLoadName[nPtnNo] = name;
		m_bAutoLoad = TRUE;
	}
}

void CConfigDlg::SetAutoLoadD(int nPtnNo, LPCTSTR name)
{
	if( nPtnNo < NumSteelKind )
	{
		m_strAutoLoadNameD[nPtnNo] = name;
		m_bAutoLoad = TRUE;
	}
}

void CConfigDlg::OnBnClickedRadio4()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
}

// バックアップボタン
void CConfigDlg::OnBnClickedButton1()
{
	if( IDNO == AfxMessageBox(IDS_CNF_DLG05, MB_YESNO) )
		return;

	// データ割り付けとコンビネーションのバックアップ
	if( !cnf.BackupADAndCombi() )
		AfxMessageBox(IDS_CNF_DLG07, MB_OK);
}

// 復旧ボタン
void CConfigDlg::OnBnClickedButton2()
{
	_TCHAR buf[256];
	// The TC Combination definition backup temp.
	TCCombination	tcCombinationTmp[NumCombination];
	int				numCombinationTmp;

	m_textSelConfName.GetWindowText(buf, 256);

	// 現在の設定を一時保持
	memcpy(tcCombinationTmp, cnf.tcCombination, sizeof(tcCombinationTmp));
	numCombinationTmp = cnf.numCombination;

	// Backupデータを読み込み
	cnf.SetupADChannels();
	cnf.SetupCombination();
	if( cnf.ReadADConfigFileBk(buf) == FALSE || 
		cnf.ReadCombinationBk(buf) == FALSE )
	{
		AfxMessageBox(IDS_CNF_DLG08, MB_OK);

		// 保持データに戻す
		memcpy(cnf.tcCombination, tcCombinationTmp, sizeof(cnf.tcCombination));
		cnf.numCombination = numCombinationTmp;

		// 画面を再描画
		Invalidate(FALSE);
		
		return;
	}

	// 画面を再描画
	Invalidate(FALSE);
		
	// 復旧確認
	if( IDYES == AfxMessageBox(IDS_CNF_DLG06, MB_YESNO) )
	{

		// store word directory name.
		cnf.SetWorkDirName(buf);

		// save configuration
		if( cnf.SaveConfiguration() )
		{
			// get work dir name
			m_textConfName.SetWindowText(cnf.GetWorkDirName());

			// reset BOP
			BopReset(buf);

			// work directory name copy.
			cnf.CopyWorkDirName();
			
			// neural config cnv. send
			ConfReset();
		}
		else
		{
			AfxMessageBox(IDS_CNF_DLG00, MB_ICONEXCLAMATION | MB_OK);
		}

		return;
	}

	// 保持データに戻す
	memcpy(cnf.tcCombination, tcCombinationTmp, sizeof(cnf.tcCombination));
	cnf.numCombination = numCombinationTmp;

	// 画面を再描画
	Invalidate(FALSE);
}

// Tc Name 読み込み
void CConfigDlg::GetTcName(void)
{
	CTcName	cls(-1);
	cls.ReadTcNameFile(tcnm);
}

