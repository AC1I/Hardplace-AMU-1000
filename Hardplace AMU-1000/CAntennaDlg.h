#pragma once
#include "afxdialogex.h"


// CAntennaDlg dialog

class CAntennaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAntennaDlg)

public:
	CAntennaDlg(CWnd* pParent = nullptr, int iAntennaMode = -1);   // standard constructor
	virtual ~CAntennaDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANTENNA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	int AntennaMode(void) const {
		return m_iAntenna;
	}
protected:
	virtual BOOL OnInitDialog();
	int m_iAntenna;
};
