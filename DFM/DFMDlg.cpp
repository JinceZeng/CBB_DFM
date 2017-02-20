
// DFMDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DFM.h"
#include "DFMDlg.h"
#include "afxdialogex.h"
#include "LoginDlg.h"
#include "ChangePwdDlg.h"
#include "ProductNewDlg.h"
#include "ProductNew1Dlg.h"
#include "ProductNew2Dlg.h"
#include "ProductNew3Dlg.h"
#include "ChooseEvalType.h"
#include "ResultCheck.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDFMDlg �Ի���




CDFMDlg::CDFMDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDFMDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDFMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRODUCTINFO, m_ProductInfoList);
}

BEGIN_MESSAGE_MAP(CDFMDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_COMMAND(ID_MENU_LOGIN, &CDFMDlg::OnMenuLogin)
	ON_COMMAND(ID_MENU_ABOUT, &CDFMDlg::OnMenuAbout)
	ON_COMMAND(ID_MENU_PSW, &CDFMDlg::OnMenuPsw)
	ON_COMMAND(ID_EVALNEW, &CDFMDlg::OnEvalnew)//�½����ֲ˵���Ӧ
	ON_COMMAND(IDC_MODELNEW, &CDFMDlg::OnEvalnew)//�½����ְ�ť��Ӧ
	ON_COMMAND(ID_PRODUCT_EVALVAL, &CDFMDlg::OnProductEvalval)//���ֲ�ѯ�˵���Ӧ
	ON_COMMAND(IDC_EVALRESULT, &CDFMDlg::OnProductEvalval)//���ֲ�ѯ��ť��Ӧ

	ON_MESSAGE(WM_DELETEDATA,&CDFMDlg::OnDeleteData)
	ON_MESSAGE(WM_UPDATELIST,&CDFMDlg::OnUpdateList)
	ON_MESSAGE(WM_EVALIN,&CDFMDlg::OnEvalIn)
	ON_MESSAGE(WM_CHECK,&CDFMDlg::OnCheck)
END_MESSAGE_MAP()


//�ؼ���Сλ�ñ仯(��δʹ�ô˹���)
//BEGIN_EASYSIZE_MAP(CDFMDlg, CDialogEx) 
//	EASYSIZE(IDC_MODELNEW,ES_BORDER,ES_BORDER,IDC_EVALRESULT,ES_KEEPSIZE,ES_HCENTER) //�˴������Լ����� 
//	EASYSIZE(IDC_EVALRESULT,IDC_MODELNEW,ES_BORDER,ES_BORDER,ES_KEEPSIZE,ES_HCENTER)
//	EASYSIZE(IDC_LIST_PRODUCTINFO,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
//END_EASYSIZE_MAP 

// CDFMDlg ��Ϣ��������

BOOL CDFMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), TRUE);


	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	UINT indicators[] = 
	{
		IDS_INDICATOR_MESSAGE,
		IDS_INDICATOR_USER,
		IDS_INDICATOR_TIME
	};
	ModifyStyle(WS_THICKFRAME,0);
	if(!m_Statusbar.Create(this)||
		!m_Statusbar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Can't create status bar\n");
		return false;
	}
	ModifyStyle(0,WS_THICKFRAME);

	//��ȡIDS_INDICATOR_MESSAGE,IDS_INDICATOR_USER��IDS_INDICATOR_TIME�������Ͷ�Ӧ�Ŀ���
	index1 = m_Statusbar.CommandToIndex(IDS_INDICATOR_MESSAGE);
	index2 = m_Statusbar.CommandToIndex(IDS_INDICATOR_USER);
	index3 = m_Statusbar.CommandToIndex(IDS_INDICATOR_TIME);

	m_Statusbar.GetPaneInfo(index1,nID,nStyle,nWidth1);
	m_Statusbar.GetPaneInfo(index2,nID,nStyle,nWidth2);
	m_Statusbar.GetPaneInfo(index3,nID,nStyle,nWidth3);

	CRect rect;
	GetClientRect(&rect);

	//�������Ŀ���
	m_Statusbar.SetPaneInfo(index1,nID,nStyle,rect.Width()/2);
	m_Statusbar.SetPaneInfo(index2,nID,nStyle,rect.Width()/3);
	m_Statusbar.SetPaneInfo(index3,nID,nStyle,rect.Width()/6);

	//���°ڷſؼ� ��Ϊ����״̬��֮�� �ؼ����λ�÷����仯  ���°ڷŲ�����ʾ����
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0); 

	//�����ı� index=0��2�����ı�����
	CString str1=_T("������������");
	CTime time;
	time = CTime::GetCurrentTime();
	CString str2 = time.Format("%Y-%m-%d");
	m_Statusbar.SetPaneText(0,str1);
	m_Statusbar.SetPaneText(2,str2);


    //�˵����ʼ����Ҳ���
	CMenu *pMenu = GetMenu();
	pMenu->GetSubMenu(1)->EnableMenuItem(0, MF_BYPOSITION | MF_GRAYED);
	pMenu->GetSubMenu(1)->EnableMenuItem(1, MF_BYPOSITION | MF_GRAYED);
	pMenu->GetSubMenu(2)->EnableMenuItem(0, MF_BYPOSITION | MF_GRAYED);
	pMenu->GetSubMenu(2)->EnableMenuItem(1, MF_BYPOSITION | MF_GRAYED);
	pMenu->GetSubMenu(3)->EnableMenuItem(0, MF_BYPOSITION | MF_GRAYED);
	pMenu->GetSubMenu(3)->EnableMenuItem(1, MF_BYPOSITION | MF_GRAYED);
	pMenu->GetSubMenu(4)->EnableMenuItem(0, MF_BYPOSITION | MF_GRAYED);
	//pMenu->Detach();
	//DrawMenuBar();    //���²˵���ʾ
	//��ť���
	GetDlgItem(IDC_MODELNEW)->EnableWindow(FALSE);
	GetDlgItem(IDC_EVALRESULT)->EnableWindow(FALSE);

	ChangeStyle(m_ProductInfoList);//list���ı�
	//���б��ؼ��ȳ�ʼ��,�����ͷ
	CRect rec1;
	int width1;
	m_ProductInfoList.GetClientRect(&rec1);
	width1=rec1.Width();

	m_ProductInfoList.InsertColumn(0,_T("���"), LVCFMT_CENTER,width1/16);
	m_ProductInfoList.InsertColumn(1,_T("ID"), LVCFMT_CENTER,width1/16);
	m_ProductInfoList.InsertColumn(2,_T("��Ʒ����"),LVCFMT_CENTER,width1/8);
	m_ProductInfoList.InsertColumn(3,_T("��Ʒ���"),LVCFMT_CENTER,width1/8);
	m_ProductInfoList.InsertColumn(4,_T("��������"),LVCFMT_CENTER,width1/8);
	m_ProductInfoList.InsertColumn(5,_T("����ģ��"),LVCFMT_CENTER,width1/8);
	m_ProductInfoList.InsertColumn(6,_T("����״̬"),LVCFMT_CENTER,width1/8);
	m_ProductInfoList.InsertColumn(7,_T("������"),LVCFMT_CENTER,width1/8);
	m_ProductInfoList.InsertColumn(8,_T("����ʱ��"),LVCFMT_CENTER,width1/8);

	OnMenuLogin();
	//INIT_EASYSIZE;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDFMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDFMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDFMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��Ӧ���ڴ�С�仯����δʹ�ã�
void CDFMDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	//�ı�״̬����С
	CRect rectDlg,rectBar;
	GetClientRect(rectDlg);
	if(m_Statusbar&&(0 != rectDlg.Width()))

	{

		CStatusBar *m_pwndStatusBar = (CStatusBar*)&m_Statusbar;  

		m_pwndStatusBar->GetClientRect(&rectBar);  

		m_pwndStatusBar->SetPaneInfo(index1,nID,nStyle,rectDlg.Width()/2); 
		m_pwndStatusBar->SetPaneInfo(index2,nID,nStyle,rectDlg.Width()/3);
		m_pwndStatusBar->SetPaneInfo(index3,nID,nStyle,rectDlg.Width()/6); 
		m_pwndStatusBar->MoveWindow(0,cy-rectBar.Height(),rectDlg.Width(),rectBar.Height());  

	}

	//UPDATE_EASYSIZE;

}


void CDFMDlg::ChangeStyle(CPInfoListCtrl &m_ListCtrl)
{
	//���ԭ�з��
	DWORD dwStyle =::GetWindowLong(m_ListCtrl.m_hWnd,GWL_STYLE);
	dwStyle &= ~(LVS_TYPEMASK); 
	dwStyle &= ~(LVS_EDITLABELS);

	//�����·��
	SetWindowLong(m_ListCtrl.m_hWnd, GWL_STYLE,dwStyle|LVS_REPORT|LVS_NOLABELWRAP|LVS_SHOWSELALWAYS);

	//������չ���
	DWORD styles = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES/*|LVS_EX_CHECKBOXES*/;  
	ListView_SetExtendedListViewStyleEx(m_ListCtrl.m_hWnd, styles, styles ); 

}


