#pragma once


// CCloseDlg �_�C�A���O

class CCloseDlg : public CDialog
{
	DECLARE_DYNAMIC(CCloseDlg)

public:
	CCloseDlg(CDialog* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CCloseDlg();

protected:
	CDialog		*m_pParent;

public:
// �_�C�A���O �f�[�^
	enum { IDD = IDD_CLOSE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual	void PostNcDestroy();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnBnClickedButton1();
};
