// ProductStep4Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "DFM.h"
#include "ProductStep4Dlg.h"
#include "afxdialogex.h"


// CProductStep4Dlg dialog

IMPLEMENT_DYNAMIC(CProductStep4Dlg, CDialogEx)

CProductStep4Dlg::CProductStep4Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProductStep4Dlg::IDD, pParent)
	,m_LowValItemNum(0)
{
	m_ListCtrlItem.clear();
	m_Lvl3TechID.clear();
	m_IndexValInfo.clear();
}

CProductStep4Dlg::~CProductStep4Dlg()
{
	vector<CString>().swap(m_Lvl3TechID);
	vector<CTechChartItem>().swap(m_ListCtrlItem);
	vector<vector<CString>>().swap(m_IndexValInfo);
	vector<CLowValItem>().swap(m_LowValItem);
}

void CProductStep4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_THREEPROVAL, m_ThreeProValList);
}


BEGIN_MESSAGE_MAP(CProductStep4Dlg, CDialogEx)
	ON_MESSAGE(WM_SETINDEXVAL,&CProductStep4Dlg::OnSetIndexVal)
END_MESSAGE_MAP()


// CProductStep4Dlg message handlers
BOOL CProductStep4Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//���б��ؼ��ȳ�ʼ��,�����ͷ
	CRect rect1;
	int width1;
	m_ThreeProValList.GetClientRect(&rect1);
	width1=rect1.Width();

	m_ThreeProValList.InsertColumn(0,CString("���"),LVCFMT_CENTER,width1/20);
	m_ThreeProValList.InsertColumn(1,CString("���"),LVCFMT_CENTER,width1*3/10);
	m_ThreeProValList.InsertColumn(2,CString("������"),LVCFMT_CENTER,width1/4);
	m_ThreeProValList.InsertColumn(3,CString("����������"),LVCFMT_CENTER,width1*3/10);
	m_ThreeProValList.InsertColumn(4,CString("����ֵ"),LVCFMT_CENTER,width1/10);


	//���ò��ɱ༭��
	vector<int> nNoEdit;
	nNoEdit.push_back(0);               //�����в����Ա༭
	nNoEdit.push_back(1);  
	nNoEdit.push_back(2);  
	m_ThreeProValList.SetnNoEditList(nNoEdit);
	vector<int>().swap(nNoEdit);//�ͷ�vector

	vector<int> nComboLis;
	nComboLis.push_back(3);            //����������Ͽ����
	m_ThreeProValList.SetnComboList(nComboLis);
	vector<int>().swap(nComboLis);//�ͷ�vector

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////���ǰҳ(����һҳ����һҳת����ҳ�������)
//����ֻ��һ���ṹ��Ϣ������CListCtl�Ļ�����Ա�����Ϣ�������Լ�����CListCtrl
DWORD CProductStep4Dlg::OnWizardActive()
{
	//������Ϣ���룬��ɳ�ʼ������
	//m_LowValItem.clear();
	//m_LowValItemNum=0;      //���ڼ�������¼���
	ShowWindow(SW_SHOW);
	return 0;
}

/////////////////////////////////////////////ǰ����һҳ
//���Լ��鲢���浱ǰ����,����-1���л�������0�л�
DWORD CProductStep4Dlg::OnWizardNext()
{
	//���浱ǰ������������Ϣ
	if(m_ThreeProValList.m_bEditing==TRUE)
	{
		MessageBox(_T("����:�б��ؼ����ڱ༭״̬"));
		return -1;
	}
	for (int i=0;i<m_ListCtrlItem.size();++i)
	{
		CString str=m_ThreeProValList.GetItemText(i,3);
		if(str==CString(""))
		{
			AfxMessageBox(CString("������δ���"));
			return -1;
		}
	}
	SaveChartInfo();                  //�����������
	SaveLowValItem(m_ListCtrlItem);   //��m_ListCtrlItem��Ϣ������ɣ���洢���еͷ���

	ShowWindow(SW_HIDE);     //��ʱ����д�����ڼӼ����ж�
	return 0;

}

/////////////////////////////////////////ǰ����һҳ
//���Լ�����һ���������ñ���
DWORD CProductStep4Dlg::OnWizardPrevious()
{
	if(m_ThreeProValList.m_bEditing==TRUE)
	{
		MessageBox(_T("����:�б��ؼ����ڱ༭״̬"));
		return -1;
	}
	else
	{
		//m_bLegal=FALSE;
		ShowWindow(SW_HIDE);
		return 0;
	}
}



void CProductStep4Dlg::ReadTechChart(CProductInfo& m_ProductInfo)
{
	_RecordsetPtr m_pRs0;
	m_Lvl3TechID.clear();
	int UpID=12;             //��Ѱ����ָ�ָ꣨��IDΪ12��������Ӧ�ԣ�������
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
}



