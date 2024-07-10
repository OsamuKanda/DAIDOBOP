// BoRec.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "BoRec.h"

///////////////////////////////////////////////////////////////////////////////
// definition

// reference data
static CConfig	&cnf = theApp.m_cnf;

// CBoRec dialog
IMPLEMENT_DYNAMIC(CBoRec, CDialog)
CBoRec::CBoRec(CWnd* pParent /*=NULL*/)
	: CDialog(CBoRec::IDD, pParent)
{
	// initialize member data.
	m_nSelectInfo = -1;
}

CBoRec::~CBoRec()
{
}

void CBoRec::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PTN_NO, m_textPtnNo);
	DDX_Control(pDX, IDC_EDIT1, m_editComment);
	DDX_Control(pDX, IDC_LIST1, m_listBoRec);
	DDX_Control(pDX, IDC_LIST2, m_listPowder);
	DDX_Control(pDX, IDC_EDIT5, m_editThreshold);
	DDX_Control(pDX, IDC_EDIT7, m_editNumTimes);
	DDX_Control(pDX, IDC_EDIT11, m_editWatchTime);
	DDX_Control(pDX, IDC_EDIT12, m_editAlmCombi);
	DDX_Control(pDX, IDC_EDIT13, m_editAlmCtlCombi);
	DDX_Control(pDX, IDC_EDIT6, m_editPowder);
	DDX_Control(pDX, IDC_COMBO1, m_cmbConfName);
	DDX_Control(pDX, IDC_COMBO2, m_cmbConfName2);
}

BEGIN_MESSAGE_MAP(CBoRec, CDialog)
	ON_BN_CLICKED(IDC_BUTTON3, OnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnClickedButton2)
	//ON_BN_CLICKED(IDC_BUTTON5, &CBoRec::OnBnClickedButton5)
	//ON_BN_CLICKED(IDC_BUTTON6, &CBoRec::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON11, &CBoRec::OnBnClickedButton11)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CBoRec message handler
BOOL CBoRec::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString	str;
	
	// setup steel group screen
	DispSteelGroupInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
}

// B.O.認識登録
void CBoRec::OnClickedButton1()
{
//	CheckRegistration();
}

// B.O.認識削除
void CBoRec::OnClickedButton2()
{
//	DeleteBoRecognition();
}

//// パウダー名称登録
//void CBoRec::OnBnClickedButton5()
//{
//	CheckPowder();
//}
//
//// パウダー名称削除
//void CBoRec::OnBnClickedButton6()
//{
//	DeletePowder();
//}
//
// ＯＫボタン
void CBoRec::OnClickedButton3()
{
	if( CheckBosysInfo() )
		EndDialog(IDOK);
}

// 削除ボタン
void CBoRec::OnBnClickedButton11()
{
	if( IDYES == AfxMessageBox(_T("削除してもよろしいですか？"), MB_YESNO) ) 
	{
		memset(&m_stBsInfo, 0, sizeof(m_stBsInfo));
		EndDialog(IDOK);
	}
}

// キャンセルボタン
void CBoRec::OnClickedButton4()
{
	EndDialog(IDCANCEL);
}


///////////////////////////////////////////////////////////////////////////////
// CBoRec member function

// set bosys info
void CBoRec::SetInfo(int no, BoSysInfo *bs)
{
	m_nSelectInfo = no;
	memcpy(&m_stBsInfo, bs, sizeof(m_stBsInfo));
}

// get bosys info
void CBoRec::GetInfo(BoSysInfo *bs)
{
	memcpy(bs, &m_stBsInfo, sizeof(m_stBsInfo));
}

// display steel group Information.
void CBoRec::DispSteelGroupInfo(void)
{
	CString	str;
	int		i;

	// set pattern no.
	str.Format(_T("%d"), m_nSelectInfo+1);
	m_textPtnNo.SetWindowText(str);

	// display comment.
	str = m_stBsInfo.szComment;
	m_editComment.SetWindowText(str);

	// set up configration name box.
	SetupConfigNameBox();

	//// display the powder name.
	//m_listPowder.ResetContent();
	//for(i = 0;i < m_stBsInfo.numRegPow;i++)
	//{
	//	m_listPowder.InsertString(-1, m_stBsInfo.szPowder[i]);
	//}

	// display Number of combinations.
	str.Format(_T("%d"), m_stBsInfo.numCombiBA);
	m_editAlmCombi.SetWindowText(str);
	str.Format(_T("%d"), m_stBsInfo.numCombiBAC);
	m_editAlmCtlCombi.SetWindowText(str);
}

