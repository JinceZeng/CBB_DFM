// ResultCheck.cpp : implementation file
//

#include "stdafx.h"
#include "DFM.h"
#include "ResultCheck.h"
#include "afxdialogex.h"


// CResultCheck dialog

IMPLEMENT_DYNAMIC(CResultCheck, CDialogEx)

CResultCheck::CResultCheck(CWnd* pParent /*=NULL*/)
	: CDialogEx(CResultCheck::IDD, pParent)
	, m_CheckNum(_T(""))
	, m_CheckUName(_T(""))
	, m_CheckName(_T(""))
{

}

CResultCheck::~CResultCheck()
{
}

void CResultCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CHECKNUM, m_CheckNum);
	DDX_Text(pDX, IDC_CHECKUNAME, m_CheckUName);
	DDX_Text(pDX, IDC_CHECKNAME, m_CheckName);
}


BEGIN_MESSAGE_MAP(CResultCheck, CDialogEx)
	ON_BN_CLICKED(IDOK, &CResultCheck::OnBnClickedOk)
END_MESSAGE_MAP()


// CResultCheck message handlers


void CResultCheck::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_CheckNum==CString("")||m_CheckName==CString("")||m_CheckUName==CString(""))
	{
		AfxMessageBox(CString("请先完成输入！"));
		return;
	}
	CDialogEx::OnOK();
}
