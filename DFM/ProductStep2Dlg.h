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
	//////////////////////////上一页到下一页的操作
	DWORD OnWizardActive();     //激活执行初始化操作

	////////////////////////当前页进入下一页
	DWORD OnWizardNext();       //可以检验并保存当前工作

	///////////////////////当前页进入上一页（由CListCtrl能自动保存信息，所以只需在 OnWizardNext保存信息传递到下一页）
	DWORD OnWizardPrevious();   //可以检验上一步工作不用保存

	CTechValListCtrl m_MatInfoList;
	vector<CMatChartItem> m_ListCtrlItem;//存储List条目
	CProductInfo m_ProductInfo;//产品信息
	void GetProInfo(CProductInfo& m_ProductInfo);//获取产品信息

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedInputmat();//导入材料表

	//用于连接Excel
	void ReadMatChart(CString excelFile,int sheetIndex,bool header);//读取excel表

	void SetListItem(vector<vector<CString>>& str_AllItem);         //设置list条目
	void MatchMatVal(vector<CMatChartItem>& m_ListCtrlItem,CProductInfo& m_ProductInfo);        //匹配材料得分

	vector<CLowValItem> m_LowValItem;          //存储低分项
	int m_LowValItemNum;                     //记录低分项条目数
	void SaveLowValItem(vector<CMatChartItem>& m_ListCtrlItem);   //保存低分项
};
