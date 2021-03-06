// ProductNew3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "DFM.h"
#include "ProductNew3Dlg.h"
#include "afxdialogex.h"


// CProductNew3Dlg dialog

IMPLEMENT_DYNAMIC(CProductNew3Dlg, CDialogEx)

CProductNew3Dlg::CProductNew3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProductNew3Dlg::IDD, pParent)
	,m_nCurrentPage(0)              //当前显示页
	,bInfoWrited(FALSE)
	,m_EvalType(CString(""))
	,str_EvalTypeID(CString(""))
{
	m_pPageList.clear();            //记录各子对话框
	m_rectPanel=CRect(0,0,0,0);     //子对话框的包围圈
}

CProductNew3Dlg::~CProductNew3Dlg()
{
	int n=m_pPageList.size();

	for(int i=0;i<n;i++)
	{
		delete m_pPageList[i];      //释放内存，在这里释放内存因为要把树信息传出去
	}

	//m_pPageList.clear();            //记录各子对话框
	vector<CDialogEx*>().swap(m_pPageList);//清除容器并最小化它的容量

	//delete m_csCaption;             //释放cstatic指针内存
	vector<CString>().swap(m_ItemVal);
	vector<CString>().swap(m_Lvl1TechNam);
	vector<vector<CString>>().swap(m_Lvl2TechNam);
	vector<CLowValItem>().swap(m_LowValItem);
	vector<CIndexValItem>().swap(m_IndexVal);
}

void CProductNew3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CHART30, m_btnChart30);
	DDX_Control(pDX, IDC_BUTTON_CHART31, m_btnChart31);
	DDX_Control(pDX, IDC_BUTTON_CHART32, m_btnChart32);
	DDX_Control(pDX, IDC_BUTTON_CHART33, m_btnChart33);
	DDX_Control(pDX, IDC_BUTTON_CHART34, m_btnChart34);
}


BEGIN_MESSAGE_MAP(CProductNew3Dlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_EVALPRE3, &CProductNew3Dlg::OnBnClickedEvalpre3)
	ON_BN_CLICKED(IDC_EVALNEXT3, &CProductNew3Dlg::OnBnClickedEvalnext3)
	ON_BN_CLICKED(IDC_EVALIN3, &CProductNew3Dlg::OnBnClickedEvalin3)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//控件大小位置变化
