#pragma once
#include "afxcmn.h"
#include "goldata.h"
#include "TechValListCtrl.h"

// CProductStep7Dlg dialog

class CProductStep7Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProductStep7Dlg)

public:
	CProductStep7Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProductStep7Dlg();

// Dialog Data
	enum { IDD = IDD_PRODUCTSTEP7_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	//////////////////////////��һҳ����һҳ�Ĳ���
	DWORD OnWizardActive();     //����ִ�г�ʼ������

	////////////////////////��ǰҳ������һҳ
	DWORD OnWizardNext();       //���Լ��鲢���浱ǰ����

	///////////////////////��ǰҳ������һҳ����CListCtrl���Զ�������Ϣ������ֻ���� OnWizardNext������Ϣ���ݵ���һҳ��
	DWORD OnWizardPrevious();   //���Լ�����һ���������ñ���

	CTechValListCtrl m_MatInfoList;
	virtual BOOL OnInitDialog();
	void ReadTechChart();                       //��ȡ���ϱ�
	vector<vector<CString>> m_MatInfo;          //�洢���Ӳ��ϱ�������Ϣ   
	vector<CString> m_MatType;                  //�洢���в������ͣ�����combo��ʼ��
	vector<CMatChartItem1> m_ListCtrlItem;    //�洢List��Ŀ
	CProductInfo m_ProductInfo;//�ṹ�屣���Ʒ��Ϣ

	vector<CLowValItem> m_LowValItem;          //�洢�ͷ���
	int m_LowValItemNum;                     //��¼�ͷ�����Ŀ��
	void SaveLowValItem();   //����ͷ���

	void SaveChartInfo();                                         //������������Ϣ���´ν���ֱ�ӳ�ʼ��
	void InitChartInfo();                                         //���ֱ���Ϣ��ʼ��

	afx_msg LRESULT OnDeleteIndexItem(WPARAM wParam,LPARAM lParam); //ɾ��ĳ��������Ϣ
	afx_msg LRESULT OnAddIndexItem(WPARAM wParam,LPARAM lParam);//���ĳ��������Ϣ
	afx_msg LRESULT OnSetIndexVal(WPARAM wParam,LPARAM lParam);//�жϲ����Ƿ��ظ�ѡ��
	

};