void CProductStep4Dlg::ShowListCtrl()
{
	_RecordsetPtr m_pRs,m_pRs2;
	m_ListCtrlItem.clear();
	m_IndexValInfo.clear();

	CTechChartItem m_OneItem;//ÿ��list����Ϣ
	int nItem=1;             //���
	for (int i=0;i<m_Lvl3TechID.size();i++)
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

			vector<CString> m_ValComboStr;  //���ڴ洢����combo�ַ�
			vector<CString> vTemp;          //�����洢������Ͷ�Ӧ��ֵ
			CString sql2 = CString("select * from EvalIndexVal where TechEvalIndexID=")+m_Lvl3TechID[i];
			m_pRs2 = theApp.m_pConnect->Execute(_bstr_t(sql2), NULL, adCmdText);
			while(!m_pRs2->adoEOF)
			{
				CString strValInfo=(CString)(m_pRs2->GetCollect("TechEvalIndexValInfo"));
				CString strVal=(CString)(m_pRs2->GetCollect("TechDeductVal"));
				m_ValComboStr.push_back(strValInfo);   //����combo��ʼ��

				CString str=strValInfo+CString(":")+strVal;//Ϊ��������άvector������������ʾ
				vTemp.push_back(str);

				m_pRs2->MoveNext();
			}

			m_IndexValInfo.push_back(vTemp);
			vector<CString>().swap(vTemp);//�ͷ���ʱvector


			//ÿ��list��Ϣ
			m_OneItem.nItem=strItem;
			m_OneItem.m_Classify=strClassify;
			m_OneItem.m_TechEvalIndex=strTechNam;
			m_OneItem.m_ValComboStr=m_ValComboStr;
			m_OneItem.m_LowValAdvice=strLowValAdvice;

			vector<CString>().swap(m_ValComboStr);//�ͷ�vector
			nItem++;

			m_ListCtrlItem.push_back(m_OneItem);//���δ���list��ÿ����Ϣ
		}

		catch(_com_error e)
		{
			CString temp;
			temp.Format(e.Description());
			AfxMessageBox(temp);
		}
	}
	InitChartInfo();   //�����Ƿ���ǰ���й�����洢����ȡ�������������
	SetListItem(m_ListCtrlItem);
}



void CProductStep4Dlg::SetListItem(vector<CTechChartItem>& m_ListCtrlItem)
{
	m_ThreeProValList.DeleteAllItems();
	for (int i=0;i<m_ListCtrlItem.size();i++)
	{
		m_ThreeProValList.InsertItem(i,m_ListCtrlItem[i].nItem);
		m_ThreeProValList.SetItemText(i,1,m_ListCtrlItem[i].m_Classify);
		m_ThreeProValList.SetItemText(i,2,m_ListCtrlItem[i].m_TechEvalIndex);
		m_ThreeProValList.SetComboString(m_ListCtrlItem[i].m_ValComboStr);   //��combo�ַ�����listctrl��չ���е�m_strlisCombo����combo��ʼ��
		m_ThreeProValList.SetItemText(i,3,m_ListCtrlItem[i].m_ComboStrChoose);
		m_ThreeProValList.SetItemText(i,4,m_ListCtrlItem[i].m_IndexScore);

	}
}


LRESULT CProductStep4Dlg::OnSetIndexVal(WPARAM wParam,LPARAM lParam)
{
	//��Ӧ��ֵ������
	CString strValInfo=m_ThreeProValList.GetItemText(wParam,3);

	CStringArray strArrayNam,strArrayVal;
	//��ȡָ���������
	for (int i=0;i<m_IndexValInfo[wParam].size();++i)
	{
		strArrayNam.Add(m_IndexValInfo[wParam][i].Left(m_IndexValInfo[wParam][i].Find(':')));
		strArrayVal.Add(m_IndexValInfo[wParam][i].Right(m_IndexValInfo[wParam][i].GetLength()-m_IndexValInfo[wParam][i].Find(':')-1));//��ȡָ������������

	}

	//����ƥ��ѡȡ��Ӧ����
	int j=0;
	for (;j<strArrayNam.GetCount();++j)
	{
		if(strArrayNam[j]==strValInfo) break;
	}
	CString strDeductVal=strArrayVal[j];                                //��ȡ����
	m_ThreeProValList.SetItemText(wParam,4,strDeductVal);               //���÷�ֵ

	m_ListCtrlItem[wParam].m_ComboStrChoose=strValInfo;                  //�洢��ѡ��������	
	m_ListCtrlItem[wParam].m_IndexScore=strDeductVal;                    //�洢��ֵ�����ں���ģ���ۺϷ���


	return 0;
}