void CDFMDlg::UpdateListCtrl()
{
	m_ProductInfoList.DeleteAllItems();
	_RecordsetPtr m_pRs1;

	CString sql = CString("select * from ProductInfo");
	m_pRs1 = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);

	int n=0;
	while (!m_pRs1->adoEOF)
	{
		CString str0;
		str0.Format(CString("%d"),n+1); //intתcstring
		CString str8 = (m_pRs1->GetCollect("ProductID"));       //���ݿ���int�����ݻ�ȡ��ɲ���cstringǿ��ת��
		CString str1 = (CString)(m_pRs1->GetCollect("ProductNam"));
		CString str2 = (CString)(m_pRs1->GetCollect("ProductNum"));
		CString str3 = (CString)(m_pRs1->GetCollect("ProductSub")); 

		CString str4;
		CString str44 = m_pRs1->GetCollect("EvalModelID");
		_RecordsetPtr m_pRs2;
		CString sql = CString("select * from EvalModelInfo where EvalModelID=") + str44 ;
		m_pRs2 = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
		str4 =(CString)m_pRs2->GetCollect("EvalModelNam");
		

		CString str5;
		CString str55;
		str55 = m_pRs1->GetCollect("IsEval");
		int i = _ttoi(str55);                //cstringתint  
		//int i=atoi((char *)(_bstr_t)str55);//cstringתint
		if(i==0) str5=CString("������");
		else     str5=CString("�ѷ���");

		CString str6 = theApp.name;
		CString str7 = (CString)(m_pRs1->GetCollect("EvalTime"));

		m_ProductInfoList.InsertItem(n,str0);
		m_ProductInfoList.SetItemText(n,1,str8);
		m_ProductInfoList.SetItemText(n,2,str1);
		m_ProductInfoList.SetItemText(n,3,str2);
		m_ProductInfoList.SetItemText(n,4,str3);
		m_ProductInfoList.SetItemText(n,5,str4);
		m_ProductInfoList.SetItemText(n,6,str5);
		m_ProductInfoList.SetItemText(n,7,str6);
		m_ProductInfoList.SetItemText(n,8,str7);

	    n++;
		m_pRs1->MoveNext();
	}


}

void CDFMDlg::OnMenuLogin()
{
	// TODO: Add your command handler code here
	CLoginDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		_RecordsetPtr m_pRs;
		CString sql = CString("select * from Login where Uname='") + theApp.name + "'";
		m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
		CString str_power = m_pRs->GetCollect("Power");
		int power = atoi((char *)(_bstr_t)str_power);
		if (power == 1)
		{
			//m_Toolbar.GetToolBarCtrl().EnableButton(IDB_addcai, true);
			//m_Toolbar.GetToolBarCtrl().EnableButton(IDB_kaitai, true);
			//m_Toolbar.GetToolBarCtrl().EnableButton(IDB_jiezhang, true);
			//m_Toolbar.GetToolBarCtrl().EnableButton(IDB_jiesuan, true);
			//m_Toolbar.GetToolBarCtrl().EnableButton(IDB_yuangong, true);
			//m_Toolbar.GetToolBarCtrl().EnableButton(IDB_logout, true);
			CMenu *pMenu = GetMenu();           //���Ĳ˵���ʾ
			pMenu->GetSubMenu(1)->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
			pMenu->GetSubMenu(1)->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
			pMenu->GetSubMenu(2)->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
			pMenu->GetSubMenu(2)->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
			pMenu->GetSubMenu(3)->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
			pMenu->GetSubMenu(3)->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
			pMenu->GetSubMenu(4)->EnableMenuItem(0, MF_BYPOSITION | MF_ENABLED);
			pMenu->EnableMenuItem(0,MF_BYPOSITION | MF_GRAYED);
			AfxGetMainWnd()->DrawMenuBar();    //���²˵���ʾ

			//���°�ť
			GetDlgItem(IDC_MODELNEW)->EnableWindow(true);
			GetDlgItem(IDC_EVALRESULT)->EnableWindow(true);
		}
		m_Statusbar.SetPaneText(1, CString("��ǰ�û�: ") + theApp.name);
		SetWindowText(CString("������������      ��ǰ��¼�û�: ") + theApp.name);


        UpdateListCtrl();
	}
}


