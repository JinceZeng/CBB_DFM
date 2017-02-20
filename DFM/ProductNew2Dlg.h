#pragma once
#include "afxwin.h"
#include "goldata.h"

//���������eigen3.3
#include <Eigen/Dense>
using namespace std;
using namespace Eigen;


#include "ProductStep0Dlg.h"
#include "ProductStep1Dlg.h"
#include "ProductStep4Dlg.h"
#include "ProductStep5Dlg.h"
#include "ProductStep6Dlg.h"
#include "SetWeighDlg.h"
#include "ProductOutDlg.h"
#include "btnst.h"

// CProductNew2Dlg dialog

class CProductNew2Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProductNew2Dlg)

public:
	CProductNew2Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProductNew2Dlg();

// Dialog Data
	enum { IDD = IDD_PRODUCTNEW2_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();

	vector<CDialogEx*> m_pPageList;   //��¼���ӶԻ���
	int m_nCurrentPage;               //��ǰ��ʾҳ
	bool bInfoWrited;                 //��Ʒ��Ϣ�Ƿ���д��
	CString m_EvalType;               //������������
	CString str_EvalTypeID;           //��������ID
	CString m_TypeInfo;               //�������ͽ���

	CString strProductID;             //list��ȡ�Ĳ�ƷID,�˴�ΪCString��
	CRect m_rectPanel;                //�ӶԻ���İ�ΧȦ
	void ShowPage(UINT nPos);         //��ʾnPosҳ���0��ʼ����
	void SetWizButton(UINT nPos);     //����ҳ��������ð�ť״̬
	void SetTextShow(UINT nPos);      //����ҳ����������ı���ʾ

	afx_msg void OnBnClickedEvalpre2();
	afx_msg void OnBnClickedEvalnext2();
	afx_msg void OnBnClickedEvalin2();

	//״̬��ʾ
	CStatic m_csCaption;
	CButtonST m_btnChart20;
	CButtonST m_btnChart21;
	CButtonST m_btnChart22;
	CButtonST m_btnChart23;
	CButtonST m_btnChart24;


	//�������
	vector<CString> m_Lvl1TechNam;                      //��һ�㹤��ָ����
	vector<vector<CString>> m_Lvl2TechNam;              //�ڶ��㹤��ָ����
	RowVectorXd m_dB1; //������������ۼ���������
	RowVectorXd m_dB2; ////����ָ������ۼ���������
	VectorXd m_dC1; //һ��ָ������ֵ
	VectorXd m_dC2; //����ָ������ֵ
	double m_W;//��������ֵ

	void GetIndexVal(VectorXd& dA1,VectorXd& dA2);                   //��ȡָ������
	double MinVal(vector<double>& IndexVal);                         //ȡ��С�۷�

	//�����ʾ���
	vector<CString>& SetResultVal(vector<CString>& m_ItemVal);      //���۽�����������ʾ��
	vector<CString> m_ItemVal;            //��������ʾֵ
	vector<CLowValItem> m_LowValItem; //����ͷ���Ľ����
	vector<CIndexValItem> m_IndexVal;   //����ָ��÷�
	void SaveResultInfo(vector<CLowValItem>& m_LowValItem,vector<CIndexValItem>& m_IndexVal);          //��m_LowValItem��m_IndexVal��ֵ

};
