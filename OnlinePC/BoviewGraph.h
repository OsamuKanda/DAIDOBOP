#pragma once

#include "../include/ChartLib.h"


#define	WM_BOVIEWLOAD	(WM_APP + 0)
#define	WM_BOVIEWREDRAW	(WM_APP + 1)
#define	WM_BOVIEWRELOAD	(WM_APP + 2)

#define	SAVE_DATA_NUM		(60*60*2)
#define	FRZ_DATA_NUM		(1280)
#define	BOVIEW_GRAPH_NUM	5

// CBoviewGraph
class CBoviewGraph : public CWnd
{
	DECLARE_DYNAMIC(CBoviewGraph)

// 
public:
	CBoviewGraph();
	virtual ~CBoviewGraph();

// member data
public:
	CString			m_szFilePath;
	StGraphParam	m_gp;
	SampledData		m_data[SAVE_DATA_NUM];
	SampledData		m_fdat[FRZ_DATA_NUM];
	int				m_nCurIdx;
	int				m_nCount;
	int				m_nCountFrz;
	int				m_nScX;
	CDC				m_mdc[BOVIEW_GRAPH_NUM];
	CBitmap			m_bitmap[BOVIEW_GRAPH_NUM];
	int				m_cur;
	BOOL			m_bFirst;

// member function
public:
	void BoviewLoad(LPCTSTR name, StGraphParam *gp);
	void BoviewReload(LPCTSTR name, StGraphParam *gp);
	void AppendData(SampledData *data);
	void ChangeView(int no);
	void SetScooterX(int x);
	void DrawScooterLine(CDC &dc);
	BOOL GetScooterVal(SampledData *pDat);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg LRESULT OnTrendLoad(WPARAM, LPARAM); 
	afx_msg LRESULT OnAppendData(WPARAM, LPARAM);
	afx_msg LRESULT OnTrendReload(WPARAM, LPARAM); 
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


