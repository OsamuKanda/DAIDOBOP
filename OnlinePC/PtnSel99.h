#pragma once
#include "afxcmn.h"
#include "config.h"


// CPtnSel99 �_�C�A���O

class CPtnSel99 : public CDialog
{
	DECLARE_DYNAMIC(CPtnSel99)

public:
	CPtnSel99(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CPtnSel99();

	void SetParam(CConfig &config, int pno);
	void GetParam(int *pno);
	int GetCurSel(CListCtrl& list);

// �����o�ϐ�
public:
	int				m_nPtnNo;
	CImageList		m_image;					// �����𒲐�����ׂ̃C���[�W���X�g
		
public:
// �_�C�A���O �f�[�^
	enum { IDD = IDD_PTN_SEL99 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListCtrl m_listGC;
	virtual BOOL OnInitDialog();
};
