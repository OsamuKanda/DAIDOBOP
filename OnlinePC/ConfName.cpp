// ConfName.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "ConfName.h"

///////////////////////////////////////////////////////////////////////////////
// definition

// reference data
static CConfig	&cnf = theApp.m_cnf;


///////////////////////////////////////////////////////////////////////////////
// CConfName dialog
IMPLEMENT_DYNAMIC(CConfName, CDialog)
CConfName::CConfName(CWnd* pParent /*=NULL*/)
	: CDialog(CConfName::IDD, pParent)
{
	// initialize member data
	m_szSelectName  = _T("");
}

CConfName::~CConfName()
{
}

void CConfName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONFIG_NAME, m_editName);
	DDX_Control(pDX, IDC_CONFIG_LIST, m_listConf);
}

BEGIN_MESSAGE_MAP(CConfName, CDialog)
	ON_BN_CLICKED(IDOK, OnClickedOk)
	ON_BN_CLICKED(IDC_DELETE, OnClickedDelete)
	ON_BN_CLICKED(IDCANCEL, OnClickedCancel)
	ON_LBN_SELCHANGE(IDC_CONFIG_LIST, OnSelchangeConfigList)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CConfName message handler

// It is only 1 execute when the dialog opens.
BOOL CConfName::OnInitDialog()
{
	CDialog::OnInitDialog();

	// setup config name dialog.
	SetupConfigNameDlg();

	return TRUE;
}

// clicked ok button.
void CConfName::OnClickedOk()
{
	// store configration name.
	StoreConfigName();

	OnOK();
}

// clicked delete button.
void CConfName::OnClickedDelete()
{
	// delete configration name.
	DeleteConfigName();
}

// clicked cancel button.
void CConfName::OnClickedCancel()
{
	OnCancel();
}

// changed config list selection.
void CConfName::OnSelchangeConfigList()
{
	CString	str;
	int		nSel;

	// It displays a chosen text in the edit box.
	nSel = m_listConf.GetCurSel();
	m_listConf.GetText(nSel, str);
	m_editName.SetWindowText(str);
}

///////////////////////////////////////////////////////////////////////////////
// CConfName member function

// set up configration name column.
void CConfName::SetupConfigNameDlg(void)
{
	_TCHAR	confTable[NumConfig][SizeConfig];
	int		numConfig;
	int		i, n;

	// store now configration name into editting column.
	m_szSelectName = cnf.GetWorkDirName();
	m_editName.SetWindowText(m_szSelectName);

	// get configration name list.
	numConfig = cnf.GetSubDirList(cnf.WorkRootDir, confTable, NumConfig);

	// get now configration entry number. 
	for (n = 0; n < numConfig; n++)
		if (!_tcscmp(cnf.GetWorkDirName(), confTable[n]))
			break;

	// reset configration list. 
	m_listConf.SetRedraw(FALSE);
	m_listConf.ResetContent();

	// for each entry, 
	for (i = 0; i < numConfig; i++) {
		// append configration entry. 
		m_listConf.AddString(confTable[i]);
	}

	// terminate configration entry. 
	m_listConf.SetRedraw(TRUE);

	// set high light now configration. 
	m_listConf.SetCurSel(n);
}

// store configration name.
void CConfName::StoreConfigName(void)
{
	// get editted string.
	m_editName.GetWindowText(m_szSelectName);
}

// delete configration name.
void CConfName::DeleteConfigName(void)
{
	LPTSTR	p, q;
	_TCHAR	buf[SizeConfig];
	int		ret;

	// get editted string. 
	m_editName.GetWindowText(buf, SizeConfig);

	// change configration name.
	for (p = buf, q = buf; *p; p++) 
		if (IsCharAlphaNumeric(*p))
			*q++ = *p;
	*q = '\0';
	CharLowerBuff(buf, static_cast<DWORD>(_tcslen(buf)));

	// It checks whether or not the configuration name to delete exists at the list box.
	ret = m_listConf.FindString(0, buf);

	// The checking which has used configuration name.
	if(ret != -1 && cnf.DeleteCheck(buf))
	{
		if(IDYES == AfxMessageBox(IDS_NM_CNF00,	MB_ICONEXCLAMATION | MB_YESNO))
		{
			// delete configuration files
			cnf.DeleteConfigFile(buf);

			// setup config name dialog
			SetupConfigNameDlg();
		}
	}
	else
		AfxMessageBox(IDS_NM_CNF01, MB_ICONEXCLAMATION | MB_OK);
}
