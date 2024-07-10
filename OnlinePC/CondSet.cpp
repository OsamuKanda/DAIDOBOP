// CondSet.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "CondSet.h"
#include ".\condset.h"

static CString	strTypeTemp[] = 
{
	_T(""),
	_T("A"),
	_T("B"),
	_T("C"),
	_T("D"),
	_T("E"),
	_T("F")
};

static CString	strTypeCalc[] = 
{
	_T(""),
	_T("+"),
	_T("-"),
	_T("*"),
	_T("/")
};

static CString	strTypeLRO[] = 
{
	_T(">"),
	_T("<"),
	_T(">="),
	_T("<=")
};

static CString	strTypeLogic[] = 
{
	_T("AND"),
	_T("OR")
};


// CCondSet ダイアログ

IMPLEMENT_DYNAMIC(CCondSet, CDialog)
CCondSet::CCondSet(CWnd* pParent /*=NULL*/)
	: CDialog(CCondSet::IDD, pParent)
{
	m_nNo = 0;
	memset( &m_stItem, 0, sizeof(m_stItem) );
}

CCondSet::~CCondSet()
{
}

void CCondSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMBO_F1, m_cmbF1);
	DDX_Control(pDX, IDC_CMBO_F2, m_cmbF2);
	DDX_Control(pDX, IDC_CMBO_F3, m_cmbF3);
	DDX_Control(pDX, IDC_CMBO_F4, m_cmbF4);
	DDX_Control(pDX, IDC_CMBO_F5, m_cmbF5);
	DDX_Control(pDX, IDC_CMBO_F6, m_cmbF6);
	DDX_Control(pDX, IDC_CMBO_F7, m_cmbF7);
	DDX_Control(pDX, IDC_CMBO_F8, m_cmbF8);
	DDX_Control(pDX, IDC_CMBO_F9, m_cmbF9);
	DDX_Control(pDX, IDC_CMBO_L1, m_cmbL1);
	DDX_Control(pDX, IDC_RDO_F1, m_rdoF1);
	DDX_Control(pDX, IDC_RDO_F2, m_rdoF2);
	DDX_Control(pDX, IDC_RDO_F3, m_rdoF3);
	DDX_Control(pDX, IDC_RDO_F4, m_rdoF4);
	DDX_Control(pDX, IDC_RDO_F5, m_rdoF5);
	DDX_Control(pDX, IDC_EDIT_COND, m_editCond);
	DDX_Control(pDX, IDC_EDIT_CONDTIME, m_editTime);
	DDX_Control(pDX, IDC_EDIT_CONDREMARK, m_editRemark);
	DDX_Control(pDX, IDC_EDIT_INVAL, m_editInVal);
	DDX_Control(pDX, IDC_COND_NO, m_txtCondNo);
	DDX_Control(pDX, IDC_EDIT_FORMULA, m_editFormula);
}


BEGIN_MESSAGE_MAP(CCondSet, CDialog)
	ON_BN_CLICKED(IDC_F1_SET_BTN, OnBnClickedF1SetBtn)
	ON_BN_CLICKED(IDC_L1_SET_BTN, OnBnClickedL1SetBtn)
	ON_BN_CLICKED(IDC_COND_CLR_BTN, OnBnClickedCondClrBtn)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_COND_BACK_BTN, OnBnClickedCondBackBtn)
	ON_CBN_SELCHANGE(IDC_CMBO_F1, OnCbnSelchangeCmboF1)
	ON_CBN_SELCHANGE(IDC_CMBO_F2, OnCbnSelchangeCmboF2)
	ON_CBN_SELCHANGE(IDC_CMBO_F3, OnCbnSelchangeCmboF3)
	ON_CBN_SELCHANGE(IDC_CMBO_F5, OnCbnSelchangeCmboF5)
	ON_CBN_SELCHANGE(IDC_CMBO_F6, OnCbnSelchangeCmboF6)
	ON_CBN_SELCHANGE(IDC_CMBO_F7, OnCbnSelchangeCmboF7)
	ON_CBN_SELCHANGE(IDC_CMBO_F8, OnCbnSelchangeCmboF8)
	ON_BN_CLICKED(IDC_RDO_F4, OnBnClickedRdoF4)
	ON_BN_CLICKED(IDC_RDO_F5, OnBnClickedRdoF5)
	ON_EN_CHANGE(IDC_EDIT_INVAL, OnEnChangeEditInval)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CCondSet メッセージ ハンドラ

