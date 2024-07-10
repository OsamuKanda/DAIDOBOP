// TrendPen.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "TrendPen.h"
#include ".\trendpen.h"

///////////////////////////////////////////////////////////////////////////////
// definistion

static CConfig	&cnf = theApp.m_cnf;
static TcName	*tcnm	= &theApp.m_stTcNm;

///////////////////////////////////////////////////////////////////////////////
// CTrendPen dialog

IMPLEMENT_DYNAMIC(CTrendPen, CDialog)
CTrendPen::CTrendPen(CWnd* pParent /*=NULL*/)
	: CDialog(CTrendPen::IDD, pParent)
{
	m_nPen = -1;
}

CTrendPen::~CTrendPen()
{
}

void CTrendPen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_chkPen01);
	DDX_Control(pDX, IDC_CHECK2, m_chkPen02);
	DDX_Control(pDX, IDC_CHECK3, m_chkPen03);
	DDX_Control(pDX, IDC_CHECK4, m_chkPen04);
	DDX_Control(pDX, IDC_CHECK5, m_chkPen05);
	DDX_Control(pDX, IDC_CHECK6, m_chkPen06);
	DDX_Control(pDX, IDC_CHECK7, m_chkPen07);
	DDX_Control(pDX, IDC_CHECK8, m_chkPen08);
	DDX_Control(pDX, IDC_CHECK9, m_chkPen09);
	DDX_Control(pDX, IDC_CHECK10, m_chkPen10);
	DDX_Control(pDX, IDC_CHECK11, m_chkPen11);
	DDX_Control(pDX, IDC_CHECK12, m_chkPen12);
	DDX_Control(pDX, IDC_CHECK13, m_chkPen13);
	DDX_Control(pDX, IDC_CHECK14, m_chkPen14);
	DDX_Control(pDX, IDC_CHECK15, m_chkPen15);
	DDX_Control(pDX, IDC_CHECK16, m_chkPen16);
	DDX_Control(pDX, IDC_EDIT1, m_editPenName01);
	DDX_Control(pDX, IDC_EDIT2, m_editPenName02);
	DDX_Control(pDX, IDC_EDIT3, m_editPenName03);
	DDX_Control(pDX, IDC_EDIT4, m_editPenName04);
	DDX_Control(pDX, IDC_EDIT5, m_editPenName05);
	DDX_Control(pDX, IDC_EDIT6, m_editPenName06);
	DDX_Control(pDX, IDC_EDIT7, m_editPenName07);
	DDX_Control(pDX, IDC_EDIT8, m_editPenName08);
	DDX_Control(pDX, IDC_EDIT9, m_editPenName09);
	DDX_Control(pDX, IDC_EDIT10, m_editPenName10);
	DDX_Control(pDX, IDC_EDIT11, m_editPenName11);
	DDX_Control(pDX, IDC_EDIT12, m_editPenName12);
	DDX_Control(pDX, IDC_EDIT13, m_editPenName13);
	DDX_Control(pDX, IDC_EDIT14, m_editPenName14);
	DDX_Control(pDX, IDC_EDIT15, m_editPenName15);
	DDX_Control(pDX, IDC_EDIT16, m_editPenName16);
	DDX_Control(pDX, IDC_LIST1, m_listDispArg);
}

BEGIN_MESSAGE_MAP(CTrendPen, CDialog)
	ON_BN_CLICKED(IDOK, OnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnClickedCancel)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CTrendPen message handler

BOOL CTrendPen::OnInitDialog()
{
	CDialog::OnInitDialog();

	// set up trend chart pen column.
	SetupTrendPenDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
}

// clicked OK button.
void CTrendPen::OnClickedOk()
{
	UpdatePenArgSetting();
	OnOK();
}

// clicked Cancel button.
void CTrendPen::OnClickedCancel()
{
	OnCancel();
}

// changed pen selection list
void CTrendPen::OnSelchangeList1()
{
	int		sel;

	sel = m_listDispArg.GetCurSel();

	SetPenArgument(m_nPen, sel);
}

