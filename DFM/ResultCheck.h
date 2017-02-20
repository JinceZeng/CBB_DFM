#pragma once


// CResultCheck dialog

class CResultCheck : public CDialogEx
{
	DECLARE_DYNAMIC(CResultCheck)

public:
	CResultCheck(CWnd* pParent = NULL);   // standard constructor
	virtual ~CResultCheck();

// Dialog Data
	enum { IDD = IDD_RESULTCHECK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_CheckNum;           //查询编号
	CString m_CheckUName;         //查询评价人
	CString m_CheckName;          //查询产品名
	afx_msg void OnBnClickedOk();
};
