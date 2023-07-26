#pragma once
#include <afxdialogex.h>
#include "resource.h"
class CSettingsDlg : public CDialogEx
{
public:
	enum { IDD = IDD_SETTINGS };
	CSettingsDlg();
	~CSettingsDlg();
	int OnInitDialog();
	void SetPly(int ply);
	int GetPly();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
private:
	CStatic m_lblPly;
	CSliderCtrl m_SliderPly;
	int m_plyVal;
public:
	DECLARE_MESSAGE_MAP()
//		afx_msg void OnHScrollClipboard(CWnd* pClipAppWnd, UINT nSBCode, UINT nPos);
	afx_msg void OnBnClickedOk();
	afx_msg void onBnCancelClicked();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};