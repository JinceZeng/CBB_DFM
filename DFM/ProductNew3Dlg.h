#pragma once
#include "afxwin.h"
#include "goldata.h"
#include "EasySize.h"

//矩阵运算库eigen3.3
#include <Eigen/Dense>
using namespace std;
using namespace Eigen;


#include "ProductStep0Dlg.h"
#include "ProductStep1Dlg.h"
#include "ProductStep2Dlg.h"
#include "ProductStep5Dlg.h"
#include "ProductStep6Dlg.h"
#include "SetWeighDlg.h"
#include "ProductOutDlg.h"
#include "btnst.h"


// CProductNew3Dlg dialog

class CProductNew3Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProductNew3Dlg)
	DECLARE_EASYSIZE

public:
	CProductNew3Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProductNew3Dlg();

// Dialog Data
	enum { IDD = IDD_PRODUCTNEW3_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();

	vector<CDialogEx*> m_pPageList;   //记录各子对话框
	int m_nCurrentPage;               //当前显示页
	bool bInfoWrited;                 //产品信息是否已写入
	CString m_EvalType;               //评价类型名称
	CString str_EvalTypeID;           //评价类型ID
	CString m_TypeInfo;               //评价类型介绍

	CString strProductID;             //list获取的产品ID,此处为CString型
	CRect m_rectPanel;                //子对话框的包围圈
	void ShowPage(UINT nPos);         //显示nPos页面从0开始索引
	void SetWizButton(UINT nPos);     //根据页面情况设置按钮状态
	void SetTextShow(UINT nPos);      //根据页面情况设置文本显示

	afx_msg void OnBnClickedEvalpre3();
	afx_msg void OnBnClickedEvalnext3();
	afx_msg void OnBnClickedEvalin3();

	//状态显示
	CStatic m_csCaption;
	CButtonST m_btnChart30;
	CButtonST m_btnChart31;
	CButtonST m_btnChart32;
	CButtonST m_btnChart33;
	CButtonST m_btnChart34;


	//计算相关
	vector<CString> m_Lvl1TechNam;                      //第一层工艺指标名
	vector<vector<CString>> m_Lvl2TechNam;              //第二层工艺指标名
	RowVectorXd m_dB1; //评价整体对评价集的隶属度
	RowVectorXd m_dB2; ////技术指标对评价集的隶属度
	VectorXd m_dC1; //一级指标评分值
	VectorXd m_dC2; //二级指标评分值
	double m_W;//最终评分值

	void GetIndexVal(VectorXd& dA1,VectorXd& dA2);                   //获取指标评分
	double MinVal(vector<double>& IndexVal);                         //取最小扣分

	//结果显示相关
	vector<CString>& SetResultVal(vector<CString>& m_ItemVal);      //评价结果赋给结果显示类
	vector<CString> m_ItemVal;            //保存结果显示值
	vector<CLowValItem> m_LowValItem; //保存低分项及改进意见
	vector<CIndexValItem> m_IndexVal;   //保存指标得分
	void SaveResultInfo(vector<CLowValItem>& m_LowValItem,vector<CIndexValItem>& m_IndexVal);          //给m_LowValItem、m_IndexVal赋值
};
