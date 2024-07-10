// NnStruct.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "NnStruct.h"
#include "Config.h"
#include ".\nnstruct.h"

///////////////////////////////////////////////////////////////////////////////
// definition

// reference data
static NeuralNetConf	*neuralnetConf	= theApp.m_cnf.neuralnetConf;


///////////////////////////////////////////////////////////////////////////////
// CNnStruct class

IMPLEMENT_DYNAMIC(CNnStruct, CDialog)
CNnStruct::CNnStruct(int nAnsNet, CWnd* pParent /*=NULL*/)
	: CDialog(CNnStruct::IDD, pParent)
{
	m_nAsnNet = nAnsNet;
	memset(&m_stNetConf, 0, sizeof(m_stNetConf));
	memcpy(&m_stNetConf, &neuralnetConf[m_nAsnNet], sizeof(m_stNetConf));
}

CNnStruct::~CNnStruct()
{
}

void CNnStruct::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NNREMARK, m_editRemark);
	DDX_Control(pDX, IDC_PATNN, m_editPat);
	DDX_Control(pDX, IDC_CORNN, m_editCor);
	DDX_Control(pDX, IDC_EDIT11, m_editWatchTime);
	DDX_Control(pDX, IDC_EDIT31, m_editAlmCombi);
	DDX_Control(pDX, IDC_EDIT41, m_editAlmCtlCombi);
	DDX_Control(pDX, IDC_LIST1, m_listBoRec);
	DDX_Control(pDX, IDC_EDIT5, m_editThreshold);
	DDX_Control(pDX, IDC_EDIT7, m_editNumTimes);
}


BEGIN_MESSAGE_MAP(CNnStruct, CDialog)
	ON_BN_CLICKED(IDOK, OnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CNnStruct::OnBnClickedButton3)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CNnStruct message handler

BOOL CNnStruct::OnInitDialog()
{
	CDialog::OnInitDialog();

	// set up netral net structure dialog.
	SetupNetStructDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
}

// ニューラルネット構造を確定（OK）するボタンが押されたとき
void CNnStruct::OnClickedOk()
{
	if( TRUE == StoreNetStructDlg() )
		OnOK();
}


// ニューラルネット構造を削除するボタンが押されたとき
void CNnStruct::OnBnClickedButton3()
{
	// 情報を削除
	memset(&neuralnetConf[m_nAsnNet], 0, sizeof(neuralnetConf[m_nAsnNet]));
	OnOK();
}

void CNnStruct::OnClickedCancel()
{
	OnCancel();
}

// BO認識の登録
void CNnStruct::OnBnClickedButton1()
{
	CheckRegistration();
}

// BO認識の削除
void CNnStruct::OnBnClickedButton2()
{
	DeleteBoRecognition();
}

///////////////////////////////////////////////////////////////////////////////
// CNnStruct member function

// set up netral net structure dialog editting string.
void CNnStruct::SetupNetStructDlg(void)
{
	CString	str;
	_TCHAR	buf[256];
	int		j;

	// set remark string into editting column. 
	m_editRemark.SetWindowText(m_stNetConf.remark);

	// make up pattern neural net structure string. 
	MakeupNetStructString(&m_stNetConf.patternNN, buf);

	// It deletes ",1" of the neural net structure.
	if(buf[0] != _T('\0'))
		buf[_tcslen(buf) - 2] = _T('\0');
	
	// set patten neural net structure string into editting column. 
	m_editPat.SetWindowText(buf);

	// make up correlation neural net structure string. 
	MakeupNetStructString(&m_stNetConf.corrNN, buf);

	// It deletes "2,,1" of the neural net structure.
	if(buf[0] != _T('\0'))
	{
		FillMemory(&buf[_tcslen(buf) - 2],3,_T('\0'));
		CopyMemory(buf,&buf[2],_tcslen(buf));
	}

	// set correlation neural net structure string into editting column. 
	m_editCor.SetWindowText(buf);

	// display B.O watch time.
	str.Format(_T("%d"), m_stNetConf.boWatchTime);
	m_editWatchTime.SetWindowText(str);

	// The contents of ListBox are cleared.
	m_listBoRec.ResetContent();

	// display B.O recognition.
	for(j = 0;j < m_stNetConf.numRegister;j++)
	{
		str.LoadString(IDS_BO_SYS04);
		_stprintf(buf, str, m_stNetConf.boAlarm[j], m_stNetConf.boTimes[j]);
		m_listBoRec.InsertString(-1, buf);
	}

	// display Number of combinations.
	//str.Format(_T("%d"), m_stNetConf.numCombiBA);
	//m_editAlmCombi.SetWindowText(str);
	//str.Format(_T("%d"), m_stNetConf.numCombiBAC);
	//m_editAlmCtlCombi.SetWindowText(str);
}

