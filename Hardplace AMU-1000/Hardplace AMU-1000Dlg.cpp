
// Hardplace AMU-1000Dlg.cpp : implementation file

#include "pch.h"
#include "framework.h"
#include "Hardplace AMU-1000.h"
#include "Hardplace AMU-1000Dlg.h"
#include "afxdialogex.h"
#include <cstdlib>
#include "CMatchingMode.h"
#include "CAntennaDlg.h"

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


// CHardplaceAMU1000Dlg dialog

constexpr auto IDM_MATCHING_MODE = 0x0020;
constexpr auto IDM_ANTENNA_MODE = 0x0030;

CHardplaceAMU1000Dlg::CHardplaceAMU1000Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HARDPLACE_AMU1000_DIALOG, pParent)
	, m_fPlacementSet(false), m_cNoComm(0), m_MatchingMode(1), m_iAntennaMode(-1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHardplaceAMU1000Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMMPORT, m_AMU1000_Port);
}

BEGIN_MESSAGE_MAP(CHardplaceAMU1000Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCOMMOPEN, &CHardplaceAMU1000Dlg::OnCommOpened)
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CHardplaceAMU1000Dlg message handlers

BOOL CHardplaceAMU1000Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	ASSERT((IDM_MATCHING_MODE & 0xFFF0) == IDM_MATCHING_MODE);
	ASSERT(IDM_MATCHING_MODE < 0xF000);
	ASSERT((IDM_ANTENNA_MODE & 0xFFF0) == IDM_ANTENNA_MODE);
	ASSERT(IDM_ANTENNA_MODE < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		CString strMatchingMode;
		CString strAntennaMode;

		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);

		bNameValid = strMatchingMode.LoadString(IDS_MATCHINGMODE);
		ASSERT(bNameValid);

		bNameValid = strAntennaMode.LoadString(IDS_ANTENNA_MODE);
		ASSERT(bNameValid);

		if (!strMatchingMode.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_MATCHING_MODE, strMatchingMode);
			pSysMenu->AppendMenu(MF_STRING, IDM_ANTENNA_MODE, strAntennaMode);
		}
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

	resetDlg();

	ULONG uPortsFound(0);
	if (GetCommPorts(0, 0, &uPortsFound) == ERROR_MORE_DATA)
	{
#pragma warning(push)
#pragma warning(disable : 6001 )
		CAutoPtr<ULONG> pPortBuf(new ULONG[uPortsFound]);
#pragma warning(pop)

		if (GetCommPorts(pPortBuf, uPortsFound, &uPortsFound) == ERROR_SUCCESS)
		{
			for (unsigned nIndex(0); nIndex < uPortsFound; nIndex++)
			{
				CString comPort;

				comPort.Format(_T("COM%u"), pPortBuf[nIndex]);
				if (m_AMU1000_Port.FindStringExact(-1, comPort) == CB_ERR)
				{
					int iIndex(m_AMU1000_Port.AddString(comPort));
					if (iIndex >= 0)
					{
						m_AMU1000_Port.SetItemData(iIndex, pPortBuf[nIndex]);
					}
				}
			}

			UINT uDefaultPort(theApp.GetProfileInt(_T("Settings"), _T("AMU-1000_Port"), 0));
			CString findString;

			findString.Format(_T("COM%u"), uDefaultPort);
			int nIndex(m_AMU1000_Port.FindStringExact(-1, findString));

			if (nIndex != CB_ERR)
			{
				m_AMU1000_Port.SetCurSel(nIndex);
				OpenCommPort(uDefaultPort, m_AMU1000_Serial, true);
			}
			else
			{
				m_AMU1000_Port.SelectString(-1, _T("COM"));
			}
		}
	}
	SetTimer(m_idTimerEvent, m_TimerInterval, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHardplaceAMU1000Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == IDM_MATCHING_MODE)
	{
		CMatchingMode Dlg(nullptr, m_MatchingMode);
		
		if (Dlg.DoModal() == IDOK)
		{
			CStringA strCmd("MM");

			strCmd += char(Dlg.MatchingMode() + '0');
			strCmd += ";\r";
			m_AMU1000_Serial.Write(strCmd, strCmd.GetLength());
		}
	}
	else if ((nID & 0xFFF0) == IDM_ANTENNA_MODE)
	{
		CAntennaDlg Dlg(nullptr, m_iAntennaMode);

		if (Dlg.DoModal() == IDOK)
		{
			CStringA strCmd("MA");

			strCmd += char(Dlg.AntennaMode() + '0');
			strCmd += ";\r";
			m_AMU1000_Serial.Write(strCmd, strCmd.GetLength());
		}
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHardplaceAMU1000Dlg::OnPaint()
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
HCURSOR CHardplaceAMU1000Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHardplaceAMU1000Dlg::OnClose()
{
	KillTimer(m_idTimerEvent);
	try
	{
		if (m_AMU1000_Serial.IsOpen())
		{
			m_AMU1000_Serial.Write("MR0;\r", 5);
			m_AMU1000_Serial.Flush();
			m_AMU1000_Serial.Close();
		}
	}
	catch (CSerialException ex)
	{
	}

	CDialogEx::OnClose();
}

void CHardplaceAMU1000Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_idTimerEvent)
	{
		if (m_AMU1000_Serial.IsOpen())
		{
			DWORD dwAvailable(0);
			DWORD dwErrors;
			COMSTAT Status;

			if (ClearCommError(HANDLE(m_AMU1000_Serial), &dwErrors, &Status)
				&& Status.cbOutQue <= 5)
			{
				dwAvailable = Status.cbInQue;
				if (dwAvailable) {
					m_cNoComm = 0;
				}
				else if (++m_cNoComm > 50)
				{
					try
					{
						m_AMU1000_Serial.Write("MR1;\r", 5);
						m_cNoComm = 0;
					}
					catch (CSerialException ex)
					{
						onSerialException(ex, m_AMU1000_Serial);
						m_MsgBuf.Empty();
					}
				}
				for (uint8_t uchByte(0); dwAvailable; dwAvailable--)
				{
					try
					{
						m_AMU1000_Serial.Read(&uchByte, 1);

						if (static_cast<char>(uchByte) == '\r')
						{
							int nIndex = m_MsgBuf.Find("MA");

							if (nIndex >= 0)
							{
								m_MsgBuf = m_MsgBuf.Right(m_MsgBuf.GetLength() - (2 + nIndex));
							}
							int nCtrl(0);
							char* context(0);
							CAutoPtr<char> pBuf(_strdup(m_MsgBuf));

							for (char* pPart(strtok_s(pBuf, ";", &context)); pPart != nullptr; pPart = strtok_s(NULL, ";", &context))
							{
								switch (nCtrl++)
								{
								case 0:
									SetDlgItemTextA(GetSafeHwnd(), IDC_FREQUENCY, pPart);
									break;

								case 1:
									m_MatchingMode = pPart[0] - '0';
									switch (pPart[0])
									{
									case '0':
										SetDlgItemTextA(GetSafeHwnd(), IDC_MODE, "Auto from bypass");
										break;

									case '1':
										SetDlgItemTextA(GetSafeHwnd(), IDC_MODE, "Auto from matching network");
										break;

									case '2':
										SetDlgItemTextA(GetSafeHwnd(), IDC_MODE, "Bypass");
										break;

									case '3':
										SetDlgItemTextA(GetSafeHwnd(), IDC_MODE, "Manual");
										break;

									default:
										break;
									}
									break;

								case 2:
									if (pPart[0] == '0')
									{
										pPart++;
										if (pPart[0] == '0')
										{
											pPart++;
										}
									}
									SetDlgItemTextA(GetSafeHwnd(), IDC_POWER, pPart);
									break;

								case 3:
								{
									CStringA szText;

									szText = pPart[0];
									szText += '.';
									szText += pPart[1];
									SetDlgItemTextA(GetSafeHwnd(), IDC_SWR, szText);
								}
									break;

								case 4:
									m_iAntennaMode = pPart[0] - '0';
									SetDlgItemTextA(GetSafeHwnd(), IDC_ANTENNA, pPart);
									break;

								case 5:
								{
									CStringA szLValue(pPart);

									szLValue.Insert(szLValue.GetLength() - 1, ".");
									if (szLValue[0] == '0')
									{
										szLValue.Delete(0);
									}
									SetDlgItemTextA(GetSafeHwnd(), IDC_LVALUE, szLValue);
								}
									break;

								case 6:
								{
									CStringA szCValue(pPart);

									for (int nIndex(0);
										szCValue[0] == '0' && nIndex < szCValue.GetLength() - 1;
										nIndex++)
									{
										szCValue.Delete(0);
									}
									SetDlgItemTextA(GetSafeHwnd(), IDC_CVALUE, szCValue);
								}
									break;

								case 7:
									SetDlgItemTextA(GetSafeHwnd(), IDC_TRANSMIT, (pPart[0] == '0') ? "" : "Transmitting");
									break;

								case 8:
									SetDlgItemTextA(GetSafeHwnd(), IDC_MATCHED, (pPart[0] == '0') ? "Not Matched" : "Matched");
									break;

								default:
									break;
								}

							}
							m_MsgBuf.Empty();
						}
						else
						{
							m_MsgBuf += static_cast<char>(uchByte);
						}
					}
					catch (CSerialException ex) {
						onSerialException(ex, m_AMU1000_Serial);
						m_MsgBuf.Empty();
						break;
					}
				}
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

bool CHardplaceAMU1000Dlg::OpenCommPort(int iPort, CSerialPort& Port, bool fQuiet)
{
	try
	{
		if (!Port.IsOpen())
		{
			CWaitCursor wait;

			Port.Open(iPort, DWORD(9600));
			if (Port.IsOpen())
			{
				COMMTIMEOUTS timeOuts;

				Port.GetTimeouts(timeOuts);
				timeOuts.ReadIntervalTimeout = 250;
				timeOuts.ReadTotalTimeoutConstant = 250;
				timeOuts.WriteTotalTimeoutConstant = 10000;
				Port.SetTimeouts(timeOuts);

				if (Port == m_AMU1000_Serial) {
					SetDlgItemText(IDCOMMOPEN, _T("Close"));
					m_AMU1000_Serial.Write("MR1;\r", 5);
				}
			}
			else if (!fQuiet)
			{
				AfxMessageBox(IDS_PORTBUSY);
			}
		}
	}
	catch (CSerialException ex) {
		if (!fQuiet)
		{
			onSerialException(ex, Port);
		}
	}

	return Port.IsOpen();
}

void CHardplaceAMU1000Dlg::onSerialException(CSerialException& ex, CSerialPort& Port)
{
	TCHAR achErrorStr[80];

	ex.GetErrorMessage2(achErrorStr, sizeof achErrorStr / sizeof(TCHAR));
	try
	{
		if (Port.IsOpen())
		{
			Port.Close();
		}
	}
	catch (CSerialException ex)
	{
	}
	AfxMessageBox(achErrorStr);
}


void CHardplaceAMU1000Dlg::OnCommOpened()
{
	try
	{
		if (m_AMU1000_Serial.IsOpen())
		{
			m_AMU1000_Serial.Write("MR0;\r", 5);
			m_AMU1000_Serial.Flush();
			m_AMU1000_Serial.Close();
			SetDlgItemText(IDCOMMOPEN, _T("Open"));
			resetDlg();
		}
		else
		{
			OpenCommPort(int(int(m_AMU1000_Port.GetItemData(m_AMU1000_Port.GetCurSel()))), m_AMU1000_Serial);
			theApp.WriteProfileInt(_T("Settings"), _T("AMU-1000_Port"), int(m_AMU1000_Port.GetItemData(m_AMU1000_Port.GetCurSel())));
		}
	}
	catch (CSerialException ex)
	{
	}
}

void CHardplaceAMU1000Dlg::resetDlg()
{
	SetDlgItemText(IDC_FREQUENCY, _T(""));
	SetDlgItemText(IDC_MODE, _T(""));
	SetDlgItemText(IDC_POWER, _T(""));
	SetDlgItemText(IDC_SWR, _T(""));
	SetDlgItemText(IDC_ANTENNA, _T(""));
	SetDlgItemText(IDC_LVALUE, _T(""));
	SetDlgItemText(IDC_CVALUE, _T(""));
	SetDlgItemText(IDC_TRANSMIT, _T(""));
	SetDlgItemText(IDC_MATCHED, _T(""));
}

void CHardplaceAMU1000Dlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow && !IsWindowVisible() && !m_fPlacementSet)
	{
		WINDOWPLACEMENT* lwp(0);
		UINT nl;

		m_fPlacementSet = true;

		if (theApp.GetProfileBinary(_T("Settings"), _T("Window"), (LPBYTE*)&lwp, &nl))
		{
			SetWindowPlacement(lwp);
			delete[] lwp;
		}
	}
}

void CHardplaceAMU1000Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	WINDOWPLACEMENT wp = { 0 };
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wp);
	theApp.WriteProfileBinary(_T("Settings"), _T("Window"), (LPBYTE)&wp, wp.length);
}
