#pragma once

#include "../include/ChartLib.h"


#define	WM_TCLOAD	(WM_APP + 0)
#define	WM_TCREDRAW	(WM_APP + 1)
#define	WM_TCRELOAD	(WM_APP + 2)

//#define	TC_NO_MAX	32
//#define	TC_NO_MAX	6
#define	TC_NO_MAX	12

// CTcGraph
class CTcGraph : public CWnd
{
	DECLARE_DYNAMIC(CTcGraph)

// 
public:
	CTcGraph(int no);
	virtual ~CTcGraph();

// member data
public:
	CString			m_szFilePath;
	StGraphParam	m_gp;
	SampledData		m_data;
	CChartLib		*m_pLib;
	CChartLib		*m_pLib2;
	BOOL			m_bAlarm;

// member function
public:
	void InitLib(int no);
	void TcLoad(LPCTSTR name, StGraphParam *gp);
	void AppendData(SampledData *data);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg LRESULT OnTcLoad(WPARAM, LPARAM); 
	afx_msg LRESULT OnAppendData(WPARAM, LPARAM);
};


