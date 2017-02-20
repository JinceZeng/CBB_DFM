// ChooseEvalType.cpp : implementation file
//

#include "stdafx.h"
#include "DFM.h"
#include "ChooseEvalType.h"
#include "afxdialogex.h"


// CChooseEvalType dialog

IMPLEMENT_DYNAMIC(CChooseEvalType, CDialogEx)

CChooseEvalType::CChooseEvalType(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChooseEvalType::IDD, pParent)
	, m_EvalType(_T(""))
	, m_nModelID(0)
{

}

CChooseEvalType::~CChooseEvalType()
{
}

void CChooseEvalType::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CHOOSETYPE, m_cmbEvalType);
	DDX_CBString(pDX, IDC_COMBO_CHOOSETYPE, m_EvalType);
}


BEGIN_MESSAGE_MAP(CChooseEvalType, CDialogEx)
	ON_BN_CLICKED(IDOK, &CChooseEvalType::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CChooseEvalType::OnBnClickedCancel)
END_MESSAGE_MAP()


// CChooseEvalType message handlers


BOOL CChooseEvalType::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//初始化下拉菜单
	CString sql = CString("select * from EvalModelInfo");
	m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
	int i=0;
	while (!m_pRs->adoEOF)
	{
		CString str = (CString)(m_pRs->GetCollect("EvalModelNam")); 
		m_cmbEvalType.InsertString(i,str);
		i++;
		m_pRs->MoveNext();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CChooseEvalType::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_nModelID=m_cmbEvalType.GetCurSel()+1;
	CDialogEx::OnOK();
}


void CChooseEvalType::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}



