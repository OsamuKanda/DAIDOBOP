// NetCalc.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "OnlinePC.h"
#include "NetCalc.h"

#define	WM_CALC_UPD		(WM_APP+0)
#define	WM_CALC_END		(WM_APP+1)

// CNetCalc dialog

IMPLEMENT_DYNAMIC(CNetCalc, CDialog)
CNetCalc::CNetCalc(CWnd* pParent /*=NULL*/)
	: CDialog(CNetCalc::IDD, pParent)
	, m_nPer(0)
	, m_nPos(0)
	, m_nMax(100)
	, m_nMin(0)
{
}

CNetCalc::~CNetCalc()
{
}

void CNetCalc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SIM_PER, m_nPer);
}


BEGIN_MESSAGE_MAP(CNetCalc, CDialog)	
	ON_MESSAGE(WM_CALC_UPD, OnCalcUpdata)
	ON_MESSAGE(WM_CALC_END, OnCalcEnd)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// CNetCalc message handler

// clicked ok button.
void CNetCalc::OnOK()
{
	CDialog::OnOK();
}

// clicked cancel button.
void CNetCalc::OnCancel()
{
	CDialog::OnCancel();
}

LRESULT CNetCalc::OnCalcUpdata(WPARAM, LPARAM)
{
	UpdateData(FALSE);
	return 0;
}

LRESULT CNetCalc::OnCalcEnd(WPARAM, LPARAM)
{
	EndDialog(IDOK);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// CNetCalc member function

void CNetCalc::SetRange(int min, int max)
{
    if( (max - min) == 0 ) 
		return;

	m_nMin = min;
	m_nMax = max;
	m_nPer = (int)((double)(m_nPos - m_nMin) / (m_nMax - m_nMin) * 100.0);
}

void CNetCalc::SetPos(int pos)
{
	if( pos < m_nMin || pos > m_nMax )
		return;

	m_nPos = pos;
	m_nPer = (int)((double)(pos - m_nMin) / (m_nMax - m_nMin) * 100.0);
}

void CNetCalc::Notify(void)
{
	SendMessage(WM_CALC_UPD);
}

void CNetCalc::End(void)
{
	SendMessage(WM_CALC_END);
}