BEGIN_EASYSIZE_MAP(CProductNew3Dlg, CDialogEx) 
	EASYSIZE(IDC_STATIC_PLANE3,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
END_EASYSIZE_MAP 
// CProductNew3Dlg message handlers


BOOL CProductNew3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//获取子对话框的包容矩形
	GetDlgItem(IDC_STATIC_PLANE3)->GetWindowRect(&m_rectPanel);
	ScreenToClient(&m_rectPanel);

	//添加四个属性页

	//////////////////////////////////////////////页面0
	CProductStep0Dlg *pStep0=new CProductStep0Dlg();
	if(pStep0==NULL)
	{
		MessageBox(_T("页面0创建失败"));
		AfxAbort();
	}

	pStep0->Create(IDD_PRODUCTSTEP0_DLG,this);

	if(bInfoWrited)//如果数据库中已经写入，直接进行初始化
	{
		_RecordsetPtr m_pRs,m_pRs1;
		CString sql = CString("select * from ProductInfo where ProductID=") + strProductID ;
		m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
		CString str1 = (CString)m_pRs->GetCollect("ProductNam");
		CString str2 = (CString)m_pRs->GetCollect("ProductNum");
		CString str3 = (CString)(m_pRs->GetCollect("ProductSub")); 

		str_EvalTypeID =  m_pRs->GetCollect("EvalModelID");
		CString str4 = (CString)(m_pRs->GetCollect("EvalTypeIntro")); 
		//int int4 = _ttoi(str44)-1; //combo从零开始-1
		CString sql1 = CString("select * from EvalModelInfo where EvalModelID=")+str_EvalTypeID;
		m_pRs1 = theApp.m_pConnect->Execute(_bstr_t(sql1), NULL, adCmdText);
		m_EvalType = (CString)(m_pRs1->GetCollect("EvalModelNam")); 


		pStep0->m_ProductName=str1;
		pStep0->m_ProductNum=str2;
		pStep0->m_ProductSub=str3;
		pStep0->str_EvalTypeID=str_EvalTypeID;
		pStep0->m_EvalType=m_EvalType;
		pStep0->m_TypeInfo=str4;

		pStep0->SaveProductInfo();//保存产品数据
		::SendMessage(pStep0->m_hWnd,WM_UPDATEDATA,FALSE,FALSE);//更新控件信息
	}
	else
	{
		pStep0->str_EvalTypeID=str_EvalTypeID;
		pStep0->m_EvalType=m_EvalType;
		pStep0->m_TypeInfo=m_TypeInfo;
		::SendMessage(pStep0->m_hWnd,WM_UPDATEDATA,FALSE,FALSE);//更新控件信息

	}
	//显示
	pStep0->MoveWindow(m_rectPanel);
	pStep0->ShowWindow(SW_HIDE);
	m_pPageList.push_back(pStep0);

	//////////////////////////////////////////////页面1
	CProductStep6Dlg *pStep1=new CProductStep6Dlg();
	if(pStep1==NULL)
	{
		MessageBox(_T("页面1创建失败"));
		AfxAbort();
	}

	pStep1->Create(IDD_PRODUCTSTEP6_DLG,this);

	//显示
	pStep1->MoveWindow(m_rectPanel);
	pStep1->ShowWindow(SW_HIDE);
	m_pPageList.push_back(pStep1);



	///////////////////////////////////////////页面2
	CProductStep1Dlg *pStep2=new CProductStep1Dlg();
	if(pStep2==NULL)
	{
		MessageBox(_T("页面2创建失败"));
		AfxAbort();
	}

	pStep2->Create(IDD_PRODUCTSTEP1_DLG,this);

	//显示
	pStep2->MoveWindow(m_rectPanel);
	pStep2->ShowWindow(SW_HIDE);
	m_pPageList.push_back(pStep2);


	////////////////////////////////////////页面3
	CProductStep2Dlg *pStep3=new CProductStep2Dlg();
	if(pStep3==NULL)
	{
		MessageBox(_T("页面3创建失败"));
		AfxAbort();
	}

	pStep3->Create(IDD_PRODUCTSTEP2_DLG,this);

	//显示
	pStep3->MoveWindow(m_rectPanel);
	pStep3->ShowWindow(SW_HIDE);
	m_pPageList.push_back(pStep3);

	////////////////////////////////////////页面4
	CProductStep5Dlg *pStep4=new CProductStep5Dlg();
	if(pStep4==NULL)
	{
		MessageBox(_T("页面4创建失败"));
		AfxAbort();
	}

	pStep4->Create(IDD_PRODUCTSTEP5_DLG,this);

	//显示
	pStep4->MoveWindow(m_rectPanel);
	pStep4->ShowWindow(SW_HIDE);
	m_pPageList.push_back(pStep4);

	///////////////////////////////////////显示第一页
	ShowPage(0);
	UpdateWindow();



	//////////////////////////////////////状态显示初始化
	m_csCaption.Create(m_EvalType,WS_CHILD|WS_VISIBLE|SS_CENTER,CRect(20,30,130,50),this);

	short	shBtnColor = 30;
	m_btnChart30.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnChart31.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnChart32.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnChart33.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnChart34.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);

	INIT_EASYSIZE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CProductNew3Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	LockWindowUpdate();
	CWnd *pWnd;         //调整子窗口大小
	pWnd=GetDlgItem(IDC_STATIC_PLANE3);
	if (pWnd)      //获取控件句柄不为空时执行
	{
		CRect rect;
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		if (0 != rect.Width())//最小化显示时不执行，否则窗口会隐藏
		{
			for(int i=0;i<m_pPageList.size();++i)
			{
				m_pPageList[i]->MoveWindow(rect);
			}
		}
	}
	UPDATE_EASYSIZE;
	UnlockWindowUpdate(); 
}

HBRUSH CProductNew3Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_STATIC)  
	{
		//if(pWnd->GetDlgCtrlID()!=IDC_STATIC_PLANE)
		//	return (HBRUSH)::GetStockObject(NULL_BRUSH);   //设置透明画刷
		CFont font;
		font.CreatePointFont(125,_T("隶书"));    //设置字体背景
		pDC->SelectObject(&font);
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkMode(TRANSPARENT);
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CProductNew3Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	int n=m_pPageList.size();

	for(int i=0;i<n;i++)
	{
		m_pPageList[i]->DestroyWindow();  //销毁窗口资源
	}
}


