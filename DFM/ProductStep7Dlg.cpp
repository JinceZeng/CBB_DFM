// ProductStep7Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "DFM.h"
#include "ProductStep7Dlg.h"
#include "afxdialogex.h"


// CProductStep7Dlg dialog

IMPLEMENT_DYNAMIC(CProductStep7Dlg, CDialogEx)

CProductStep7Dlg::CProductStep7Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProductStep7Dlg::IDD, pParent)
	,m_LowValItemNum(0)
{
	m_MatInfo.clear();
	m_MatType.clear();
	m_ListCtrlItem.clear();
}

CProductStep7Dlg::~CProductStep7Dlg()
{
	vector<vector<CString>>().swap(m_MatInfo);
	vector<CString>().swap(m_MatType);
	vector<CMatChartItem1>().swap(m_ListCtrlItem);
	vector<CLowValItem>().swap(m_LowValItem);
}

void CProductStep7Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MAT1, m_MatInfoList);
}


BEGIN_MESSAGE_MAP(CProductStep7Dlg, CDialogEx)
	ON_MESSAGE(WM_DETELE_INDEXITEM,&CProductStep7Dlg::OnDeleteIndexItem)
	ON_MESSAGE(WM_ADD_INDEXITEM,&CProductStep7Dlg::OnAddIndexItem)
	ON_MESSAGE(WM_SETINDEXVAL,&CProductStep7Dlg::OnSetIndexVal)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//控件大小位置变化