//�Զ�����Ϣɾ��ĳһ����Ʒ��Ϣ
LRESULT CDFMDlg::OnDeleteData(WPARAM wParam,LPARAM lParam)
{
	m_ProductInfoList.DeleteItem(wParam);
	CString str((TCHAR*)lParam);   //lparamתcstring(�˴�������tchar,char�ᵼ�¾������)
	CString strProductNam = m_ProductInfoList.GetItemText(wParam,2);
	CString strProductNum = m_ProductInfoList.GetItemText(wParam,3);
	CString strUserNam    = m_ProductInfoList.GetItemText(wParam,7);
	_RecordsetPtr m_pRs;
	CString sql = CString("delete * from ProductInfo where ProductID=") + str;//ɾ����Ʒ��Ϣ����Ӧ���м��������ݺ����۽������
	CString sql1= CString("delete * from ChartInfoSave where ProductNam='") + strProductNam+CString("'and ProductNum='")+strProductNum+CString("'and Uname='")+strUserNam+("'");
	CString sql2= CString("delete * from EvalResult where ProductNam='") + strProductNam+CString("'and ProductNum='")+strProductNum+CString("'and Uname='")+strUserNam+("'");
	CString sql3= CString("delete * from LowValResult where ProductNam='") + strProductNam+CString("'and ProductNum='")+strProductNum+CString("'and Uname='")+strUserNam+("'");
	CString sql4= CString("delete * from TechEvalResult where ProductNam='") + strProductNam+CString("'and ProductNum='")+strProductNum+CString("'and Uname='")+strUserNam+("'");
	
	try
	{
		m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
		m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql1), NULL, adCmdText);
		m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql2), NULL, adCmdText);
		m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql3), NULL, adCmdText);
		m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql4), NULL, adCmdText);
		AfxMessageBox(CString("��Ʒ�����Ϣɾ���ɹ���"));
	}
	catch(_com_error &e)
	{        
		CString temp;
		temp.Format(e.Description());
		AfxMessageBox(temp);
	}

	UpdateListCtrl();
	return 0;
}


//�Զ�����Ϣˢ��list
LRESULT CDFMDlg::OnUpdateList(WPARAM wParam,LPARAM lParam)
{
	UpdateListCtrl();
	return 0;
}

//�Զ�����Ϣ��������
LRESULT CDFMDlg::OnEvalIn(WPARAM wParam,LPARAM lParam)
{
	CString str;
	str=m_ProductInfoList.GetItemText(wParam,1);
	_RecordsetPtr m_pRs;
	CString sql = CString("select * from ProductInfo where ProductID=") + str ;
	m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
	CString str1 =  m_pRs->GetCollect("EvalModelID");
	int nModelID= _ttoi(str1);

	CProductNewDlg dlg;
	CProductNew1Dlg dlg1;
	CProductNew2Dlg dlg2;
	CProductNew3Dlg dlg3;
	switch(nModelID)
	{
	case 1:                 //ģ��һ��������һ
		dlg.strProductID=str;   //��ȡ��Ʒ��ϢID
		dlg.bInfoWrited=true;   //�Ҽ��˵����±༭����Ʒ��Ϣ��д��
		dlg.DoModal();
		break;
	case 2:
		dlg1.strProductID=str;   //��ȡ��Ʒ��ϢID
		dlg1.bInfoWrited=true;   //�Ҽ��˵����±༭����Ʒ��Ϣ��д��
		dlg1.DoModal();
		break;
	case 3:
		dlg2.strProductID=str;   //��ȡ��Ʒ��ϢID
		dlg2.bInfoWrited=true;   //�Ҽ��˵����±༭����Ʒ��Ϣ��д��
		dlg2.DoModal();
		break;
	case 4:
		dlg3.strProductID=str;   //��ȡ��Ʒ��ϢID
		dlg3.bInfoWrited=true;   //�Ҽ��˵����±༭����Ʒ��Ϣ��д��
		dlg3.DoModal();
		break;
	}
	return 0;
}