//�洢�ͷ���(��m_ListCtrlItem��ֵ��ɺ����)
void CProductStep4Dlg::SaveLowValItem(vector<CTechChartItem>& m_ListCtrlItem)
{
	m_LowValItem.clear();
	m_LowValItemNum=0;
	for (int i=0;i<m_ListCtrlItem.size();++i)
	{
		CString strDeductVal=m_ListCtrlItem[i].m_IndexScore;//��ȡ����
		CLowValItem OneLowValItem;
		int nDeductVal= _ttoi(strDeductVal);
		if (nDeductVal<-1)
		{
			CString strComment;
			switch(nDeductVal)
			{
			case 0:
				strComment=CString("��");
				break;
			case -1:
				strComment=CString("��");
				break;
			case -2:
				strComment=CString("��");
				break;
			case -3:
				strComment=CString("������");
				break;
			}
			CString str;
			str.Format(CString("%d"),m_LowValItemNum+1);
			OneLowValItem.m_Item=str;
			OneLowValItem.m_ChartNam=CString("������Ӧ�Ա�");
			OneLowValItem.m_Classify=m_ListCtrlItem[i].m_Classify;
			OneLowValItem.m_TechEvalIndex=m_ListCtrlItem[i].m_TechEvalIndex;
			OneLowValItem.m_IndexComment=strComment;
			OneLowValItem.m_LowValAdvice=m_ListCtrlItem[i].m_LowValAdvice;


			//CStringArray strArrayNam,strArrayVal;
			//for (int j=0;j<m_IndexValInfo[i].size();++j)
			//{
			//	strArrayNam.Add(m_IndexValInfo[i][j].Left(m_IndexValInfo[i][j].Find(':')));   //��ȡָ����������
			//	strArrayVal.Add(m_IndexValInfo[i][j].Right(m_IndexValInfo[i][j].GetLength()-m_IndexValInfo[i][j].Find(':')-1));//��ȡָ������������
			//}

			//int k=0;      //��ȡ����������Ϊ0�������۷ֵ�����Ϊ����
			//for (;k<strArrayVal.GetCount();++k)
			//{
			//	if(strArrayVal[k]==CString("0")) break;
			//}
			//OneLowValItem.m_LowValAdvice=strArrayNam[k];

			m_LowValItem.push_back(OneLowValItem);

			++m_LowValItemNum;
		}
	}
}



//�������������Ϣ���´ν���ֱ�ӳ�ʼ��
void CProductStep4Dlg::SaveChartInfo()
{
	_RecordsetPtr m_pRs;  //��ɾ��֮ǰ��¼��Ϣ�����������ظ�����
	CString sql = CString("delete * from ChartInfoSave where ProductNam='") + m_ProductInfo.m_ProductName+CString("'and ProductNum='")+m_ProductInfo.m_ProductNum+CString("'and ChartName='")+CString("������Ӧ�Ա�")+CString("'and Uname='")+theApp.name+("'");
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
			theApp.m_pConnect->Execute((_bstr_t)(CString("insert into ChartInfoSave(ProductNam,ProductNum,Uname,ChartName,Row1,Row2,Row3,Row4,Row5) values('")
				+m_ProductInfo.m_ProductName+"','"+m_ProductInfo.m_ProductNum+"','"+theApp.name+"','"+CString("������Ӧ�Ա�")+"','"+m_ListCtrlItem[i].m_Classify+"','"+m_ListCtrlItem[i].m_TechEvalIndex+"','"+m_ListCtrlItem[i].m_ComboStrChoose+"','"+m_ListCtrlItem[i].m_IndexScore+"','"+m_ListCtrlItem[i].m_LowValAdvice+("')")) , NULL, adCmdText);   //insert����ʱ���ݿ��ж�Ӧ�ַ�����ӡ���
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
void CProductStep4Dlg::InitChartInfo()
{
	_RecordsetPtr m_pRs;    //��ȡ��ʷ����������Ϣ
	CString sql = CString("select * from ChartInfoSave where ProductNum='") + m_ProductInfo.m_ProductNum+CString("'and ChartName= '")+CString("������Ӧ�Ա�")+CString("'and Uname= '")+theApp.name+CString("'");
	m_pRs = theApp.m_pConnect->Execute(_bstr_t(sql), NULL, adCmdText);
	int k=0;
	while (!m_pRs->adoEOF)
	{
		m_ListCtrlItem[k].m_ComboStrChoose =  m_pRs->GetCollect("Row3");  //ֻ�貹��֮ǰ���������
		m_ListCtrlItem[k].m_IndexScore     =  m_pRs->GetCollect("Row4");

		++k;
		m_pRs->MoveNext();
	}
	if (k==0)        //����޸�Ŀ����ֱ������
		return;
}