BEGIN_EASYSIZE_MAP(CProductStep7Dlg, CDialogEx) 
	EASYSIZE(IDC_LIST_MAT1,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
END_EASYSIZE_MAP 
// CProductStep7Dlg message handlers
DWORD CProductStep7Dlg::OnWizardActive()
{
	ShowWindow(SW_SHOW);
	return 0;
}

/////////////////////////////////////////////前往下一页
//可以检验并保存当前工作,返回-1不切换，返回0切换
DWORD CProductStep7Dlg::OnWizardNext()
{
	int n=m_MatInfoList.GetItemCount();
	if (n>0&&(m_MatInfoList.GetItemText(n-1,1)==CString("")||m_MatInfoList.GetItemText(n-1,2)==CString("")||m_MatInfoList.GetItemText(n-1,3)==CString("")||m_MatInfoList.GetItemText(n-1,4)==CString("")))
	{
		AfxMessageBox(CString("请先完善材料信息再添加！"));
		return -1;
	}
	if (n>0)   //保存最后一条材料信息
	{
		m_ListCtrlItem[n-1].m_MatNam=m_MatInfoList.GetItemText(n-1,1);
		m_ListCtrlItem[n-1].m_Thick=m_MatInfoList.GetItemText(n-1,2);
		m_ListCtrlItem[n-1].m_Length=m_MatInfoList.GetItemText(n-1,3);
		m_ListCtrlItem[n-1].m_Width=m_MatInfoList.GetItemText(n-1,4);
	}
	if(m_MatInfoList.m_bEditing==TRUE)
	{
		MessageBox(_T("错误:列表控件处于编辑状态"));
		return -1;
	}
	if(m_ListCtrlItem.size()==0)
	{
		AfxMessageBox(CString("请添加使用材料"));
		return -1;
	}
	SaveChartInfo(); //保存输入操作
	SaveLowValItem();//获取分值、保存低分项

	ShowWindow(SW_HIDE);
	return 0;

}

/////////////////////////////////////////前往上一页
//可以检验上一步工作不用保存
DWORD CProductStep7Dlg::OnWizardPrevious()
{
	if(m_MatInfoList.m_bEditing==TRUE)
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

BOOL CProductStep7Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rect1;
	int width1;
	m_MatInfoList.GetClientRect(&rect1);
	width1=rect1.Width();

	m_MatInfoList.InsertColumn(0,CString("序号"),LVCFMT_CENTER,width1/5);
	m_MatInfoList.InsertColumn(1,CString("材料名称"),LVCFMT_CENTER,width1/5);
	m_MatInfoList.InsertColumn(2,CString("厚度"),LVCFMT_CENTER,width1/5);
	m_MatInfoList.InsertColumn(3,CString("长度"),LVCFMT_CENTER,width1/5);
	m_MatInfoList.InsertColumn(4,CString("宽度"),LVCFMT_CENTER,width1/5);


	//设置不可编辑列
	vector<int> nNoEdit;
	nNoEdit.push_back(0);               //第零列不可以编辑
	nNoEdit.push_back(1);  
	m_MatInfoList.SetnNoEditList(nNoEdit);
	vector<int>().swap(nNoEdit);//释放vector

	vector<int> nComboLis;
	nComboLis.push_back(1);             //第三列有组合框控制
	m_MatInfoList.SetnComboList(nComboLis);
	vector<int>().swap(nComboLis);//释放vector

	INIT_EASYSIZE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CProductStep7Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	LockWindowUpdate();
	if(m_MatInfoList.m_hWnd != NULL)
	{
		CRect rc;
		m_MatInfoList.GetClientRect(rc);
		//m_ProductInfoList.MoveWindow(rc);
		//
		int nScrollWidth = GetSystemMetrics(SM_CXVSCROLL) + 1;
		int nWidth = rc.Width() - nScrollWidth - 2;
		if(nWidth > 200)
		{
			m_MatInfoList.SetColumnWidth(0,nWidth/5);
			m_MatInfoList.SetColumnWidth(1,nWidth/5);
			m_MatInfoList.SetColumnWidth(2,nWidth/5);
			m_MatInfoList.SetColumnWidth(3,nWidth/5);
			m_MatInfoList.SetColumnWidth(4,nWidth/5);
		}  
	}
	UPDATE_EASYSIZE;
	UnlockWindowUpdate(); 
}

LRESULT CProductStep7Dlg::OnDeleteIndexItem(WPARAM wParam,LPARAM lParam)
{
	if(m_MatInfoList.m_bEditing)
	{
		AfxMessageBox(CString("正在进行编辑项，无法删除"));
		return 0;
	}
	else if (m_ListCtrlItem.size()==0)
		return 0;

	m_ListCtrlItem.pop_back();       //弹出最后一项
	int n=m_MatInfoList.GetItemCount();
	m_MatInfoList.DeleteItem(n-1);
	return 0;
}


LRESULT CProductStep7Dlg::OnAddIndexItem(WPARAM wParam,LPARAM lParam)
{
	int n=m_MatInfoList.GetItemCount();
	if (n>0&&(m_MatInfoList.GetItemText(n-1,1)==CString("")||m_MatInfoList.GetItemText(n-1,2)==CString("")||m_MatInfoList.GetItemText(n-1,3)==CString("")||m_MatInfoList.GetItemText(n-1,4)==CString("")))
	{
		AfxMessageBox(CString("请先完善材料信息再添加！"));
		return 0;
	}
	if (n>0)   //保存上一条材料信息
	{
		m_ListCtrlItem[n-1].m_MatNam=m_MatInfoList.GetItemText(n-1,1);
		m_ListCtrlItem[n-1].m_Thick=m_MatInfoList.GetItemText(n-1,2);
		m_ListCtrlItem[n-1].m_Length=m_MatInfoList.GetItemText(n-1,3);
		m_ListCtrlItem[n-1].m_Width=m_MatInfoList.GetItemText(n-1,4);
	}
	CString  str1;
	str1.Format(_T("%d"),n+1);
	m_MatInfoList.InsertItem(n,str1);   //填充ID第一项
	m_MatInfoList.SetItemText(n,1,_T(""));
	m_MatInfoList.SetItemText(n,2,_T(""));
	m_MatInfoList.SetItemText(n,3,_T(""));
	m_MatInfoList.SetItemText(n,4,_T(""));

	m_MatInfoList.SetComboString(m_MatType);   //将combo字符赋给listctrl扩展类中的m_strlisCombo用于combo初始化

	//保存到m_ListCtrlItem
	CMatChartItem1 OneItem;//临时存储每条list的信息
	OneItem.m_Item=str1;
	m_ListCtrlItem.push_back(OneItem);

	return 0;
}


void CProductStep7Dlg::ReadTechChart()
{
	m_MatInfo.clear();
	m_MatType.clear();
	_RecordsetPtr m_pRs;
	CString sql = CString("select * from Material1");
	m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
	while (!m_pRs->adoEOF)
	{
		vector<CString> vtemp;
		CString str1 = (CString)(m_pRs->GetCollect("MaterialName"));
		CString str2 = (CString)(m_pRs->GetCollect("Thickness"));
		CString str3 = (CString)(m_pRs->GetCollect("Length"));
		CString str4 = (CString)(m_pRs->GetCollect("Width"));
		CString str5 = (CString)(m_pRs->GetCollect("DeductVal"));


		vtemp.push_back(str1);
		vtemp.push_back(str2);
		vtemp.push_back(str3);
		vtemp.push_back(str4);
		vtemp.push_back(str5);

		m_MatType.push_back(str1);       //单独存储用于combo初始化
		m_MatInfo.push_back(vtemp);

		vector<CString>().swap(vtemp);//用完释放掉

		m_pRs->MoveNext();

	}
}


LRESULT CProductStep7Dlg::OnSetIndexVal(WPARAM wParam,LPARAM lParam)
{
	int n=m_MatInfoList.GetItemCount();

	CString strValInfo=m_MatInfoList.GetItemText(wParam,1);
	for(int i=0; i<n-1;i++)           //进行重复判断
	{
		CString str=m_MatInfoList.GetItemText(i,1);
		if(str==strValInfo)   
		{
			AfxMessageBox(CString("所选材料重复！"));
			m_ListCtrlItem.pop_back();       //弹出最后一项
			int n=m_MatInfoList.GetItemCount();
			m_MatInfoList.DeleteItem(n-1);
			return 0;
		}
	}

	return 0;
}



//存储低分项(待m_ListCtrlItem赋值完成后调用)
void CProductStep7Dlg::SaveLowValItem()
{
	m_LowValItem.clear();
	m_LowValItemNum=0;
	for (int i=0;i<m_ListCtrlItem.size();++i)
	{
		CString strMatNam=m_ListCtrlItem[i].m_MatNam;//提取评分
		CString strThick=m_ListCtrlItem[i].m_Thick;
		CString strLength=m_ListCtrlItem[i].m_Length;
		CString strWidth=m_ListCtrlItem[i].m_Width;
		int nThick= _ttoi(strThick),nLength= _ttoi(strLength),nWidth= _ttoi(strWidth);

		int k=0;                               //匹配数据库中材料尺寸上限，得出分值
		for (;k<m_MatInfo.size();++k)
		{
			if(m_MatInfo[k][0]==strMatNam) break;
		} 
		int nThickLimit=_ttoi(m_MatInfo[k][1]),nLengthLimit=_ttoi(m_MatInfo[k][2]),nWidthLimit=_ttoi(m_MatInfo[k][3]);
		CString strDeductVal;
		if (nThick>nThickLimit||nLength>nLengthLimit||nWidth>nWidthLimit)
			strDeductVal=m_MatInfo[k][4];
		else
			strDeductVal=CString("0");

		m_ListCtrlItem[i].m_MatScore=strDeductVal; //存储该项得分

		CLowValItem OneLowValItem;      //判断低分项，确定建议
		int nDeductVal= _ttoi(strDeductVal);
		if (nDeductVal<-1)
		{
			CString strComment;
			switch(nDeductVal)
			{
			case 0:
				strComment=CString("推荐");
				break;
			case -1:
				strComment=CString("可用");
				break;
			case -2:
				strComment=CString("限用");
				break;
			case -3:
				strComment=CString("不可用");
				break;
			}
			CString str;
			str.Format(CString("%d"),m_LowValItemNum+1);
			OneLowValItem.m_Item=str;
			OneLowValItem.m_ChartNam=CString("材料可取性");
			OneLowValItem.m_Classify=CString("材料");
			OneLowValItem.m_TechEvalIndex=m_ListCtrlItem[i].m_MatNam;
			OneLowValItem.m_IndexComment=strComment;
			OneLowValItem.m_LowValAdvice=CString("请重新选择材料");

			m_LowValItem.push_back(OneLowValItem);

			++m_LowValItemNum;
		}
	}
}



//保存表的输入信息，下次进入直接初始化
void CProductStep7Dlg::SaveChartInfo()
{
	_RecordsetPtr m_pRs;  //先删除之前记录信息，避免数据重复储存
	CString sql = CString("delete * from ChartInfoSave where ProductNam='") + m_ProductInfo.m_ProductName+CString("'and ProductSub='")+m_ProductInfo.m_ProductSub+CString("'and ChartName='")+CString("材料可取性")+CString("'and Uname='")+theApp.name+("'");
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
			theApp.m_pConnect->Execute((_bstr_t)(CString("insert into ChartInfoSave(ProductNam,ProductSub,Uname,ChartName,Row1,Row2,Row3,Row4) values('")
				+m_ProductInfo.m_ProductName+"','"+m_ProductInfo.m_ProductSub+"','"+theApp.name+"','"+CString("材料可取性")+"','"+m_ListCtrlItem[i].m_MatNam+"','"+m_ListCtrlItem[i].m_Length+"','"+m_ListCtrlItem[i].m_Width+"','"+m_ListCtrlItem[i].m_Thick+("')")) , NULL, adCmdText);   //insert操作时数据库中对应字符型需加‘’
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
void CProductStep7Dlg::InitChartInfo()
{
	_RecordsetPtr m_pRs;    //读取历史评价输入信息
	CMatChartItem1 OneItem;//临时存储每条list的信息
	CString sql = CString("select * from ChartInfoSave where ProductSub='") + m_ProductInfo.m_ProductSub+CString("'and ChartName= '")+CString("材料可取性")+CString("'and Uname= '")+theApp.name+CString("'");
	m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
	int k=0;
	while (!m_pRs->adoEOF)
	{
		CString  str1;
		str1.Format(_T("%d"),k+1);
		OneItem.m_Item=str1;
		OneItem.m_MatNam =  m_pRs->GetCollect("Row1");  //只需补上之前的输入项即可
		OneItem.m_Length =  m_pRs->GetCollect("Row2");
		OneItem.m_Width  =  m_pRs->GetCollect("Row3");
		OneItem.m_Thick  =  m_pRs->GetCollect("Row4");

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
			m_MatInfoList.InsertItem(i,m_ListCtrlItem[i].m_Item);
			m_MatInfoList.SetItemText(i,1,m_ListCtrlItem[i].m_MatNam);
			m_MatInfoList.SetItemText(i,2,m_ListCtrlItem[i].m_Length);
			m_MatInfoList.SetItemText(i,3,m_ListCtrlItem[i].m_Width);
			m_MatInfoList.SetItemText(i,4,m_ListCtrlItem[i].m_Thick);

			m_MatInfoList.SetComboString(m_MatType);   //将combo字符赋给listctrl扩展类中的m_strlisCombo用于combo初始化

		}
	}
}


