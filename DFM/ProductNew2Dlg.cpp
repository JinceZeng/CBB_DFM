// ProductNew2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "DFM.h"
#include "ProductNew2Dlg.h"
#include "afxdialogex.h"


// CProductNew2Dlg dialog

IMPLEMENT_DYNAMIC(CProductNew2Dlg, CDialogEx)

CProductNew2Dlg::CProductNew2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProductNew2Dlg::IDD, pParent)
	,m_nCurrentPage(0)              //��ǰ��ʾҳ
	,bInfoWrited(FALSE)
	,m_EvalType(CString(""))
	,str_EvalTypeID(CString(""))
{
	m_pPageList.clear();            //��¼���ӶԻ���
	m_rectPanel=CRect(0,0,0,0);     //�ӶԻ���İ�ΧȦ
}

CProductNew2Dlg::~CProductNew2Dlg()
{
	int n=m_pPageList.size();

	for(int i=0;i<n;i++)
	{
		delete m_pPageList[i];      //�ͷ��ڴ棬�������ͷ��ڴ���ΪҪ������Ϣ����ȥ
	}

	//m_pPageList.clear();            //��¼���ӶԻ���
	vector<CDialogEx*>().swap(m_pPageList);//�����������С����������

	//delete m_csCaption;             //�ͷ�cstaticָ���ڴ�
	vector<CString>().swap(m_ItemVal);
	vector<CString>().swap(m_Lvl1TechNam);
	vector<vector<CString>>().swap(m_Lvl2TechNam);
	vector<CLowValItem>().swap(m_LowValItem);
	vector<CIndexValItem>().swap(m_IndexVal);
}

void CProductNew2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CHART20, m_btnChart20);
	DDX_Control(pDX, IDC_BUTTON_CHART21, m_btnChart21);
	DDX_Control(pDX, IDC_BUTTON_CHART22, m_btnChart22);
	DDX_Control(pDX, IDC_BUTTON_CHART23, m_btnChart23);
	DDX_Control(pDX, IDC_BUTTON_CHART24, m_btnChart24);
}


BEGIN_MESSAGE_MAP(CProductNew2Dlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_EVALPRE2, &CProductNew2Dlg::OnBnClickedEvalpre2)
	ON_BN_CLICKED(IDC_EVALNEXT2, &CProductNew2Dlg::OnBnClickedEvalnext2)
	ON_BN_CLICKED(IDC_EVALIN2, &CProductNew2Dlg::OnBnClickedEvalin2)
END_MESSAGE_MAP()


// CProductNew2Dlg message handlers


