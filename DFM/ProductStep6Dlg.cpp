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

//�ؼ���Сλ�ñ仯
BEGIN_EASYSIZE_MAP(CProductStep6Dlg, CDialogEx) 
	EASYSIZE(IDC_LIST_TECHMATURY,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
END_EASYSIZE_MAP 

// CProductStep6Dlg message handlers
/////////////////////////////////////���ǰҳ(����һҳ����һҳת����ҳ�������)
//����ֻ��һ���ṹ��Ϣ������CListCtl�Ļ�����Ա�����Ϣ�������Լ�����CListCtrl
DWORD CProductStep6Dlg::OnWizardActive()
{
	//������Ϣ���룬��ɳ�ʼ������
	//m_TechMaturyList.DeleteAllItems();
	//m_ListCtrlItem.clear();
	//m_LowValItem.clear();
	//m_LowValItemNum=0;      //���ڼ�������¼���

	ShowWindow(SW_SHOW);
	return 0;
}

/////////////////////////////////////////////ǰ����һҳ
//���Լ��鲢���浱ǰ����,����-1���л�������0�л�
DWORD CProductStep6Dlg::OnWizardNext()
{
	if(m_ListCtrlItem.size()==0)
	{
		AfxMessageBox(CString("�����ʹ�ù���"));
		return -1;
	}
	if(m_TechMaturyList.m_bEditing==TRUE)
	{
		MessageBox(_T("����:�б�ؼ����ڱ༭״̬"));
		return -1;
	}
	for (int i=0;i<m_ListCtrlItem.size();++i)
	{
		if(m_ListCtrlItem[i].m_TechNam==CString(""))
		{
			AfxMessageBox(CString("����ɹ�����Ϣ"));
			return -1;
		}
	}
	SaveChartInfo(); //�����������
	SaveLowValItem();//�ȱ���ͷ���

	ShowWindow(SW_HIDE);
	return 0;

}

/////////////////////////////////////////ǰ����һҳ
//���Լ�����һ���������ñ���
DWORD CProductStep6Dlg::OnWizardPrevious()
{
	if(m_TechMaturyList.m_bEditing==TRUE)
	{
		MessageBox(_T("����:�б�ؼ����ڱ༭״̬"));
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

	m_TechMaturyList.InsertColumn(0,CString("���"),LVCFMT_CENTER,width1/8);
	m_TechMaturyList.InsertColumn(1,CString("��������"),LVCFMT_CENTER,width1*3/8);
	m_TechMaturyList.InsertColumn(2,CString("����Ӧ��״̬"),LVCFMT_CENTER,width1/4);
	m_TechMaturyList.InsertColumn(3,CString("���ճ��������"),LVCFMT_CENTER,width1/4);


	//���ò��ɱ༭��
	vector<int> nNoEdit;
	nNoEdit.push_back(0);               //�����в����Ա༭
	nNoEdit.push_back(1);  
	nNoEdit.push_back(2); 
	nNoEdit.push_back(3); 
	m_TechMaturyList.SetnNoEditList(nNoEdit);
	vector<int>().swap(nNoEdit);//�ͷ�vector

	vector<int> nComboLis;
	nComboLis.push_back(1);            //����������Ͽ����
	m_TechMaturyList.SetnComboList(nComboLis);
	vector<int>().swap(nComboLis);//�ͷ�vector


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
		AfxMessageBox(CString("���ڽ��б༭��޷�ɾ��"));
		return 0;
	}
	else if (m_ListCtrlItem.size()==0)
		return 0;

	m_ListCtrlItem.pop_back();       //�������һ��
	int n=m_TechMaturyList.GetItemCount();
	m_TechMaturyList.DeleteItem(n-1);
	return 0;
}

LRESULT CProductStep6Dlg::OnAddIndexItem(WPARAM wParam,LPARAM lParam)
{
	int n=m_TechMaturyList.GetItemCount();
	if (n>0&&m_TechMaturyList.GetItemText(n-1,1)==CString(""))
	{
		AfxMessageBox(CString("����ѡ��������ӣ�"));
		return 0;
	}
	CString  str1;
	str1.Format(_T("%d"),n+1);
	m_TechMaturyList.InsertItem(n,str1);   //���ID��һ��
	m_TechMaturyList.SetItemText(n,1,_T(""));
	m_TechMaturyList.SetItemText(n,2,_T(""));
	m_TechMaturyList.SetItemText(n,3,_T(""));

	m_TechMaturyList.SetComboString(m_TechType);   //��combo�ַ�����listctrl��չ���е�m_strlisCombo����combo��ʼ��

	//���浽m_ListCtrlItem
	CMaturyChartItem OneItem;//��ʱ�洢ÿ��list����Ϣ
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

		m_TechType.push_back(str1);       //�����洢����combo��ʼ��
		m_TechMaturityInfo.push_back(vtemp);

		vector<CString>().swap(vtemp);//�����ͷŵ�

		m_pRs->MoveNext();

	}
}
//��Ӧ��ֵ������
LRESULT CProductStep6Dlg::OnSetIndexVal(WPARAM wParam,LPARAM lParam)
{
	int n=m_TechMaturyList.GetItemCount();

	CString strValInfo=m_TechMaturyList.GetItemText(wParam,1);
	for(int i=0; i<n-1;i++)           //�����ظ��ж�
	{
		CString str=m_TechMaturyList.GetItemText(i,1);
		if(str==strValInfo)   
		{
			AfxMessageBox(CString("��ѡ�����ظ���"));
			m_ListCtrlItem.pop_back();       //�������һ��
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

	//��������
	m_ListCtrlItem[n-1].m_TechNam=strValInfo;
	m_ListCtrlItem[n-1].m_TechUseStatus=m_TechMaturityInfo[i][2];
	m_ListCtrlItem[n-1].m_TechMaturyVal=m_TechMaturityInfo[i][1];

	m_TechMaturyList.SetItemText(wParam,2,m_ListCtrlItem[n-1].m_TechUseStatus);                    //����ʹ��״̬
	m_TechMaturyList.SetItemText(wParam,3,m_ListCtrlItem[n-1].m_TechMaturyVal);                    //���÷�ֵ

	return 0;
}

//�洢�ͷ���(��m_ListCtrlItem��ֵ��ɺ����)
void CProductStep6Dlg::SaveLowValItem()
{
	m_LowValItem.clear();
	m_LowValItemNum=0;
	for (int i=0;i<m_ListCtrlItem.size();++i)
	{
		CString strVal=m_ListCtrlItem[i].m_TechMaturyVal;//��ȡ����
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
				strComment=CString("�ǳ�����");
				break;
			case -1:
				strComment=CString("����");
				break;
			case -2:
				strComment=CString("������");
				break;
			case -3:
				strComment=CString("������");
				break;
			}
			CString str;
			str.Format(CString("%d"),m_LowValItemNum+1);
			OneLowValItem.m_Item=str;
			OneLowValItem.m_ChartNam=CString("���ճ���ȱ�");
			OneLowValItem.m_Classify=CString("����");
			OneLowValItem.m_TechEvalIndex=m_ListCtrlItem[i].m_TechNam;
			OneLowValItem.m_IndexComment=strComment;
			OneLowValItem.m_LowValAdvice=CString("������ѡ����");

			m_LowValItem.push_back(OneLowValItem);

			++m_LowValItemNum;
		}
	}
}



