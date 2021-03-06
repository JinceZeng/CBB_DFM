// ProductStep1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "DFM.h"
#include "ProductStep1Dlg.h"
#include "afxdialogex.h"
#include "goldata.h"
#include "MatchChart1Dlg.h"
#include "MatchChart2Dlg.h"
#include "MatchChart3Dlg.h"

// CProductStep1Dlg dialog

IMPLEMENT_DYNAMIC(CProductStep1Dlg, CDialogEx)

CProductStep1Dlg::CProductStep1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProductStep1Dlg::IDD, pParent)
	,m_bLegal(false)
	,m_LowValItemNum(0) //初始为零个
{
	m_Lvl3TechID.clear();
	//m_Lvl4TechID.clear();
	//m_Lvl4TechWeight.clear();
	m_IndexValInfo.clear();
	m_ListCtrlItem.clear();

}

CProductStep1Dlg::~CProductStep1Dlg()
{

	vector<CString>().swap(m_Lvl3TechID);
	vector<CTechChartItem>().swap(m_ListCtrlItem);
	vector<vector<CString>>().swap(m_IndexValInfo);
	vector<CLowValItem>().swap(m_LowValItem);

}

void CProductStep1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TECHVAL, m_TechValList);
}


BEGIN_MESSAGE_MAP(CProductStep1Dlg, CDialogEx)
	ON_MESSAGE(WM_SETINDEXVAL,&CProductStep1Dlg::OnSetIndexVal)
	ON_MESSAGE(WM_INDEXMATCH,&CProductStep1Dlg::OnIndexMatch)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//控件大小位置变化
BEGIN_EASYSIZE_MAP(CProductStep1Dlg, CDialogEx) 
	EASYSIZE(IDC_LIST_TECHVAL,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
END_EASYSIZE_MAP 
// CProductStep1Dlg message handlers


BOOL CProductStep1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	//将列表控件先初始化,插入表头
	CRect rect1;
	int width1;
	m_TechValList.GetClientRect(&rect1);
	width1=rect1.Width();

	m_TechValList.InsertColumn(0,CString("序号"),LVCFMT_CENTER,width1/10);
	m_TechValList.InsertColumn(1,CString("类别"),LVCFMT_CENTER,width1/10);
	m_TechValList.InsertColumn(2,CString("评分项"),LVCFMT_CENTER,width1*3/10);
	m_TechValList.InsertColumn(3,CString("评分项特征"),LVCFMT_CENTER,width1*2/5);
	m_TechValList.InsertColumn(4,CString("评分值"),LVCFMT_CENTER,width1/10);


	//设置不可编辑列
	vector<int> nNoEdit;
	nNoEdit.push_back(0);               //第零列不可以编辑
	nNoEdit.push_back(1);  
	nNoEdit.push_back(2);  
	nNoEdit.push_back(3);  
	m_TechValList.SetnNoEditList(nNoEdit);
	vector<int>().swap(nNoEdit);//释放vector

	vector<int> nComboLis;
	nComboLis.push_back(3);            //第三列有组合框控制
	m_TechValList.SetnComboList(nComboLis);
	vector<int>().swap(nComboLis);//释放vector

	INIT_EASYSIZE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CProductStep1Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	LockWindowUpdate();
	if(m_TechValList.m_hWnd != NULL)
	{
		CRect rc;
		m_TechValList.GetClientRect(rc);
		//m_ProductInfoList.MoveWindow(rc);
		//
		int nScrollWidth = GetSystemMetrics(SM_CXVSCROLL) + 1;
		int nWidth = rc.Width() - nScrollWidth - 2;
		if(nWidth > 200)
		{
			m_TechValList.SetColumnWidth(0,nWidth/10);
			m_TechValList.SetColumnWidth(1,nWidth/10);
			m_TechValList.SetColumnWidth(2,nWidth*3/10);
			m_TechValList.SetColumnWidth(3,nWidth*2/5);
			m_TechValList.SetColumnWidth(4,nWidth/10);
		}  
	}
	UPDATE_EASYSIZE;
	UnlockWindowUpdate(); 

}



//激活执行初始化操作
DWORD CProductStep1Dlg::OnWizardActive()
{
	//接收外界信息,可以完成一些初始化工作

	//ReadTechChart();
	//ShowListCtrl(m_Lvl4TechID);
	//m_LowValItem.clear();
	//m_LowValItemNum=0;      //窗口激活就重新计数  (//////不能写在这里，上一步也会调用清空)
	ShowWindow(SW_SHOW);
	return 0;
}

