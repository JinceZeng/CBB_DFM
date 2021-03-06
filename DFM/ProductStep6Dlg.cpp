// ProductStep6Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "DFM.h"
#include "ProductStep6Dlg.h"
#include "afxdialogex.h"


// CProductStep6Dlg dialog

IMPLEMENT_DYNAMIC(CProductStep6Dlg, CDialogEx)

CProductStep6Dlg::CProductStep6Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProductStep6Dlg::IDD, pParent)
	,m_LowValItemNum(0)
{
	m_TechMaturityInfo.clear();
	m_TechType.clear();
	m_ListCtrlItem.clear();
}

CProductStep6Dlg::~CProductStep6Dlg()
{
	vector<vector<CString>>().swap(m_TechMaturityInfo);
	vector<CString>().swap(m_TechType);
	vector<CMaturyChartItem>().swap(m_ListCtrlItem);
	vector<CLowValItem>().swap(m_LowValItem);
}

void CProductStep6Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TECHMATURY, m_TechMaturyList);
}


BEGIN_MESSAGE_MAP(CProductStep6Dlg, CDialogEx)
	ON_MESSAGE(WM_DETELE_INDEXITEM,&CProductStep6Dlg::OnDeleteIndexItem)
	ON_MESSAGE(WM_ADD_INDEXITEM,&CProductStep6Dlg::OnAddIndexItem)
	ON_MESSAGE(WM_SETINDEXVAL,&CProductStep6Dlg::OnSetIndexVal)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//控件大小位置变化
BEGIN_EASYSIZE_MAP(CProductStep6Dlg, CDialogEx) 
	EASYSIZE(IDC_LIST_TECHMATURY,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
END_EASYSIZE_MAP 

// CProductStep6Dlg message handlers
/////////////////////////////////////激活当前页(从上一页或下一页转到本页都会调用)
//由于只有一个结构信息，利用CListCtl的缓存可以保存信息，不用自己更新CListCtrl
DWORD CProductStep6Dlg::OnWizardActive()
{
	//接受信息输入，完成初始化工作
	//m_TechMaturyList.DeleteAllItems();
	//m_ListCtrlItem.clear();
	//m_LowValItem.clear();
	//m_LowValItemNum=0;      //窗口激活就重新计数

	ShowWindow(SW_SHOW);
	return 0;
}

/////////////////////////////////////////////前往下一页
//可以检验并保存当前工作,返回-1不切换，返回0切换
DWORD CProductStep6Dlg::OnWizardNext()
{
	if(m_ListCtrlItem.size()==0)
	{
		AfxMessageBox(CString("请添加使用工艺"));
		return -1;
	}
	if(m_TechMaturyList.m_bEditing==TRUE)
	{
		MessageBox(_T("错误:列表控件处于编辑状态"));
		return -1;
	}
	for (int i=0;i<m_ListCtrlItem.size();++i)
	{
		if(m_ListCtrlItem[i].m_TechNam==CString(""))
		{
			AfxMessageBox(CString("请完成工艺信息"));
			return -1;
		}
	}
	SaveChartInfo(); //保存输入操作
	SaveLowValItem();//先保存低分项

	ShowWindow(SW_HIDE);
	return 0;

}

/////////////////////////////////////////前往上一页
//可以检验上一步工作不用保存
DWORD CProductStep6Dlg::OnWizardPrevious()
{
	if(m_TechMaturyList.m_bEditing==TRUE)
	{
		MessageBox(_T("错误:列表控件处于编辑状态"));
		return -1;
	}
	else
	{
		//m_bLegal=FALSE;
		ShowWindow(SW_HIDE);
		return 0;
	}
}


BOOL CProductStep6Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rect1;
	int width1;
	m_TechMaturyList.GetClientRect(&rect1);
	width1=rect1.Width();

	m_TechMaturyList.InsertColumn(0,CString("序号"),LVCFMT_CENTER,width1/8);
	m_TechMaturyList.InsertColumn(1,CString("工艺名称"),LVCFMT_CENTER,width1*3/8);
	m_TechMaturyList.InsertColumn(2,CString("所内应用状态"),LVCFMT_CENTER,width1/4);
	m_TechMaturyList.InsertColumn(3,CString("工艺成熟度评分"),LVCFMT_CENTER,width1/4);


	//设置不可编辑列
	vector<int> nNoEdit;
	nNoEdit.push_back(0);               //第零列不可以编辑
	nNoEdit.push_back(1);  
	nNoEdit.push_back(2); 
	nNoEdit.push_back(3); 
	m_TechMaturyList.SetnNoEditList(nNoEdit);
	vector<int>().swap(nNoEdit);//释放vector

	vector<int> nComboLis;
	nComboLis.push_back(1);            //第三列有组合框控制
	m_TechMaturyList.SetnComboList(nComboLis);
	vector<int>().swap(nComboLis);//释放vector


	INIT_EASYSIZE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CProductStep6Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	LockWindowUpdate();
	if(m_TechMaturyList.m_hWnd != NULL)
	{
		CRect rc;
		m_TechMaturyList.GetClientRect(rc);
		//m_ProductInfoList.MoveWindow(rc);
		//
		int nScrollWidth = GetSystemMetrics(SM_CXVSCROLL) + 1;
		int nWidth = rc.Width() - nScrollWidth - 2;
		if(nWidth > 200)
		{
			m_TechMaturyList.SetColumnWidth(0,nWidth/8);
			m_TechMaturyList.SetColumnWidth(1,nWidth*3/8);
			m_TechMaturyList.SetColumnWidth(2,nWidth/4);
			m_TechMaturyList.SetColumnWidth(3,nWidth/4);
		}  
	}
	UPDATE_EASYSIZE;
	UnlockWindowUpdate(); 
}


