#pragma once
#include "afxwin.h"


// CNetSel dialog
class CNetSel : public CDialog
{
	DECLARE_DYNAMIC(CNetSel)

public:
	CNetSel(CWnd* pParent = NULL);
	virtual ~CNetSel();

// member data
public:
	int		m_nNeuralnetTab[NumNeuralNet];
	int		m_nNeuralnetIdx;
	int		m_nCombinationTab[NumCombination];
	int		m_nCombinationIdx;

// member function
public:
	void SetupNetworkList(void);
	void SetupCombinationList(int sel);

// dialog data
public:
	enum { IDD = IDD_NET_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbNet;
	CComboBox m_cmbCombi;
	afx_msg void OnClickedOk();
	afx_msg void OnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	CEdit m_editNeuralNet;
};
