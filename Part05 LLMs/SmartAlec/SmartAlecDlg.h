#include "LlamaCPPWrapper.h"
using namespace std;

// SentimentAnalyzerDlg.h : header file
//

#pragma once


// CSentimentAnalyzerDlg dialog
class CSmartAlecDlg : public CDialogEx
{
// Construction
public:
	CSmartAlecDlg(CWnd* pParent = nullptr);	// standard constructor
//	CWnd* lblCpu;
//	CWnd* lblMem;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENTIMENTANALYZER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_lblcpu;
	CStatic m_lblMem;
	CProgressCtrl m_prgCPU;
	CProgressCtrl m_prgMem;
	CButton m_btnAnalyze;
	CStatic m_lblOutput;
	CEdit m_txtQuery;
	uint8_t* queryModelState;
	uint8_t* parserModelState;
	string input;
	string output;
	string prompt;
	LlamaCPPWrapper* lcw;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:

	void InitializeLLama();
	
public:
	afx_msg void OnClickedAnalyze();
};