void CProductNew3Dlg::OnBnClickedEvalpre3()
{
	// TODO: Add your control notification handler code here
	int nPrePage=m_nCurrentPage-1;      //确定前一页的索引位置
	ShowPage(nPrePage);
	UpdateWindow();
}


void CProductNew3Dlg::OnBnClickedEvalnext3()
{
	// TODO: Add your control notification handler code here
	int nNextPage=m_nCurrentPage+1;
	ShowPage(nNextPage);
	UpdateWindow();
}


void CProductNew3Dlg::OnBnClickedEvalin3()
{
	// TODO: Add your control notification handler code here
	if (((CProductStep5Dlg*)m_pPageList[4])->OnWizardNext()==-1)
		return;
	else
	{
		CSetWeighDlg dlg;
		dlg.ReadTechChart(((CProductStep0Dlg*)m_pPageList[0])->m_ProductInfo);//先读取表信息
		if(dlg.DoModal()==IDOK)
		{
			GetIndexVal(dlg.m_dA1,dlg.m_dA2);
			m_Lvl1TechNam=dlg.m_Lvl1TechNam;     //传递各级指标名
			m_Lvl2TechNam=dlg.m_Lvl2TechNam;

			//结果输出
			SaveResultInfo(m_LowValItem,m_IndexVal);    //给m_LowValItem、m_IndexVal赋值
			CProductOutDlg dlg1;
			dlg1.GetItemInfo(SetResultVal(m_ItemVal));
			dlg1.GetResultInfo(m_LowValItem,m_IndexVal);
			dlg1.DoModal();
		}
	}
	CDialogEx::OnOK();
}



//根据页面情况设置按钮状态
void CProductNew3Dlg::SetWizButton(UINT nPos)
{
	GetDlgItem(IDC_EVALPRE3)->EnableWindow(TRUE);
	GetDlgItem(IDC_EVALNEXT3)->EnableWindow(TRUE);
	GetDlgItem(IDC_EVALIN3)->EnableWindow(TRUE);

	switch(nPos)
	{
	case 0: //第一步(上一步失效)
		GetDlgItem(IDC_EVALPRE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EVALIN3)->EnableWindow(FALSE);
		break;
	case 1: //中间步
		GetDlgItem(IDC_EVALIN3)->EnableWindow(FALSE);
		break;
	case 2://中间步
		GetDlgItem(IDC_EVALIN3)->EnableWindow(FALSE);
		break;
	case 3://中间步
		GetDlgItem(IDC_EVALIN3)->EnableWindow(FALSE);
		break;
	case 4://最后一步
		GetDlgItem(IDC_EVALNEXT3)->EnableWindow(FALSE);
		break;
	}
}




