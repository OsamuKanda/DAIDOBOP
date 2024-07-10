// AdChSign.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "AdChSign.h"


///////////////////////////////////////////////////////////////////////////////
// definition

// reference data
static InputPoint	*inputPoint	= theApp.m_cnf.inputPoint;
static ADChannel	*adChannel	= theApp.m_cnf.adChannel;


///////////////////////////////////////////////////////////////////////////////
// CAdChSign ダイアログ

IMPLEMENT_DYNAMIC(CAdChSign, CDialog)
CAdChSign::CAdChSign(int nAnsArg, CWnd* pParent /*=NULL*/)
	: CDialog(CAdChSign::IDD, pParent)
{
	m_nAsnArg = nAnsArg;
}

CAdChSign::~CAdChSign()
{
}

void CAdChSign::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CH, m_editChNo);
	DDX_Control(pDX, IDC_EDIT_MIN, m_editMinVal);
	DDX_Control(pDX, IDC_EDIT_MAX, m_editMaxVal);
	DDX_Control(pDX, IDC_EDIT_UNIT, m_editUnit);
}


BEGIN_MESSAGE_MAP(CAdChSign, CDialog)
	ON_BN_CLICKED(IDOK, OnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnClickedCancel)
	ON_BN_CLICKED(IDC_DELETE, &CAdChSign::OnBnClickedDelete)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CAdChSign message handler

BOOL CAdChSign::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetupADAssignFactor(m_nAsnArg);

	m_editChNo.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
}

void CAdChSign::OnClickedOk()
{
	StoreADAssignment(m_nAsnArg);
	OnOK();
}

// 割り付けの削除
void CAdChSign::OnBnClickedDelete()
{
	m_editChNo.SetWindowText(_T("-1"));
	StoreADAssignment(m_nAsnArg);
	OnOK();
}

void CAdChSign::OnClickedCancel()
{
	OnCancel();
}

///////////////////////////////////////////////////////////////////////////////
// CAdChSign member function

// set up A/D Assignment Factor.
void CAdChSign::SetupADAssignFactor(int arg)
{
	CString		str;
	_TCHAR	buf[64];
	int	ch;

	if ((ch = inputPoint[arg].adCh) >= 0) {
		_stprintf(buf, _T("%d"), ch+1);
		m_editChNo.SetWindowText(buf);
		_stprintf(buf,_T("%lg"),adChannel[ch].min);
		m_editMinVal.SetWindowText(buf);
		_stprintf(buf, _T("%lg"), adChannel[ch].max);
		m_editMaxVal.SetWindowText(buf);
		m_editUnit.SetWindowText(adChannel[ch].unit);
	}
	else if (arg < NumTCTap) {
		m_editChNo.SetWindowText(_T(""));
		str.LoadString(IDS_DEF_TEMP_MIN);
		m_editMinVal.SetWindowText(str);
		str.LoadString(IDS_DEF_TEMP_MAX);
		m_editMaxVal.SetWindowText(str);
		str.LoadString(IDS_DEF_TEMP_UNIT);
		m_editUnit.SetWindowText(str);
	}
	else if (arg == SNID) {
		m_editChNo.SetWindowText(_T(""));
		str.LoadString(IDS_DEF_SN_MIN);
		m_editMinVal.SetWindowText(str);
		str.LoadString(IDS_DEF_SN_MAX);
		m_editMaxVal.SetWindowText(str);
		str.LoadString(IDS_DEF_SN_UNIT);
		m_editUnit.SetWindowText(str);
	}
	else if (arg == LevelID) {
		m_editChNo.SetWindowText(_T(""));
		str.LoadString(IDS_DEF_LEVEL_MIN);
		m_editMinVal.SetWindowText(str);
		str.LoadString(IDS_DEF_LEVEL_MAX);
		m_editMaxVal.SetWindowText(str);
		str.LoadString(IDS_DEF_LEVEL_UNIT);
		m_editUnit.SetWindowText(str);
	}
	else if (arg == VcID) {
		m_editChNo.SetWindowText(_T(""));
		str.LoadString(IDS_DEF_VC_MIN);
		m_editMinVal.SetWindowText(str);
		str.LoadString(IDS_DEF_VC_MAX);
		m_editMaxVal.SetWindowText(str);
		str.LoadString(IDS_DEF_VC_UNIT);
		m_editUnit.SetWindowText(str);
	}
	//else if (arg == WidthID) {
	//	m_editChNo.SetWindowText(_T(""));
	//	str.LoadString(IDS_DEF_WIDTH_MIN);
	//	m_editMinVal.SetWindowText(str);
	//	str.LoadString(IDS_DEF_WIDTH_MAX);
	//	m_editMaxVal.SetWindowText(str);
	//	str.LoadString(IDS_DEF_WIDTH_UNIT);
	//	m_editUnit.SetWindowText(str);
	//}
	//else if (arg == WidthLID || arg == WidthRID) {
	//	m_editChNo.SetWindowText(_T(""));
	//	str.LoadString(IDS_DEF_WIDTH_MIN);
	//	m_editMinVal.SetWindowText(str);
	//	str.LoadString(IDS_DEF_WIDTH_MAX);
	//	m_editMaxVal.SetWindowText(str);
	//	str.LoadString(IDS_DEF_WIDTH_UNIT);
	//	m_editUnit.SetWindowText(str);
	//}
	else if (arg == LengthID) {
		m_editChNo.SetWindowText(_T(""));
		str.LoadString(IDS_DEF_LEN_MIN);
		m_editMinVal.SetWindowText(str);
		str.LoadString(IDS_DEF_LEN_MAX);
		m_editMaxVal.SetWindowText(str);
		str.LoadString(IDS_DEF_LEN_UNIT);
		m_editUnit.SetWindowText(str);
	}
	//////////////////////////////////////////////////////////////////////////////
	// CSV保存のみの項目
	else if (arg == TdWeiID) {
		m_editChNo.SetWindowText(_T(""));
		str.LoadString(IDS_DEF_TDW_MIN);
		m_editMinVal.SetWindowText(str);
		str.LoadString(IDS_DEF_TDW_MAX);
		m_editMaxVal.SetWindowText(str);
		str.LoadString(IDS_DEF_TDW_UNIT);
		m_editUnit.SetWindowText(str);
	}
	else if (arg == MdWtAID) {
		m_editChNo.SetWindowText(_T(""));
		str.LoadString(IDS_DEF_MDWTA_MIN);
		m_editMinVal.SetWindowText(str);
		str.LoadString(IDS_DEF_MDWTA_MAX);
		m_editMaxVal.SetWindowText(str);
		str.LoadString(IDS_DEF_MDWTA_UNIT);
		m_editUnit.SetWindowText(str);
	}
	else if (arg == MdWtCID) {
		m_editChNo.SetWindowText(_T(""));
		str.LoadString(IDS_DEF_MDWTC_MIN);
		m_editMinVal.SetWindowText(str);
		str.LoadString(IDS_DEF_MDWTC_MAX);
		m_editMaxVal.SetWindowText(str);
		str.LoadString(IDS_DEF_MDWTC_UNIT);
		m_editUnit.SetWindowText(str);
	}
	else if (arg == MdWtDID) {
		m_editChNo.SetWindowText(_T(""));
		str.LoadString(IDS_DEF_MDWTD_MIN);
		m_editMinVal.SetWindowText(str);
		str.LoadString(IDS_DEF_MDWTD_MAX);
		m_editMaxVal.SetWindowText(str);
		str.LoadString(IDS_DEF_MDWTD_UNIT);
		m_editUnit.SetWindowText(str);
	}
	else if (arg == MdTmpDID) {
		m_editChNo.SetWindowText(_T(""));
		str.LoadString(IDS_DEF_MDTMPD_MIN);
		m_editMinVal.SetWindowText(str);
		str.LoadString(IDS_DEF_MDTMPD_MAX);
		m_editMaxVal.SetWindowText(str);
		str.LoadString(IDS_DEF_MDTMPD_UNIT);
		m_editUnit.SetWindowText(str);
	}
}

