// SkAssign.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "SkAssign.h"
#include "Config.h"

// reference data
static CConfig &cnf = theApp.m_cnf;

// CSkAssign ダイアログ
IMPLEMENT_DYNAMIC(CSkAssign, CDialog)
CSkAssign::CSkAssign(CWnd* pParent /*=NULL*/)
	: CDialog(CSkAssign::IDD, pParent)
{
}

CSkAssign::~CSkAssign()
{
}

void CSkAssign::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cmbSk01);
	DDX_Control(pDX, IDC_COMBO2, m_cmbSk02);
	DDX_Control(pDX, IDC_COMBO3, m_cmbSk03);
	DDX_Control(pDX, IDC_COMBO4, m_cmbSk04);
	DDX_Control(pDX, IDC_COMBO5, m_cmbSk05);
	DDX_Control(pDX, IDC_COMBO6, m_cmbSk06);
	DDX_Control(pDX, IDC_COMBO7, m_cmbSk07);
	DDX_Control(pDX, IDC_COMBO8, m_cmbSk08);
	DDX_Control(pDX, IDC_COMBO9, m_cmbSk09);
	DDX_Control(pDX, IDC_STATIC1, m_textSG1);
	DDX_Control(pDX, IDC_STATIC2, m_textSG2);
	DDX_Control(pDX, IDC_STATIC3, m_textSG3);
	DDX_Control(pDX, IDC_STATIC4, m_textSG4);
	DDX_Control(pDX, IDC_STATIC5, m_textSG5);
	DDX_Control(pDX, IDC_STATIC6, m_textSG6);
	DDX_Control(pDX, IDC_STATIC7, m_textSG7);
	DDX_Control(pDX, IDC_STATIC8, m_textSG8);
	DDX_Control(pDX, IDC_STATIC9, m_textSG9);
}

BEGIN_MESSAGE_MAP(CSkAssign, CDialog)
	ON_BN_CLICKED(IDOK, OnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnClickedCancel)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CSkAssign message handler

BOOL CSkAssign::OnInitDialog()
{
	CDialog::OnInitDialog();

	// set up steel kind column.
	SetupSteelKindDlg();

	return TRUE;
}

void CSkAssign::OnClickedOk()
{
	// store steel kind column.
	if( StoreSteelKindDlg() )
		OnOK();
}

void CSkAssign::OnClickedCancel()
{
	OnCancel();
}

///////////////////////////////////////////////////////////////////////////////
// CSkAssign membaer function

// set up steel kind column.
void CSkAssign::SetupSteelKindDlg(void)
{
	CComboBox	*cb[] = {
					&m_cmbSk01, &m_cmbSk02, &m_cmbSk03, &m_cmbSk04, &m_cmbSk05,
					&m_cmbSk06, &m_cmbSk07, &m_cmbSk08, &m_cmbSk09, NULL
				};
	CStatic		*st[] = {
					&m_textSG1, &m_textSG2, &m_textSG3, &m_textSG4, &m_textSG5,
					&m_textSG6, &m_textSG7, &m_textSG8, &m_textSG9, NULL
				};
	_TCHAR	confTable[NumConfig][SizeConfig];
	BoSysInfo	bs;
	int			i, n, numConfig;

	// get configration name list.
	numConfig	= cnf.GetSubDirList(cnf.WorkRootDir, confTable, NumConfig);

	for(i = 0;i < NumSteelKind;i++)
	{
		// read sg name
		if( cnf.ReadBosysInfo(i, &bs) )
			st[i]->SetWindowText(bs.szComment);
		else
			st[i]->SetWindowText(_T(""));

		// reset list
		cb[i]->ResetContent();

		// for each entry,
		for (n = 0; n < numConfig; n++) 
		{
			// append configration entry.
			cb[i]->AddString(confTable[n]);
		}

		// get now configration entry number.
		for (n = 0; n < numConfig; n++)
			if (!_tcscmp(cnf.steel[i].confName, confTable[n]))
				break;

		cb[i]->SetCurSel(n);
		cb[i]->SetWindowText(cnf.steel[i].confName);
	}
}

// store steel kind data.
BOOL CSkAssign::StoreSteelKindDlg(void)
{
	CComboBox	*cb[] = {
					&m_cmbSk01, &m_cmbSk02, &m_cmbSk03, &m_cmbSk04, &m_cmbSk05,
					&m_cmbSk06, &m_cmbSk07, &m_cmbSk08, &m_cmbSk09, NULL
				};
	CStatic		*st[] = {
					&m_textSG1, &m_textSG2, &m_textSG3, &m_textSG4, &m_textSG5,
					&m_textSG6, &m_textSG7, &m_textSG8, &m_textSG9, NULL
				};
	CString		str, str2;
	SteelKind	dummy[NumSteelKind];
	int			i;

	// for each Steel Kind,
	for( i = 0;i < NumSteelKind; i++)
	{
		// store configuration name into combo box.
		st[i]->GetWindowText(str);

		// store configuration name into combo box.
		cb[i]->GetWindowText(str2);

		if(str.GetLength() != 0 && cb[i]->FindString(-1, str2) == CB_ERR)
		{
			AfxMessageBox(IDS_SK_ASSIGN00, MB_ICONEXCLAMATION | MB_OK);
			return FALSE;
		}
		_tcscpy(dummy[i].confName, str2);
	}

	// Steel Kind Establishment Data is adopted.
	memcpy(cnf.steel, &dummy, sizeof(cnf.steel));

	return TRUE;
}