BOOL CTrendPen::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	CButton	*bn[] = {
		&m_chkPen01,&m_chkPen02,&m_chkPen03,&m_chkPen04,
		&m_chkPen05,&m_chkPen06,&m_chkPen07,&m_chkPen08,
		&m_chkPen09,&m_chkPen10,&m_chkPen11,&m_chkPen12,
		&m_chkPen13,&m_chkPen14,&m_chkPen15,&m_chkPen16,
		NULL
	};
	CEdit	*ed[] = {
		&m_editPenName01,&m_editPenName02,&m_editPenName03,&m_editPenName04,
		&m_editPenName05,&m_editPenName06,&m_editPenName07,&m_editPenName08,
		&m_editPenName09,&m_editPenName10,&m_editPenName11,&m_editPenName12,
		&m_editPenName13,&m_editPenName14,&m_editPenName15,&m_editPenName16,
		NULL
	};
	CButton	*chkBn;
	CEdit	*chkEd;
	int		i, pen;

	switch(nID)
	{
	case IDC_CHECK1:  case IDC_CHECK2:  case IDC_CHECK3:  case IDC_CHECK4:
	case IDC_CHECK5:  case IDC_CHECK6:  case IDC_CHECK7:  case IDC_CHECK8:
	case IDC_CHECK9:  case IDC_CHECK10: case IDC_CHECK11: case IDC_CHECK12:
	case IDC_CHECK13: case IDC_CHECK14: case IDC_CHECK15: case IDC_CHECK16:
		if (nCode == BN_CLICKED) {
			pen = -1;
			chkBn = (CButton *)GetDlgItem(nID);
			for( i = 0; i < NumTrendChannel; i++ )
				if( chkBn == bn[i] )
					if( chkBn->GetCheck() )
						pen = i;
			m_nPen = pen;
		}
		break;

	case IDC_EDIT1:	case IDC_EDIT2:	case IDC_EDIT3:	case IDC_EDIT4:
	case IDC_EDIT5:	case IDC_EDIT6:	case IDC_EDIT7:	case IDC_EDIT8:
	case IDC_EDIT9:	case IDC_EDIT10:	case IDC_EDIT11:	case IDC_EDIT12:
	case IDC_EDIT13:case IDC_EDIT14:	case IDC_EDIT15:	case IDC_EDIT16:
		if (nCode == EN_SETFOCUS) {
			pen = -1;
			chkEd = (CEdit *)GetDlgItem(nID);
			for( i = 0; i < NumTrendChannel; i++ )
				if( chkEd == ed[i] )
					pen = i;
			m_nPen = pen;
		}
		break;
	}

	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

///////////////////////////////////////////////////////////////////////////////
// CTrendPen member function

// set up trend chart pen column.
void CTrendPen::SetupTrendPenDlg(void)
{
	CButton	*bn[] = {
		&m_chkPen01,&m_chkPen02,&m_chkPen03,&m_chkPen04,
		&m_chkPen05,&m_chkPen06,&m_chkPen07,&m_chkPen08,
		&m_chkPen09,&m_chkPen10,&m_chkPen11,&m_chkPen12,
		&m_chkPen13,&m_chkPen14,&m_chkPen15,&m_chkPen16,
		NULL
	};
	CEdit	*ed[] = {
		&m_editPenName01,&m_editPenName02,&m_editPenName03,&m_editPenName04,
		&m_editPenName05,&m_editPenName06,&m_editPenName07,&m_editPenName08,
		&m_editPenName09,&m_editPenName10,&m_editPenName11,&m_editPenName12,
		&m_editPenName13,&m_editPenName14,&m_editPenName15,&m_editPenName16,
		NULL
	};
	CString	str;
	int		arg, i;

	// reset list
	m_listDispArg.SetRedraw(FALSE);
	m_listDispArg.ResetContent();

	// for each entry,
	for (i = 0; i < LevelID; i++) {
		// append trend TCTap entry.
		str.Format(_T("%s"), tcnm->name[i]);
		m_listDispArg.AddString(str);
	}
	// append casting 
	// append meniscas level.
	str.LoadString(IDS_DEF_LEVEL);
	m_listDispArg.AddString(str);
	// append casting 
	str.LoadString(IDS_DEF_VC);
	m_listDispArg.AddString(str);
	str.LoadString(IDS_DEF_SN);
	m_listDispArg.AddString(str);
	// append casting 
//	str.LoadString(IDS_DEF_WIDTH);
//	m_listDispArg.AddString(str);

	// terminate trend file entry. 
	m_listDispArg.SetRedraw(TRUE);

	// for each pen, 
	for (i = 0; i < NumTrendChannel; i++) 
	{
		arg = cnf.m_nTrendArg[i];
		if (arg >= TCTapID && arg < LevelID)	str.Format(_T("%s"), tcnm->name[arg]);
		else if (arg == LevelID)				str.LoadString(IDS_DEF_LEVEL); 
		else if (arg == VcID)					str.LoadString(IDS_DEF_VC);	
		else if (arg == SNID)					str.LoadString(IDS_DEF_SN);	
		//else if (arg == WidthID){				str.LoadString(IDS_DEF_WIDTH);}
		//else if (arg == WidthLID){				str.LoadString(IDS_DEF_WIDTH); str+=_T("L");}
		//else if (arg == WidthRID){				str.LoadString(IDS_DEF_WIDTH); str+=_T("R"); }
		else if (arg == LengthID)				str.LoadString(IDS_DEF_LENGTH); 
		else									str = "";
		// set editting argument.
		ed[i]->SetWindowText(str);
		// set the check box.
		bn[i]->SetCheck(cnf.m_isEnable[i]);
	}
}

