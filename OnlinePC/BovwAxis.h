#pragma once
#include "afxwin.h"


// CBovwAxis �_�C�A���O

class CBovwAxis : public CDialog
{
	DECLARE_DYNAMIC(CBovwAxis)

public:
	CBovwAxis(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CBovwAxis();

private:
	void SetupTrendAxisDlg(void);
	BOOL UpdateAxisSetting(void);

public:
	void SetAxis(ChartAxis &axis);
	void GetAxis(ChartAxis &axis);

private:
	ChartAxis	m_stTempAxis;

public:
// dialog data.
	enum { IDD = IDD_BOVIEW_AXIS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editTempScale;
	CEdit m_editTempZero;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};