LRESULT CProductStep6Dlg::OnDeleteIndexItem(WPARAM wParam,LPARAM lParam)
{
	if(m_TechMaturyList.m_bEditing)
	{
		AfxMessageBox(CString("正在进行编辑项，无法删除"));
		return 0;
	}
	else if (m_ListCtrlItem.size()==0)
		return 0;

	m_ListCtrlItem.pop_back();       //弹出最后一项
	int n=m_TechMaturyList.GetItemCount();
	m_TechMaturyList.DeleteItem(n-1);
	return 0;
}

LRESULT CProductStep6Dlg::OnAddIndexItem(WPARAM wParam,LPARAM lParam)
{
	int n=m_TechMaturyList.GetItemCount();
	if (n>0&&m_TechMaturyList.GetItemText(n-1,1)==CString(""))
	{
		AfxMessageBox(CString("请先选择工艺再添加！"));
		return 0;
	}
	CString  str1;
	str1.Format(_T("%d"),n+1);
	m_TechMaturyList.InsertItem(n,str1);   //填充ID第一项
	m_TechMaturyList.SetItemText(n,1,_T(""));
	m_TechMaturyList.SetItemText(n,2,_T(""));
	m_TechMaturyList.SetItemText(n,3,_T(""));

	m_TechMaturyList.SetComboString(m_TechType);   //将combo字符赋给listctrl扩展类中的m_strlisCombo用于combo初始化

	//保存到m_ListCtrlItem
	CMaturyChartItem OneItem;//临时存储每条list的信息
	OneItem.nItem=str1;
	m_ListCtrlItem.push_back(OneItem);

	return 0;
}


void CProductStep6Dlg::ReadTechChart(CProductInfo& m_ProductInfo)
{
	m_TechMaturityInfo.clear();
	m_TechType.clear();
	_RecordsetPtr m_pRs;
	CString sql = CString("select * from TechMaturity where EvalModelID=")+m_ProductInfo.str_EvalType;
	try
	{
		m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
	}
	catch(_com_error e)
	{
		CString temp;
		temp.Format(e.Description());
		AfxMessageBox(temp);
	}
	while (!m_pRs->adoEOF)
	{
		vector<CString> vtemp;
		CString str1 = (CString)(m_pRs->GetCollect("TechName"));
		CString str2 = (CString)(m_pRs->GetCollect("TechMaturityVal"));
		CString str3 = (CString)(m_pRs->GetCollect("UseStatus"));

		vtemp.push_back(str1);
		vtemp.push_back(str2);
		vtemp.push_back(str3);

		m_TechType.push_back(str1);       //单独存储用于combo初始化
		m_TechMaturityInfo.push_back(vtemp);

		vector<CString>().swap(vtemp);//用完释放掉

		m_pRs->MoveNext();

	}
}
//对应分值的输入
LRESULT CProductStep6Dlg::OnSetIndexVal(WPARAM wParam,LPARAM lParam)
{
	int n=m_TechMaturyList.GetItemCount();

	CString strValInfo=m_TechMaturyList.GetItemText(wParam,1);
	for(int i=0; i<n-1;i++)           //进行重复判断
	{
		CString str=m_TechMaturyList.GetItemText(i,1);
		if(str==strValInfo)   
		{
			AfxMessageBox(CString("所选工艺重复！"));
			m_ListCtrlItem.pop_back();       //弹出最后一项
			int n=m_TechMaturyList.GetItemCount();
			m_TechMaturyList.DeleteItem(n-1);
			return 0;
		}
	}
	int i=0;
	for (;i<m_TechMaturityInfo.size();++i)
	{
		if(m_TechMaturityInfo[i][0]==strValInfo) break;
	}

	//逐条保存
	m_ListCtrlItem[n-1].m_TechNam=strValInfo;
	m_ListCtrlItem[n-1].m_TechUseStatus=m_TechMaturityInfo[i][2];
	m_ListCtrlItem[n-1].m_TechMaturyVal=m_TechMaturityInfo[i][1];

	m_TechMaturyList.SetItemText(wParam,2,m_ListCtrlItem[n-1].m_TechUseStatus);                    //设置使用状态
	m_TechMaturyList.SetItemText(wParam,3,m_ListCtrlItem[n-1].m_TechMaturyVal);                    //设置分值

	return 0;
}

