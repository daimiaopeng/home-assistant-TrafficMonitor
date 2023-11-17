// OptionsDlg.cpp: 实现文件
//

#include "pch.h"
#include "homeAssistantPlugin.h"
#include "OptionsDlg.h"

// COptionsDlg 对话框

IMPLEMENT_DYNAMIC(COptionsDlg, CDialog)

COptionsDlg::COptionsDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_OPTIONS_DIALOG, pParent)
{
}

COptionsDlg::~COptionsDlg()
{
}

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST8, m_clist);
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &COptionsDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT2, &COptionsDlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &COptionsDlg::OnEnChangeEdit3)
	ON_BN_CLICKED(IDOK, &COptionsDlg::OnBnClickedOk)

END_MESSAGE_MAP()


// COptionsDlg 消息处理程序


BOOL COptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_editBox_token.SubclassDlgItem(IDC_EDIT3, this);
	m_editBox_token.SetWindowText(m_data.token);
	m_editBox_ip_address.SubclassDlgItem(IDC_EDIT2, this);
	m_editBox_ip_address.SetWindowText(m_data.baseUrl);
	loadlist();
	return TRUE; // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void COptionsDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	auto assist = homeAssistant(CCommon::UnicodeToStr(m_data.baseUrl), CCommon::UnicodeToStr(m_data.token));
	if (assist.checkApi())
	{
		AfxMessageBox(L"success");
		loadlist();
	}
	else
	{
		AfxMessageBox(L"fail");
	}
}


void COptionsDlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT2, m_data.baseUrl);
}


void COptionsDlg::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT3, m_data.token);
}


void COptionsDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();

	int count = m_clist.GetCount();
	for (int i = 0; i < count; ++i)
	{
		// 用户选择了第 i 项
		CString selectedItem;
		m_clist.GetText(i, selectedItem);
		if (m_clist.GetSel(i) > 0)
		{
			// 处理选中的项...
			m_data.entity[CCommon::UnicodeToStr(selectedItem)] = true;
		}
		else
		{
			m_data.entity[CCommon::UnicodeToStr(selectedItem)] = false;
		}
	}
}


void COptionsDlg::loadlist()
{
	map<string, int> clist_id;
	auto assist = homeAssistant(CCommon::UnicodeToStr(m_data.baseUrl), CCommon::UnicodeToStr(m_data.token));

	m_clist.ResetContent();
	for (auto item : assist.getAllData())
	{
		auto entity_id = item.at("entity_id").get<string>();
		int n = m_clist.AddString(CCommon::StrToUnicode(entity_id.c_str()).c_str());
		clist_id[entity_id] = n;
	}
	//选择已启用的项目
	for (auto id : m_data.entity)
	{
		if (clist_id.find(id.first) != clist_id.end() && id.second && id.first != "")
		{
			m_clist.SetSel(clist_id[id.first], true);
		}
	}
}