//可以检验并保存当前工作
DWORD CProductStep1Dlg::OnWizardNext()
{
	//保存当前工作，传递信息
	if(m_TechValList.m_bEditing==TRUE)
	{
		MessageBox(_T("错误:列表控件处于编辑状态"));
		return -1;
	}
	for (int i=0;i<m_ListCtrlItem.size();++i)
	{
		CString str=m_TechValList.GetItemText(i,3);
		if(str==CString(""))
		{
			AfxMessageBox(CString("评分项未完成"));
			return -1;
		}
	}
	SaveChartInfo();                  //保存输入操作
	SaveLowValItem(m_ListCtrlItem);   //若m_ListCtrlItem信息输入完成，则存储其中低分项
	ShowWindow(SW_HIDE);
	return 0;
}

//可以检验上一步工作（不会被调用）
DWORD CProductStep1Dlg::OnWizardPrevious()
{
	if(m_TechValList.m_bEditing==TRUE)
	{
		MessageBox(_T("错误:列表控件处于编辑状态"));
		return -1;
	}
	else
	{
		m_bLegal=FALSE;
		ShowWindow(SW_HIDE);
		return 0;
	}
}


void CProductStep1Dlg::ReadTechChart(CProductInfo& m_ProductInfo)
{
	_RecordsetPtr m_pRs0;
	m_Lvl3TechID.clear();
	int UpID=11;             //搜寻工艺指标（指标ID为11）的子项
	CString strUpID=(CString)(char *)(_bstr_t)(UpID);

	CString sql0 = CString("select * from CreateIndexTree where EvalModelID=")+m_ProductInfo.str_EvalType+CString("and TechEvalIndexUpID=")+strUpID;
	try
	{
	m_pRs0 = theApp.m_pConnect->Execute(_bstr_t(sql0), NULL, adCmdText);
	}
	catch(_com_error e)
	{
		CString temp;
		temp.Format(e.Description());
		AfxMessageBox(temp);
	}
	while (!m_pRs0->adoEOF)
	{
		
		CString str = (CString)(m_pRs0->GetCollect("TechEvalIndexID"));
		m_Lvl3TechID.push_back(str);
		m_pRs0->MoveNext();

	}
	//m_Lvl4TechID.push_back(m_Lvl3TechID);//三级指标也存进去
	//for(int i=0;i<m_Lvl3TechID.size();i++)
	//{
	//	CString sql1 = CString("select * from CreateIndexTree where EvalModelID=")+m_ProductInfo.str_EvalType+CString("and TechEvalIndexUpID=")+m_Lvl3TechID[i];
	//	m_pRs1 = theApp.m_pConnect->Execute(_bstr_t(sql1), NULL, adCmdText);
	//	vector<CString> vtemp1,vtemp2;
	//	while (!m_pRs1->adoEOF)
	//	{

	//		CString str1 = (CString)(m_pRs1->GetCollect("TechEvalIndexID"));
	//		CString str2 = (CString)(m_pRs1->GetCollect("EvalWeight"));
	//		vtemp1.push_back(str1);
	//		vtemp2.push_back(str2);
	//		m_pRs1->MoveNext();

	//	}
	//	m_Lvl4TechID.push_back(vtemp1);
	//	m_Lvl4TechWeight.push_back(vtemp2);
	//}
}



