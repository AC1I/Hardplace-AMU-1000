
// Hardplace AMU-1000Dlg.h : header file
//

#pragma once

#include <afxwin.h>
#include "SerialPort.h"

// CHardplaceAMU1000Dlg dialog
class CHardplaceAMU1000Dlg : public CDialogEx
{
// Construction
public:
	CHardplaceAMU1000Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HARDPLACE_AMU1000_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	const UINT_PTR m_idTimerEvent = 1;
	const UINT m_TimerInterval = 50;
	bool m_fPlacementSet;
	unsigned m_cNoComm;
	CSerialPort m_AMU1000_Serial;
	CStringA m_MsgBuf;

	bool OpenCommPort(int iPort, CSerialPort& Port, bool fQuiet = false);
	void onSerialException(CSerialException& ex, CSerialPort& Port);
	void resetDlg();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	CComboBox m_AMU1000_Port;
public:
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCommOpened();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
};
