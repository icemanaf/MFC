
// DialogAndHashingDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "DialogAndHashing.h"
#include "DialogAndHashingDlg.h"
#include "afxdialogex.h"
#include "sha256.h"
#include <ctime>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <string>


// CDialogAndHashingDlg dialog



CDialogAndHashingDlg::CDialogAndHashingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOGANDHASHING_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDialogAndHashingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, lvEntry);
	DDX_Control(pDX, IDOK, btnOk);
}

BEGIN_MESSAGE_MAP(CDialogAndHashingDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CDialogAndHashingDlg::OnLvnItemchangedList1)
ON_BN_CLICKED(IDOK, &CDialogAndHashingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDialogAndHashingDlg message handlers

BOOL CDialogAndHashingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here


	
	lvEntry.InsertColumn(1, _T("SHA256"), LVCFMT_CENTER, 200);
	lvEntry.InsertColumn(2, _T("Nonce"), LVCFMT_LEFT, 100);
	lvEntry.InsertColumn(3, _T("Time"), LVCFMT_LEFT, 80);
	lvEntry.InsertColumn(4, _T("kH/s"), LVCFMT_LEFT, 80);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDialogAndHashingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDialogAndHashingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//void CDialogAndHashingDlg::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}

std::string gen_random(const int len) {
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	std::string tmp_s;
	tmp_s.reserve(len);

	for (int i = 0; i < len; ++i) {
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	return tmp_s;
}


void CDialogAndHashingDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	int item=0;

	SHA256 sha256;

	std::string strPrevBlock("Genesis Block");
	std::string strNewTransaction("something");
	const int DIFICULTY = 5;



	btnOk.EnableWindow(FALSE);

	//run code here
	long i = 0;
	int score = 0;

	clock_t startTime = clock();
	
	while (true)
	{
		i++;

		std::string nonce = sha256(gen_random(20));

		std::string input = strPrevBlock + strNewTransaction + nonce;

		std::string hash= sha256(input);

		const char  *charArray = hash.c_str();

		score = 0;

		for (int j = 0;j < DIFICULTY;j++)
		{
			if (charArray[j ]== '0') {
				score++;
			}
			else {
				break;
			}
		}

		if (score >= DIFICULTY)
		{
			clock_t endTime = clock();

			clock_t timeDiff = endTime - startTime;

			float diffMs = ((float)timeDiff) / CLOCKS_PER_SEC;

			CString timeDuration(std::to_string(diffMs).c_str());
			
			CString hashRate(std::to_string(i / diffMs).c_str());
			
			CString finalHash(hash.c_str());

			CString finalNonce(nonce.c_str());


			lvEntry.InsertItem(0, finalHash);
			lvEntry.SetItemText(item, 1, finalNonce);
			lvEntry.SetItemText(item, 2, timeDuration);
			lvEntry.SetItemText(item, 3, hashRate);

			break;
		}


	}


	btnOk.EnableWindow(TRUE);


	return;

	CDialogEx::OnOK();
}