void CProductStep1Dlg::ShowListCtrl()
{
	_RecordsetPtr m_pRs,m_pRs2;
	m_ListCtrlItem.clear();
	m_IndexValInfo.clear();
	//vector<CString> m_Lvl3TechNam;    //存储三级指标名
	CTechChartItem m_OneItem;//每条list的信息
	int nItem=1;             //序号
	//vector<vector<CString>> m_Lvl4TechNam;
	for (int i=0;i<m_Lvl3TechID.size();i++)    //依次访问评价指标表，读取指标相关信息
	{
		CString strItem;
		strItem.Format(CString("%d"),nItem);
		CString sql = CString("select * from TechEvalIndex where TechEvalIndexID=")+m_Lvl3TechID[i];
		try
		{
		m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
		CString strTechNam=(CString)(m_pRs->GetCollect("TechEvalIndexNam"));
		CString strClassify=(CString)(m_pRs->GetCollect("TechClassify"));
		CString strLowValAdvice=(CString)(m_pRs->GetCollect("LowValAdvice"));

		//访问指标评分表，读取评分项信息
		CString sql2 = CString("select * from EvalIndexVal where TechEvalIndexID=")+m_Lvl3TechID[i];
		m_pRs2 = theApp.m_pConnect->Execute(_bstr_t(sql2), NULL, adCmdText);

		vector<CString> m_ValComboStr;  //用于存储评分combo字符
		vector<CString> vTemp;          //临时存储某一指标对应的评分信息
		while(!m_pRs2->adoEOF)
		{
			CString strValInfo=(CString)(m_pRs2->GetCollect("TechEvalIndexValInfo"));
			CString strVal=(CString)(m_pRs2->GetCollect("TechDeductVal"));
			m_ValComboStr.push_back(strValInfo);   //用于combo初始化

			CString str=strValInfo+CString(":")+strVal;//为避免用三维vector，所以这样表示
			vTemp.push_back(str);

			m_pRs2->MoveNext();
		}

		m_IndexValInfo.push_back(vTemp);//存储所有指标的评分信息
		vector<CString>().swap(vTemp);//释放vector

        //每条list信息
		m_OneItem.nItem=strItem;
		m_OneItem.m_Classify=strClassify;
		m_OneItem.m_TechEvalIndex=strTechNam;
		m_OneItem.m_ValComboStr=m_ValComboStr;
		m_OneItem.m_LowValAdvice=strLowValAdvice;

		vector<CString>().swap(m_ValComboStr);//释放vector
		nItem++;
		
		m_ListCtrlItem.push_back(m_OneItem);//依次存入list的每行信息
		}

		catch(_com_error e)
		{
			CString temp;
			temp.Format(e.Description());
			AfxMessageBox(temp);
		}
	}

	//
	//for (int j=1;j<=m_Lvl4TechID[0].size();j++)
	//{
	//	for (int k=0;k<m_Lvl4TechID[j].size();k++)
	//	{
	//		CString strItem;
	//		strItem.Format(CString("%d"),nItem);

	//		CString sql1 = CString("select * from TechEvalIndex where TechEvalIndexID=")+m_Lvl4TechID[j][k];
	//		m_pRs1 = theApp.m_pConnect->Execute(_bstr_t(sql1), NULL, adCmdText);
	//		CString strTechNam=(CString)(m_pRs1->GetCollect("TechEvalIndexNam"));

	//		CString str1=(CString)(m_pRs1->GetCollect("ReqClassify"));
	//		int nReqclass=_ttoi(str1);
	//		CString strReqClassify;
	//		if(nReqclass==1) strReqClassify=CString("基本要求");
	//		else             strReqClassify=CString("进阶要求");

	//	    vector<CString> m_ValComboStr;  //用于存储评分combo字符
	//		CString sql2 = CString("select * from EvalIndexVal where TechEvalIndexID=")+m_Lvl4TechID[j][k];
	//		m_pRs2 = theApp.m_pConnect->Execute(_bstr_t(sql2), NULL, adCmdText);
	//		while(!m_pRs2->adoEOF)
	//		{
	//			CString strValInfo=(CString)(m_pRs2->GetCollect("TechEvalIndexValInfo"));
	//			m_ValComboStr.push_back(strValInfo);
	//			m_pRs2->MoveNext();
	//		}
	//		
	//		//每条list信息
	//		m_OneItem.nItem=strItem;
	//		m_OneItem.m_ChooseTech=m_Lvl3TechNam[j-1];
	//		m_OneItem.m_ReqClassify=strReqClassify;
	//		m_OneItem.m_TechEvalIndex=strTechNam;
	//		m_OneItem.m_ValComboStr=m_ValComboStr;
	//		m_OneItem.m_IndexWeigh=m_Lvl4TechWeight[j-1][k];

	//		nItem++;

	//		m_ListCtrlItem.push_back(m_OneItem);
	//	}
	//	
	//}
	InitChartInfo();   //搜索是否以前进行过输入存储，获取储存的输入数据
	SetListItem(m_ListCtrlItem);
}



