// SimAxis.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "SimAxis.h"


///////////////////////////////////////////////////////////////////////////////
// definition
static CConfig	&cnf = theApp.m_cnf;


// CSimAxis dialog
IMPLEMENT_DYNAMIC(CSimAxis, CDialog)
CSimAxis::CSimAxis(CWnd* pParent /*=NULL*/)
	: CDialog(CSimAxis::IDD, pParent)
{
}

CSimAxis::~CSimAxis()
{
}

void CSimAxis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cmbTmScale);
	DDX_Control(pDX, IDC_EDIT1, m_editTempScale);
	DDX_Control(pDX, IDC_EDIT25, m_editTempZero);
}


BEGIN_MESSAGE_MAP(CSimAxis, CDialog)
	ON_BN_CLICKED(IDOK, OnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnClickedCancel)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CTrendAxis message handler

BOOL CSimAxis::OnInitDialog()
{
	CDialog::OnInitDialog();

	// setup trend axis
	SetupSimAxisDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSimAxis::OnClickedOk()
{
	if( UpdateAxisSetting() )
		OnOK();
}

void CSimAxis::OnClickedCancel()
{
	OnCancel();
}

///////////////////////////////////////////////////////////////////////////////
// CSimAxis user function

// set up trend graph pens.
void CSimAxis::SetupSimAxisDlg(void)
{
	CString	str;

	// set the time scale combo-box.
	m_cmbTmScale.ResetContent();
	str.LoadString(IDS_TRD_AXIS00);
	m_cmbTmScale.AddString(str);
	str.LoadString(IDS_TRD_AXIS01);
	m_cmbTmScale.AddString(str);
	str.LoadString(IDS_TRD_AXIS02);
	m_cmbTmScale.AddString(str);
	str.LoadString(IDS_TRD_AXIS03);
	m_cmbTmScale.AddString(str);
	str.LoadString(IDS_TRD_AXIS04);
	m_cmbTmScale.AddString(str);
	str.LoadString(IDS_TRD_AXIS05);
	m_cmbTmScale.AddString(str);
	str.LoadString(IDS_TRD_AXIS06);
	m_cmbTmScale.AddString(str);

	if (cnf.m_stTimeAxis.scale <= 0.3)
		m_cmbTmScale.SetCurSel(0);
	else if (cnf.m_stTimeAxis.scale <= 0.6)
		m_cmbTmScale.SetCurSel(1);
	else if (cnf.m_stTimeAxis.scale <= 1.5)
		m_cmbTmScale.SetCurSel(2);
	else if (cnf.m_stTimeAxis.scale <= 2.5)
		m_cmbTmScale.SetCurSel(3);
	else if (cnf.m_stTimeAxis.scale <= 6.0)
		m_cmbTmScale.SetCurSel(4);
	else if (cnf.m_stTimeAxis.scale <= 15.0)
		m_cmbTmScale.SetCurSel(5);
	else if (cnf.m_stTimeAxis.scale <= 25.0)
		m_cmbTmScale.SetCurSel(6);

	// set the temperature axis text.
	str.Format("%lf", cnf.m_stTempAxis.scale);
	m_editTempScale.SetWindowText(str);

	str.Format("%lf", cnf.m_stTempAxis.zero);
	m_editTempZero.SetWindowText(str);
}

// update chart axis setting.
BOOL CSimAxis::UpdateAxisSetting(void)
{
	ChartAxis	timeAxis;
	ChartAxis	tempAxis;
	CString		str;
	double		v;
	long		sel;

	// store time axis.
	sel = m_cmbTmScale.GetCurSel();
	m_cmbTmScale.GetWindowText(str);
	_stscanf(str, _T("%lf"), &v);
	if      (v <=  0.3)	timeAxis.scale =  0.25;
	else if (v <=  0.6)	timeAxis.scale =  0.5;
	else if (v <=  1.5)	timeAxis.scale =  1.0;
	else if (v <=  2.5)	timeAxis.scale =  2.0;
	else if (v <=  6.0)	timeAxis.scale =  5.0;
	else if (v <= 15.0)	timeAxis.scale = 10.0;
	else if (v <= 25.0)	timeAxis.scale = 20.0;
	timeAxis.zero = 0;

	// store temperature axis.
	m_editTempScale.GetWindowText(str);
	_stscanf(str, _T("%lf"), &tempAxis.scale);
	if (tempAxis.scale == 0.0) {
		// error message display
		AfxMessageBox(IDS_TRD_AXIS07, MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	m_editTempZero.GetWindowText(str);
	_stscanf(str, _T("%lf"), &tempAxis.zero);

	cnf.m_stTimeAxis	= timeAxis;
	cnf.m_stTempAxis	= tempAxis;

	// write axis file.
	cnf.WriteAxisFile();

	return TRUE;
}

