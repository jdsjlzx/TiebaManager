// SuperFunctionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SuperFunctionDlg.h"
#include "TiebaManagerDlg.h"
#include "Setting.h"
#include <zlib.h>


// CSuperFunctionDlg 对话框

IMPLEMENT_DYNAMIC(CSuperFunctionDlg, CNormalDlg)

// 构造函数
CSuperFunctionDlg::CSuperFunctionDlg(CWnd* pParent /*=NULL*/)
	: CNormalDlg(CSuperFunctionDlg::IDD, pParent),
	m_pagesResize(&m_tab)
{
	// 初始化m_pages
	int i = 0;
	m_pages[i++] = &m_loopBanPage;
}

#pragma region MFC
CSuperFunctionDlg::~CSuperFunctionDlg()
{
}

void CSuperFunctionDlg::DoDataExchange(CDataExchange* pDX)
{
	CNormalDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Control(pDX, IDOK, m_okButton);
	DDX_Control(pDX, IDCANCEL, m_cancelButton);
}


BEGIN_MESSAGE_MAP(CSuperFunctionDlg, CNormalDlg)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CSuperFunctionDlg::OnTcnSelchangeTab1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()
#pragma endregion

// CSuperFunctionDlg 消息处理程序

// 初始化
BOOL CSuperFunctionDlg::OnInitDialog()
{
	CNormalDlg::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);			// 设置大图标
	SetIcon(hIcon, FALSE);			// 设置小图标

	SetWindowText(_T("超级功能"));

	// 初始化m_tab
	int i = 0;
	m_tab.InsertItem(i++, _T("循环封"));

	// 初始化各页
	m_loopBanPage.Create(IDD_LIST_PAGE, &m_tab);

	CRect rect;
	m_tab.GetClientRect(&rect);
	rect.left += 1; rect.right -= 3; rect.top += 23; rect.bottom -= 2;
	m_pages[0]->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
	for (i = 1; i < _countof(m_pages); i++)
		m_pages[i]->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);

	m_resize.AddControl(&m_tab, RT_NULL, NULL, RT_NULL, NULL, RT_KEEP_DIST_TO_RIGHT, this, RT_KEEP_DIST_TO_BOTTOM, this);
	m_resize.AddControl(&m_okButton, RT_KEEP_DIST_TO_RIGHT, this, RT_KEEP_DIST_TO_BOTTOM, &m_tab);
	m_resize.AddControl(&m_cancelButton, RT_KEEP_DIST_TO_RIGHT, this, RT_KEEP_DIST_TO_BOTTOM, &m_tab);
	for (i = 0; i < _countof(m_pages); i++)
		m_pagesResize.AddControl(m_pages[i], RT_NULL, NULL, RT_NULL, NULL, RT_KEEP_DIST_TO_RIGHT, &m_tab, RT_KEEP_DIST_TO_BOTTOM, &m_tab);

	// 显示配置
	ShowCurrentOptions();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

#pragma region UI
// 窗口 /////////////////////////////////////////////////////////////////////////////////

// 取消
void CSuperFunctionDlg::OnCancel()
{
	DestroyWindow();
}

// 提示是否保存
void CSuperFunctionDlg::OnClose()
{
	int result = AfxMessageBox(_T("保存设置？"), MB_ICONQUESTION | MB_YESNOCANCEL);
	if (result == IDYES)
	{
		OnOK();
		return;
	}
	else if (result == IDCANCEL)
		return;

	DestroyWindow();
}

// 释放this
void CSuperFunctionDlg::PostNcDestroy()
{
	CNormalDlg::PostNcDestroy();

	((CTiebaManagerDlg*)AfxGetApp()->m_pMainWnd)->m_superFunctionDlg = NULL;
	delete this;
}

// 限制最小尺寸
void CSuperFunctionDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 666;
	lpMMI->ptMinTrackSize.y = 576;

	CNormalDlg::OnGetMinMaxInfo(lpMMI);
}

// 改变尺寸
void CSuperFunctionDlg::OnSize(UINT nType, int cx, int cy)
{
	CNormalDlg::OnSize(nType, cx, cy);
	m_pagesResize.Resize();
}

// 切换标签
void CSuperFunctionDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	int index = m_tab.GetCurSel();
	for (int i = 0; i < _countof(m_pages); i++)
		m_pages[i]->ShowWindow(i == index ? SW_SHOW : SW_HIDE);
}
#pragma endregion

// 显示当前设置
void CSuperFunctionDlg::ShowCurrentOptions()
{
	gzFile f = gzopen_w(CURRENT_USER_PATH + _T("\\options2.tb"), "rb");
	if (f == NULL)
		return;

	// 头部
	char header[2];
	gzread(f, header, sizeof(header));
	if (header[0] != 'T' || header[1] != 'B')
	{
		gzclose(f);
		return;
	}

	// 循环封
	int size;
	gzread(f, &size, sizeof(int)); // 长度
	m_loopBanPage.m_list.ResetContent();
	m_loopBanPage.m_pid.resize(size);
	CString strBuf;
	for (int i = 0; i < size; i++)
	{
		ReadText(f, strBuf);
		m_loopBanPage.m_list.AddString(strBuf);
		ReadText(f, m_loopBanPage.m_pid[i]);
	}

	gzclose(f);
}

// 应用对话框中的设置
void CSuperFunctionDlg::ApplyOptionsInDlg()
{
	gzFile f = gzopen_w(CURRENT_USER_PATH + _T("\\options2.tb"), "wb");
	if (f == NULL)
		return;

	// 头部
	gzwrite(f, "TB", 2);

	// 循环封
	int size;
	gzwrite(f, &(size = m_loopBanPage.m_list.GetCount()), sizeof(int)); // 长度
	CString strBuf;
	for (int i = 0; i < size; i++)
	{
		m_loopBanPage.m_list.GetText(i, strBuf);
		WriteText(f, strBuf);
		WriteText(f, m_loopBanPage.m_pid[i]);
	}

	gzclose(f);
}

// 确认
void CSuperFunctionDlg::OnOK()
{
	ApplyOptionsInDlg();

	DestroyWindow();
}
