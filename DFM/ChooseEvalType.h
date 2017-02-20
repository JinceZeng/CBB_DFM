#pragma once
#include "afxwin.h"


// CChooseEvalType dialog

class CChooseEvalType : public CDialogEx
{
	DECLARE_DYNAMIC(CChooseEvalType)

public:
	CChooseEvalType(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChooseEvalType();

// Dialog Data
	enum { IDD = IDD_EVALTYPE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	_RecordsetPtr m_pRs;
	CComboBox m_cmbEvalType;//comboøÿ÷∆±‰¡ø
	CString m_EvalType;
	int m_nModelID;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