BOOL CProductNew2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//��ȡ�ӶԻ���İ��ݾ���
	GetDlgItem(IDC_STATIC_PLANE2)->GetWindowRect(&m_rectPanel);
	ScreenToClient(&m_rectPanel);

	//����ĸ�����ҳ

	//////////////////////////////////////////////ҳ��0
	CProductStep0Dlg *pStep0=new CProductStep0Dlg();
	if(pStep0==NULL)
	{
		MessageBox(_T("ҳ��0����ʧ��"));
		AfxAbort();
	}

	pStep0->Create(IDD_PRODUCTSTEP0_DLG,this);

	if(bInfoWrited)//������ݿ����Ѿ�д�룬ֱ�ӽ��г�ʼ��
	{
		_RecordsetPtr m_pRs,m_pRs1;
		CString sql = CString("select * from ProductInfo where ProductID=") + strProductID ;
		m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
		CString str1 = (CString)m_pRs->GetCollect("ProductNam");
		CString str2 = (CString)m_pRs->GetCollect("ProductNum");
		CString str3 = (CString)(m_pRs->GetCollect("ProductSub")); 

		str_EvalTypeID =  m_pRs->GetCollect("EvalModelID");
		CString str4 = (CString)(m_pRs->GetCollect("EvalTypeIntro")); 
		//int int4 = _ttoi(str44)-1; //combo���㿪ʼ-1
		CString sql1 = CString("select * from EvalModelInfo where EvalModelID=")+str_EvalTypeID;
		m_pRs1 = theApp.m_pConnect->Execute(_bstr_t(sql1), NULL, adCmdText);
		m_EvalType = (CString)(m_pRs1->GetCollect("EvalModelNam")); 


		pStep0->m_ProductName=str1;
		pStep0->m_ProductNum=str2;
		pStep0->m_ProductSub=str3;
		pStep0->str_EvalTypeID=str_EvalTypeID;
		pStep0->m_EvalType=m_EvalType;
		pStep0->m_TypeInfo=str4;

		pStep0->SaveProductInfo();//�����Ʒ����
		::SendMessage(pStep0->m_hWnd,WM_UPDATEDATA,FALSE,FALSE);//���¿ؼ���Ϣ
	}
	else
	{
		pStep0->str_EvalTypeID=str_EvalTypeID;
		pStep0->m_EvalType=m_EvalType;
		::SendMessage(pStep0->m_hWnd,WM_UPDATEDATA,FALSE,FALSE);//���¿ؼ���Ϣ

	}
	//��ʾ
	pStep0->MoveWindow(m_rectPanel);
	pStep0->ShowWindow(SW_HIDE);
	m_pPageList.push_back(pStep0);

	//////////////////////////////////////////////ҳ��1
	CProductStep6Dlg *pStep1=new CProductStep6Dlg();
	if(pStep1==NULL)
	{
		MessageBox(_T("ҳ��1����ʧ��"));
		AfxAbort();
	}

	pStep1->Create(IDD_PRODUCTSTEP6_DLG,this);

	//��ʾ
	pStep1->MoveWindow(m_rectPanel);
	pStep1->ShowWindow(SW_HIDE);
	m_pPageList.push_back(pStep1);



	///////////////////////////////////////////ҳ��2
	CProductStep1Dlg *pStep2=new CProductStep1Dlg();
	if(pStep2==NULL)
	{
		MessageBox(_T("ҳ��2����ʧ��"));
		AfxAbort();
	}

	pStep2->Create(IDD_PRODUCTSTEP1_DLG,this);

	//��ʾ
	pStep2->MoveWindow(m_rectPanel);
	pStep2->ShowWindow(SW_HIDE);
	m_pPageList.push_back(pStep2);


	////////////////////////////////////////ҳ��3
	CProductStep4Dlg *pStep3=new CProductStep4Dlg();
	if(pStep3==NULL)
	{
		MessageBox(_T("ҳ��3����ʧ��"));
		AfxAbort();
	}

	pStep3->Create(IDD_PRODUCTSTEP4_DLG,this);

	//��ʾ
	pStep3->MoveWindow(m_rectPanel);
	pStep3->ShowWindow(SW_HIDE);
	m_pPageList.push_back(pStep3);

	////////////////////////////////////////ҳ��4
	CProductStep5Dlg *pStep4=new CProductStep5Dlg();
	if(pStep4==NULL)
	{
		MessageBox(_T("ҳ��4����ʧ��"));
		AfxAbort();
	}

	pStep4->Create(IDD_PRODUCTSTEP5_DLG,this);

	//��ʾ
	pStep4->MoveWindow(m_rectPanel);
	pStep4->ShowWindow(SW_HIDE);
	m_pPageList.push_back(pStep4);



	///////////////////////////////////////��ʾ��һҳ
	ShowPage(0);
	UpdateWindow();



	//////////////////////////////////////״̬��ʾ��ʼ��
	m_csCaption.Create(m_EvalType,WS_CHILD|WS_VISIBLE|SS_CENTER,CRect(20,30,130,50),this);

	short	shBtnColor = 30;
	m_btnChart20.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnChart21.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnChart22.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnChart23.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnChart24.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


HBRUSH CProductNew2Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_STATIC)  
	{
		//if(pWnd->GetDlgCtrlID()!=IDC_STATIC_PLANE)
		//	return (HBRUSH)::GetStockObject(NULL_BRUSH);   //����͸����ˢ
		CFont font;
		font.CreatePointFont(125,_T("����"));    //�������屳��
		pDC->SelectObject(&font);
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkMode(TRANSPARENT);
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CProductNew2Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	int n=m_pPageList.size();

	for(int i=0;i<n;i++)
	{
		m_pPageList[i]->DestroyWindow();  //���ٴ�����Դ
	}
}


void CProductNew2Dlg::OnBnClickedEvalpre2()
{
	// TODO: Add your control notification handler code here
	int nPrePage=m_nCurrentPage-1;      //ȷ��ǰһҳ������λ��
	ShowPage(nPrePage);
	UpdateWindow();
}


void CProductNew2Dlg::OnBnClickedEvalnext2()
{
	// TODO: Add your control notification handler code here
	int nNextPage=m_nCurrentPage+1;
	ShowPage(nNextPage);
	UpdateWindow();
}


