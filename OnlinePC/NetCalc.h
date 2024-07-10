#pragma once


// CNetCalc dialog

class CNetCalc : public CDialog
{
	DECLARE_DYNAMIC(CNetCalc)

public:
	CNetCalc(CWnd* pParent = NULL); 
	virtual ~CNetCalc();

// member data
public:
	int		m_nPos;
	int		m_nMax;
	int		m_nMin;

// member data
public:
	void SetRange(int min, int max);
	void SetPos(int pos);
	void Notify(void);
	void End(void);

// dialog data
public:
	enum { IDD = IDD_SIM_EXE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg LRESULT OnCalcUpdata(WPARAM, LPARAM);
	afx_msg LRESULT OnCalcEnd(WPARAM, LPARAM);
public:
	int m_nPer;
};

