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
	CString m_CheckSub;           //��ѯ��Ʒ�������
	CString m_CheckUName;         //��ѯ������
	CString m_CheckName;          //��ѯ��Ʒ��
	afx_msg void OnBnClickedOk();
};