void CProductNew2Dlg::OnBnClickedEvalin2()
{
	// TODO: Add your control notification handler code here
	if (((CProductStep5Dlg*)m_pPageList[4])->OnWizardNext()==-1)
		return;
	else
	{
		CSetWeighDlg dlg;
		dlg.ReadTechChart(((CProductStep0Dlg*)m_pPageList[0])->m_ProductInfo);//�ȶ�ȡ����Ϣ
		if(dlg.DoModal()==IDOK)
		{
			GetIndexVal(dlg.m_dA1,dlg.m_dA2);
			m_Lvl1TechNam=dlg.m_Lvl1TechNam;     //���ݸ���ָ����
			m_Lvl2TechNam=dlg.m_Lvl2TechNam;

			//������
			SaveResultInfo(m_LowValItem,m_IndexVal);    //��m_LowValItem��m_IndexVal��ֵ
			CProductOutDlg dlg1;
			dlg1.GetItemInfo(SetResultVal(m_ItemVal));
			dlg1.GetResultInfo(m_LowValItem,m_IndexVal);
			dlg1.DoModal();
		}
	}
}


//����ҳ��������ð�ť״̬
void CProductNew2Dlg::SetWizButton(UINT nPos)
{
	GetDlgItem(IDC_EVALPRE2)->EnableWindow(TRUE);
	GetDlgItem(IDC_EVALNEXT2)->EnableWindow(TRUE);
	GetDlgItem(IDC_EVALIN2)->EnableWindow(TRUE);

	switch(nPos)
	{
	case 0: //��һ��(��һ��ʧЧ)
		GetDlgItem(IDC_EVALPRE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EVALIN2)->EnableWindow(FALSE);
		break;
	case 1: //�м䲽
		GetDlgItem(IDC_EVALIN2)->EnableWindow(FALSE);
		break;
	case 2://�м䲽
		GetDlgItem(IDC_EVALIN2)->EnableWindow(FALSE);
		break;
	case 3://�м䲽
		GetDlgItem(IDC_EVALIN2)->EnableWindow(FALSE);
		break;
	case 4://���һ��
		GetDlgItem(IDC_EVALNEXT2)->EnableWindow(FALSE);
		break;
	}
}


//��ʾnPosҳ��
void CProductNew2Dlg::ShowPage(UINT nPos) 
{
	//��ʾ��һҳ,���ص�ǰҳ��������һҳ
	if(nPos==m_nCurrentPage)
	{
		((CProductStep0Dlg*)m_pPageList[0])->OnWizardActive();
		SetWizButton(0);
		SetTextShow(0);
	}
	else if(nPos>(UINT)m_nCurrentPage)
	{
		//ִ�е�����һ���Ĳ���,
		switch(m_nCurrentPage)
		{
		case 0:
			if(((CProductStep0Dlg*)m_pPageList[m_nCurrentPage])->OnWizardNext()==-1)  //���浱ǰ�������ɹ�������ǰҳ
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
			((CProductStep4Dlg*)m_pPageList[nPos])->m_ProductInfo=((CProductStep0Dlg*)m_pPageList[0])->m_ProductInfo;
			((CProductStep4Dlg*)m_pPageList[nPos])->ReadTechChart(((CProductStep0Dlg*)m_pPageList[0])->m_ProductInfo);
			((CProductStep4Dlg*)m_pPageList[nPos])->ShowListCtrl();

			((CProductStep4Dlg*)m_pPageList[nPos])->OnWizardActive();
			break;
		case 3:
			if(((CProductStep4Dlg*)m_pPageList[m_nCurrentPage])->OnWizardNext()==-1)
				return;
			((CProductStep5Dlg*)m_pPageList[nPos])->m_ProductInfo=((CProductStep0Dlg*)m_pPageList[0])->m_ProductInfo;
			((CProductStep5Dlg*)m_pPageList[nPos])->ReadTechChart(((CProductStep0Dlg*)m_pPageList[0])->m_ProductInfo);
			((CProductStep5Dlg*)m_pPageList[nPos])->ShowListCtrl();

			((CProductStep5Dlg*)m_pPageList[nPos])->OnWizardActive();
			break;
		default:
			AfxMessageBox(_T("û����һҳ,��һ����������"));
			break;
		}

		m_nCurrentPage=nPos;
		SetWizButton(nPos);
		SetTextShow(nPos);
	}
	else
	{
		//ִ����һ�����������ص�ǰҳ������ǰһҳ,��ǰ���ô�����Ϣ
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
			if(((CProductStep4Dlg*)m_pPageList[m_nCurrentPage])->OnWizardPrevious()==-1)
				return;
			((CProductStep1Dlg*)m_pPageList[nPos])->OnWizardActive();
			break;
		case 4:
			if(((CProductStep5Dlg*)m_pPageList[m_nCurrentPage])->OnWizardPrevious()==-1)
				return;
			((CProductStep4Dlg*)m_pPageList[nPos])->OnWizardActive();
			break;
		default:
			AfxMessageBox(_T("û����һҳ,��һ����������"));
			break;
		}
		m_nCurrentPage=nPos;
		SetWizButton(nPos);
		SetTextShow(nPos);
	}

}