void CProductStep1Dlg::SetListItem(vector<CTechChartItem>& m_ListCtrlItem)
{
	m_TechValList.DeleteAllItems();
	for (int i=0;i<m_ListCtrlItem.size();i++)
	{
		m_TechValList.InsertItem(i,m_ListCtrlItem[i].nItem);
		m_TechValList.SetItemText(i,1,m_ListCtrlItem[i].m_Classify);
		m_TechValList.SetItemText(i,2,m_ListCtrlItem[i].m_TechEvalIndex);
		m_TechValList.SetComboString(m_ListCtrlItem[i].m_ValComboStr);   //将combo字符赋给listctrl扩展类中的m_strlisCombo用于combo初始化
		m_TechValList.SetItemText(i,3,m_ListCtrlItem[i].m_ComboStrChoose);
		m_TechValList.SetItemText(i,4,m_ListCtrlItem[i].m_IndexScore);

	}
}



LRESULT CProductStep1Dlg::OnSetIndexVal(WPARAM wParam,LPARAM lParam)
{
	//对应分值的输入
	//CString strIndexNam=m_TechValList.GetItemText(wParam,2);
	CString strValInfo=m_TechValList.GetItemText(wParam,3);
	///*CString sql = CString("select * from TechEvalIndex where TechEvalIndexNam= '")+strIndexNam+CString("'");*/
	//CString sql_tabnam;	
	//sql_tabnam.Format(_T("select * from TechEvalIndex where TechEvalIndexNam='%s'"),strIndexNam);
	//_bstr_t bstrsql_tabnam=(_bstr_t)sql_tabnam;

	//_RecordsetPtr m_pRecordset_tab;
	//m_pRecordset_tab=theApp.m_pConnect->Execute(bstrsql_tabnam, NULL, adCmdText);
	///*m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);*/
	//CString strIndexID=(CString)(m_pRecordset_tab->GetCollect("TechEvalIndexID"));  //查询指标ID
	//m_pRecordset_tab.Release();

	//CString sql1= CString("select * from EvalIndexVal where TechEvalIndexID= ")+strIndexID+CString("and TechEvalIndexValInfo = '")+strValInfo+CString("'");

	//m_pRecordset_tab= theApp.m_pConnect->Execute(_bstr_t(sql1), NULL, adCmdText);
	//CString strDeductVal=(CString)(m_pRecordset_tab->GetCollect("TechDeductVal"));  //查询分值

	CStringArray strArrayNam,strArrayVal;
	//提取指标变量描述
	for (int i=0;i<m_IndexValInfo[wParam].size();++i)
	{
		strArrayNam.Add(m_IndexValInfo[wParam][i].Left(m_IndexValInfo[wParam][i].Find(':')));   //提取指标评分项名
		strArrayVal.Add(m_IndexValInfo[wParam][i].Right(m_IndexValInfo[wParam][i].GetLength()-m_IndexValInfo[wParam][i].Find(':')-1));//提取指标评分项评分
	}

	//进行匹配选取对应评分
	int j=0;
	for (;j<strArrayNam.GetCount();++j)
	{
		if(strArrayNam[j]==strValInfo) break;
	}
	CString strDeductVal=strArrayVal[j];//提取评分
	m_TechValList.SetItemText(wParam,4,strDeductVal);                    //设置分值

	m_ListCtrlItem[wParam].m_ComboStrChoose=strValInfo;                  //存储所选的评分项
	m_ListCtrlItem[wParam].m_IndexScore=strDeductVal;                    //存储分值，用于后期模糊综合分析

	///////////////保存低分项,用于结果显示              //这种写法会使低分项重复保存，且保存顺序会随操作顺序变乱
	//CLowValItem OneLowValItem;
	//int nDeductVal= _ttoi(strDeductVal);
	//if (nDeductVal<0)
	//{
	//	CString str;
	//	str.Format(CString("%d"),m_LowValItemNum+1);
	//	OneLowValItem.m_Item=str;
	//	OneLowValItem.m_ChartNam=CString("工艺性表");
	//	OneLowValItem.m_Classify=m_ListCtrlItem[wParam].m_Classify;
	//	OneLowValItem.m_TechEvalIndex=m_ListCtrlItem[wParam].m_TechEvalIndex;
	//	OneLowValItem.m_IndexScore=m_ListCtrlItem[wParam].m_IndexScore;

	//	int k=0;      //获取评分项评分为0，即不扣分的项作为建议
	//	for (;k<strArrayVal.GetCount();++k)
	//	{
	//		if(strArrayVal[k]==CString("0")) break;
	//	}
	//	OneLowValItem.m_LowValAdvice=strArrayNam[k];

	//	m_LowValItem.push_back(OneLowValItem);

	//	++m_LowValItemNum;
	//}

	return 0;
}


