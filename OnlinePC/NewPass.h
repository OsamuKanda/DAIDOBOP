#pragma once
#include "afxwin.h"


// CNewPass �_�C�A���O

class CNewPass : public CDialog
{
	DECLARE_DYNAMIC(CNewPass)

public:
	CNewPass(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CNewPass();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_NEW_PASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnOK();
public:
	CEdit m_editPass;
	CEdit m_editNewPass;
	CEdit m_editNewPassAgain;
};
