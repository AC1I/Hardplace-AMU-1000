// CMatchingMode.cpp : implementation file
//

#include "pch.h"
#include "Hardplace AMU-1000.h"
#include "afxdialogex.h"
#include "CMatchingMode.h"


// CMatchingMode dialog

IMPLEMENT_DYNAMIC(CMatchingMode, CDialogEx)

CMatchingMode::CMatchingMode(CWnd* pParent /*=nullptr*/, unsigned uMode)
	: CDialogEx(IDD_MODE, pParent)
	, m_MatchingMode(uMode)
{

}

CMatchingMode::~CMatchingMode()
{
}

void CMatchingMode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_AUTO_FROM_BYPASS, m_MatchingMode);
}


BEGIN_MESSAGE_MAP(CMatchingMode, CDialogEx)
END_MESSAGE_MAP()


// CMatchingMode message handlers

BOOL CMatchingMode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMatchingMode::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData();

	CDialogEx::OnOK();
}
