
// DialogAndHashingDlg.h : header file
//

#pragma once


// CDialogAndHashingDlg dialog
class CDialogAndHashingDlg : public CDialogEx
{
// Construction
public:
	CDialogAndHashingDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGANDHASHING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	CListCtrl lvEntry;
	afx_msg void OnBnClickedOk();
	CButton btnOk;
};