// set pen argument selection.
void CTrendPen::SetPenArgument(int pen, int sel)
{
	CEdit	*ed[] = {
		&m_editPenName01,&m_editPenName02,&m_editPenName03,&m_editPenName04,
		&m_editPenName05,&m_editPenName06,&m_editPenName07,&m_editPenName08,
		&m_editPenName09,&m_editPenName10,&m_editPenName11,&m_editPenName12,
		&m_editPenName13,&m_editPenName14,&m_editPenName15,&m_editPenName16,
		NULL
	};
	CString	str;

	// make up display string.
	if (sel >= TCTapID && sel < LevelID)	str.Format(_T("%s"), tcnm->name[sel]);
	else if (sel == LevelID)			str.LoadString(IDS_DEF_LEVEL); 
	else if (sel == VcID)				str.LoadString(IDS_DEF_VC);	
	else if (sel == SNID)				str.LoadString(IDS_DEF_SN);	
	//else if (sel == WidthID){			str.LoadString(IDS_DEF_WIDTH);}
//	else if (sel == WidthLID){			str.LoadString(IDS_DEF_WIDTH);  str+=_T("L");}
//	else if (sel == WidthRID){			str.LoadString(IDS_DEF_WIDTH);  str+=_T("R"); }
	else if (sel == LengthID)			str.LoadString(IDS_DEF_LENGTH); 
	else								str = "";

	// set editting argument.
	if( pen != -1 )
		ed[pen]->SetWindowText(str);
}

// update chart pen argument setting.
void CTrendPen::UpdatePenArgSetting(void)
{
	CButton	*bn[] = {
		&m_chkPen01,&m_chkPen02,&m_chkPen03,&m_chkPen04,
		&m_chkPen05,&m_chkPen06,&m_chkPen07,&m_chkPen08,
		&m_chkPen09,&m_chkPen10,&m_chkPen11,&m_chkPen12,
		&m_chkPen13,&m_chkPen14,&m_chkPen15,&m_chkPen16,
		NULL
	};
	CEdit	*ed[] = {
		&m_editPenName01,&m_editPenName02,&m_editPenName03,&m_editPenName04,
		&m_editPenName05,&m_editPenName06,&m_editPenName07,&m_editPenName08,
		&m_editPenName09,&m_editPenName10,&m_editPenName11,&m_editPenName12,
		&m_editPenName13,&m_editPenName14,&m_editPenName15,&m_editPenName16,
		NULL
	};
	CString	str, strSN, strLevel, strVc, strWidth, strLength;
	_TCHAR	*p;
	int		i, arg;

	// for each pen,
	for (i = 0; i < NumTrendChannel; i++) 
	{
		// get editted string.
		ed[i]->GetWindowText(str);

		// parse the string.
		arg	= -1;

		strSN.LoadString(IDS_DEF_SN);
		strLevel.LoadString(IDS_DEF_LEVEL);
		strVc.LoadString(IDS_DEF_VC);
		strWidth.LoadString(IDS_DEF_WIDTH);
//		strWidthL.LoadString(IDS_DEF_WIDTH);
//		strWidthL += _T("L");
//		strWidthR.LoadString(IDS_DEF_WIDTH);
//		strWidthR += _T("R");
		strLength.LoadString(IDS_DEF_LENGTH);
		p = str.GetBuffer();
		if (!str.Compare( strLevel ))		arg	= LevelID;
		else if (!str.Compare( strVc ))		arg	= VcID;
		else if (!str.Compare( strSN ))		arg	= SNID;
		//else if (!str.Compare( strWidth ))	arg	= WidthID;
//		else if (!str.Compare( strWidthL ))	arg	= WidthLID;
//		else if (!str.Compare( strWidthR ))	arg	= WidthRID;
		else if (!str.Compare( strLength ))	arg	= LengthID;
		else arg = GetTcNo(str);
		str.ReleaseBuffer();

		// get Pen CheckBox
		cnf.m_isEnable[i] = bn[i]->GetCheck();
		if (!cnf.m_isEnable[i])
			arg	= -1;

		// store the argument.
		cnf.m_nTrendArg[i]	= arg;
	}

	// save trend graph pen.
	cnf.WriteTrendGraphPen();
}

// get tc no
int CTrendPen::GetTcNo(LPCTSTR pc)
{
	CString	strIn, strChk;
	int		i, no = -1;

	strIn = pc;
	for( i = 0; i < NumTCTap; i++ )
	{
		strChk = tcnm->name[i];
		if( strIn.Compare(strChk) == 0 )
		{
			no = i;
			break;
		}
	}

	return no;
}