//显示nPos页面
void CProductNew3Dlg::ShowPage(UINT nPos) 
{
	//显示第一页,隐藏当前页，激活下一页
	if(nPos==m_nCurrentPage)
	{
		((CProductStep0Dlg*)m_pPageList[0])->OnWizardActive();
		SetWizButton(0);
		SetTextShow(0);
	}
	else if(nPos>(UINT)m_nCurrentPage)
	{
		//执行的是下一步的操作,
		switch(m_nCurrentPage)
		{
		case 0:
			if(((CProductStep0Dlg*)m_pPageList[m_nCurrentPage])->OnWizardNext()==-1)  //保存当前工作不成功继续当前页
				return;
			((CProductStep6Dlg*)m_pPageList[nPos])->m_TechMaturyList.DeleteAllItems();
			((CProductStep6Dlg*)m_pPageList[nPos])->m_ProductInfo=((CProductStep0Dlg*)m_pPageList[0])->m_ProductInfo;
			((CProductStep6Dlg*)m_pPageList[nPos])->ReadTechChart(((CProductStep0Dlg*)m_pPageList[0])->m_ProductInfo);
			((CProductStep6Dlg*)m_pPageList[nPos])->m_ListCtrlItem.clear();
			((CProductStep6Dlg*)m_pPageList[nPos])->InitChartInfo();

			((CProductStep6Dlg*)m_pPageList[nPos])->OnWizardActive();
			break;
		case 1:
			if(((CProductStep6Dlg*)m_pPageList[m_nCurrentPage])->OnWizardNext()==-1)
				return;
			((CProductStep1Dlg*)m_pPageList[nPos])->m_ProductInfo=((CProductStep0Dlg*)m_pPageList[0])->m_ProductInfo;
			((CProductStep1Dlg*)m_pPageList[nPos])->ReadTechChart(((CProductStep0Dlg*)m_pPageList[0])->m_ProductInfo);
			((CProductStep1Dlg*)m_pPageList[nPos])->ShowListCtrl();

			((CProductStep1Dlg*)m_pPageList[nPos])->OnWizardActive();
			break;
		case 2:
			if(((CProductStep1Dlg*)m_pPageList[m_nCurrentPage])->OnWizardNext()==-1)
				return;
			((CProductStep2Dlg*)m_pPageList[nPos])->m_ProductInfo=((CProductStep0Dlg*)m_pPageList[0])->m_ProductInfo;
			((CProductStep2Dlg*)m_pPageList[nPos])->m_MatInfoList.DeleteAllItems();
			((CProductStep2Dlg*)m_pPageList[nPos])->GetProInfo(((CProductStep0Dlg*)m_pPageList[0])->m_ProductInfo);

			((CProductStep2Dlg*)m_pPageList[nPos])->OnWizardActive();
			break;
		case 3:
			if(((CProductStep2Dlg*)m_pPageList[m_nCurrentPage])->OnWizardNext()==-1)
				return;
			((CProductStep5Dlg*)m_pPageList[nPos])->m_ProductInfo=((CProductStep0Dlg*)m_pPageList[0])->m_ProductInfo;
			((CProductStep5Dlg*)m_pPageList[nPos])->ReadTechChart(((CProductStep0Dlg*)m_pPageList[0])->m_ProductInfo);
			((CProductStep5Dlg*)m_pPageList[nPos])->ShowListCtrl();

			((CProductStep5Dlg*)m_pPageList[nPos])->OnWizardActive();
			break;
		default:
			AfxMessageBox(_T("没有下一页,下一步操作有误"));
			break;
		}

		m_nCurrentPage=nPos;
		SetWizButton(nPos);
		SetTextShow(nPos);
	}
	else
	{
		//执行上一步操作，隐藏当前页，激活前一页,向前不用传递信息
		switch(m_nCurrentPage)
		{
		case 1:
			if(((CProductStep6Dlg*)m_pPageList[m_nCurrentPage])->OnWizardPrevious()==-1)
				return;
			((CProductStep0Dlg*)m_pPageList[nPos])->OnWizardActive();
			break;
		case 2:
			if(((CProductStep1Dlg*)m_pPageList[m_nCurrentPage])->OnWizardPrevious()==-1)
				return;
			((CProductStep6Dlg*)m_pPageList[nPos])->OnWizardActive();
			break;
		case 3:
			if(((CProductStep2Dlg*)m_pPageList[m_nCurrentPage])->OnWizardPrevious()==-1)
				return;
			((CProductStep1Dlg*)m_pPageList[nPos])->OnWizardActive();
			break;
		case 4:
			if(((CProductStep5Dlg*)m_pPageList[m_nCurrentPage])->OnWizardPrevious()==-1)
				return;
			((CProductStep2Dlg*)m_pPageList[nPos])->OnWizardActive();
			break;
		default:
			AfxMessageBox(_T("没有上一页,上一步操作有误"));
			break;
		}
		m_nCurrentPage=nPos;
		SetWizButton(nPos);
		SetTextShow(nPos);
	}

}


//设置文本状态显示变化
void CProductNew3Dlg::SetTextShow(UINT nPos)
{

	switch(nPos)
	{
	case 0:
		m_btnChart30.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 0, 0));
		m_btnChart31.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart32.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart33.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart34.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		break;
	case 1:
		m_btnChart30.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart31.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 0, 0));
		m_btnChart32.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart33.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart34.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		break;
	case 2:
		m_btnChart30.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart31.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart32.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 0, 0));
		m_btnChart33.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart34.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		break;
	case 3:
		m_btnChart30.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart31.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart32.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart33.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 0, 0));
		m_btnChart34.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		break;
	case 4:
		m_btnChart30.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart31.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart32.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart33.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart34.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 0, 0));
		break;
	default:
		m_btnChart30.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart31.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart32.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart33.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart34.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		break;
	}
}