// set up configration name box.
void CBoRec::SetupConfigNameBox(void)
{
	_TCHAR	confTable[NumConfig][SizeConfig];
	int		numConfig;
	int		i, n1, n2;

	// get configration name list.
	numConfig = cnf.GetSubDirList(cnf.WorkRootDir, confTable, NumConfig);

	// get now configration entry number. 
	for (n1 = 0; n1 < numConfig; n1++)
		if (!_tcscmp(m_stBsInfo.szConfName, confTable[n1]))
			break;

	// get now configration entry number. 
	for (n2 = 0; n2 < numConfig; n2++)
		if (!_tcscmp(m_stBsInfo.szConfNameD, confTable[n2]))
			break;

	// reset configration list. 
	m_cmbConfName.SetRedraw(FALSE);
	m_cmbConfName.ResetContent();
	m_cmbConfName2.SetRedraw(FALSE);
	m_cmbConfName2.ResetContent();

	// for each entry, 
	for (i = 0; i < numConfig; i++) {
		// append configration entry. 
		m_cmbConfName.AddString(confTable[i]);
		m_cmbConfName2.AddString(confTable[i]);
	}

	// terminate configration entry. 
	m_cmbConfName.SetRedraw(TRUE);
	m_cmbConfName2.SetRedraw(TRUE);

	// set high light now configration. 
	m_cmbConfName.SetCurSel(n1);
	m_cmbConfName2.SetCurSel(n2);
}

//// check registration.
//void CBoRec::CheckRegistration(void)
//{
//	CString	str;
//	_TCHAR	buf[64];
//	int		i;
//	float	numBoAlarm;
//	int		numBoWatch, numTimes;

	//// check B.O watch time
	//m_editWatchTime.GetWindowText(buf, 32);
	//if(_stscanf(buf, _T("%d"), &numBoWatch) != 1 ||	numBoWatch < 500)
	//{
	//	m_editWatchTime.SetWindowText(_T(""));
	//	AfxMessageBox(IDS_BO_SYS06, MB_ICONEXCLAMATION | MB_OK);
	//	return;
	//}

	//// check Registration number
	//if(m_stBsInfo.numRegister >= NumBoOccur)
	//{
	//	AfxMessageBox(IDS_BO_SYS11, MB_ICONEXCLAMATION | MB_OK);
	//	return;
	//}
	//// check threshold
	//m_editThreshold.GetWindowText(buf, 64);
	//if(_stscanf(buf, _T("%f"), &numBoAlarm) != 1)
	//{
	//	m_editThreshold.SetWindowText(_T(""));
	//	AfxMessageBox(IDS_BO_SYS12, MB_ICONEXCLAMATION | MB_OK);
	//	return;
	//}
	//if(numBoAlarm < 0.0 || numBoAlarm > 1.0)
	//{
	//	m_editThreshold.SetWindowText(_T(""));
	//	AfxMessageBox(IDS_BO_SYS12, MB_ICONEXCLAMATION | MB_OK);
	//	return;
	//}
	//_stprintf(buf, _T("%.2f"), numBoAlarm);
	//m_editThreshold.SetWindowText(buf);

	//// check Un-registration
	//for(i = 0;i < m_stBsInfo.numRegister;i++)
	//{
	//	if(numBoAlarm == m_stBsInfo.boAlarm[i])
	//	{
	//		AfxMessageBox(IDS_BO_SYS13, MB_ICONEXCLAMATION | MB_OK);
	//		return;
	//	}
	//}

	//// check Number of times
	//m_editNumTimes.GetWindowText(buf, 64);
	//if(_stscanf(buf, _T("%d"), &numTimes) != 1 ||
	//	numTimes <= 0)
	//{
	//	m_editNumTimes.SetWindowText(_T(""));
	//	AfxMessageBox(IDS_BO_SYS14, MB_ICONEXCLAMATION | MB_OK);
	//	return;
	//}
	//_stprintf(buf, _T("%d"), numTimes);
	//m_editNumTimes.SetWindowText(buf);
	//
	//if((int)(numBoWatch / 500) < numTimes)
	//{
	//	AfxMessageBox(IDS_BO_SYS09, MB_ICONEXCLAMATION | MB_OK);
	//	return;
	//}

	//// Adoption
	//m_stBsInfo.boWatchTime = numBoWatch;
	//m_stBsInfo.boAlarm[m_stBsInfo.numRegister] = numBoAlarm;
	//m_stBsInfo.boTimes[m_stBsInfo.numRegister] = numTimes;

	//str.LoadString(IDS_BO_SYS04);
	//_stprintf(buf, str, m_stBsInfo.boAlarm[i], m_stBsInfo.boTimes[i]);
	//m_listBoRec.InsertString(-1, buf);

	//m_stBsInfo.numRegister++;
//}

//// delete operation.
//void CBoRec::DeleteBoRecognition(void)
//{
//	int		i, sel;
//	float	numBoAlarm;
//	_TCHAR	buf[64];
//	_TCHAR	*p;
//
//	// If an item isn't chosen,
//	if((sel = m_listBoRec.GetCurSel()) == -1)
//		return;
//
//	// An item is acquired.
//	m_listBoRec.GetText(sel, buf);
//
//	// An item is removed.
//	m_listBoRec.DeleteString(sel);
//
//	// threshold is acquired from the character line.
//	p = _tcsstr(buf, _T("."));
//	_stscanf(--p, _T("%f"), &numBoAlarm);
//
//	// The position where value is registered is looked up.
//	for(sel = 0;sel < m_stBsInfo.numRegister;sel++)
//		if(m_stBsInfo.boAlarm[sel] == numBoAlarm)
//			break;
//	
//	// Inside data are removed..
//	for(i = sel;i < m_stBsInfo.numRegister - 1;i++)
//	{
//		m_stBsInfo.boAlarm[i] = m_stBsInfo.boAlarm[i+1];
//		m_stBsInfo.boTimes[i] = m_stBsInfo.boTimes[i+1];
//	}
//	m_stBsInfo.boAlarm[i] = 0;
//	m_stBsInfo.boTimes[i] = 0;
//	m_stBsInfo.numRegister--;
//}