//�����ı�״̬��ʾ�仯
void CProductNew2Dlg::SetTextShow(UINT nPos)
{

	switch(nPos)
	{
	case 0:
		m_btnChart20.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 0, 0));
		m_btnChart21.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart22.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart23.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart24.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		break;
	case 1:
		m_btnChart20.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart21.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 0, 0));
		m_btnChart22.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart23.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart24.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		break;
	case 2:
		m_btnChart20.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart21.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart22.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 0, 0));
		m_btnChart23.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart24.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		break;
	case 3:
		m_btnChart20.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart21.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart22.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart23.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 0, 0));
		m_btnChart24.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		break;
	case 4:
		m_btnChart20.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart21.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart22.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart23.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart24.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(255, 0, 0));
		break;
	default:
		m_btnChart20.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart21.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart22.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart23.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		m_btnChart24.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 0));
		break;
	}
}



///////////////////////////////////////////////////////////////////ģ����η������������
void CProductNew2Dlg::GetIndexVal(VectorXd& dA1,VectorXd& dA2)
{
	/////////////�����ֱ��л�ȡ��cstring����ֵת��Ϊdouble
	int n1,n2,n3,n4;
	n1=((CProductStep6Dlg*)m_pPageList[1])->m_ListCtrlItem.size();
	n2=((CProductStep1Dlg*)m_pPageList[2])->m_ListCtrlItem.size();
	n3=((CProductStep4Dlg*)m_pPageList[3])->m_ListCtrlItem.size();
	n4=((CProductStep5Dlg*)m_pPageList[4])->m_ListCtrlItem.size();
	vector<double> TechMaturyVal,TechIndexVal,ThreeProVal,EconomyVal;

	for(int i=0;i<n1;++i)
	{
		TechMaturyVal.push_back(_ttol(((CProductStep6Dlg*)m_pPageList[1])->m_ListCtrlItem[i].m_TechMaturyVal));     //cstringתdouble

	}
	for(int i=0;i<n2;++i)
	{
		TechIndexVal.push_back(_ttol(((CProductStep1Dlg*)m_pPageList[2])->m_ListCtrlItem[i].m_IndexScore));     //cstringתdouble

	}
	for(int i=0;i<n3;++i)
	{
		ThreeProVal.push_back(_ttol(((CProductStep4Dlg*)m_pPageList[3])->m_ListCtrlItem[i].m_IndexScore));     //cstringתdouble

	}
	for(int i=0;i<n4;++i)
	{
		EconomyVal.push_back(_ttol(((CProductStep5Dlg*)m_pPageList[4])->m_ListCtrlItem[i].m_IndexScore));     //cstringתdouble

	}
	double c1,c2,c3,c4;
	c1=MinVal(TechMaturyVal);    //�����ֱ�����֣�����ѡ����Ϳ۷���������
	c2=3+MinVal(TechIndexVal);
	c3=3+MinVal(ThreeProVal);
	c4=3+MinVal(EconomyVal);

	vector<double>().swap(TechMaturyVal);//�ͷ�vector
	vector<double>().swap(TechIndexVal);
	vector<double>().swap(ThreeProVal);
	vector<double>().swap(EconomyVal);


	//�������ָ������
	VectorXd vTemp1(4);
	vTemp1<<c1,c2,c3,c4;
	m_dC2=vTemp1;

	VectorXd dC2(3);      //���ڹ���ģ������
	dC2<<c1,c2,c3;

	///////////////////////////////////////ͨ�����ְ�0-1�͵�������������ģ������
	MatrixXd dR2(dC2.size(),4); //���ۼ���Ϊ�ĸ�����0��1��2��3
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
	m_dB2=dA2.transpose()*dR2;//����ָ������ۼ���������
	double cc1=m_dB2*dV;//����ָ�����������
	double cc2=c4;      //��Ϊ����ָ��ֻ��һ����ָ�꣬������ֱ��Ϊ��ָ������

	//����һ��ָ������
	VectorXd vTemp2(2);
	vTemp2<<cc1,cc2;
	m_dC1=vTemp2;


	//�����ϼ�ģ������
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
	m_dB1=dA1.transpose()*dR1; //������������ۼ���������
	m_W=m_dB1*dV;  //��������ֵ
}



