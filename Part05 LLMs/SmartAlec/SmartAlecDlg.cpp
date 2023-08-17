
// SentimentAnalyzerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "SmartAlec.h"
#include "ExternalQuery.h"
#include "SmartAlecDlg.h"
#include "afxdialogex.h"
#include "SysInfo.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include "Prompts.h"
#include "constants.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSentimentAnalyzerDlg dialog



CSmartAlecDlg::CSmartAlecDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SENTIMENTANALYZER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSmartAlecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, lbl_CPU, m_lblcpu);
	DDX_Control(pDX, lbl_Mem, m_lblMem);
	DDX_Control(pDX, prg_CPU, m_prgCPU);
	DDX_Control(pDX, prg_Mem, m_prgMem);
	DDX_Control(pDX, IDC_ANALYZE, m_btnAnalyze);
	DDX_Control(pDX, IDC_LBL_OUTPUT, m_lblOutput);
	DDX_Control(pDX, IDC_QUERY_TEXT, m_txtQuery);
}

BEGIN_MESSAGE_MAP(CSmartAlecDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ANALYZE, &CSmartAlecDlg::OnClickedAnalyze)
END_MESSAGE_MAP()




void CSmartAlecDlg::InitializeLLama()
{
	lcw = new LlamaCPPWrapper(MODEL_PATH);

	int stateSize = lcw->GetStateSize();

	ifstream rf(QUERY_STATE_FILE, ios::out | ios::binary);

	if (!rf)
	{
		//warm up query state and create file
		lcw->Evaluate(Prompts::GetInitialPrompt() + "### Input:What is the official Name of France?");

		queryModelState = lcw->SaveState();

		ofstream wf(QUERY_STATE_FILE, ios::out | ios::binary);
		
		if (!wf)
		{
			throw("Error creating query state file.");
		}

		wf.write((char*)queryModelState, stateSize);

		wf.close();
	}
	else
	{
		queryModelState = new uint8_t[stateSize];

		rf.read((char*)queryModelState, stateSize);

		rf.close();
	
	}


	

	ifstream prf(PARSER_STATE_FILE, ios::out | ios::binary);

	if (!prf)
	{
		LlamaCPPWrapper parser(MODEL_PATH);

		//warm up parser state and create file
		auto x=parser.Evaluate(Prompts::GetWikpediaParserPrompt() + "### Input:Paris is the capital of france.What is the capital of France?");

		parserModelState = parser.SaveState();

		ofstream pwf(PARSER_STATE_FILE, ios::out | ios::binary);

		if (!pwf)
		{
			throw("Error creating query state file.");
		}

		pwf.write((char*)parserModelState, stateSize);

		pwf.close();

		
	}
	else
	{
		parserModelState = new uint8_t[stateSize];

		prf.read((char*)parserModelState, stateSize);

		prf.close();

	}
	



	output = "Awaiting user input.";


}

BOOL CSmartAlecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	MoveWindow(0, 0, 2048, 2000);
	
	m_lblcpu.SetWindowTextW(_T("Process CPU %"));
	m_lblMem.SetWindowTextW(_T("Process Ram (GB)"));

	m_lblOutput.SetWindowTextW(_T(""));
	m_btnAnalyze.SetWindowTextW(_T("Analyze"));

	m_prgCPU.SetRange(0,100);
	m_prgMem.SetRange(0, 32);

	m_prgCPU.SetPos(50);
	
	SetTimer(0, 500, NULL);

	SetWindowTextW(_T("Smart Alec"));

	InitializeLLama();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSmartAlecDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSmartAlecDlg::OnPaint()
{
	//if (IsIconic())
	//{
	//	CPaintDC dc(this); // device context for painting

	//	SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

	//	// Center icon in client rectangle
	//	int cxIcon = GetSystemMetrics(SM_CXICON);
	//	int cyIcon = GetSystemMetrics(SM_CYICON);
	//	CRect rect;
	//	GetClientRect(&rect);
	//	int x = (rect.Width() - cxIcon + 1) / 2;
	//	int y = (rect.Height() - cyIcon + 1) / 2;

	//	// Draw the icon
	//	dc.DrawIcon(x, y, m_hIcon);
	//}
	//else
	//{
		CDialogEx::OnPaint();
	//}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSmartAlecDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CSmartAlecDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	auto pos = GetProcessCPU();

	auto mem = GetProcessMemory()/(1024*1024*1024);

	m_prgCPU.SetPos(pos);

	m_prgMem.SetPos(mem);

	m_lblOutput.SetWindowTextW(CA2T(output.c_str()));;

	CDialogEx::OnTimer(nIDEvent);

	
}



void CSmartAlecDlg::OnClickedAnalyze()
{
	// TODO: Add your control notification handler code here

	CString txtInput;
	


	m_txtQuery.GetWindowTextW(txtInput);

	if (!txtInput.IsEmpty())
	{
		output = "";

		string temp(CW2A(txtInput.GetString()));

		input = temp;

		lcw->LoadState(queryModelState);

		std::thread th([this] {

			auto retVal = lcw->Evaluate(Prompts::GetInputPrompt() + input);

			auto queryStartPos = retVal.find("[Query]");

			auto queryEndPos = retVal.find("### Input:");

			if (queryStartPos !=std::string::npos)
			{
				//we need to do an external call
				std::string query = "";

				if (queryEndPos == std::string::npos)
				{
					queryEndPos = retVal.length();
				}
				query = retVal.substr(7 + queryStartPos, queryEndPos - (7 + queryStartPos));

				query=query.erase(query.find_last_not_of(" \n\r\t") + 1);

				output = "Doing external search..";

				auto wikiOutput=DoWikipediaQuery(query);

				if (wikiOutput.length() > 1900)
				{
					wikiOutput = output.substr(0, 1900) + ";\n";
				}

				lcw->LoadState(parserModelState);

				output = lcw->Evaluate(Prompts::GetInputPrompt() + wikiOutput + input);

				lcw->LoadState(queryModelState);

				queryEndPos =output.find("### Input:");

				if (queryEndPos != std::string::npos)
				{
					output = output.substr(0, queryEndPos);
				}
				
			}
			else 
			{
				if (queryEndPos == std::string::npos)
				{
					queryEndPos = retVal.length();
				}

				output = retVal.substr(0, queryEndPos);
			}
			
			});

		th.detach();

	}


}