//�Զ�����Ϣ�鿴���۽��
LRESULT CDFMDlg::OnCheck(WPARAM wParam,LPARAM lParam)
{
	CString strName=m_ProductInfoList.GetItemText(wParam,2);
	CString strNum=m_ProductInfoList.GetItemText(wParam,3);

	_RecordsetPtr m_pRs;    //��ȡ�ۺ����۽����
	vector<CString> m_ItemVal; //���ֵ����
	CString sql = CString("select * from EvalResult where ProductNum='") + strNum +CString("'and Uname= '")+theApp.name+CString("'");
	m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
	CString str1 =  m_pRs->GetCollect("IntegEvalVal");
	CString str2 =  m_pRs->GetCollect("IntegEvalResult");
	CString str3 =  m_pRs->GetCollect("LowValResult");
	CString str4 =  m_pRs->GetCollect("IndexValResult");
	m_ItemVal.push_back(strName);  //��Ʒ����
	m_ItemVal.push_back(strNum);   //��Ʒ���
	m_ItemVal.push_back(str1);      //�ۺ����۷�ֵ
	m_ItemVal.push_back(str2);   //�ۺ����۽��
	m_ItemVal.push_back(str3);   //�ͷ���Ľ���ʾ
	m_ItemVal.push_back(str4);   //ָ��÷�ֵ��ʾ

	_RecordsetPtr m_pRs1;    //��ȡ�ͷ����
	vector<CLowValItem> m_LowValItem;//�ͷ����
	CString sql1 = CString("select * from LowValResult where ProductNum='") + strNum +CString("'and Uname= '")+theApp.name+CString("'");
	m_pRs1 = theApp.m_pConnect->Execute(_bstr_t(sql1), NULL, adCmdText);
	int n=0;
	while (!m_pRs1->adoEOF)
	{
		CLowValItem OneLowValItem;
		CString str;
		str.Format(CString("%d"),n+1);
		OneLowValItem.m_Item=str;
		OneLowValItem.m_ChartNam=m_pRs1->GetCollect("ChartName");
		OneLowValItem.m_Classify=m_pRs1->GetCollect("IndexClass");
		OneLowValItem.m_TechEvalIndex=m_pRs1->GetCollect("IndexName");
		OneLowValItem.m_IndexComment=m_pRs1->GetCollect("IndexComment");
		OneLowValItem.m_LowValAdvice=m_pRs1->GetCollect("Advice");

		m_LowValItem.push_back(OneLowValItem);
		n++;
		m_pRs1->MoveNext();
	}


	_RecordsetPtr m_pRs2;    //��ȡָ�����ֱ�
	vector<CIndexValItem> m_IndexVal;//ָ�����ֱ���
	CString sql2 = CString("select * from TechEvalResult where ProductNum='") + strNum +CString("'and Uname= '")+theApp.name+CString("'");
	m_pRs2 = theApp.m_pConnect->Execute(_bstr_t(sql2), NULL, adCmdText);
	int m=0;
	while (!m_pRs2->adoEOF)
	{
		CIndexValItem OneItem;
		CString str;
		str.Format(CString("%d"),m+1);
		OneItem.m_Item=str;
		OneItem.m_IndexNam=m_pRs2->GetCollect("IndexName");
		OneItem.m_IndexVal=m_pRs2->GetCollect("IndexVal");

		m_IndexVal.push_back(OneItem);
		m++;
		m_pRs2->MoveNext();
	}

	CProductOutDlg dlg;  //������
	dlg.GetItemInfo(m_ItemVal);
	dlg.GetResultInfo(m_LowValItem,m_IndexVal);
	dlg.DoModal();
	return 0;
}