BOOL CCondSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetupCondition();
	SetupEdit();
	SetupComboBox();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CCondSet::OnBnClickedF1SetBtn()
{
	CString		strVal, strVal2;
	StCondBlock	stBlock;
	int			nSel;

	if( m_stItem.nItemNum >= COND_BLOCK_MAX )
	{
		AfxMessageBox(_T("Can't add!! (Max20)"));
		return;
	}

	if( m_stItem.nItemNum > 0 && m_stItem.data[m_stItem.nItemNum-1].nmLogic == ID_LTypeNone )
	{
		AfxMessageBox(_T("Can't add!! (The logic is necessary.)"));
		return;
	}

	// get current condition
	m_editCond.GetWindowText(strVal);

	///////////////////
	// check left block

	// type "A - B"
	if( m_cmbF1.GetCurSel() > 0 && m_cmbF2.GetCurSel() > 0 && m_cmbF3.GetCurSel() > 0 )
	{
		stBlock.stElemL.nmBlock = ID_BType1;

		nSel = m_cmbF1.GetCurSel();
		stBlock.stElemL.nmElem[0] = GetIdxToElemTemp(nSel);
		nSel = m_cmbF2.GetCurSel();
		stBlock.stElemL.nmElem[1] = GetIdxToElemCalc(nSel);
		nSel = m_cmbF3.GetCurSel();
		stBlock.stElemL.nmElem[2] = GetIdxToElemTemp(nSel);

		m_cmbF1.GetWindowText(strVal2);
		strVal += _T(" ");
		strVal += _T(strVal2);
		m_cmbF2.GetWindowText(strVal2);
		strVal += _T(" ");
		strVal += _T(strVal2);
		m_cmbF3.GetWindowText(strVal2);
		strVal += _T(" ");
		strVal += _T(strVal2);
	}
	// type "B"
	else if( m_cmbF1.GetCurSel() > 0 || m_cmbF3.GetCurSel() > 0 )
	{
		stBlock.stElemL.nmBlock = ID_BType2;

		if( m_cmbF1.GetCurSel() > 0 )
		{
			nSel = m_cmbF1.GetCurSel();
			stBlock.stElemL.nmElem[0] = GetIdxToElemTemp(nSel);
			m_cmbF1.GetWindowText(strVal2);
		}
		else
		{
			nSel = m_cmbF3.GetCurSel();
			stBlock.stElemL.nmElem[0] = GetIdxToElemTemp(nSel);
			m_cmbF3.GetWindowText(strVal2);
		}
		strVal += _T(" ");
		strVal += _T(strVal2);
	}
	else
	{
		AfxMessageBox(_T("Please check!!"));
		return;
	}

	///////////////////
	// check left block

	nSel = m_cmbF5.GetCurSel();
	stBlock.nmElemC = GetIdxToElemLRO(nSel);

	m_cmbF5.GetWindowText(strVal2);
	strVal += _T(" ");
	strVal += _T(strVal2);

	///////////////////
	// check right block

	// type "A - B"
	if( m_cmbF6.GetCurSel() > 0 && m_cmbF7.GetCurSel() > 0 && m_cmbF8.GetCurSel() > 0 && m_rdoF4.GetCheck() )
	{
		stBlock.stElemR.nmBlock = ID_BType1;

		nSel = m_cmbF6.GetCurSel();
		stBlock.stElemR.nmElem[0] = GetIdxToElemTemp(nSel);
		nSel = m_cmbF7.GetCurSel();
		stBlock.stElemR.nmElem[1] = GetIdxToElemCalc(nSel);
		nSel = m_cmbF8.GetCurSel();
		stBlock.stElemR.nmElem[2] = GetIdxToElemTemp(nSel);

		m_cmbF6.GetWindowText(strVal2);
		strVal += _T(" ");
		strVal += _T(strVal2);
		m_cmbF7.GetWindowText(strVal2);
		strVal += _T(" ");
		strVal += _T(strVal2);
		m_cmbF8.GetWindowText(strVal2);
		strVal += _T(" ");
		strVal += _T(strVal2);
	}
	// type "A"
	else if( (m_cmbF6.GetCurSel() > 0 || m_cmbF8.GetCurSel() > 0) && m_rdoF4.GetCheck() )
	{
		stBlock.stElemR.nmBlock = ID_BType2;

		if( m_cmbF6.GetCurSel() > 0 )
		{
			nSel = m_cmbF6.GetCurSel();
			stBlock.stElemR.nmElem[0] = GetIdxToElemTemp(nSel);
			m_cmbF6.GetWindowText(strVal2);
		}
		else
		{
			nSel = m_cmbF8.GetCurSel();
			stBlock.stElemR.nmElem[0] = GetIdxToElemTemp(nSel);
			m_cmbF8.GetWindowText(strVal2);
		}

		strVal += _T(" ");
		strVal += _T(strVal2);
	}
	// type "10"
	else if( m_rdoF5.GetCheck() )
	{
		stBlock.stElemR.nmBlock = ID_BType3;

		m_editInVal.GetWindowText(strVal2);
		stBlock.stElemR.nmElem[0] = ID_TypeInp;
		stBlock.stElemR.dVal = _tstof(strVal2);

		strVal2.Format(_T("%.0lf"), stBlock.stElemR.dVal);
		strVal += _T(" ");
		strVal += _T(strVal2);
	}
	else
	{
		AfxMessageBox(_T("Please check!!"));
		return;
	}

	stBlock.nmLogic = ID_LTypeNone;
	m_stItem.data[m_stItem.nItemNum] = stBlock;
	m_stItem.nItemNum++;

	// set condition.
	m_editCond.SetWindowText(strVal);
}

