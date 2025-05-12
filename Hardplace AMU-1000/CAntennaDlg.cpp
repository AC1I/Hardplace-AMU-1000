// CAntennaDlg.cpp : implementation file
//

#include "pch.h"
#include "Hardplace AMU-1000.h"
#include "afxdialogex.h"
#include "CAntennaDlg.h"


// CAntennaDlg dialog

IMPLEMENT_DYNAMIC(CAntennaDlg, CDialogEx)

CAntennaDlg::CAntennaDlg(CWnd* pParent /*=nullptr*/, int iAntennaMode)
	: CDialogEx(IDD_ANTENNA, pParent)
	, m_iAntenna(iAntennaMode)
{

}

CAntennaDlg::~CAntennaDlg()
{
}

void CAntennaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_ANT_AUTO, m_iAntenna);
}


BEGIN_MESSAGE_MAP(CAntennaDlg, CDialogEx)
END_MESSAGE_MAP()


// CAntennaDlg message handlers

void CAntennaDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData();
	CDialogEx::OnOK();
}

BOOL CAntennaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
