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
	//////////////////////////上一页到下一页的操作
	DWORD OnWizardActive();     //激活执行初始化操作

	////////////////////////当前页进入下一页
	DWORD OnWizardNext();       //可以检验并保存当前工作

	///////////////////////当前页进入上一页（由CListCtrl能自动保存信息，所以只需在 OnWizardNext保存信息传递到下一页）
	DWORD OnWizardPrevious();   //可以检验上一步工作不用保存

	CTechValListCtrl m_MatInfoList;
	virtual BOOL OnInitDialog();
	void ReadTechChart();                       //读取材料表
	vector<vector<CString>> m_MatInfo;          //存储机加材料表所有信息   
	vector<CString> m_MatType;                  //存储表中材料类型，用于combo初始化
	vector<CMatChartItem1> m_ListCtrlItem;    //存储List条目
	CProductInfo m_ProductInfo;//结构体保存产品信息

	vector<CLowValItem> m_LowValItem;          //存储低分项
	int m_LowValItemNum;                     //记录低分项条目数
	void SaveLowValItem();   //保存低分项

	void SaveChartInfo();                                         //保存表的输入信息，下次进入直接初始化
	void InitChartInfo();                                         //评分表信息初始化

	afx_msg LRESULT OnDeleteIndexItem(WPARAM wParam,LPARAM lParam); //删除某条材料信息
	afx_msg LRESULT OnAddIndexItem(WPARAM wParam,LPARAM lParam);//添加某条材料信息
	afx_msg LRESULT OnSetIndexVal(WPARAM wParam,LPARAM lParam);//判断材料是否重复选择
	

};