void CCondSet::OnBnClickedL1SetBtn()
{
	CString	strVal, strVal2;
	NmCondLogicType	nmLogic;
	int				nSel;

	if( m_stItem.nItemNum == 0 ||
		(m_stItem.nItemNum > 0 && m_stItem.data[m_stItem.nItemNum-1].nmLogic != ID_LTypeNone) )
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

void CCondSet::OnBnClickedCondBackBtn()
{
	if( m_stItem.nItemNum > 0 )
	{
		if( m_stItem.data[m_stItem.nItemNum-1].nmLogic == ID_LTypeNone )
		{
			m_stItem.nItemNum--;
			memset( &m_stItem.data[m_stItem.nItemNum], 0, sizeof(m_stItem.data[m_stItem.nItemNum]) );
		}
		else
		{
			m_stItem.data[m_stItem.nItemNum-1].nmLogic = ID_LTypeNone;
		}

		SetupCondition();
	}
}

void CCondSet::OnBnClickedCondClrBtn()
{
	memset( &m_stItem, 0, sizeof(m_stItem) );
	SetupCondition();
}

void CCondSet::OnBnClickedOk()
{
	// store condition.
	if( StoreCondition() )
		OnOK();
}

void CCondSet::OnBnClickedCancel()
{
	OnCancel();
}

//
void CCondSet::OnCbnSelchangeCmboF1()
{
	m_editFormula.SetWindowText(GetFormulaString());
}

void CCondSet::OnCbnSelchangeCmboF2()
{
	m_editFormula.SetWindowText(GetFormulaString());
}

void CCondSet::OnCbnSelchangeCmboF3()
{
	m_editFormula.SetWindowText(GetFormulaString());
}

void CCondSet::OnCbnSelchangeCmboF5()
{
	m_editFormula.SetWindowText(GetFormulaString());
}

void CCondSet::OnCbnSelchangeCmboF6()
{
	m_editFormula.SetWindowText(GetFormulaString());
}

void CCondSet::OnCbnSelchangeCmboF7()
{
	m_editFormula.SetWindowText(GetFormulaString());
}

void CCondSet::OnCbnSelchangeCmboF8()
{
	m_editFormula.SetWindowText(GetFormulaString());
}

void CCondSet::OnBnClickedRdoF4()
{
	m_editFormula.SetWindowText(GetFormulaString());
}

void CCondSet::OnBnClickedRdoF5()
{
	m_editFormula.SetWindowText(GetFormulaString());
}

void CCondSet::OnEnChangeEditInval()
{
	m_editFormula.SetWindowText(GetFormulaString());
}

///////////////////////////////////////////////////////////////////////////////
// member function

// setup edit.
void CCondSet::SetupEdit( void )
{
	CString	strVal;

    strVal.Format(_T("%02d"), m_nNo);
	m_txtCondNo.SetWindowText(strVal);

	strVal.Format(_T("%d"), m_stItem.nTime);
	m_editTime.SetWindowText(strVal);

	strVal = m_stItem.cRemark;
	m_editRemark.SetWindowText(strVal);
}

// setup combo box.
void CCondSet::SetupComboBox( void )
{
	int		i;

	for( i = 0; i < 7; i++ )
		m_cmbF1.AddString(strTypeTemp[i]);

	for( i = 0; i < 5; i++ )
		m_cmbF2.AddString(strTypeCalc[i]);

	for( i = 0; i < 7; i++ )
		m_cmbF3.AddString(strTypeTemp[i]);

	for( i = 0; i < 7; i++ )
		m_cmbF4.AddString(strTypeTemp[i]);

	for( i = 0; i < 4; i++ )
		m_cmbF5.AddString(strTypeLRO[i]);

	for( i = 0; i < 7; i++ )
		m_cmbF6.AddString(strTypeTemp[i]);

	for( i = 0; i < 5; i++ )
		m_cmbF7.AddString(strTypeCalc[i]);

	for( i = 0; i < 7; i++ )
		m_cmbF8.AddString(strTypeTemp[i]);

	for( i = 0; i < 7; i++ )
		m_cmbF9.AddString(strTypeTemp[i]);

	for( i = 0; i < 2; i++ )
		m_cmbL1.AddString(strTypeLogic[i]);

	m_cmbF1.SetCurSel(0);
	m_cmbF2.SetCurSel(0);
	m_cmbF3.SetCurSel(0);
	m_cmbF4.SetCurSel(0);
	m_cmbF5.SetCurSel(0);
	m_cmbF6.SetCurSel(0);
	m_cmbF7.SetCurSel(0);
	m_cmbF8.SetCurSel(0);
	m_cmbF9.SetCurSel(0);
	m_cmbL1.SetCurSel(0);

	m_rdoF4.SetCheck(1);
}

NmCondElemType CCondSet::GetIdxToElemTemp( int nSel )
{
	NmCondElemType nmElem;

	if( nSel == -1 ) nSel = 0;
	switch( nSel )
	{
	case 1: nmElem = ID_TypeA; break;
	case 2: nmElem = ID_TypeB; break;
	case 3: nmElem = ID_TypeC; break;
	case 4: nmElem = ID_TypeD; break;
	case 5: nmElem = ID_TypeE; break;
	case 6: nmElem = ID_TypeF; break;
	default:nmElem = ID_TypeA; break;
	}
	return nmElem;
}

NmCondElemType CCondSet::GetIdxToElemCalc( int nSel )
{
	NmCondElemType nmElem;

	if( nSel == -1 ) nSel = 0;
	switch( nSel )
	{
	case 1: nmElem = ID_TypeAdd; break;
	case 2: nmElem = ID_TypeSub; break;
	case 3: nmElem = ID_TypeMul; break;
	case 4: nmElem = ID_TypeDiv; break;
	default:nmElem = ID_TypeAdd; break;
	}
	return nmElem;
}

NmCondElemType CCondSet::GetIdxToElemLRO( int nSel )
{
	NmCondElemType nmElem;

	if( nSel == -1 ) nSel = 0;
	switch( nSel )
	{
	case 0: nmElem = ID_TypeLO; break;
	case 1: nmElem = ID_TypeRO; break;
	case 2: nmElem = ID_TypeLOE; break;
	case 3: nmElem = ID_TypeROE; break;
	default:nmElem = ID_TypeLO; break;
	}
	return nmElem;
}

NmCondLogicType CCondSet::GetIdxToElemLogic( int nSel )
{
	NmCondLogicType nmLogic;

	if( nSel == -1 ) nSel = 0;
	switch( nSel )
	{
	case 0: nmLogic = ID_LTypeAnd; break;
	case 1: nmLogic = ID_LTypeOr; break;
	default:nmLogic = ID_LTypeAnd; break;
	}
	return nmLogic;
}

// set data
void CCondSet::SetData( int nNo, StCondItem &stItem )
{
	m_nNo		= nNo;
	m_stItem	= stItem; 
}

// get data
void CCondSet::GetData( StCondItem &stItem )
{
	stItem = m_stItem; 
}

// setup condition
void CCondSet::SetupCondition(void)
{
	CString	strVal;

	strVal = GetCondString(m_stItem);

	// set condition.
	m_editCond.SetWindowText(strVal);
}

// store condition.
BOOL CCondSet::StoreCondition(void)
{
	CString	strVal;
	int		nNum;

	nNum = m_stItem.nItemNum;
	if( nNum > 0 && m_stItem.data[nNum-1].nmLogic != ID_LTypeNone )
	{
		AfxMessageBox(_T("The Formula is necessary last!!"));
		return FALSE;
	}

	m_editTime.GetWindowText(strVal);
	m_stItem.nTime = _tstoi(strVal);

	m_editRemark.GetWindowText(strVal);
	_tcsncpy(m_stItem.cRemark, strVal, COND_REMARK_MAX);

	return TRUE;
}

// get condition string.
CString CCondSet::GetCondString( StCondItem &stItem )
{
	CString	strVal = _T("");
	CString	strVal2;
	int		i;

	for( i = 0; i < stItem.nItemNum; i++ )
	{
		switch(	stItem.data[i].stElemL.nmBlock )
		{
		case ID_BType1:
			strVal += _T(" ");
			strVal += GetTypeString(stItem.data[i].stElemL.nmElem[0]);
			strVal += _T(" ");
			strVal += GetTypeString(stItem.data[i].stElemL.nmElem[1]);
			strVal += _T(" ");
			strVal += GetTypeString(stItem.data[i].stElemL.nmElem[2]);
			break;
		case ID_BType2:
			strVal += _T(" ");
			strVal += GetTypeString(stItem.data[i].stElemL.nmElem[0]);
			break;
		}

		strVal += _T(" ");
		strVal += GetTypeString(stItem.data[i].nmElemC);

		switch(	stItem.data[i].stElemR.nmBlock )
		{
		case ID_BType1:
			strVal += _T(" ");
			strVal += GetTypeString(stItem.data[i].stElemR.nmElem[0]);
			strVal += _T(" ");
			strVal += GetTypeString(stItem.data[i].stElemR.nmElem[1]);
			strVal += _T(" ");
			strVal += GetTypeString(stItem.data[i].stElemR.nmElem[2]);
			break;
		case ID_BType2:
			strVal += _T(" ");
			strVal += GetTypeString(stItem.data[i].stElemR.nmElem[0]);
			break;
		case ID_BType3:
			strVal += _T(" ");
			strVal2.Format(_T("%.0lf"), stItem.data[i].stElemR.dVal);
			strVal += strVal2;
			break;
		}

		if( stItem.data[i].nmLogic != ID_LTypeNone )
		{
			strVal += _T(" ");
			strVal += GetLogicString(stItem.data[i].nmLogic);
		}
	}

	return strVal;
}

CString	CCondSet::GetTypeString( NmCondElemType nmType )
{
	CString	strVal = _T("");

	switch(nmType)
	{
	case ID_TypeA:
		strVal = strTypeTemp[1];
		break;
	case ID_TypeB:
		strVal = strTypeTemp[2];
		break;
	case ID_TypeC:
		strVal = strTypeTemp[3];
		break;
	case ID_TypeD:
		strVal = strTypeTemp[4];
		break;
	case ID_TypeE:
		strVal = strTypeTemp[5];
		break;
	case ID_TypeF:
		strVal = strTypeTemp[6];
		break;
	case ID_TypeAdd:
		strVal = strTypeCalc[1];
		break;
	case ID_TypeSub:
		strVal = strTypeCalc[2];
		break;
	case ID_TypeMul:
		strVal = strTypeCalc[3];
		break;
	case ID_TypeDiv:
		strVal = strTypeCalc[4];
		break;
	case ID_TypeLO:
		strVal = strTypeLRO[0];
		break;
	case ID_TypeRO:
		strVal = strTypeLRO[1];
		break;
	case ID_TypeLOE:
		strVal = strTypeLRO[2];
		break;
	case ID_TypeROE:
		strVal = strTypeLRO[3];
		break;
	}

	return strVal;
}

CString	CCondSet::GetLogicString( NmCondLogicType nmType )
{
	CString	strVal = _T("");

	switch(nmType)
	{
	case ID_LTypeAnd:
		strVal = strTypeLogic[0];
		break;
	case ID_LTypeOr:
		strVal = strTypeLogic[1];
		break;
	}

	return strVal;
}

// get condition string.
CString CCondSet::GetFormulaString( void )
{
	CString	strVal = _T("");
	CString	strVal2;

	// type "A - B"
	if( m_cmbF1.GetCurSel() > 0 && m_cmbF2.GetCurSel() > 0 && m_cmbF3.GetCurSel() > 0 )
	{
		m_cmbF1.GetWindowText(strVal2);
		strVal += strVal2;
		if( strVal2.GetLength() > 0 ) strVal += _T(" ");

		m_cmbF2.GetWindowText(strVal2);
		strVal += strVal2;
		if( strVal2.GetLength() > 0 ) strVal += _T(" ");

		m_cmbF3.GetWindowText(strVal2);
		strVal += strVal2;
		if( strVal2.GetLength() > 0 ) strVal += _T(" ");
	}
	// type "B"
	else if( m_cmbF1.GetCurSel() > 0 || m_cmbF3.GetCurSel() > 0 )
	{
		if( m_cmbF1.GetCurSel() > 0 )
		{
			m_cmbF1.GetWindowText(strVal2);
			strVal += strVal2;
			if( strVal2.GetLength() > 0 ) strVal += _T(" ");
		}
		else
		{
			m_cmbF3.GetWindowText(strVal2);
			strVal += strVal2;
			if( strVal2.GetLength() > 0 ) strVal += _T(" ");
		}
	}

	m_cmbF5.GetWindowText(strVal2);
	strVal += strVal2;
	if( strVal2.GetLength() > 0 ) strVal += _T(" ");

	if( m_cmbF6.GetCurSel() > 0 && m_cmbF7.GetCurSel() > 0 && m_cmbF8.GetCurSel() > 0 && m_rdoF4.GetCheck() )
	{
		m_cmbF6.GetWindowText(strVal2);
		strVal += strVal2;
		if( strVal2.GetLength() > 0 ) strVal += _T(" ");

		m_cmbF7.GetWindowText(strVal2);
		strVal += strVal2;
		if( strVal2.GetLength() > 0 ) strVal += _T(" ");

		m_cmbF8.GetWindowText(strVal2);
		strVal += strVal2;
	}
	else if( (m_cmbF6.GetCurSel() > 0 || m_cmbF8.GetCurSel() > 0) && m_rdoF4.GetCheck() )
	{
		if( m_cmbF6.GetCurSel() > 0 )
		{
			m_cmbF6.GetWindowText(strVal2);
			strVal += strVal2;
			if( strVal2.GetLength() > 0 ) strVal += _T(" ");
		}
		else
		{
			m_cmbF8.GetWindowText(strVal2);
			strVal += strVal2;
			if( strVal2.GetLength() > 0 ) strVal += _T(" ");
		}
	}
	else if( m_rdoF5.GetCheck() )
	{
		m_editInVal.GetWindowText(strVal2);
		strVal += strVal2;
	}

	return strVal;
}
