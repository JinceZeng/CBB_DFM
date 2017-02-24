// ProductOutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DFM.h"
#include "ProductOutDlg.h"
#include "afxdialogex.h"


// CProductOutDlg dialog

IMPLEMENT_DYNAMIC(CProductOutDlg, CDialogEx)

CProductOutDlg::CProductOutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProductOutDlg::IDD, pParent)
{

}

CProductOutDlg::~CProductOutDlg()
{
	vector<CResultChartItem>().swap(m_ListCtrlItem);
}

void CProductOutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RESULT, m_ValueResultList);
}


BEGIN_MESSAGE_MAP(CProductOutDlg, CDialogEx)
	ON_MESSAGE(WM_INDEXMATCH,&CProductOutDlg::OnIndexMatch)
	ON_BN_CLICKED(IDOK, &CProductOutDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CProductOutDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CProductOutDlg message handlers


BOOL CProductOutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//���б�ؼ��ȳ�ʼ��,�����ͷ
	CRect rect1;
	int width1;
	m_ValueResultList.GetClientRect(&rect1);
	width1=rect1.Width();

	m_ValueResultList.InsertColumn(0,CString("���"),LVCFMT_CENTER,width1/4);
	m_ValueResultList.InsertColumn(1,CString("��Ŀ"),LVCFMT_CENTER,width1/4);
	m_ValueResultList.InsertColumn(2,CString("Ŀ��ֵ"),LVCFMT_CENTER,width1/2);


	//���ò��ɱ༭��
	vector<int> nNoEdit;
	nNoEdit.push_back(0);               //�����в����Ա༭
	nNoEdit.push_back(1);  
	nNoEdit.push_back(2);  
	m_ValueResultList.SetnNoEditList(nNoEdit);
	vector<int>().swap(nNoEdit);        //�ͷ�vector

	SetListItem(m_ListCtrlItem);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CProductOutDlg::GetItemInfo(vector<CString>& ItemVal)
{
	int nItem=1;
	vector<CString> strItemNam;
	strItemNam.push_back(CString("��Ʒ����"));
	strItemNam.push_back(CString("��Ʒ�������"));
	strItemNam.push_back(CString("�ۺ����۷�ֵ"));
	strItemNam.push_back(CString("�ۺ����۽��"));
	strItemNam.push_back(CString("�ͷ���Ľ�����"));
	strItemNam.push_back(CString("ָ��÷�ֵ"));

	for (int i=0;i<strItemNam.size();++i,++nItem)
	{
		CString strItem;
		strItem.Format(CString("%d"),nItem);

		CResultChartItem tempItem;
		tempItem.nItem=strItem;
		tempItem.m_ItemNam=strItemNam[i];
		tempItem.m_ItemVal=ItemVal[i];

		m_ListCtrlItem.push_back(tempItem);
	}
	vector<CString>().swap(strItemNam);

}


void CProductOutDlg::SetListItem(vector<CResultChartItem> &m_ListCtrlItem)
{
	//list��Ϣд��
	m_ValueResultList.DeleteAllItems();
	for (int i=0;i<m_ListCtrlItem.size();++i)
	{
		m_ValueResultList.InsertItem(i,m_ListCtrlItem[i].nItem);
		m_ValueResultList.SetItemText(i,1,m_ListCtrlItem[i].m_ItemNam);
		m_ValueResultList.SetItemText(i,2,m_ListCtrlItem[i].m_ItemVal);
	}
}

//��ȡCProductNewDlg�����Ϣ����m_LowValItem��m_IndexVal��ֵ
void  CProductOutDlg::GetResultInfo(vector<CLowValItem>& m_LowValItem,vector<CIndexValItem>& m_IndexVal)
{
	this->m_LowValItem=m_LowValItem;
	this->m_IndexVal=m_IndexVal;
}

LRESULT CProductOutDlg::OnIndexMatch(WPARAM wParam,LPARAM lParam)
{
	CString strItemName=m_ValueResultList.GetItemText(wParam,1);
	if(strItemName==CString("�ͷ���Ľ�����"))
	{
		CLowValChartDlg dlg;
		dlg.GetInfo(m_LowValItem);
		dlg.DoModal();
	}
	else if (strItemName==CString("ָ��÷�ֵ"))
	{
		CIndexValChartDlg dlg;
		dlg.GetInfo(m_IndexVal);
		dlg.DoModal();
	}
	return 0;
}

void CProductOutDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//�������ݿ���Ᵽ����ͬ��Ʒ��Ϣ
	_RecordsetPtr m_pRs;
	CString sql=CString("select * from EvalResult where ProductNam ='")+m_ListCtrlItem[0].m_ItemVal+CString("'and ProductSub='")+m_ListCtrlItem[1].m_ItemVal+CString("'and Uname='")+theApp.name+("'");
	m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
	int nCount=0;
	while (!m_pRs->adoEOF)
	{
		nCount++;
		m_pRs->MoveNext();

	}
	if(nCount>0)
	{
		AfxMessageBox(CString("�����۸ò�Ʒ���������±���"));
	}
	else
	{
		//������������ֵ
		theApp.m_pConnect->Execute((_bstr_t)(CString("insert into EvalResult(ProductNam,ProductSub,Uname,IntegEvalVal,IntegEvalResult,LowValResult,IndexValResult) values('")
			+m_ListCtrlItem[0].m_ItemVal+"','"+m_ListCtrlItem[1].m_ItemVal+"','"+theApp.name+"','"+m_ListCtrlItem[2].m_ItemVal+"','"+m_ListCtrlItem[3].m_ItemVal+"','"+m_ListCtrlItem[4].m_ItemVal+"','"+m_ListCtrlItem[5].m_ItemVal+("')")) , NULL, adCmdText);   //insert����ʱ���ݿ��ж�Ӧ�ַ�����ӡ���
		
		//����ͷ���
		for(int i=0;i<m_LowValItem.size();++i)
		{
			theApp.m_pConnect->Execute((_bstr_t)(CString("insert into LowValResult(ProductNam,ProductSub,Uname,ChartName,IndexClass,IndexName,IndexComment,Advice) values('")
				+m_ListCtrlItem[0].m_ItemVal+"','"+m_ListCtrlItem[1].m_ItemVal+"','"+theApp.name+"','"+m_LowValItem[i].m_ChartNam+"','"+m_LowValItem[i].m_Classify+"','"+m_LowValItem[i].m_TechEvalIndex+"','"+m_LowValItem[i].m_IndexComment+"','"+m_LowValItem[i].m_LowValAdvice+("')")) , NULL, adCmdText);   //insert����ʱ���ݿ��ж�Ӧ�ַ�����ӡ���
		}

		//����ָ������ֵ
		for(int i=0;i<m_IndexVal.size();++i)
		{
			theApp.m_pConnect->Execute((_bstr_t)(CString("insert into TechEvalResult(ProductNam,ProductSub,Uname,IndexName,IndexVal) values('")
				+m_ListCtrlItem[0].m_ItemVal+"','"+m_ListCtrlItem[1].m_ItemVal+"','"+theApp.name+"','"+m_IndexVal[i].m_IndexNam+"','"+m_IndexVal[i].m_IndexVal+("')")) , NULL, adCmdText);   //insert����ʱ���ݿ��ж�Ӧ�ַ�����ӡ���
		}

		//���²�Ʒ����״̬
		theApp.m_pConnect->Execute((_bstr_t)(CString("update ProductInfo set IsEval=")+CString("1 where ProductNam='")+m_ListCtrlItem[0].m_ItemVal+CString("' and ProductSub='")+m_ListCtrlItem[1].m_ItemVal+CString("'and Uname='")+theApp.name+CString("'")) , NULL, adCmdText);
		
		AfxMessageBox(CString("���۽������ɹ�"));	
	}

	

	CDialogEx::OnOK();
}



void CProductOutDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