///////////////////////////////////////////////////////////////////模糊层次分析法计算相关
void CProductNew3Dlg::GetIndexVal(VectorXd& dA1,VectorXd& dA2)
{
	/////////////将评分表中获取的cstring评分值转化为double
	int n1,n2,n3,n4;
	n1=((CProductStep6Dlg*)m_pPageList[1])->m_ListCtrlItem.size();
	n2=((CProductStep1Dlg*)m_pPageList[2])->m_ListCtrlItem.size();
	n3=((CProductStep2Dlg*)m_pPageList[3])->m_ListCtrlItem.size();
	n4=((CProductStep5Dlg*)m_pPageList[4])->m_ListCtrlItem.size();
	vector<double> TechMaturyVal,TechIndexVal,MatInfoVal,EconomyVal;

	for(int i=0;i<n1;++i)
	{
		TechMaturyVal.push_back(_ttol(((CProductStep6Dlg*)m_pPageList[1])->m_ListCtrlItem[i].m_TechMaturyVal));     //cstring转double

	}
	for(int i=0;i<n2;++i)
	{
		TechIndexVal.push_back(_ttol(((CProductStep1Dlg*)m_pPageList[2])->m_ListCtrlItem[i].m_IndexScore));     //cstring转double

	}
	for(int i=0;i<n3;++i)
	{
		MatInfoVal.push_back(_ttol(((CProductStep2Dlg*)m_pPageList[3])->m_ListCtrlItem[i].m_MatScore));     //cstring转double

	}
	for(int i=0;i<n4;++i)
	{
		EconomyVal.push_back(_ttol(((CProductStep5Dlg*)m_pPageList[4])->m_ListCtrlItem[i].m_IndexScore));     //cstring转double

	}
	double c1,c2,c3,c4;
	c1=MinVal(TechMaturyVal);    //成熟度直接评分，其余选择最低扣分项间接评分
	c2=3+MinVal(TechIndexVal);
	c3=3+MinVal(MatInfoVal);
	c4=3+MinVal(EconomyVal);

	vector<double>().swap(TechMaturyVal);//释放vector
	vector<double>().swap(TechIndexVal);
	vector<double>().swap(MatInfoVal);
	vector<double>().swap(EconomyVal);


	//保存二级指标评分
	VectorXd vTemp1(4);
	vTemp1<<c1,c2,c3,c4;
	m_dC2=vTemp1;

	VectorXd dC2(3);      //用于构造模糊矩阵
	dC2<<c1,c2,c3;

	///////////////////////////////////////通过评分按0-1型的隶属函数构建模糊矩阵
	MatrixXd dR2(dC2.size(),4); //评价集项为四个，即0，1，2，3
	VectorXd dV(4);
	dV<<0,1,2,3;
	for(int i=0;i<dR2.rows();++i)
	{
		for (int j=0;j<dR2.cols();++j)
		{
			if(dC2[i]==dV[j]) dR2(i,j)=1;
			else dR2(i,j)=0;
		}
	}
	m_dB2=dA2.transpose()*dR2;//技术指标对评价集的隶属度
	double cc1=m_dB2*dV;//技术指标的最终评分
	double cc2=c4;      //因为经济指标只有一个子指标，故评分直接为该指标评分

	//保存一级指标评分
	VectorXd vTemp2(2);
	vTemp2<<cc1,cc2;
	m_dC1=vTemp2;


	if(c1==0||c2==0||c3==0||c4==0)
		m_W=0;
	else
	{
		//构建上级模糊矩阵
		MatrixXd dR1(2,4);
		for (int i=0;i<dR1.rows();++i)
		{
			for (int j=0;j<dR1.cols();++j)
			{
				if(i==0) dR1(i,j)=m_dB2[j];
				else if(cc2==dV[j]) dR1(i,j)=1;
				else dR1(i,j)=0;
			}
		}
		m_dB1=dA1.transpose()*dR1; //评价整体对评价集的隶属度
		m_W=m_dB1*dV;  //最终评分值
	}
}



double CProductNew3Dlg::MinVal(vector<double>& IndexVal)
{
	double dMin=3.0;
	for (int i=0;i<IndexVal.size();++i)
	{
		if (IndexVal[i]<dMin)
			dMin=IndexVal[i];

	}
	return dMin;
}



