#pragma once
#include "afxcmn.h"
#include "goldata.h"
#include "TechValListCtrl.h"
#include "EasySize.h"

// CProductStep2Dlg dialog

class CProductStep2Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProductStep2Dlg)
	DECLARE_EASYSIZE
public:
	CProductStep2Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProductStep2Dlg();

// Dialog Data
	enum { IDD = IDD_PRODUCTSTEP2_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
public:
	//////////////////////////��һҳ����һҳ�Ĳ���
	DWORD OnWizardActive();     //����ִ�г�ʼ������

	////////////////////////��ǰҳ������һҳ
	DWORD OnWizardNext();       //���Լ��鲢���浱ǰ����

	///////////////////////��ǰҳ������һҳ����CListCtrl���Զ�������Ϣ������ֻ���� OnWizardNext������Ϣ���ݵ���һҳ��
	DWORD OnWizardPrevious();   //���Լ�����һ���������ñ���

	CTechValListCtrl m_MatInfoList;
	vector<CMatChartItem> m_ListCtrlItem;//�洢List��Ŀ
	CProductInfo m_ProductInfo;//��Ʒ��Ϣ
	void GetProInfo(CProductInfo& m_ProductInfo);//��ȡ��Ʒ��Ϣ

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedInputmat();//������ϱ�

	//��������Excel
	void ReadMatChart(CString excelFile,int sheetIndex,bool header);//��ȡexcel��

	void SetListItem(vector<vector<CString>>& str_AllItem);         //����list��Ŀ
	void MatchMatVal(vector<CMatChartItem>& m_ListCtrlItem,CProductInfo& m_ProductInfo);        //ƥ����ϵ÷�

	vector<CLowValItem> m_LowValItem;          //�洢�ͷ���
	int m_LowValItemNum;                     //��¼�ͷ�����Ŀ��
	void SaveLowValItem(vector<CMatChartItem>& m_ListCtrlItem);   //����ͷ���
};
