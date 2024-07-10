// NetSel.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "NetSel.h"


///////////////////////////////////////////////////////////////////////////////
// definition
static CConfig	&cnf = theApp.m_cnf;
static TcName	*tcnm	= &theApp.m_stTcNm;


// CNetSel dialog
IMPLEMENT_DYNAMIC(CNetSel, CDialog)
CNetSel::CNetSel(CWnd* pParent /*=NULL*/)
	: CDialog(CNetSel::IDD, pParent)
{
	m_nNeuralnetIdx = -1;
	m_nCombinationIdx = -1;
}

CNetSel::~CNetSel()
{
}

void CNetSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cmbNet);
	DDX_Control(pDX, IDC_COMBO2, m_cmbCombi);
	DDX_Control(pDX, IDC_EDIT7, m_editNeuralNet);
}


BEGIN_MESSAGE_MAP(CNetSel, CDialog)
	ON_BN_CLICKED(IDOK, OnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CNetSel message handler

BOOL CNetSel::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetupNetworkList();
	SetupCombinationList(-1);

	return TRUE;  // return TRUE unless you set the focus to a control
}

// clicked OK button.
void CNetSel::OnClickedOk()
{
	int	sel;
	CString s;

	// store neural network index.
	sel = m_cmbNet.GetCurSel();
	if (sel >= 0)
		m_nNeuralnetIdx	= m_nNeuralnetTab[sel];

	// store combination index.
	sel = m_cmbCombi.GetCurSel();
	if (sel >= 0) {
		m_nCombinationIdx = m_nCombinationTab[sel];
	}
	
	//M
	m_cmbNet.GetWindowText(s);
	m_editNeuralNet.SetWindowText(s);
	OnOK();
}

// clicked Cancel button.
void CNetSel::OnClickedCancel()
{
	OnCancel();
}

// change network combobox
void CNetSel::OnSelchangeCombo1()
{
	int	sel;
	CString s;
	

	sel = m_cmbNet.GetCurSel();
	SetupCombinationList(sel);
	//M
	m_cmbNet.GetWindowText(s);
	m_editNeuralNet.SetWindowText(s);

}


///////////////////////////////////////////////////////////////////////////////
// CNetSel user function

// set up Network list.
void CNetSel::SetupNetworkList(void)
{
	int	i, numNNList;
	int	sel	= -1;
	CString s;

	// reset neural network list.
	m_cmbNet.SetRedraw(FALSE);
	m_cmbNet.ResetContent();

	// for each neural network entry,
	for (i = 0, numNNList = 0; i < NumNeuralNet; i++) {
		// if neural network structure definition is valid,
		if (cnf.neuralnetConf[i].patternNN.numLayer &&
			cnf.neuralnetConf[i].corrNN.numLayer) {
			// test index.
			if (m_nNeuralnetIdx == i)
				sel	= numNNList;
			// store the identifier of neural network.
			m_nNeuralnetTab[numNNList++]	= i;
			// append neural net entry.
			m_cmbNet.AddString(cnf.neuralnetConf[i].remark);
		}
	}

	// terminate neural network list.
	m_cmbNet.SetRedraw(TRUE);

	// set high light now selection.
	if (sel >= 0)
		m_cmbNet.SetCurSel(sel);

	
	//M
	m_cmbNet.GetWindowText(s);
	m_editNeuralNet.SetWindowText(s);	

}

// set up Combination list.
void CNetSel::SetupCombinationList(int sel)
{
	int	i, numCombiList;
	int	nId, upper, lower;
	int	cIdx;
	_TCHAR	buf[32];

	nId		= (sel >= 0) ?	m_nNeuralnetTab[sel] : m_nNeuralnetIdx;
	cIdx	= m_nCombinationIdx;

	// reset combination list.
	m_cmbCombi.SetRedraw(FALSE);
	m_cmbCombi.ResetContent();

	numCombiList = 0;
	sel	= -1;
	for (i = 0; i < cnf.numCombination; i++) {
		upper = cnf.tcCombination[i].upper;
		lower = cnf.tcCombination[i].lower;
		if (cnf.tcCombination[i].netID == nId && upper >= 0 && upper < NumTCTap && lower >= 0 && lower < NumTCTap) {
			// test combination index.
			if (i == cIdx)
				sel	= numCombiList;
			// store entry.
			m_nCombinationTab[numCombiList++]	= i;
			_stprintf(buf, _T("%s,%s"), tcnm->name[upper], tcnm->name[lower]);
			// append combination entry.
			m_cmbCombi.AddString(buf);
		}
	}

	// terminate combination list.
	m_cmbCombi.SetRedraw(TRUE);

	// set high light now selection.
	if (sel >= 0)
		m_cmbCombi.SetCurSel(sel);
	else if (numCombiList)
		m_cmbCombi.SetCurSel(0);
}


