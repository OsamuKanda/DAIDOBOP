// CTrendView.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "TrendView.h"


///////////////////////////////////////////////////////////////////////////////
// definition

// CTrendView dialog
IMPLEMENT_DYNAMIC(CTrendView, CDialog)
CTrendView::CTrendView(CWnd* pParent /*=NULL*/)
	: CDialog(CTrendView::IDD, pParent)
{
}

CTrendView::~CTrendView()
{
}

void CTrendView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listData);
}


BEGIN_MESSAGE_MAP(CTrendView, CDialog)
	ON_BN_CLICKED(IDOK, OnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnClickedCancel)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CTrendView message handler

// WM_INITDIALOG proc.
BOOL CTrendView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// setup trend view dialog.
	SetupTrendViewDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CTrendView::OnClickedOk()
{
	// store trend view dialog.
	StoreTrendViewDlg();

	OnOK();
}

void CTrendView::OnClickedCancel()
{
	OnCancel();
}

///////////////////////////////////////////////////////////////////////////////
// CCTrendView message handler

// set trend select data.
void CTrendView::SetTrendSelect(TrendSelect *data)
{
	memcpy(m_stTdSel, data, sizeof(m_stTdSel));
}

// get trend select data.
void CTrendView::GetTrendSelect(TrendSelect *data)
{
	memcpy(data, m_stTdSel, sizeof(m_stTdSel));
}

// set up Trend indication switching listbox.
void CTrendView::SetupTrendViewDlg(void)
{
	_TCHAR	disp[64];
	int		i,ch;
	int		nNumSelItems;
	int		nBuffer[128];
	CString	str;

	m_listData.ResetContent();

	// for each A/D Channel,
	for(nNumSelItems = 0,i = 0;i < LastID-1; i++)
	{
		if(m_stTdSel[i].fIndication)
			nBuffer[nNumSelItems++] = m_stTdSel[i].arg;
		ch = m_stTdSel[i].arg;
		if(ch >= 0)
		{
			switch(ch)
			{
			case LevelID:
				// An A/D channel is changed into the character line..
				str.LoadString(IDS_DEF_LEVEL);
				_stprintf(disp, str);
				break;
			case VcID:
				// An A/D channel is changed into the character line..
				str.LoadString(IDS_DEF_VC);
				_stprintf(disp, str);
				break;
			case SNID:				// HOZAN
				// An A/D channel is changed into the character line..
				str.LoadString(IDS_DEF_SN);
				_stprintf(disp, str);
				break;
			//case WidthID:
			//	// An A/D channel is changed into the character line..
			//	str.LoadString(IDS_DEF_WIDTH);
			//	_stprintf(disp, str);
			//	break;
			//case WidthLID:
			//	// An A/D channel is changed into the character line..
			//	str.LoadString(IDS_DEF_WIDTH);
			//	str += _T("L");
			//	_stprintf(disp, str);
			//	break;
			//case WidthRID:
			//	// An A/D channel is changed into the character line..
			//	str.LoadString(IDS_DEF_WIDTH);
			//	str += _T("R");
			//	_stprintf(disp, str);
			//	break;
			case LengthID:
				// An A/D channel is changed into the character line..
				str.LoadString(IDS_DEF_LENGTH);
				_stprintf(disp, str);
				break;
			default:
				// An A/D channel is changed into the character line..
				str.Format(_T("#%2d"), ch + 1);
				_stprintf(disp, str);
				break;
			}

			// store the character line into listbox.
			m_listData.AddString(disp);
		}
	}
	for(i = 0;i < nNumSelItems;i++)
	{
		switch(nBuffer[i])
		{
			case LevelID:
				// An A/D channel is changed into the character line..
				str.LoadString(IDS_DEF_LEVEL);
				_stprintf(disp, str);
				break;
			case VcID:
				// An A/D channel is changed into the character line..
				str.LoadString(IDS_DEF_VC);
				_stprintf(disp, str);
				break;
			case SNID:				// HOZAN
				// An A/D channel is changed into the character line..
				str.LoadString(IDS_DEF_SN);
				_stprintf(disp, str);
				break;
			//case WidthID:
			//	// An A/D channel is changed into the character line..
			//	str.LoadString(IDS_DEF_WIDTH);
			//	_stprintf(disp, str);
			//	break;
			//case WidthLID:
			//	// An A/D channel is changed into the character line..
			//	str.LoadString(IDS_DEF_WIDTH);
			//	str += _T("L");
			//	_stprintf(disp, str);
			//	break;
			//case WidthRID:
			//	// An A/D channel is changed into the character line..
			//	str.LoadString(IDS_DEF_WIDTH);
			//	str += _T("R");
			//	_stprintf(disp, str);
			//	break;
			case LengthID:
				// An A/D channel is changed into the character line..
				str.LoadString(IDS_DEF_LENGTH);
				_stprintf(disp, str);
				break;
			default:
				// An A/D channel is changed into the character line..
				str.Format(_T("#%2d"), nBuffer[i] + 1);
				_stprintf(disp, str);
				break;
		}
		m_listData.SetSel(m_listData.FindString(-1, disp), TRUE);
	}

}

// set up Trend indication switching listbox.
void CTrendView::StoreTrendViewDlg(void)
{
	_TCHAR	disp[64];
	CString	strLevel, strVc, strSN, strWidth, strLength, strIndi, strNonIndi;
	int		i,n,arg;
	int		nNumSelItems;
	int		nBuffer[128];

	for(i = 0;i < NumADChannel;i++)
		m_stTdSel[i].fIndication = FALSE;

	// The index of the chosen item and the number of the items are acquired.
	nNumSelItems = m_listData.GetSelItems(128, nBuffer);

	// for each  the chosen  items
    for (n = 0 ; n < nNumSelItems; n++) 
	{
		// An item is acquired.
		m_listData.GetText(nBuffer[n], disp);

		// A taps number is acquired more than the item.
		strLevel.LoadString(IDS_DEF_LEVEL);
		strVc.LoadString(IDS_DEF_VC);
		strSN.LoadString(IDS_DEF_SN);
//		strWidth.LoadString(IDS_DEF_WIDTH);
		strLength.LoadString(IDS_DEF_LENGTH);
		if(NULL != _tcsstr(disp, strLevel))
			arg = LevelID;
		else if(NULL != _tcsstr(disp, strVc))
			arg = VcID;
		else if(NULL != _tcsstr(disp, strSN))
			arg = SNID;
//		else if(NULL != _tcsstr(disp, strWidth))
//			arg = WidthID;
		else if(NULL != _tcsstr(disp, strLength))
			arg = LengthID;
		else
			arg = _tstoi(&disp[1]) - 1;

		// The indication condition of chosen TC Taps is reversed.
		for(i = 0;i < NumADChannel;i++)
		{
			if(m_stTdSel[i].arg == arg)
			{
				m_stTdSel[i].fIndication = TRUE;
				break;
			}
		}
	}
}
