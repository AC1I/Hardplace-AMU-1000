#pragma once
#include "afxdialogex.h"


// CMatchingMode dialog

class CMatchingMode : public CDialogEx
{
	DECLARE_DYNAMIC(CMatchingMode)

public:
	CMatchingMode(CWnd* pParent = nullptr, unsigned uMode = 1);   // standard constructor
	virtual ~CMatchingMode();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int MatchingMode(void) {
		return m_MatchingMode;
	}
protected:
	int m_MatchingMode;
	virtual void OnOK();
};
