#pragma once
#include "afxwin.h"


// CNnStruct ダイアログ

class CNnStruct : public CDialog
{
	DECLARE_DYNAMIC(CNnStruct)

public:
	CNnStruct(int nAnsNet, CWnd* pParent = NULL); 
	virtual ~CNnStruct();

// member data
public:
	int				m_nAsnNet;
	NeuralNetConf	m_stNetConf;	// neural network set

// member function
public:
	void SetupNetStructDlg(void);
	void MakeupNetStructString(NNTemplate *net, LPTSTR str);
	BOOL StoreNetStructDlg(void);
	int  ParseNetStructString(NNTemplate *net, LPTSTR str, LPTSTR err, LPCTSTR name, int numInput);
	void CheckRegistration(void);
	void DeleteBoRecognition(void);

// dialog data
public:
	enum { IDD = IDD_NNSTRUCT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editRemark;
	CEdit m_editPat;
	CEdit m_editCor;
	CEdit m_editThreshold;
	CEdit m_editNumTimes;
	CEdit m_editWatchTime;
	CEdit m_editAlmCombi;
	CEdit m_editAlmCtlCombi;
	CListBox m_listBoRec;
	afx_msg void OnClickedOk();
	afx_msg void OnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
