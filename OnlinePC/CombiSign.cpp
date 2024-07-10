// CombiSign.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "CombiSign.h"

///////////////////////////////////////////////////////////////////////////////
// definition

// reference data
static CConfig			&cnf	= theApp.m_cnf;
static TcName			*tcnm	= &theApp.m_stTcNm;


///////////////////////////////////////////////////////////////////////////////
// CCombiSign ダイアログ

IMPLEMENT_DYNAMIC(CCombiSign, CDialog)
CCombiSign::CCombiSign(int nCombiUp, int nCombiLo, CWnd* pParent /*=NULL*/)
	: CDialog(CCombiSign::IDD, pParent)
{
	m_nCombiUp = nCombiUp;
	m_nCombiLo = nCombiLo;
}

CCombiSign::~CCombiSign()
{
}

void CCombiSign::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBI_UP, m_textCombiUp);
	DDX_Control(pDX, IDC_COMBI_LO, m_textCombiLo);
	DDX_Control(pDX, IDC_COMBI_NET, m_editCombiNet);
}


BEGIN_MESSAGE_MAP(CCombiSign, CDialog)
	ON_BN_CLICKED(IDC_COMBI_APPEND, OnClickedCombiAppend)
	ON_BN_CLICKED(IDC_COMBI_DEL, OnClickedCombiDel)
	ON_BN_CLICKED(IDCANCEL, OnClickedCancel)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CCombiSign メッセージ ハンドラ

BOOL CCombiSign::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetupCombinationDlg(&m_nCombiUp, &m_nCombiLo);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CCombiSign::OnClickedCombiAppend()
{
	AppendCombination(m_nCombiUp, m_nCombiLo);
	EndDialog(IDOK);
}

void CCombiSign::OnClickedCombiDel()
{
	DeleteCombination(m_nCombiUp, m_nCombiLo);
	EndDialog(IDOK);
}

void CCombiSign::OnClickedCancel()
{
	OnCancel();
}

///////////////////////////////////////////////////////////////////////////////
// CCombiSign member function

// set up TC combination dialog editting string.
void CCombiSign::SetupCombinationDlg(int *upper, int *lower)
{
	_TCHAR	buf[32];
	int		i;

	// sort combination point. 
	SortCombiPoint(upper, lower);

	// set upper TC ID into text. 
	if( *upper >= 0 && *upper < NumTCTap )
		_stprintf(buf, _T("%s"), tcnm->name[*upper]);
	else
		buf[0] = 0;
	m_textCombiUp.SetWindowText(buf);

	// set lower TC ID into text. 
	if( *lower >= 0 && *lower < NumTCTap )
		_stprintf(buf, _T("%s"), tcnm->name[*lower]);
	else
		buf[0] = 0;
	m_textCombiLo.SetWindowText(buf);

	// search same combination. 
	for (i = 0; i < cnf.numCombination; i++) {
		if (cnf.tcCombination[i].upper == *upper && cnf.tcCombination[i].lower == *lower)
			break;
	}
	// set Neural Network ID into editting column. 
	if (i != cnf.numCombination)
		_stprintf(buf, _T("%d"), cnf.tcCombination[i].netID + 1);
	else
		*buf = '\0';
	m_editCombiNet.SetWindowText(buf);
}

// sort combination point.
void CCombiSign::SortCombiPoint(int *pt0, int *pt1)
{
	int	upper, lower;

	if (cnf.inputPoint[*pt0].gy < cnf.inputPoint[*pt1].gy)
		upper	= *pt0,		lower	= *pt1;
	else if (cnf.inputPoint[*pt0].gy > cnf.inputPoint[*pt1].gy)
		upper	= *pt1,		lower	= *pt0;
	else if (cnf.inputPoint[*pt0].gx < cnf.inputPoint[*pt1].gx)
		upper	= *pt1,		lower	= *pt0;
	else
		upper	= *pt0,		lower	= *pt1;

	*pt0	= upper,	*pt1	= lower;
}

// append thermo-couple combination.
void CCombiSign::AppendCombination(int upper, int lower)
{
	_TCHAR	buf[32];
	int	net;
	int	i;

	// get network identifier. 
	m_editCombiNet.GetWindowText(buf, 32);
	if (sscanf(buf, _T("%d"), &net) != 1 || net <= 0 || net > NumNeuralNet) {
		return;
	}
	net--;

	// search entry. 
	for (i = 0; i < cnf.numCombination; i++) {
		if (cnf.tcCombination[i].upper == upper &&
			cnf.tcCombination[i].lower == lower)
			break;
	}
	// update entry. 
	cnf.tcCombination[i].upper = upper;
	cnf.tcCombination[i].lower = lower;
	cnf.tcCombination[i].netID = net;
	if (i == cnf.numCombination)
		cnf.numCombination++;
}

// delete thermo-couple combination.
void CCombiSign::DeleteCombination(int upper, int lower)
{
	int		i;

	// search entry. 
	for (i = 0; i < cnf.numCombination; i++) {
		if (cnf.tcCombination[i].upper == upper && cnf.tcCombination[i].lower == lower)
			break;
	}
	// if found, 
	if (i < cnf.numCombination) {
		// delete the entry. 
		for ( ; i < cnf.numCombination - 1; i++)
			cnf.tcCombination[i] = cnf.tcCombination[i + 1];
		cnf.numCombination--;
	}
}

