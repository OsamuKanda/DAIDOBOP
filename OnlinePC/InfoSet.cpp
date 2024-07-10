// InfoSet.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "InfoSet.h"

static CString	strTypeLogic[] = 
{
	_T("AND"),
	_T("OR")
};

// CInfoSet ダイアログ

IMPLEMENT_DYNAMIC(CInfoSet, CDialog)
CInfoSet::CInfoSet(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoSet::IDD, pParent)
{
	m_nNo = 0;
	memset( &m_stItem, 0, sizeof(m_stItem) );
	m_nNumList = 0;
	memset( &m_nIDs, 0, sizeof(m_nIDs) );
}

CInfoSet::~CInfoSet()
{
}

void CInfoSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMBO_C1, m_cmbC1);
	DDX_Control(pDX, IDC_CMBO_L1, m_cmbL1);
	DDX_Control(pDX, IDC_EDIT_COND, m_editCond);
	DDX_Control(pDX, IDC_EDIT_INFORMATION, m_editInfo);
	DDX_Control(pDX, IDC_INFO_NO, m_txtInfoNo);
}


BEGIN_MESSAGE_MAP(CInfoSet, CDialog)
	ON_BN_CLICKED(IDC_C1_SET_BTN, OnBnClickedC1SetBtn)
	ON_BN_CLICKED(IDC_L1_SET_BTN, OnBnClickedL1SetBtn)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_COND_CLR_BTN, OnBnClickedCondClrBtn)
	ON_BN_CLICKED(IDC_COND_BACK_BTN, OnBnClickedCondBackBtn)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CInfoSet メッセージ ハンドラ

BOOL CInfoSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetupCondition();
	SetupEdit();
	SetupComboBox();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CInfoSet::OnBnClickedC1SetBtn()
{
	CString		strVal, strVal2;
	StInfoElem	stElem;
	int			nSel;

	if( m_stItem.nItemNum >= INFO_ELEM_MAX )
	{
		AfxMessageBox(_T("Can't add!! (Max50)"));
		return;
	}

	if( m_stItem.nItemNum > 0 && m_stItem.data[m_stItem.nItemNum-1].nmLogic == ID_LogicNone )
	{
		AfxMessageBox(_T("Can't add!! (The logic is necessary.)"));
		return;
	}

	// get current condition
	m_editCond.GetWindowText(strVal);

	///////////////////
	// check element

	nSel = m_cmbC1.GetCurSel();
	stElem.nElem = m_nIDs[nSel];

	m_cmbC1.GetWindowText(strVal2);
	strVal += _T(" ");
	strVal += _T(strVal2);

	stElem.nmLogic = ID_LogicNone;
	m_stItem.data[m_stItem.nItemNum] = stElem;
	m_stItem.nItemNum++;

	// set condition.
	m_editCond.SetWindowText(strVal);
}

void CInfoSet::OnBnClickedL1SetBtn()
{
	CString	strVal, strVal2;
	NmInfoLogicType	nmLogic;
	int				nSel;

	if( m_stItem.nItemNum == 0 ||
		(m_stItem.nItemNum > 0 && m_stItem.data[m_stItem.nItemNum-1].nmLogic != ID_LogicNone) )
	{
		AfxMessageBox(_T("Can't add!! (The Formula is necessary.)"));
		return;
	}

	// get current condition.
	m_editCond.GetWindowText(strVal);

	nSel = m_cmbL1.GetCurSel();
	nmLogic = GetIdxToElemLogic(nSel);

	m_cmbL1.GetWindowText(strVal2);
	strVal += _T(" ");
	strVal += _T(strVal2);

	m_stItem.data[m_stItem.nItemNum-1].nmLogic = nmLogic;

	// set condition.
	m_editCond.SetWindowText(strVal);
}