// make up neural net structure string.
void CNnStruct::MakeupNetStructString(NNTemplate *net, LPTSTR str)
{
	int	i, len;

	// initialize string.
	*str	= _T('\0');

	// for each layer,
	for (i = 0; i < net->numLayer; i++) {
		// append the number of element in this layer. 
		if (i)		strcat(str, _T(","));
		len	= (int)strlen(str);
		_stprintf(str + len, _T("%d"), net->numElm[i]);
	}
}

// store editted structure into data area.
BOOL CNnStruct::StoreNetStructDlg(void)
{
	CString		str;
	_TCHAR		buf[256];
	_TCHAR		buf2[256];
	_TCHAR		errMsg[256];
	_TCHAR		remark[80];
	NNTemplate	patternNN, corrNN;
	int			boWatchTime;				// B.O watch time
//	int			numRegister;				// Number of Registration
//	float		boAlarm[NumBoOccur];		// Threshold
//	int			boTimes[NumBoOccur];		// Number of Times
//	int			numCombiBA;					// Number of combinations for BO Alarm.
//	int			numCombiBAC;				// Number of combinations for BO Alarm Control.
	int			numCount, j;

	// get editted remark.
	m_editRemark.GetWindowText(remark, 80);

	// get editted pattern neural net structure string.
	m_editPat.GetWindowText(buf, 256);

	if(buf[0] != _T('\0'))
	{
		_stprintf(buf,_T("%s,1"),buf);
		// parse pattern neural net structure string.
		str.LoadString(IDS_NN_STRUCT05);
		if (ParseNetStructString(&patternNN, buf, errMsg, str,-1) == FALSE)
		{
			// if invalid definition, show message.
			AfxMessageBox(errMsg, MB_ICONEXCLAMATION | MB_OK);
			return FALSE;
		}
	}
	else
		ZeroMemory(&patternNN,sizeof(patternNN));

	// get editted correlation neural net structure string.
	m_editCor.GetWindowText(buf, 256);

	if(buf[0] != _T('\0'))
	{
		_stprintf(buf2,_T("2,%s,1"),buf);
		// parse correlation neural net structure string.
		str.LoadString(IDS_NN_STRUCT06);
		if (ParseNetStructString(&corrNN, buf2, errMsg, str, 2) == FALSE)
		{
			// if invalid definition, show message. 
			AfxMessageBox(errMsg, MB_ICONEXCLAMATION | MB_OK);
			return FALSE;
		}
	}
	else
		ZeroMemory(&corrNN,sizeof(corrNN));

	if(patternNN.numLayer == 0 && patternNN.numLayer == 0)
		ZeroMemory(remark,sizeof(remark));

	//@
	// check B.O watch time
	m_editWatchTime.GetWindowText(buf, 32);
	if(_stscanf(buf, _T("%d"), &boWatchTime) != 1 ||
		boWatchTime < 300 || boWatchTime > 100000)
	{
		m_editWatchTime.SetWindowText(_T(""));
		AfxMessageBox(IDS_BO_SYS06, MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}
	_stprintf(buf, _T("%d"), boWatchTime);
	m_editWatchTime.SetWindowText(buf);
    
	// check Relations between sampling period and the BO watch time
	if(boWatchTime % 500 != 0)
	{
		AfxMessageBox(IDS_BO_SYS07, MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

	// check B.O Occurrence recognition
	numCount = (int)(boWatchTime / 500);

	if(m_stNetConf.numRegister <= 0)
	{
		AfxMessageBox(IDS_BO_SYS08, MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

	for(j = 0;j < m_stNetConf.numRegister;j++)
	{
		if(m_stNetConf.boTimes[j] > numCount)
		{
			AfxMessageBox(IDS_BO_SYS09, MB_ICONEXCLAMATION | MB_OK);
			return FALSE;
		}
	}

	//// check Number of times for BO Alarm.
	//m_editAlmCombi.GetWindowText(buf, 32);
	//if(_stscanf(buf, _T("%d"), &numCombiBA) != 1)
	//{
	//	m_editAlmCombi.SetWindowText(_T(""));
	//	AfxMessageBox(IDS_BO_SYS10, MB_ICONEXCLAMATION | MB_OK);
	//	return FALSE;
	//}
	//if(numCombiBA < 1)
	//{
	//	m_editAlmCombi.SetWindowText(_T(""));
	//	AfxMessageBox(IDS_BO_SYS10, MB_ICONEXCLAMATION | MB_OK);
	//	return FALSE;
	//}
	//_stprintf(buf, _T("%d"), numCombiBA);
	//m_editAlmCombi.SetWindowText(buf);

	//// check Number of times for BO Alarm Ctl.
	//m_editAlmCtlCombi.GetWindowText(buf, 32);
	//if(_stscanf(buf, _T("%d"), &numCombiBAC) != 1)
	//{
	//	m_editAlmCtlCombi.SetWindowText(_T(""));
	//	AfxMessageBox(IDS_BO_SYS10, MB_ICONEXCLAMATION | MB_OK);
	//	return FALSE;
	//}
	//if(numCombiBAC < 1)
	//{
	//	m_editAlmCtlCombi.SetWindowText(_T(""));
	//	AfxMessageBox(IDS_BO_SYS10, MB_ICONEXCLAMATION | MB_OK);
	//	return FALSE;
	//}
	//_stprintf(buf, _T("%d"), numCombiBAC);
	//m_editAlmCtlCombi.SetWindowText(buf);

	// store valid editted structure into data area.
	_tcscpy(m_stNetConf.remark, remark);
	m_stNetConf.patternNN		= patternNN;
	m_stNetConf.corrNN			= corrNN;
	m_stNetConf.boWatchTime		= boWatchTime;
	//m_stNetConf.numCombiBA		= numCombiBA;
	//m_stNetConf.numCombiBAC		= numCombiBAC;
	memcpy(&neuralnetConf[m_nAsnNet], &m_stNetConf, sizeof(m_stNetConf));

	return TRUE;
}

// parse pattern neural net structure string.
int	CNnStruct::ParseNetStructString(NNTemplate	*net, LPTSTR str, LPTSTR err, LPCTSTR name, int numInput)
{
	CString		s;
	_TCHAR		*p, *q;
	int			i,startP,endP;
	NNTemplate	dummy = {0};

	// The character which corresponds to space and tab in the character line is erased. 
	for(p = q = str; *p; p++)
	{
		// In the case of numerical value or comma,
		if( ( _T('0') <= *p && *p <= _T('9') ) || *p == _T(',') )
		{
			// In the case of comma, 
			if(*p == _T(','))
				// the number of layers is increased
				dummy.numLayer++;

			// A character is reduced.
			*q++ = *p;
		}
		// if it is not space and tab, 
		else if( *p != _T(' ') && *p != _T('\t') )
		{
			// set up illegal network structure.
			s.LoadString(IDS_NN_STRUCT00);
			_stprintf(err,s,name);
			return	FALSE;
		}
	}
	*q = '\0';
	dummy.numLayer++;

	// test the number of layer and if error detected,
	if (dummy.numLayer <= 2 || dummy.numLayer >= NumLayer) 
	{
		// set up illegal network structure.
		s.LoadString(IDS_NN_STRUCT01);
		_stprintf( err, s, name);
		return	FALSE;
	}

	// 各層の要素数を取得しｴﾗｰﾁｪｯｸを行う.
	for(i = 0, startP = 0, endP = 0,p = str; *p; p++, endP++)
	{
		// In the case of comma,
		if( *p == _T(',') )
		{
			// A character line is changed into the numerical value.
			dummy.numElm[i] = _tstoi(&str[startP]);
			startP = endP + 1;
			// test the number of the elements of each layer and if error detected,
			if (dummy.numElm[i] >= NumElement || dummy.numElm[i] == 0) 
			{
				// set up illegal network structure.
				s.LoadString(IDS_NN_STRUCT02);
				_stprintf(err, s, name, NumElement);
				return	FALSE;
			}
			i++;
		}
	}
	// A last character line is changed into the numerical value. */
	dummy.numElm[i] = _tstoi(&str[startP]);

	// test the number of input and if error fetched, 
	if (numInput > 0 && dummy.numElm[0] != numInput) {
		// set up number of input error. 
		s.LoadString(IDS_NN_STRUCT03);
		_stprintf( err, s, name, numInput);
		return	FALSE;
	}

	// test the number of output and if error fetched, */
	if (dummy.numElm[dummy.numLayer - 1] != 1) {
		// set up number of output error. 
		s.LoadString(IDS_NN_STRUCT04);
		_stprintf( err, s, name);
		return	FALSE;
	}

	// Establishment value is adopted. 
	CopyMemory( net, &dummy, sizeof(dummy));

	// valid structure is specified, return OK. 
	return	TRUE;
}

// check registration.
void CNnStruct::CheckRegistration(void)
{
	CString	str;
	_TCHAR	buf[64];
	int		i;
	float	numBoAlarm;
	int		numTimes, boWatchTime;

	// check Registration number
	if(m_stNetConf.numRegister >= NumBoOccur)
	{
		AfxMessageBox(IDS_BO_SYS11, MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	// check threshold
	m_editThreshold.GetWindowText(buf, 64);
	if(_stscanf(buf, _T("%f"), &numBoAlarm) != 1)
	{
		m_editThreshold.SetWindowText(_T(""));
		AfxMessageBox(IDS_BO_SYS12, MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	if(numBoAlarm < 0.0 || numBoAlarm > 1.0)
	{
		m_editThreshold.SetWindowText(_T(""));
		AfxMessageBox(IDS_BO_SYS12, MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	_stprintf(buf, _T("%.2f"), numBoAlarm);
	m_editThreshold.SetWindowText(buf);

	// check Un-registration
	for(i = 0;i < m_stNetConf.numRegister;i++)
	{
		if(numBoAlarm == m_stNetConf.boAlarm[i])
		{
			AfxMessageBox(IDS_BO_SYS13, MB_ICONEXCLAMATION | MB_OK);
			return;
		}
	}

	// check Number of times
	m_editNumTimes.GetWindowText(buf, 64);
	if(_stscanf(buf, _T("%d"), &numTimes) != 1 ||
		numTimes <= 0)
	{
		m_editNumTimes.SetWindowText(_T(""));
		AfxMessageBox(IDS_BO_SYS14, MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	_stprintf(buf, _T("%d"), numTimes);
	m_editNumTimes.SetWindowText(buf);
	
	// check B.O watch time
	m_editWatchTime.GetWindowText(buf, 32);
	if(_stscanf(buf, _T("%d"), &boWatchTime) != 1 ||
		boWatchTime < 300 || boWatchTime > 100000)
	{
		m_editWatchTime.SetWindowText(_T(""));
		AfxMessageBox(IDS_BO_SYS06, MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	_stprintf(buf, _T("%d"), boWatchTime);
	m_editWatchTime.SetWindowText(buf);

	if((int)(boWatchTime / 500) < numTimes)
	{
		AfxMessageBox(IDS_BO_SYS09, MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	// Adoption
	m_stNetConf.boAlarm[m_stNetConf.numRegister] = numBoAlarm;
	m_stNetConf.boTimes[m_stNetConf.numRegister] = numTimes;

	str.LoadString(IDS_BO_SYS04);
	_stprintf(buf, str, m_stNetConf.boAlarm[i], m_stNetConf.boTimes[i]);
	m_listBoRec.InsertString(-1, buf);

	m_stNetConf.numRegister++;
}

// delete operation.
void CNnStruct::DeleteBoRecognition(void)
{
	int		i, sel;
	float	numBoAlarm;
	_TCHAR	buf[64];
	_TCHAR	*p;

	// If an item isn't chosen,
	if((sel = m_listBoRec.GetCurSel()) == -1)
		return;

	// An item is acquired.
	m_listBoRec.GetText(sel, buf);

	// An item is removed.
	m_listBoRec.DeleteString(sel);

	// threshold is acquired from the character line.
	p = _tcsstr(buf, _T("."));
	_stscanf(--p, _T("%f"), &numBoAlarm);

	// The position where value is registered is looked up.
	for(sel = 0;sel < m_stNetConf.numRegister;sel++)
		if(m_stNetConf.boAlarm[sel] == numBoAlarm)
			break;
	
	// Inside data are removed..
	for(i = sel;i < m_stNetConf.numRegister - 1;i++)
	{
		m_stNetConf.boAlarm[i] = m_stNetConf.boAlarm[i+1];
		m_stNetConf.boTimes[i] = m_stNetConf.boTimes[i+1];
	}
	m_stNetConf.boAlarm[i] = 0;
	m_stNetConf.boTimes[i] = 0;
	m_stNetConf.numRegister--;
}

