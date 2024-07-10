// TrendSel.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "TrendSel.h"


///////////////////////////////////////////////////////////////////////////////
// definition
#define	TempSpace		11
#define	LevelSpace		13
#define	VcSpace			10
#define	SNSpace			10
#define	WidthSpace		10
#define	LengthSpace		10

static TcName			*tcnm		= &theApp.m_stTcNm;


// CTrendSel dialog
IMPLEMENT_DYNAMIC(CTrendSel, CDialog)
CTrendSel::CTrendSel(CWnd* pParent /*=NULL*/)
	: CDialog(CTrendSel::IDD, pParent)
{
}

CTrendSel::~CTrendSel()
{
}

void CTrendSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listData);
}


BEGIN_MESSAGE_MAP(CTrendSel, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnClickedButton1)
	ON_BN_CLICKED(IDOK, OnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnClickedCancel)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CTrendSel message handler

// WM_INITDIALOG proc.
BOOL CTrendSel::OnInitDialog()
{
	CDialog::OnInitDialog();

	// setup trend select dialog.
	SetupTrendSelectDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
}

// clicked switching button.
void CTrendSel::OnClickedButton1()
{
	ChangeIndication();
}

void CTrendSel::OnClickedOk()
{
	OnOK();
}

void CTrendSel::OnClickedCancel()
{
	OnCancel();
}

///////////////////////////////////////////////////////////////////////////////
// CTrendSel message handler

// set trend select data.
void CTrendSel::SetTrendSelect(TrendSelect *data)
{
	memcpy(m_stTdSel, data, sizeof(m_stTdSel));
}

// get trend select data.
void CTrendSel::GetTrendSelect(TrendSelect *data)
{
	memcpy(data, m_stTdSel, sizeof(m_stTdSel));
}

// set up Trend indication switching listbox.
void CTrendSel::SetupTrendSelectDlg(void)
{
	_TCHAR	disp[64];
	int		i,j,ch,cnt;
	CString	str;

	m_listData.ResetContent();
	memset(m_argTdList, 0, sizeof(m_argTdList));
	cnt = 0;

	// for each data,
	for( i = 0; i < NumADChannel; i++)
	{
		ch = m_stTdSel[i].arg;
		if(ch >= 0 && ch < LastID-1)
		{
			switch(ch)
			{
			case LevelID:
				// An A/D channel is changed into the character line..
				str.LoadString(IDS_DEF_LEVEL);
				_stprintf(disp, str);
				for(j = lstrlen(disp) - 1;j < LevelSpace;j++)
					_tcscat(disp, _T(" "));
				break;
			case VcID:
				// An A/D channel is changed into the character line..
				str.LoadString(IDS_DEF_VC);
				_stprintf(disp, str);
				for(j = lstrlen(disp) - 1;j < VcSpace;j++)
					_tcscat(disp, _T(" "));
				break;
			case SNID:				// HOZAN
				// An A/D channel is changed into the character line..
				str.LoadString(IDS_DEF_SN);
				_stprintf(disp, str);
				for(j = lstrlen(disp) - 1;j < SNSpace;j++)
					_tcscat(disp, _T(" "));
				break;
			//case WidthID:
			//	// An A/D channel is changed into the character line..
			//	str.LoadString(IDS_DEF_WIDTH);
			//	_stprintf(disp, str);
			//	for(j = lstrlen(disp) - 1;j < WidthSpace;j++)
			//		_tcscat(disp, _T(" "));
			//	break;
			//case WidthLID:
			//	// An A/D channel is changed into the character line..
			//	str.LoadString(IDS_DEF_WIDTH);
			//	str += _T("L");
			//	_stprintf(disp, str);
			//	for(j = lstrlen(disp) - 1;j < WidthSpace;j++)
			//		_tcscat(disp, _T(" "));
			//	break;
			//case WidthRID:
			//	// An A/D channel is changed into the character line..
			//	str.LoadString(IDS_DEF_WIDTH);
			//	str += _T("R");
			//	_stprintf(disp, str);
			//	for(j = lstrlen(disp) - 1;j < WidthSpace;j++)
			//		_tcscat(disp, _T(" "));
			//	break;
			case LengthID:
				// An A/D channel is changed into the character line..
				str.LoadString(IDS_DEF_LENGTH);
				_stprintf(disp, str);
				for(j = lstrlen(disp) - 1;j < LengthSpace;j++)
					_tcscat(disp, _T(" "));
				break;
			default:
				// An A/D channel is changed into the character line..
				if( ch < NumTCTap )
					str.Format(_T("%s"), tcnm->name[ch]);
				else
					str = _T("");
				_stprintf(disp, str);
				for(j = lstrlen(disp) - 1;j < TempSpace;j++)
					_tcscat(disp, _T(" "));
				break;
			}

			if(m_stTdSel[i].fIndication == TRUE)
				str.LoadString(IDS_TD_SEL00);
			else
				str.LoadString(IDS_TD_SEL01);
			_tcscat(disp, str);

			// store the character line into listbox.
			m_listData.AddString(disp);
			m_argTdList[cnt++] = ch;
		}
	}
}

// Indication and non-indication are changed.
void CTrendSel::ChangeIndication(void)
{
	_TCHAR	disp[64];
	_TCHAR	*p;
	CString	strLevel, strVc, strSN, strWidth, strLength, strIndi, strNonIndi;
	int		i,n,arg;
	int		nNumSelItems;
	int		nBuffer[128];

	// If it isn't chosen.
	if(m_listData.GetCurSel() == -1)
		return;

	// The index of the chosen item and the number of the items are acquired.
	nNumSelItems = m_listData.GetSelItems(128, nBuffer);

	// for each  the chosen  items
    for (n = 0 ; n < nNumSelItems; n++) 
	{
		// An item is acquired.
		m_listData.GetText(nBuffer[n], disp);

		// A TC taps number is acquired more than the item.
//		strLevel.LoadString(IDS_DEF_LEVEL);
//		strVc.LoadString(IDS_DEF_VC);
//		strSN.LoadString(IDS_DEF_SN);
//		strWidth.LoadString(IDS_DEF_WIDTH);
//		strLength.LoadString(IDS_DEF_LENGTH);
//		if(NULL != _tcsstr(disp, strLevel))
//			arg = LevelID;
//		else if(NULL != _tcsstr(disp, strVc))
//			arg = VcID;
//		else if(NULL != _tcsstr(disp, strSN))
//			arg = SNID;
//		else if(NULL != _tcsstr(disp, strWidth))
//			arg = WidthID;
//		else if(NULL != _tcsstr(disp, strLength))
//			arg = LengthID;
//		else
//			arg = GetTcNo(disp);
		arg = m_argTdList[nBuffer[n]];

		// The indication condition of chosen TC Taps is reversed.
		for(i = 0;i < NumADChannel;i++)
		{
			if(m_stTdSel[i].arg == arg)
			{
				m_stTdSel[i].fIndication = !m_stTdSel[i].fIndication;
				break;
			}
		}
		
		// A choice item is removed from the list box.
		m_listData.DeleteString(nBuffer[n]);

		// A new indication item is made.
		p = _tcsstr(disp, _T("[") );
		*p = _T('\0');
		strIndi.LoadString(IDS_TD_SEL00);
		strNonIndi.LoadString(IDS_TD_SEL01);
		if( m_stTdSel[i].fIndication == TRUE )
			_tcscat(disp, strIndi);
		else
 			_tcscat(disp, strNonIndi);

		// A new item is inserted.
		m_listData.InsertString(nBuffer[n], disp);
	}
}
