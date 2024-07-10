#pragma once
#include "afxwin.h"
#include "../lib/ColorComboBox.h"

#define	PEN_NUM			5		// pen number
#define	PEN_COMBI_NUM	12		// combi number
#define	PEN_SET_NUM		7		// set pen color number
#define	PEN_COL_NUM		12		// color number

typedef struct
{
	BOOL		bEnbPen[PEN_NUM];
	int			nCombiNo;
	COLORREF	colPen[PEN_SET_NUM];
	double		dMax[PEN_NUM];
	double		dMin[PEN_NUM];
	double		dUnit[PEN_NUM];
	_TCHAR		szFmt[PEN_NUM][32];
}StPenSetInfo;

// CPenSetting dialog
class CPenSetting : public CDialog
{
	DECLARE_DYNAMIC(CPenSetting)

public:
	CPenSetting(CWnd* pParent = NULL);   // standard constract
	virtual ~CPenSetting();

// member data
public:
	StPenSetInfo	m_stInfo;

// member function
public:
	void SetParam(StPenSetInfo *pInfo);
	void GetParam(StPenSetInfo *pInfo);

// dialog data
	enum { IDD = IDD_PEN_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CColorComboBox	m_cmbCol[PEN_SET_NUM];
	CComboBox		m_cmbCombi;
	CEdit			m_editMax[PEN_NUM];
	CEdit			m_editMin[PEN_NUM];
	CButton			m_btnPen[PEN_NUM];
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedOk();
};