double CProductNew2Dlg::MinVal(vector<double>& IndexVal)
{
	double dMin=3.0;
	for (int i=0;i<IndexVal.size();++i)
	{
		if (IndexVal[i]<dMin)
			dMin=IndexVal[i];

	}
	return dMin;
}



/////////////////////////���������
vector<CString>& CProductNew2Dlg::SetResultVal(vector<CString>& m_ItemVal)
{
	//�������տ�������
	m_ItemVal.clear();

	CString strEvalVal,strEvalResult;
	if (m_W>=0&&m_W<1.5)
		strEvalResult=CString("�������Բ�");
	else if(m_W>=1.5&&m_W<2.5)
		strEvalResult=CString("��������һ��");
	else 
		strEvalResult=CString("�������Ժ�");
	strEvalVal.Format(CString("%.3f"),m_W);
	m_ItemVal.push_back(((CProductStep0Dlg*)m_pPageList[0])->m_ProductName);  //��Ʒ����
	m_ItemVal.push_back(((CProductStep0Dlg*)m_pPageList[0])->m_ProductNum);   //��Ʒ���
	m_ItemVal.push_back(strEvalVal);      //�ۺ����۷�ֵ
	m_ItemVal.push_back(strEvalResult);   //�ۺ����۽��
	m_ItemVal.push_back(CString("˫����ʾ��ϸ��Ϣ"));   //�ͷ���Ľ���ʾ
	m_ItemVal.push_back(CString("˫����ʾ��ϸ��Ϣ"));   //ָ��÷�ֵ��ʾ

	return m_ItemVal;
}




//��m_LowValItem��m_IndexVal��ֵ,���ڽ����ʾ
void CProductNew2Dlg::SaveResultInfo(vector<CLowValItem>& m_LowValItem,vector<CIndexValItem>& m_IndexVal)
{
	//�ۺϱ���ͷ���
	m_LowValItem.clear();
	m_IndexVal.clear();

	int nLowValItemNum=1; 
	for (int i=0;i<((CProductStep6Dlg*)m_pPageList[1])->m_LowValItem.size();++i)
	{
		CString str;
		str.Format(CString("%d"),nLowValItemNum);
		CLowValItem OneItem;
		OneItem=((CProductStep6Dlg*)m_pPageList[1])->m_LowValItem[i];
		OneItem.m_Item=str;  //������¸�ֵ
		m_LowValItem.push_back(OneItem);
		++nLowValItemNum;
	}
	for (int i=0;i<((CProductStep1Dlg*)m_pPageList[2])->m_LowValItem.size();++i)
	{
		CString str;
		str.Format(CString("%d"),nLowValItemNum);
		CLowValItem OneItem;
		OneItem=((CProductStep1Dlg*)m_pPageList[2])->m_LowValItem[i];
		OneItem.m_Item=str;  //������¸�ֵ
		m_LowValItem.push_back(OneItem);
		++nLowValItemNum;
	}
	for (int i=0;i<((CProductStep4Dlg*)m_pPageList[3])->m_LowValItem.size();++i)
	{
		CString str;
		str.Format(CString("%d"),nLowValItemNum);
		CLowValItem OneItem;
		OneItem=((CProductStep4Dlg*)m_pPageList[3])->m_LowValItem[i];
		OneItem.m_Item=str;  //������¸�ֵ
		m_LowValItem.push_back(OneItem);
		++nLowValItemNum;
	}
	for (int i=0;i<((CProductStep5Dlg*)m_pPageList[4])->m_LowValItem.size();++i)
	{
		CString str;
		str.Format(CString("%d"),nLowValItemNum);
		CLowValItem OneItem;
		OneItem=((CProductStep5Dlg*)m_pPageList[4])->m_LowValItem[i];
		OneItem.m_Item=str;  //������¸�ֵ
		m_LowValItem.push_back(OneItem);
		++nLowValItemNum;
	}


	//����һ����ָ������
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


	int k=0;//��¼��ֵ����
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