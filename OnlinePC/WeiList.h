#pragma once
#include "afxcmn.h"
#include "Config.h"

// CWeiList �_�C�A���O
class CWeiList : public CDialog
{
	DECLARE_DYNAMIC(CWeiList)

public:
	CWeiList(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CWeiList();

	// �ϐ�
	CString		m_szConfPath;
	int			m_nNetIdx;
	WeiInfoList	m_stWeiInfoP[WEI_BAK_CNT];
	WeiInfoList	m_stWeiInfoC[WEI_BAK_CNT];
	int			m_nWeiCntP;
	int			m_nWeiCntC;
	CString		m_szSavePathP;
	CString		m_szSavePathC;

	// �����o�[
	void DispWeiList(void);
	LPCTSTR GetTimeStr(time_t tm);
	int GetLarningTimes(LPTSTR path);
	int GetListCurSel(CListCtrl &list);

// �_�C�A���O �f�[�^
public:
	enum { IDD = IDD_RECOVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedWeiSel();
	afx_msg void OnBnClickedWeiDel();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CListCtrl m_list;
};