//指标匹配评分
LRESULT CProductStep1Dlg::OnIndexMatch(WPARAM wParam,LPARAM lParam)
{
	CString strIndexName=m_TechValList.GetItemText(wParam,2);
	//匹配表1
	if(strIndexName==CString("转接器型号与导线匹配"))
	{
		CMatchChart1Dlg dlg;
		if(dlg.DoModal()==IDOK)
		{
			if(dlg.isMatch)
			{
				m_TechValList.SetItemText(wParam,3,CString("匹配"));
				m_TechValList.SetItemText(wParam,4,CString("0"));

				m_ListCtrlItem[wParam].m_ComboStrChoose=CString("匹配");             //存储所选的评分项
				m_ListCtrlItem[wParam].m_IndexScore=CString("0");                    //存储分值，用于后期模糊综合分析
			}
			else
			{
				m_TechValList.SetItemText(wParam,3,CString("不匹配"));
				m_TechValList.SetItemText(wParam,4,CString("-3"));

				m_ListCtrlItem[wParam].m_ComboStrChoose=CString("不匹配");             //存储所选的评分项
				m_ListCtrlItem[wParam].m_IndexScore=CString("-3");                     //存储分值，用于后期模糊综合分析
			}
		}
	}
	//匹配表2
	else if(strIndexName==CString("TMS热缩管与线径匹配"))
	{
		CMatchChart2Dlg dlg;
		if(dlg.DoModal()==IDOK)
		{
			if(dlg.isMatch)
			{
				m_TechValList.SetItemText(wParam,3,CString("匹配"));
				m_TechValList.SetItemText(wParam,4,CString("0"));

				m_ListCtrlItem[wParam].m_ComboStrChoose=CString("匹配");              //存储所选的评分项
				m_ListCtrlItem[wParam].m_IndexScore=CString("0");                     //存储分值，用于后期模糊综合分析
			}
			else
			{
				m_TechValList.SetItemText(wParam,3,CString("不匹配"));
				m_TechValList.SetItemText(wParam,4,CString("-3"));

				m_ListCtrlItem[wParam].m_ComboStrChoose=CString("不匹配");             //存储所选的评分项
				m_ListCtrlItem[wParam].m_IndexScore=CString("-3");                     //存储分值，用于后期模糊综合分析
			}
		}
	}
	//匹配表3
	else if(strIndexName==CString("KT透明热缩管与TMS匹配"))
	{
		CMatchChart3Dlg dlg;
		if(dlg.DoModal()==IDOK)
		{
			if(dlg.isMatch)
			{
				m_TechValList.SetItemText(wParam,3,CString("匹配"));
				m_TechValList.SetItemText(wParam,4,CString("0"));

				m_ListCtrlItem[wParam].m_ComboStrChoose=CString("匹配");             //存储所选的评分项
				m_ListCtrlItem[wParam].m_IndexScore=CString("0");                     //存储分值，用于后期模糊综合分析
			}
			else
			{
				m_TechValList.SetItemText(wParam,3,CString("不匹配"));
				m_TechValList.SetItemText(wParam,4,CString("-3"));

				m_ListCtrlItem[wParam].m_ComboStrChoose=CString("不匹配");             //存储所选的评分项
				m_ListCtrlItem[wParam].m_IndexScore=CString("-3");                     //存储分值，用于后期模糊综合分析
			}
		}
	}
	return 0;
}