/////////////////////////结果输出相关
vector<CString>& CProductNew3Dlg::SetResultVal(vector<CString>& m_ItemVal)
{
	//评价最终可制造性
	m_ItemVal.clear();

	CString strEvalVal,strEvalResult;
	if(m_W==0)
		strEvalResult=CString("不可制造");
	else if (m_W>0&&m_W<1.5)
		strEvalResult=CString("可制造性差");
	else if(m_W>=1.5&&m_W<2.5)
		strEvalResult=CString("可制造性一般");
	else 
		strEvalResult=CString("可制造性好");
	strEvalVal.Format(CString("%.3f"),m_W);
	m_ItemVal.push_back(((CProductStep0Dlg*)m_pPageList[0])->m_ProductName);  //产品名称
	m_ItemVal.push_back(((CProductStep0Dlg*)m_pPageList[0])->m_ProductSub);   //产品隶属组件
	m_ItemVal.push_back(strEvalVal);      //综合评价分值
	m_ItemVal.push_back(strEvalResult);   //综合评价结果
	m_ItemVal.push_back(CString("双击显示详细信息"));   //低分项及改进显示
	m_ItemVal.push_back(CString("双击显示详细信息"));   //指标得分值显示

	return m_ItemVal;
}




//给m_LowValItem、m_IndexVal赋值,用于结果显示
void CProductNew3Dlg::SaveResultInfo(vector<CLowValItem>& m_LowValItem,vector<CIndexValItem>& m_IndexVal)
{
	//综合保存低分项
	m_LowValItem.clear();
	m_IndexVal.clear();

	int nLowValItemNum=1; 
	for (int i=0;i<((CProductStep6Dlg*)m_pPageList[1])->m_LowValItem.size();++i)
	{
		CString str;
		str.Format(CString("%d"),nLowValItemNum);
		CLowValItem OneItem;
		OneItem=((CProductStep6Dlg*)m_pPageList[1])->m_LowValItem[i];
		OneItem.m_Item=str;  //序号重新赋值
		m_LowValItem.push_back(OneItem);
		++nLowValItemNum;
	}
	for (int i=0;i<((CProductStep1Dlg*)m_pPageList[2])->m_LowValItem.size();++i)
	{
		CString str;
		str.Format(CString("%d"),nLowValItemNum);
		CLowValItem OneItem;
		OneItem=((CProductStep1Dlg*)m_pPageList[2])->m_LowValItem[i];
		OneItem.m_Item=str;  //序号重新赋值
		m_LowValItem.push_back(OneItem);
		++nLowValItemNum;
	}
	for (int i=0;i<((CProductStep2Dlg*)m_pPageList[3])->m_LowValItem.size();++i)
	{
		CString str;
		str.Format(CString("%d"),nLowValItemNum);
		CLowValItem OneItem;
		OneItem=((CProductStep2Dlg*)m_pPageList[3])->m_LowValItem[i];
		OneItem.m_Item=str;  //序号重新赋值
		m_LowValItem.push_back(OneItem);
		++nLowValItemNum;
	}
	for (int i=0;i<((CProductStep5Dlg*)m_pPageList[4])->m_LowValItem.size();++i)
	{
		CString str;
		str.Format(CString("%d"),nLowValItemNum);
		CLowValItem OneItem;
		OneItem=((CProductStep5Dlg*)m_pPageList[4])->m_LowValItem[i];
		OneItem.m_Item=str;  //序号重新赋值
		m_LowValItem.push_back(OneItem);
		++nLowValItemNum;
	}


	//保存一二级指标评分
	int nIndexValItemNum=1;
	for (int i=0;i<m_Lvl1TechNam.size();++i)
	{
		CString strIndexVal,strIndexValItemNum;
		strIndexValItemNum.Format(CString("%d"),nIndexValItemNum);
		strIndexVal.Format(CString("%.2f"),m_dC1[i]);

		CIndexValItem OneItem;
		OneItem.m_Item=strIndexValItemNum;
		OneItem.m_IndexNam=m_Lvl1TechNam[i];
		OneItem.m_IndexVal=strIndexVal;

		m_IndexVal.push_back(OneItem);
		++nIndexValItemNum;
	}


	int k=0;//记录赋值次数
	for (int i=0;i<m_Lvl2TechNam.size();++i)
	{
		for (int j=0;j<m_Lvl2TechNam[i].size();++j)
		{
			CString strIndexVal,strIndexValItemNum;
			strIndexValItemNum.Format(CString("%d"),nIndexValItemNum);
			strIndexVal.Format(CString("%.2f"),m_dC2[k]);

			CIndexValItem OneItem;
			OneItem.m_Item=strIndexValItemNum;
			OneItem.m_IndexNam=m_Lvl2TechNam[i][j];
			OneItem.m_IndexVal=strIndexVal;

			m_IndexVal.push_back(OneItem);
			++nIndexValItemNum,++k;
		}
	}
}