//存储低分项(待m_ListCtrlItem赋值完成后调用)
void CProductStep6Dlg::SaveLowValItem()
{
	m_LowValItem.clear();
	m_LowValItemNum=0;
	for (int i=0;i<m_ListCtrlItem.size();++i)
	{
		CString strVal=m_ListCtrlItem[i].m_TechMaturyVal;//提取评分
		CLowValItem OneLowValItem;
		int nDeductVal= _ttoi(strVal)-3;

		CString strDeductVal;
		strDeductVal.Format(CString("%d"),nDeductVal);
		if (nDeductVal<-1)
		{
			CString strComment;
			switch(nDeductVal)
			{
			case 0:
				strComment=CString("非常成熟");
				break;
			case -1:
				strComment=CString("成熟");
				break;
			case -2:
				strComment=CString("不成熟");
				break;
			case -3:
				strComment=CString("不可用");
				break;
			}
			CString str;
			str.Format(CString("%d"),m_LowValItemNum+1);
			OneLowValItem.m_Item=str;
			OneLowValItem.m_ChartNam=CString("工艺成熟度表");
			OneLowValItem.m_Classify=CString("工艺");
			OneLowValItem.m_TechEvalIndex=m_ListCtrlItem[i].m_TechNam;
			OneLowValItem.m_IndexComment=strComment;
			OneLowValItem.m_LowValAdvice=CString("请重新选择工艺");

			m_LowValItem.push_back(OneLowValItem);

			++m_LowValItemNum;
		}
	}
}



//保存表的输入信息，下次进入直接初始化
void CProductStep6Dlg::SaveChartInfo()
{
	_RecordsetPtr m_pRs;  //先删除之前记录信息，避免数据重复储存
	CString sql = CString("delete * from ChartInfoSave where ProductNam='") + m_ProductInfo.m_ProductName+CString("'and ProductSub='")+m_ProductInfo.m_ProductSub+CString("'and ChartName='")+CString("工艺成熟度表")+CString("'and Uname='")+theApp.name+("'");
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
			theApp.m_pConnect->Execute((_bstr_t)(CString("insert into ChartInfoSave(ProductNam,ProductSub,Uname,ChartName,Row1,Row2,Row3) values('")
				+m_ProductInfo.m_ProductName+"','"+m_ProductInfo.m_ProductSub+"','"+theApp.name+"','"+CString("工艺成熟度表")+"','"+m_ListCtrlItem[i].m_TechNam+"','"+m_ListCtrlItem[i].m_TechUseStatus+"','"+m_ListCtrlItem[i].m_TechMaturyVal+("')")) , NULL, adCmdText);   //insert操作时数据库中对应字符型需加‘’
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
void CProductStep6Dlg::InitChartInfo()
{
	_RecordsetPtr m_pRs;    //读取历史评价输入信息
	CMaturyChartItem OneItem;//临时存储每条list的信息
	CString sql = CString("select * from ChartInfoSave where ProductSub='") + m_ProductInfo.m_ProductSub+CString("'and ChartName= '")+CString("工艺成熟度表")+CString("'and Uname= '")+theApp.name+CString("'");
	m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
	int k=0;
	while (!m_pRs->adoEOF)
	{
		CString  str1;
		str1.Format(_T("%d"),k+1);
		OneItem.nItem=str1;
		OneItem.m_TechNam       =  m_pRs->GetCollect("Row1");  //只需补上之前的输入项即可
		OneItem.m_TechUseStatus =  m_pRs->GetCollect("Row2");
		OneItem.m_TechMaturyVal =  m_pRs->GetCollect("Row3");

		m_ListCtrlItem.push_back(OneItem);
		++k;
		m_pRs->MoveNext();
	}
	if (k==0)        //如果无该目标则直接跳出
		return;
	else             //复原list条目
	{
		for (int i=0;i<m_ListCtrlItem.size();i++)
		{
			m_TechMaturyList.InsertItem(i,m_ListCtrlItem[i].nItem);
			m_TechMaturyList.SetItemText(i,1,m_ListCtrlItem[i].m_TechNam);
			m_TechMaturyList.SetItemText(i,2,m_ListCtrlItem[i].m_TechUseStatus);
			m_TechMaturyList.SetItemText(i,3,m_ListCtrlItem[i].m_TechMaturyVal);

			m_TechMaturyList.SetComboString(m_TechType);   //将combo字符赋给listctrl扩展类中的m_strlisCombo用于combo初始化


		}
	}
}