//存储低分项(待m_ListCtrlItem赋值完成后调用)
void CProductStep1Dlg::SaveLowValItem(vector<CTechChartItem>& m_ListCtrlItem)
{
	m_LowValItem.clear();
	m_LowValItemNum=0;
	for (int i=0;i<m_ListCtrlItem.size();++i)
	{
		CString strDeductVal=m_ListCtrlItem[i].m_IndexScore;//提取评分
		CLowValItem OneLowValItem;
		int nDeductVal= _ttoi(strDeductVal);
		if (nDeductVal<-1)
		{
			CString strComment;
			switch(nDeductVal)
			{
			case 0:
				strComment=CString("优");
				break;
			case -1:
				strComment=CString("良");
				break;
			case -2:
				strComment=CString("差");
				break;
			case -3:
				strComment=CString("不可用");
				break;
			}
			CString str;
			str.Format(CString("%d"),m_LowValItemNum+1);
			OneLowValItem.m_Item=str;
			OneLowValItem.m_ChartNam=CString("工艺性");
			OneLowValItem.m_Classify=m_ListCtrlItem[i].m_Classify;
			OneLowValItem.m_TechEvalIndex=m_ListCtrlItem[i].m_TechEvalIndex;
			OneLowValItem.m_IndexComment=strComment;
			OneLowValItem.m_LowValAdvice=m_ListCtrlItem[i].m_LowValAdvice;

			//CStringArray strArrayNam,strArrayVal;
			//for (int j=0;j<m_IndexValInfo[i].size();++j)
			//{
			//	strArrayNam.Add(m_IndexValInfo[i][j].Left(m_IndexValInfo[i][j].Find(':')));   //提取指标评分项名
			//	strArrayVal.Add(m_IndexValInfo[i][j].Right(m_IndexValInfo[i][j].GetLength()-m_IndexValInfo[i][j].Find(':')-1));//提取指标评分项评分
			//}
		

			//if (OneLowValItem.m_Classify==CString("机箱（匹配）"))    //匹配项的低分项建议另行设置
			//{
			//	OneLowValItem.m_LowValAdvice=CString("匹配");
			//}
			//else                                                      //一般低分项建议设置
			//{
			//	int k=0;      //获取评分项评分为0，即不扣分的项作为建议
			//	for (;k<strArrayVal.GetCount();++k)
			//	{
			//		if(strArrayVal[k]==CString("0")) break;
			//	}
			//	OneLowValItem.m_LowValAdvice=strArrayNam[k];
			//}

			m_LowValItem.push_back(OneLowValItem);

			++m_LowValItemNum;
		}
	}
}


//保存表的输入信息，下次进入直接初始化
void CProductStep1Dlg::SaveChartInfo()
{
	_RecordsetPtr m_pRs;  //先删除之前记录信息，避免数据重复储存
	CString sql = CString("delete * from ChartInfoSave where ProductNam='") + m_ProductInfo.m_ProductName+CString("'and ProductSub='")+m_ProductInfo.m_ProductSub+CString("'and ChartName='")+CString("工艺性表")+CString("'and Uname='")+theApp.name+("'");
	try
	{
		m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
		//AfxMessageBox(CString("产品信息删除成功！"));
	}
	catch(_com_error &e)
	{        
		CString temp;
		temp.Format(e.Description());
		AfxMessageBox(temp);
	}


	//数据存储
	try
	{
		for(int i=0;i<m_ListCtrlItem.size();++i)
		{
		theApp.m_pConnect->Execute((_bstr_t)(CString("insert into ChartInfoSave(ProductNam,ProductSub,Uname,ChartName,Row1,Row2,Row3,Row4,Row5) values('")
			+m_ProductInfo.m_ProductName+"','"+m_ProductInfo.m_ProductSub+"','"+theApp.name+"','"+CString("工艺性表")+"','"+m_ListCtrlItem[i].m_Classify+"','"+m_ListCtrlItem[i].m_TechEvalIndex+"','"+m_ListCtrlItem[i].m_ComboStrChoose+"','"+m_ListCtrlItem[i].m_IndexScore+"','"+m_ListCtrlItem[i].m_LowValAdvice+("')")) , NULL, adCmdText);   //insert操作时数据库中对应字符型需加‘’
		}
	}
	catch(_com_error e)
	{
		CString temp;
		temp.Format(e.Description());
		AfxMessageBox(temp);
		return ;
	}
}

//评分表信息初始化
void CProductStep1Dlg::InitChartInfo()
{
	_RecordsetPtr m_pRs;    //读取历史评价输入信息
	CString sql = CString("select * from ChartInfoSave where ProductSub='") + m_ProductInfo.m_ProductSub+CString("'and ChartName= '")+CString("工艺性表")+CString("'and Uname= '")+theApp.name+CString("'");
	m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
	int k=0;
	while (!m_pRs->adoEOF)
	{
		m_ListCtrlItem[k].m_ComboStrChoose =  m_pRs->GetCollect("Row3");  //只需补上之前的输入项即可
		m_ListCtrlItem[k].m_IndexScore     =  m_pRs->GetCollect("Row4");

		++k;
		m_pRs->MoveNext();
	}
	if (k==0)        //如果无该目标则直接跳出
		return;
}


