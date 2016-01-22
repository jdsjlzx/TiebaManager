// ScanPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ScanPage.h"
#include "SettingDlg.h"


// CScanPage �Ի���

IMPLEMENT_DYNAMIC(CScanPage, CNormalDlg)

CScanPage::CScanPage(CWnd* pParent /*=NULL*/)
	: CNormalDlg(CScanPage::IDD, pParent)
{

}

#pragma region MFC
CScanPage::~CScanPage()
{
}

void CScanPage::DoDataExchange(CDataExchange* pDX)
{
	CNormalDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_scanIntervalEdit);
	DDX_Control(pDX, IDC_CHECK3, m_onlyScanTitleCheck);
	DDX_Control(pDX, IDC_EDIT5, m_scanPageCountEdit);
	DDX_Control(pDX, IDC_CHECK5, m_briefLogCheck);
	DDX_Control(pDX, IDC_EDIT2, m_threadCountEdit);
	DDX_Control(pDX, IDC_CHECK6, m_autoSaveLogCheck);
}


BEGIN_MESSAGE_MAP(CScanPage, CNormalDlg)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CScanPage::OnEnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT5, &CScanPage::OnEnKillfocusEdit5)
	ON_EN_CHANGE(IDC_EDIT5, &CScanPage::OnEnChangeEdit5)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CScanPage::OnEnKillfocusEdit2)
END_MESSAGE_MAP()
#pragma endregion

// CScanPage ��Ϣ��������

// ɨ����
void CScanPage::OnEnKillfocusEdit1()
{
	CString tmp;
	m_scanIntervalEdit.GetWindowText(tmp);
	int scanInterval = _ttoi(tmp);
	if (scanInterval < 0 || scanInterval > 600)
		m_scanIntervalEdit.SetWindowText(_T("5"));
}

// ɨ�����ҳ�� ʧȥ����
void CScanPage::OnEnKillfocusEdit5()
{
	CString tmp;
	m_scanPageCountEdit.GetWindowText(tmp);
	int scanPageCount = _ttoi(tmp);
	if (scanPageCount < 1)
		m_scanPageCountEdit.SetWindowText(_T("1"));
}

// ɨ�����ҳ�� ���ݸı�
void CScanPage::OnEnChangeEdit5()
{
	((CSettingDlg*)GetParent()->GetParent())->m_clearScanCache = TRUE;
}

// �߳���
void CScanPage::OnEnKillfocusEdit2()
{
	CString tmp;
	m_threadCountEdit.GetWindowText(tmp);
	int threadCount = _ttoi(tmp);
	if (threadCount < 1 || threadCount > 16)
		m_threadCountEdit.SetWindowText(_T("2"));
}