//������������Ϣ���´ν���ֱ�ӳ�ʼ��
void CProductStep6Dlg::SaveChartInfo()
{
	_RecordsetPtr m_pRs;  //��ɾ��֮ǰ��¼��Ϣ�����������ظ�����
	CString sql = CString("delete * from ChartInfoSave where ProductNam='") + m_ProductInfo.m_ProductName+CString("'and ProductSub='")+m_ProductInfo.m_ProductSub+CString("'and ChartName='")+CString("���ճ���ȱ�")+CString("'and Uname='")+theApp.name+("'");
	try
	{
		m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
		//AfxMessageBox(CString("��Ʒ��Ϣɾ���ɹ���"));
	}
	catch(_com_error &e)
	{        
		CString temp;
		temp.Format(e.Description());
		AfxMessageBox(temp);
	}


	//���ݴ洢
	try
	{
		for(int i=0;i<m_ListCtrlItem.size();++i)
		{
			theApp.m_pConnect->Execute((_bstr_t)(CString("insert into ChartInfoSave(ProductNam,ProductSub,Uname,ChartName,Row1,Row2,Row3) values('")
				+m_ProductInfo.m_ProductName+"','"+m_ProductInfo.m_ProductSub+"','"+theApp.name+"','"+CString("���ճ���ȱ�")+"','"+m_ListCtrlItem[i].m_TechNam+"','"+m_ListCtrlItem[i].m_TechUseStatus+"','"+m_ListCtrlItem[i].m_TechMaturyVal+("')")) , NULL, adCmdText);   //insert����ʱ���ݿ��ж�Ӧ�ַ�����ӡ���
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

//���ֱ���Ϣ��ʼ��
void CProductStep6Dlg::InitChartInfo()
{
	_RecordsetPtr m_pRs;    //��ȡ��ʷ����������Ϣ
	CMaturyChartItem OneItem;//��ʱ�洢ÿ��list����Ϣ
	CString sql = CString("select * from ChartInfoSave where ProductSub='") + m_ProductInfo.m_ProductSub+CString("'and ChartName= '")+CString("���ճ���ȱ�")+CString("'and Uname= '")+theApp.name+CString("'");
	m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
	int k=0;
	while (!m_pRs->adoEOF)
	{
		CString  str1;
		str1.Format(_T("%d"),k+1);
		OneItem.nItem=str1;
		OneItem.m_TechNam       =  m_pRs->GetCollect("Row1");  //ֻ�貹��֮ǰ���������
		OneItem.m_TechUseStatus =  m_pRs->GetCollect("Row2");
		OneItem.m_TechMaturyVal =  m_pRs->GetCollect("Row3");

		m_ListCtrlItem.push_back(OneItem);
		++k;
		m_pRs->MoveNext();
	}
	if (k==0)        //����޸�Ŀ����ֱ������
		return;
	else             //��ԭlist��Ŀ
	{
		for (int i=0;i<m_ListCtrlItem.size();i++)
		{
			m_TechMaturyList.InsertItem(i,m_ListCtrlItem[i].nItem);
			m_TechMaturyList.SetItemText(i,1,m_ListCtrlItem[i].m_TechNam);
			m_TechMaturyList.SetItemText(i,2,m_ListCtrlItem[i].m_TechUseStatus);
			m_TechMaturyList.SetItemText(i,3,m_ListCtrlItem[i].m_TechMaturyVal);

			m_TechMaturyList.SetComboString(m_TechType);   //��combo�ַ�����listctrl��չ���е�m_strlisCombo����combo��ʼ��


		}
	}
}