void CInfoSet::OnBnClickedCondBackBtn()
{
	if( m_stItem.nItemNum > 0 )
	{
		if( m_stItem.data[m_stItem.nItemNum-1].nmLogic == ID_LogicNone )
		{
			m_stItem.nItemNum--;
			memset( &m_stItem.data[m_stItem.nItemNum], 0, sizeof(m_stItem.data[m_stItem.nItemNum]) );
		}
		else
		{
			m_stItem.data[m_stItem.nItemNum-1].nmLogic = ID_LogicNone;
		}

		SetupCondition();
	}
}

void CInfoSet::OnBnClickedCondClrBtn()
{
	memset( &m_stItem, 0, sizeof(m_stItem) );
	SetupCondition();
}

void CInfoSet::OnBnClickedOk()
{
	// store condition.
	if( StoreCondition() )
		OnOK();
}

void CInfoSet::OnBnClickedCancel()
{
	OnCancel();
}

///////////////////////////////////////////////////////////////////////////////
// member function

// setup edit.
void CInfoSet::SetupEdit( void )
{
	CString	strVal;

    strVal.Format(_T("%02d"), m_nNo);
	m_txtInfoNo.SetWindowText(strVal);

	strVal = m_stItem.cInfo;
	m_editInfo.SetWindowText(strVal);
}

// setup combo box.
void CInfoSet::SetupComboBox( void )
{
	CString	str;
	int		i;

	for( i = 0; i < m_nNumList; i++ )
	{
		str.Format(_T("%d"), m_nIDs[i]);
		m_cmbC1.AddString(str);
	}

	for( i = 0; i < 2; i++ )
		m_cmbL1.AddString(strTypeLogic[i]);

	m_cmbC1.SetCurSel(0);
	m_cmbL1.SetCurSel(0);
}

NmInfoLogicType CInfoSet::GetIdxToElemLogic( int nSel )
{
	NmInfoLogicType nmLogic;

	if( nSel == -1 ) nSel = 0;
	switch( nSel )
	{
	case 0: nmLogic = ID_LogicAnd; break;
	case 1: nmLogic = ID_LogicOr; break;
	default:nmLogic = ID_LogicAnd; break;
	}
	return nmLogic;
}

// set data
void CInfoSet::SetData( int nNo, StInfoItem &stItem )
{
	m_nNo		= nNo;
	m_stItem	= stItem; 
}

// get data
void CInfoSet::GetData( StInfoItem &stItem )
{
	stItem = m_stItem; 
}

// setup condition
void CInfoSet::SetupCondition(void)
{
	CString	strVal;

	strVal = GetCondString(m_stItem);

	// set condition.
	m_editCond.SetWindowText(strVal);
}

// store condition.
BOOL CInfoSet::StoreCondition(void)
{
	CString	strVal;
	int		nNum;

	nNum = m_stItem.nItemNum;
	if( nNum > 0 && m_stItem.data[nNum-1].nmLogic != ID_LogicNone )
	{
		AfxMessageBox(_T("The Formula is necessary last!!"));
		return FALSE;
	}

	m_editInfo.GetWindowText(strVal);
	_tcsncpy(m_stItem.cInfo, strVal, INFO_INFO_MAX);

	return TRUE;
}

// get condition string.
CString CInfoSet::GetCondString( StInfoItem &stItem )
{
	CString	strVal = _T("");
	CString	strVal2;
	int		i;

	for( i = 0; i < stItem.nItemNum; i++ )
	{
		strVal += _T(" ");
		strVal2.Format(_T("%d"), stItem.data[i].nElem);
		strVal += strVal2;

		if( stItem.data[i].nmLogic != ID_LogicNone )
		{
			strVal += _T(" ");
			strVal += GetLogicString(stItem.data[i].nmLogic);
		}
	}

	return strVal;
}

CString	CInfoSet::GetLogicString( NmInfoLogicType nmType )
{
	CString	strVal = _T("");

	switch(nmType)
	{
	case ID_LogicAnd:
		strVal = strTypeLogic[0];
		break;
	case ID_LogicOr:
		strVal = strTypeLogic[1];
		break;
	}

	return strVal;
}

void CInfoSet::SetIDList( int *pID, int num )
{
	m_nNumList = num;
	memcpy( &m_nIDs, pID, sizeof(m_nIDs) );
}

