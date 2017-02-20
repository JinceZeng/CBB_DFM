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
END_MESSAGE_MAP()


// CProductStep7Dlg message handlers
DWORD CProductStep7Dlg::OnWizardActive()
{
	ShowWindow(SW_SHOW);
	return 0;
}

/////////////////////////////////////////////ǰ����һҳ
//���Լ��鲢���浱ǰ����,����-1���л�������0�л�
DWORD CProductStep7Dlg::OnWizardNext()
{
	int n=m_MatInfoList.GetItemCount();
	if (n>0&&(m_MatInfoList.GetItemText(n-1,1)==CString("")||m_MatInfoList.GetItemText(n-1,2)==CString("")||m_MatInfoList.GetItemText(n-1,3)==CString("")||m_MatInfoList.GetItemText(n-1,4)==CString("")))
	{
		AfxMessageBox(CString("�������Ʋ�����Ϣ����ӣ�"));
		return -1;
	}
	if (n>0)   //�������һ��������Ϣ
	{
		m_ListCtrlItem[n-1].m_MatNam=m_MatInfoList.GetItemText(n-1,1);
		m_ListCtrlItem[n-1].m_Thick=m_MatInfoList.GetItemText(n-1,2);
		m_ListCtrlItem[n-1].m_Length=m_MatInfoList.GetItemText(n-1,3);
		m_ListCtrlItem[n-1].m_Width=m_MatInfoList.GetItemText(n-1,4);
	}
	if(m_MatInfoList.m_bEditing==TRUE)
	{
		MessageBox(_T("����:�б�ؼ����ڱ༭״̬"));
		return -1;
	}
	if(m_ListCtrlItem.size()==0)
	{
		AfxMessageBox(CString("�����ʹ�ò���"));
		return -1;
	}
	SaveChartInfo(); //�����������
	SaveLowValItem();//��ȡ��ֵ������ͷ���

	ShowWindow(SW_HIDE);
	return 0;

}

/////////////////////////////////////////ǰ����һҳ
//���Լ�����һ���������ñ���
DWORD CProductStep7Dlg::OnWizardPrevious()
{
	if(m_MatInfoList.m_bEditing==TRUE)
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

BOOL CProductStep7Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rect1;
	int width1;
	m_MatInfoList.GetClientRect(&rect1);
	width1=rect1.Width();

	m_MatInfoList.InsertColumn(0,CString("���"),LVCFMT_CENTER,width1/5);
	m_MatInfoList.InsertColumn(1,CString("��������"),LVCFMT_CENTER,width1/5);
	m_MatInfoList.InsertColumn(2,CString("���"),LVCFMT_CENTER,width1/5);
	m_MatInfoList.InsertColumn(3,CString("����"),LVCFMT_CENTER,width1/5);
	m_MatInfoList.InsertColumn(4,CString("���"),LVCFMT_CENTER,width1/5);


	//���ò��ɱ༭��
	vector<int> nNoEdit;
	nNoEdit.push_back(0);               //�����в����Ա༭
	nNoEdit.push_back(1);  
	m_MatInfoList.SetnNoEditList(nNoEdit);
	vector<int>().swap(nNoEdit);//�ͷ�vector

	vector<int> nComboLis;
	nComboLis.push_back(1);             //����������Ͽ����
	m_MatInfoList.SetnComboList(nComboLis);
	vector<int>().swap(nComboLis);//�ͷ�vector
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT CProductStep7Dlg::OnDeleteIndexItem(WPARAM wParam,LPARAM lParam)
{
	if(m_MatInfoList.m_bEditing)
	{
		AfxMessageBox(CString("���ڽ��б༭��޷�ɾ��"));
		return 0;
	}
	else if (m_ListCtrlItem.size()==0)
		return 0;

	m_ListCtrlItem.pop_back();       //�������һ��
	int n=m_MatInfoList.GetItemCount();
	m_MatInfoList.DeleteItem(n-1);
	return 0;
}


LRESULT CProductStep7Dlg::OnAddIndexItem(WPARAM wParam,LPARAM lParam)
{
	int n=m_MatInfoList.GetItemCount();
	if (n>0&&(m_MatInfoList.GetItemText(n-1,1)==CString("")||m_MatInfoList.GetItemText(n-1,2)==CString("")||m_MatInfoList.GetItemText(n-1,3)==CString("")||m_MatInfoList.GetItemText(n-1,4)==CString("")))
	{
		AfxMessageBox(CString("�������Ʋ�����Ϣ����ӣ�"));
		return 0;
	}
	if (n>0)   //������һ��������Ϣ
	{
		m_ListCtrlItem[n-1].m_MatNam=m_MatInfoList.GetItemText(n-1,1);
		m_ListCtrlItem[n-1].m_Thick=m_MatInfoList.GetItemText(n-1,2);
		m_ListCtrlItem[n-1].m_Length=m_MatInfoList.GetItemText(n-1,3);
		m_ListCtrlItem[n-1].m_Width=m_MatInfoList.GetItemText(n-1,4);
	}
	CString  str1;
	str1.Format(_T("%d"),n+1);
	m_MatInfoList.InsertItem(n,str1);   //���ID��һ��
	m_MatInfoList.SetItemText(n,1,_T(""));
	m_MatInfoList.SetItemText(n,2,_T(""));
	m_MatInfoList.SetItemText(n,3,_T(""));
	m_MatInfoList.SetItemText(n,4,_T(""));

	m_MatInfoList.SetComboString(m_MatType);   //��combo�ַ�����listctrl��չ���е�m_strlisCombo����combo��ʼ��

	//���浽m_ListCtrlItem
	CMatChartItem1 OneItem;//��ʱ�洢ÿ��list����Ϣ
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

		m_MatType.push_back(str1);       //�����洢����combo��ʼ��
		m_MatInfo.push_back(vtemp);

		vector<CString>().swap(vtemp);//�����ͷŵ�

		m_pRs->MoveNext();

	}
}


