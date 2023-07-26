#include "pch.h"
#include "CSettingsDlg.h"

CSettingsDlg::CSettingsDlg():CDialogEx(IDD)
{
	
}

CSettingsDlg::~CSettingsDlg()
{
	
}

int CSettingsDlg::OnInitDialog()
{
	CString str;

	SetWindowTextW(_T("Settings"));

	CDialogEx::OnInitDialog();

	m_SliderPly.SetRangeMin(5);
	m_SliderPly.SetRangeMax(30);

	m_SliderPly.SetPos(m_plyVal);

	str.Format(_T("%d"), m_plyVal);

	m_lblPly.SetWindowTextW(str);

	return TRUE;
}
void CSettingsDlg::SetPly(int ply)
{
	m_plyVal = ply;
}

int CSettingsDlg::GetPly()
{
	return m_plyVal;
}



void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_PLY, m_SliderPly);
	DDX_Control(pDX, IDC_LABEL_PLY_VAL, m_lblPly);
}

BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
ON_BN_CLICKED(IDOK, &CSettingsDlg::OnBnClickedOk)
ON_BN_CLICKED(IDCANCEL, &CSettingsDlg::onBnCancelClicked)
ON_WM_HSCROLL(IDC_SLIDER_PLY,&CSettingsDlg::OnHScroll)
END_MESSAGE_MAP()





void CSettingsDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	m_plyVal = m_SliderPly.GetPos();

	CDialogEx::OnOK();
}

void CSettingsDlg::onBnCancelClicked()
{
	m_plyVal = -1;

	CDialogEx::OnCancel();
}




void CSettingsDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	if (pScrollBar == (CScrollBar*)&m_SliderPly)
	{
		auto value = m_SliderPly.GetPos();

		CString str;

		str.Format(_T("%d"), value);

		m_lblPly.SetWindowTextW(str);
	}


	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