//// check powder.
//void CBoRec::CheckPowder(void)
//{
//	_TCHAR	buf[NumPowderChar+1];
//	int		cnt;
//
//	// get powder name
//	cnt = m_editPowder.GetWindowText(buf, NumPowderChar+1);
//	if( cnt <= 0 )
//		return;
//
//	// check count
//	if( m_stBsInfo.numRegPow >= NumPowder )
//	{
//		AfxMessageBox(_T("これ以上登録出来ません。"), MB_ICONEXCLAMATION | MB_OK);
//		return;
//	}
//
//	// insert the powder name to the list
//	m_listPowder.InsertString(-1, buf);
//
//	// データ登録
//	memcpy(m_stBsInfo.szPowder[m_stBsInfo.numRegPow], buf, sizeof(_TCHAR)*(NumPowderChar+1));
//	m_stBsInfo.numRegPow++;
//}
//
//// delete powder.
//void CBoRec::DeletePowder(void)
//{
//	int		i, sel;
//
//	// If an item isn't chosen,
//	if((sel = m_listPowder.GetCurSel()) == -1)
//		return;
//
//	// An item is removed.
//	m_listPowder.DeleteString(sel);
//
//	// Inside data are removed..
//	for(i = sel;i < m_stBsInfo.numRegPow - 1;i++)
//	{
//		memcpy(	m_stBsInfo.szPowder[i], 
//				m_stBsInfo.szPowder[i+1], 
//				sizeof(_TCHAR)*(NumPowderChar+1));
//	}
//	m_stBsInfo.numRegPow--;
//}

// check BO system info.
BOOL CBoRec::CheckBosysInfo(void)
{
	CString		str;
	_TCHAR		buf[256];
	int			len, sel;
	BoSysInfo	dummy = m_stBsInfo;

	// check comment
	len = m_editComment.GetWindowText(buf, 32);
	if( len <= 0 )
	{
		AfxMessageBox(IDS_BO_SYS15, MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}
	_tcscpy(dummy.szComment, buf);

	// get configuration name for BO
	m_cmbConfName.GetWindowText(buf, SizeConfig);
	memcpy(dummy.szConfName, buf, sizeof(_TCHAR)*SizeConfig);

	// get configuration name for DEP
	m_cmbConfName2.GetWindowText(buf, SizeConfig);
	memcpy(dummy.szConfNameD, buf, sizeof(_TCHAR)*SizeConfig);

	// check Number of times for BO Alarm.
	m_editAlmCombi.GetWindowText(buf, 32);
	if(_stscanf(buf, _T("%d"), &dummy.numCombiBA) != 1)
	{
		m_editAlmCombi.SetWindowText(_T(""));
		AfxMessageBox(IDS_BO_SYS10, MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}
	if(dummy.numCombiBA < 1)
	{
		m_editAlmCombi.SetWindowText(_T(""));
		AfxMessageBox(IDS_BO_SYS10, MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}
	_stprintf(buf, _T("%d"), dummy.numCombiBA);
	m_editAlmCombi.SetWindowText(buf);

	// check Number of times for BO Alarm Ctl.
	m_editAlmCtlCombi.GetWindowText(buf, 32);
	if(_stscanf(buf, _T("%d"), &dummy.numCombiBAC) != 1)
	{
		m_editAlmCtlCombi.SetWindowText(_T(""));
		AfxMessageBox(IDS_BO_SYS10, MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}
	if(dummy.numCombiBAC < 1)
	{
		m_editAlmCtlCombi.SetWindowText(_T(""));
		AfxMessageBox(IDS_BO_SYS10, MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}
	_stprintf(buf, _T("%d"), dummy.numCombiBAC);
	m_editAlmCtlCombi.SetWindowText(buf);

	// check config name for BO
	if( (sel = m_cmbConfName.GetCurSel()) == -1 )
	{
		AfxMessageBox(IDS_BO_SYS16, MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}
	m_cmbConfName.GetLBText(sel, str);
	_tcscpy(dummy.szConfName, str);

	// check config name for DEP
	if( (sel = m_cmbConfName2.GetCurSel()) == -1 )
	{
		AfxMessageBox(IDS_BO_SYS16, MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}
	m_cmbConfName2.GetLBText(sel, str);
	_tcscpy(dummy.szConfNameD, str);

	//// check powder
	//if(dummy.numRegPow <= 0)
	//{
	//	AfxMessageBox(IDS_BO_SYS17, MB_ICONEXCLAMATION | MB_OK);
	//	return FALSE;
	//}

	// 設定値反映
	m_stBsInfo = dummy;

	return TRUE;
}

