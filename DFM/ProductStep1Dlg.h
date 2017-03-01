#pragma once
#include "TechValListCtrl.h"
#include "goldata.h"
#include "EasySize.h"
// CProductStep1Dlg dialog





class CProductStep1Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProductStep1Dlg)
	DECLARE_EASYSIZE
public:
	CProductStep1Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProductStep1Dlg();

// Dialog Data
	enum { IDD = IDD_PRODUCTSTEP1_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
public:
	//响应主对话框几个按钮的消息
	DWORD OnWizardActive();     //激活执行初始化操作
	DWORD OnWizardNext();       //可以检验并保存当前工作
	DWORD OnWizardPrevious();   //可以检验上一步工作，不用保存
public:
	virtual BOOL OnInitDialog();
	CTechValListCtrl m_TechValList;            //listctrl控制变量
	vector<CString> m_Lvl3TechID;              //第三层工艺指标ID
	vector<vector<CString>> m_IndexValInfo;    //存储指标评分的相关信息，用于之后匹配（依次访问数据库会引起堆栈崩溃） 
	vector<CTechChartItem> m_ListCtrlItem;     //存储List条目
	CProductInfo m_ProductInfo;//结构体保存产品信息

	vector<CLowValItem> m_LowValItem;          //存储低分项
	int m_LowValItemNum;                     //记录低分项条目数
	void SaveLowValItem(vector<CTechChartItem>& m_ListCtrlItem);   //保存低分项


	void ReadTechChart(CProductInfo &m_ProductInfo);              //读取工艺表
	void ShowListCtrl();                                          //显示评分表                   
	void SetListItem(vector<CTechChartItem> &m_ListCtrlItem);     //设置list条目

	void SaveChartInfo();                                         //保存表的输入信息，下次进入直接初始化
	void InitChartInfo();                                         //评分表信息初始化

	afx_msg LRESULT OnSetIndexVal(WPARAM wParam,LPARAM lParam);//设置指标得分
	afx_msg LRESULT OnIndexMatch(WPARAM wParam,LPARAM lParam); //特殊评分项的匹配操作

	BOOL m_bLegal;                                    //检测合理性问题
	//void DeleteStructInfo();      //当上一页结构发生更改时，要删除原结构,会被重复使用
};