LRESULT CProductStep7Dlg::OnSetIndexVal(WPARAM wParam,LPARAM lParam)
{
	int n=m_MatInfoList.GetItemCount();

	CString strValInfo=m_MatInfoList.GetItemText(wParam,1);
	for(int i=0; i<n-1;i++)           //�����ظ��ж�
	{
		CString str=m_MatInfoList.GetItemText(i,1);
		if(str==strValInfo)   
		{
			AfxMessageBox(CString("��ѡ�����ظ���"));
			OnDeleteIndexItem(0,0);         //���ѡ����ͬɾ������
			return 0;
		}
	}

	return 0;
}



//�洢�ͷ���(��m_ListCtrlItem��ֵ��ɺ����)
void CProductStep7Dlg::SaveLowValItem()
{
	m_LowValItem.clear();
	m_LowValItemNum=0;
	for (int i=0;i<m_ListCtrlItem.size();++i)
	{
		CString strMatNam=m_ListCtrlItem[i].m_MatNam;//��ȡ����
		CString strThick=m_ListCtrlItem[i].m_Thick;
		CString strLength=m_ListCtrlItem[i].m_Length;
		CString strWidth=m_ListCtrlItem[i].m_Width;
		int nThick= _ttoi(strThick),nLength= _ttoi(strLength),nWidth= _ttoi(strWidth);

		int k=0;                               //ƥ�����ݿ��в��ϳߴ����ޣ��ó���ֵ
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

		m_ListCtrlItem[i].m_MatScore=strDeductVal; //�洢����÷�

		CLowValItem OneLowValItem;      //�жϵͷ��ȷ������
		int nDeductVal= _ttoi(strDeductVal);
		if (nDeductVal<-1)
		{
			CString strComment;
			switch(nDeductVal)
			{
			case 0:
				strComment=CString("�Ƽ�");
				break;
			case -1:
				strComment=CString("����");
				break;
			case -2:
				strComment=CString("����");
				break;
			case -3:
				strComment=CString("������");
				break;
			}
			CString str;
			str.Format(CString("%d"),m_LowValItemNum+1);
			OneLowValItem.m_Item=str;
			OneLowValItem.m_ChartNam=CString("���Ͽ�ȡ��");
			OneLowValItem.m_Classify=CString("����");
			OneLowValItem.m_TechEvalIndex=m_ListCtrlItem[i].m_MatNam;
			OneLowValItem.m_IndexComment=strComment;
			OneLowValItem.m_LowValAdvice=CString("������ѡ�����");

			m_LowValItem.push_back(OneLowValItem);

			++m_LowValItemNum;
		}
	}
}



//������������Ϣ���´ν���ֱ�ӳ�ʼ��
void CProductStep7Dlg::SaveChartInfo()
{
	_RecordsetPtr m_pRs;  //��ɾ��֮ǰ��¼��Ϣ�����������ظ�����
	CString sql = CString("delete * from ChartInfoSave where ProductNam='") + m_ProductInfo.m_ProductName+CString("'and ProductNum='")+m_ProductInfo.m_ProductNum+CString("'and ChartName='")+CString("���Ͽ�ȡ��")+CString("'and Uname='")+theApp.name+("'");
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
			theApp.m_pConnect->Execute((_bstr_t)(CString("insert into ChartInfoSave(ProductNam,ProductNum,Uname,ChartName,Row1,Row2,Row3,Row4) values('")
				+m_ProductInfo.m_ProductName+"','"+m_ProductInfo.m_ProductNum+"','"+theApp.name+"','"+CString("���Ͽ�ȡ��")+"','"+m_ListCtrlItem[i].m_MatNam+"','"+m_ListCtrlItem[i].m_Length+"','"+m_ListCtrlItem[i].m_Width+"','"+m_ListCtrlItem[i].m_Thick+("')")) , NULL, adCmdText);   //insert����ʱ���ݿ��ж�Ӧ�ַ�����ӡ���
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
void CProductStep7Dlg::InitChartInfo()
{
	_RecordsetPtr m_pRs;    //��ȡ��ʷ����������Ϣ
	CMatChartItem1 OneItem;//��ʱ�洢ÿ��list����Ϣ
	CString sql = CString("select * from ChartInfoSave where ProductNum='") + m_ProductInfo.m_ProductNum+CString("'and ChartName= '")+CString("���Ͽ�ȡ��")+CString("'and Uname= '")+theApp.name+CString("'");
	m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
	int k=0;
	while (!m_pRs->adoEOF)
	{
		CString  str1;
		str1.Format(_T("%d"),k+1);
		OneItem.m_Item=str1;
		OneItem.m_MatNam =  m_pRs->GetCollect("Row1");  //ֻ�貹��֮ǰ���������
		OneItem.m_Length =  m_pRs->GetCollect("Row2");
		OneItem.m_Width  =  m_pRs->GetCollect("Row3");
		OneItem.m_Thick  =  m_pRs->GetCollect("Row4");

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
			m_MatInfoList.InsertItem(i,m_ListCtrlItem[i].m_Item);
			m_MatInfoList.SetItemText(i,1,m_ListCtrlItem[i].m_MatNam);
			m_MatInfoList.SetItemText(i,2,m_ListCtrlItem[i].m_Length);
			m_MatInfoList.SetItemText(i,3,m_ListCtrlItem[i].m_Width);
			m_MatInfoList.SetItemText(i,4,m_ListCtrlItem[i].m_Thick);
		}
	}
}