//���ڶԻ���
void CDFMDlg::OnMenuAbout()
{
	// TODO: Add your command handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}

//�޸�����
void CDFMDlg::OnMenuPsw()
{
	// TODO: Add your command handler code here
	CChangePwdDlg dlg;
	dlg.DoModal();
}



//�½�����
void CDFMDlg::OnEvalnew()
{
	// TODO: Add your command handler code here
	CChooseEvalType dlg1;
	CProductNewDlg dlg2;
	CProductNew1Dlg dlg3;
	CProductNew2Dlg dlg4;
	CProductNew3Dlg dlg5;
	if (dlg1.DoModal()==IDOK)
	{
		int nModelID=dlg1.m_nModelID;
		switch(nModelID)
		{
		case 1:               //ģ��һ��������һ
			dlg2.str_EvalTypeID=(CString)(char *)(_bstr_t)(nModelID);
			dlg2.m_EvalType=dlg1.m_EvalType;
			dlg2.DoModal();
			break;
		case 2:
			dlg3.str_EvalTypeID=(CString)(char *)(_bstr_t)(nModelID);
			dlg3.m_EvalType=dlg1.m_EvalType;
			dlg3.DoModal();
			break;
		case 3:
			dlg4.str_EvalTypeID=(CString)(char *)(_bstr_t)(nModelID);
			dlg4.m_EvalType=dlg1.m_EvalType;
			dlg4.DoModal();
			break;
		case 4:
			dlg5.str_EvalTypeID=(CString)(char *)(_bstr_t)(nModelID);
			dlg5.m_EvalType=dlg1.m_EvalType;
			dlg5.DoModal();
			break;
		default:
			break;
		}
	}
}



void CDFMDlg::OnProductEvalval()
{
	// TODO: Add your command handler code here
	CResultCheck dlg;
	if(dlg.DoModal()==IDOK)
	{
		CString strName=dlg.m_CheckName;
		CString strNum=dlg.m_CheckNum;
		CString strUName=dlg.m_CheckUName;


		_RecordsetPtr m_pRs;    //��ȡ�ۺ����۽����
		vector<CString> m_ItemVal; //���ֵ����
		CString sql = CString("select * from EvalResult where ProductNum='") + strNum +CString("'and Uname= '")+strUName+CString("'");
		m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
		int k=0;
		CString str1,str2,str3,str4;
		while (!m_pRs->adoEOF)
		{
			str1 =  m_pRs->GetCollect("IntegEvalVal");
			str2 =  m_pRs->GetCollect("IntegEvalResult");
		    str3 =  m_pRs->GetCollect("LowValResult");
			str4 =  m_pRs->GetCollect("IndexValResult");
			++k;
			m_pRs->MoveNext();
		}
		if (k==0)        //����޸�Ŀ����ֱ������
		{
			AfxMessageBox(CString("�ò�Ʒ��δ���ۣ�"));
			return;
		}
		m_ItemVal.push_back(strName);  //��Ʒ����
		m_ItemVal.push_back(strNum);   //��Ʒ���
		m_ItemVal.push_back(str1);     //�ۺ����۷�ֵ
		m_ItemVal.push_back(str2);     //�ۺ����۽��
		m_ItemVal.push_back(str3);     //�ͷ���Ľ���ʾ
		m_ItemVal.push_back(str4);     //ָ��÷�ֵ��ʾ

		_RecordsetPtr m_pRs1;    //��ȡ�ͷ����
		vector<CLowValItem> m_LowValItem;//�ͷ����
		CString sql1 = CString("select * from LowValResult where ProductNum='") + strNum +CString("'and Uname= '")+theApp.name+CString("'");
		m_pRs1 = theApp.m_pConnect->Execute(_bstr_t(sql1), NULL, adCmdText);
		int n=0;
		while (!m_pRs1->adoEOF)
		{
			CLowValItem OneLowValItem;
			CString str;
			str.Format(CString("%d"),n+1);
			OneLowValItem.m_Item=str;
			OneLowValItem.m_ChartNam=m_pRs1->GetCollect("ChartName");
			OneLowValItem.m_Classify=m_pRs1->GetCollect("IndexClass");
			OneLowValItem.m_TechEvalIndex=m_pRs1->GetCollect("IndexName");
			OneLowValItem.m_IndexComment=m_pRs1->GetCollect("IndexComment");
			OneLowValItem.m_LowValAdvice=m_pRs1->GetCollect("Advice");

			m_LowValItem.push_back(OneLowValItem);
			n++;
			m_pRs1->MoveNext();
		}


		_RecordsetPtr m_pRs2;    //��ȡָ�����ֱ�
		vector<CIndexValItem> m_IndexVal;//ָ�����ֱ���
		CString sql2 = CString("select * from TechEvalResult where ProductNum='") + strNum +CString("'and Uname= '")+theApp.name+CString("'");
		m_pRs2 = theApp.m_pConnect->Execute(_bstr_t(sql2), NULL, adCmdText);
		int m=0;
		while (!m_pRs2->adoEOF)
		{
			CIndexValItem OneItem;
			CString str;
			str.Format(CString("%d"),m+1);
			OneItem.m_Item=str;
			OneItem.m_IndexNam=m_pRs2->GetCollect("IndexName");
			OneItem.m_IndexVal=m_pRs2->GetCollect("IndexVal");

			m_IndexVal.push_back(OneItem);
			m++;
			m_pRs2->MoveNext();
		}

		CProductOutDlg dlg1;  //������
		dlg1.GetItemInfo(m_ItemVal);
		dlg1.GetResultInfo(m_LowValItem,m_IndexVal);
		dlg1.DoModal();
	}
}
