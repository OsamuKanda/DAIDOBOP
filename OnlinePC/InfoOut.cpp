// InfoOut.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "OnlinePCDlg.h"
#include "InfoOut.h"
#include ".\infoout.h"

#define	DISP_CYCLE		2	//sec


// CInfoOut ダイアログ

IMPLEMENT_DYNAMIC(CInfoOut, CDialog)
CInfoOut::CInfoOut(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoOut::IDD, pParent)
{
	m_bReset = FALSE;
	m_nDispCnt = 0;
	memset(m_out, 0, sizeof(m_out));

	// create dialog.
	Create(CInfoOut::IDD, pParent);				
}

CInfoOut::~CInfoOut()
{
}

void CInfoOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFOOUT, m_listInfoOut);
}


BEGIN_MESSAGE_MAP(CInfoOut, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_MESSAGE(WM_INFODATA, OnAddone)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CInfoOut message handler

//It evades ending by Enter Key.
void CInfoOut::OnOk(){}

//It evades ending by ESC Key.
void CInfoOut::OnCancel()
{
	ShowWindow(SW_HIDE);
}

void CInfoOut::PostNcDestroy() 
{
	delete this;
//	CDialog::PostNcDestroy();
}

BOOL CInfoOut::OnInitDialog()
{
	CDialog::OnInitDialog();

	// list setting
	m_listInfoOut.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_listInfoOut.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_listInfoOut.DeleteAllItems();
	m_listInfoOut.InsertColumn(0, _T(""), LVCFMT_LEFT, 0);
	m_listInfoOut.InsertColumn(1, _T("No."), LVCFMT_CENTER, 50);
	m_listInfoOut.InsertColumn(2, _T("Information"), LVCFMT_LEFT, 395);
	m_listInfoOut.InsertColumn(3, _T("ON/OFF"), LVCFMT_CENTER, 80);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CInfoOut::OnBnClickedOk()
{
	m_bReset = TRUE;
}

void CInfoOut::OnBnClickedCancel()
{
	ShowWindow(SW_HIDE);
}

LRESULT CInfoOut::OnAddone(WPARAM wParam, LPARAM lParam) 
{
	StInfoItem		*pInfo = (StInfoItem *)wParam;
	StInfoResult	*pResult = (StInfoResult *)lParam;
	CString			msg;
	BOOL			bCheck;
	int				i;

	// reset request
	if( m_bReset )
	{
		memset(m_out, 0, sizeof(m_out));
		m_bReset = FALSE;
	}

	// get result information.
	bCheck = FALSE;
	for( i = 0; i < INFO_ITEM_MAX; i++ )
	{
		if( pResult[i].bCondition )
		{
			// message out
			if( m_out[i].nCondition != 2 )
			{
				msg.Format(_T("%s is ON"), pInfo[i].cInfo);
				SendInfoMessage(msg);
			}
			m_out[i].nCondition = 2;
			bCheck = TRUE;
			m_out[i].nNo		= i+1;
			_tcsncpy(m_out[i].cInfo, pInfo[i].cInfo, INFO_INFO_MAX);
		}
		else if( m_out[i].nCondition == 2 && pResult[i].bCondition == FALSE )
		{
			// message out
			msg.Format(_T("%s is OFF"), pInfo[i].cInfo);
			SendInfoMessage(msg);
			m_out[i].nCondition = 1;
		}
	}

	// update list
	if( ++m_nDispCnt >= (DISP_CYCLE * 2) )
	{
//		if( bCheck )
//		{
//			ShowWindow(SW_SHOW);
//			SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
//		}
		SetupList();
		m_nDispCnt = 0;
	}

	return 0;
}

void CInfoOut::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if( bShow )
	{
		SetupList();
	}
}

///////////////////////////////////////////////////////////////////////////////
// member function

// setup list
void CInfoOut::SetupList(void)
{
	CString	strVal;
	int		i, cnt;

	m_listInfoOut.SetRedraw(FALSE);
	m_listInfoOut.DeleteAllItems();
	for( i = 0, cnt = 0; i < INFO_ITEM_MAX; i++ )
	{
		if( m_out[i].nCondition == 2 )
		{
			m_listInfoOut.InsertItem(cnt, _T(""));
			strVal.Format(_T("%02d"), m_out[i].nNo );
			m_listInfoOut.SetItemText(cnt, 1, strVal);
			strVal = m_out[i].cInfo;;
			m_listInfoOut.SetItemText(cnt, 2, strVal);
			m_listInfoOut.SetItemText(cnt, 3, _T("ON"));
			cnt++;
		}
		else if( m_out[i].nCondition == 1 )
		{
			m_listInfoOut.InsertItem(cnt, _T(""));
			strVal.Format(_T("%02d"), m_out[i].nNo );
			m_listInfoOut.SetItemText(cnt, 1, strVal);
			strVal = m_out[i].cInfo;;
			m_listInfoOut.SetItemText(cnt, 2, strVal);
			m_listInfoOut.SetItemText(cnt, 3, _T("OFF"));
			cnt++;
		}
	}
	m_listInfoOut.SetRedraw(TRUE);
}

//// send information message
//void CInfoOut::SendInfoMessage(LPCTSTR pStr)
//{
//	COnlinePCDlg *dlg = (COnlinePCDlg *)AfxGetMainWnd();
//	if( dlg->m_pDlgPredic != NULL ){
//		dlg->m_pDlgPredic->SendMessage(WM_MESSAGE_OUT, (WPARAM)0, (LPARAM)pStr);
//	}
//}
//
//
