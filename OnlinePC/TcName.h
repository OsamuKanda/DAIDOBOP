#pragma once
#include "../Include/Bodef.h"
#include "afxwin.h"

// CTcName �_�C�A���O

class CTcName : public CDialog
{
	DECLARE_DYNAMIC(CTcName)

public:
	CTcName(int nAnsArg, CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CTcName();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_TC_NAME };

// member function
public:
	int		m_nAsnArg;
	TcName	m_stTcNm;

	_TCHAR	m_cRootDir[_MAX_PATH];

public:
	BOOL ReadTcNameFile(TcName *tcnm);
	BOOL WriteTcNameFile(TcName *tcnm);
	void SetDefaultTCName(TcName *tcnm);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_editTcName;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