// store the assignment.
void CAdChSign::StoreADAssignment(int arg)
{
	_TCHAR	buf[64];
	int	ch;
	double	min, max;

	// get assigned channel from dialog text.
	m_editChNo.GetWindowText(buf, 64);
	// if the string is invalid or the channel is invalid, error.
	if (sscanf(buf, "%d", &ch) != 1)
		goto err;
	ch--;
//	if (ch >= NumADChannel)
	if (ch >= NumADChannelDisp)
		goto err;

	// get editted minmum value from dialog text.
	m_editMinVal.GetWindowText(buf, 64);
	// if the string is invalid or the channel is invalid, error.
	if (sscanf(buf, "%lf", &min) != 1)
		goto err;

	// get editted maxmum value from dialog text.
	m_editMaxVal.GetWindowText(buf, 64);
	// if the string is invalid or the channel is invalid, error.
	if (sscanf(buf, "%lf", &max) != 1)
		goto err;

	// get editted unit from dialog text.
	m_editUnit.GetWindowText(buf, 64);
	buf[31]	= '\0';

	/* if the channel has been specified by another input, */
	if ((ch >= 0 && adChannel[ch].arg >= 0 && adChannel[ch].arg != arg)) {
		/* display message box, and if the update is confirmed, */
		if (AfxMessageBox(IDS_ADCH_SIGN00, MB_ICONEXCLAMATION | MB_YESNO) == IDYES) 
		{
			/* set null specification. */
			inputPoint[adChannel[ch].arg].adCh	= -1;
		}
		/* if the update is not confirmed, do nothing. */
		else
			return;
	}
	/* if the argument has been assigned, */
	if (inputPoint[arg].adCh >= 0) {
		/* set null assignment. */
		adChannel[inputPoint[arg].adCh].arg	= -1;
	}
	/* store the A/D channel specification. */
	if (ch >= 0) {
		adChannel[ch].arg	= arg;
		adChannel[ch].min	= min;
		adChannel[ch].max	= max;
		strcpy(adChannel[ch].unit, buf);
		/* store the input point assignment. */
		inputPoint[arg].adCh = ch;
	}
	else {
		inputPoint[arg].adCh = -1;
	}

	return;

err:
	AfxMessageBox(IDS_ADCH_SIGN01, MB_ICONEXCLAMATION | MB_OK);
	return;
}

