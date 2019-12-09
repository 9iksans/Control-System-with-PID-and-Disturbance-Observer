
// teechartsDlg.cpp : implementation file
//


#include "stdafx.h"
#include "stdlib.h"
#include "teecharts.h"
#include "teechartsDlg.h"
#include "afxdialogex.h"
#include "stdio.h"
#include "CSeries0.h"
#include <string>
#include <iostream>


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


// CteechartsDlg dialog



CteechartsDlg::CteechartsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEECHARTS_DIALOG, pParent)
	, m_edit1(_T(""))
	, m_edit2(_T(""))
	//, m_chart(0)
	, m_edit3(_T(""))
	, m_edit4(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CteechartsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, m_comm);
	//  DDX_Control(pDX, IDC_EDIT2, m_edit1);
	DDX_Text(pDX, IDC_EDIT2, m_edit1);
	DDX_Text(pDX, IDC_EDIT1, m_edit2);
	DDX_Control(pDX, IDC_TCHART1, m_chart);
	DDX_Text(pDX, IDC_EDIT4, m_edit3);
	DDX_Text(pDX, IDC_EDIT3, m_edit4);
}

BEGIN_MESSAGE_MAP(CteechartsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CteechartsDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CteechartsDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT2, &CteechartsDlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT1, &CteechartsDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT4, &CteechartsDlg::OnEnChangeEdit4)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, &CteechartsDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CteechartsDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CteechartsDlg message handlers

BOOL CteechartsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CteechartsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CteechartsDlg::OnPaint()
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
HCURSOR CteechartsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CteechartsDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	CString sSetting = _T("115200,N,8,1");
	m_comm.put_CommPort(1);
	m_comm.put_Settings(sSetting);
	m_comm.put_InputMode(0);
	m_comm.put_InputLen(30);
	m_comm.put_RTSEnable(TRUE);
	m_comm.put_RThreshold(1);
	m_comm.put_PortOpen(TRUE);
	
	UpdateData(FALSE);

}

BEGIN_EVENTSINK_MAP(CteechartsDlg, CDialogEx)
	ON_EVENT(CteechartsDlg, IDC_MSCOMM1, 1, CteechartsDlg::OnCommMscomm1, VTS_NONE)
	ON_EVENT(CteechartsDlg, IDC_TCHART1, 31, CteechartsDlg::OnTimerTchart1, VTS_NONE)
END_EVENTSINK_MAP()


unsigned char in;
int data1,data2, data1last, data2last, selisih, selisih2;
int time_x, panjang;

void CteechartsDlg::OnCommMscomm1()
{

	int aa = 0;
	int nTokenPos = 0;
	data1 = data1last;
	data2 = data2last;
	if (m_comm.get_CommEvent() == 2) {

		VARIANT in_dat;
		in_dat = m_comm.get_Input();
		CString strInput(in_dat.bstrVal);
		CString strToken[5];

		strInput.Trim();

		
		strToken[aa] = strInput.Tokenize(_T(" "), nTokenPos);
		while (!strToken[aa].IsEmpty()) {
			aa++;
			if (aa == 3) {
				break;
			}
			strToken[aa] = strInput.Tokenize(_T(" "), nTokenPos);
		}


		UpdateData(TRUE);
			if (aa == 2) {
				
				if (data1last - _ttoi(strToken[0]) <= 150 && data1last - _ttoi(strToken[0]) >= -150) {
					m_edit2 = strInput;
					m_edit3 = strToken[0];
					
					data1last = _ttof(m_edit3);
				}
				
				if (data2last - _ttoi(strToken[1]) <= 150 && data2last - _ttoi(strToken[1]) >= -150) {
					m_edit4 = strToken[1];
					data2last = _ttof(m_edit4);
				}
				
			}
		UpdateData(FALSE);
/*
		if (data1 > 0) {
			data1last = data1;
		}
		if (data2 > 0) {
			data2last = data2;
		}


		selisih = data1last - data1;
		selisih2 = data2last - data2;

		if (selisih < 0) {
			selisih = -selisih;
		}
		if (selisih2 < 0) {
			selisih2 = -selisih2;
		}

		if (selisih > 50) {
			data1 = data1last;
		}
		if (selisih2 > 50) {
			data2 = data2last;
		}*/
		
	}
}




void CteechartsDlg::OnBnClickedButton2()
{
	CString sSatu(_T(" "));
	UpdateData(TRUE);
	CString sKirim = sSatu + m_edit1;
	UpdateData(FALSE);

	char strPass[256];
	strcpy_s(strPass, CStringA(sKirim).GetString());

	CByteArray hexdata;
	hexdata.SetSize(sKirim.GetLength());

	for (int i = 0; i < sKirim.GetLength(); i++) {
		hexdata[i] = strPass[i];
	}

	UpdateData(TRUE);
	m_comm.put_Output(COleVariant(hexdata));
	UpdateData(FALSE);
}


void CteechartsDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CteechartsDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CteechartsDlg::OnTimerTchart1()
{

}


void CteechartsDlg::OnEnChangeEdit4()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CteechartsDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	// TODO: Add your message handler code here


	((CSeries0)(m_chart.get_Series(0))).AddXY(time_x, _ttoi(m_edit3), NULL, 536870912);
	((CSeries0)(m_chart.get_Series(1))).AddXY(time_x, _ttoi(m_edit4), NULL, 536870912);

	time_x++;
	CDialogEx::OnTimer(nIDEvent);
}


void CteechartsDlg::OnBnClickedButton3()
{
	
	// TODO: Add your control notification handler code here
	//time_x = 0;
	//((CSeries0)(m_chart.get_Series(0))).Clear();
	//((CSeries0)(m_chart.get_Series(1))).Clear();
	CString sSatu(_T("1"));
	UpdateData(TRUE);
	CString sKirim = sSatu;
	UpdateData(FALSE);

	char strPass[256];
	strcpy_s(strPass, CStringA(sKirim).GetString());

	CByteArray hexdata;
	hexdata.SetSize(sKirim.GetLength());

	for (int i = 0; i < sKirim.GetLength(); i++) {
		hexdata[i] = strPass[i];
	}

	UpdateData(TRUE);
	m_comm.put_Output(COleVariant(hexdata));
	UpdateData(FALSE);
	SetTimer(0, 50, NULL);
}


void CteechartsDlg::OnBnClickedButton4()
{
	KillTimer(0);
	data1last = 0;
	data2last = 0;
	m_edit2.Empty();
	m_edit3.Empty();
	m_edit4.Empty();

	m_comm.put_PortOpen(FALSE);
	m_comm.put_PortOpen(TRUE);
	CString sSatu(_T("2"));
	UpdateData(TRUE);
	CString sKirim = sSatu;
	UpdateData(FALSE);

	char strPass[256];
	strcpy_s(strPass, CStringA(sKirim).GetString());

	CByteArray hexdata;
	hexdata.SetSize(sKirim.GetLength());

	for (int i = 0; i < sKirim.GetLength(); i++) {
		hexdata[i] = strPass[i];
	}

	UpdateData(TRUE);
	m_comm.put_Output(COleVariant(hexdata));
	UpdateData(FALSE);
}
