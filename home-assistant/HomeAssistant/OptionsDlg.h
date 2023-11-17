#pragma once
#include "DataManager.h"
#include "afxdialogex.h"

// COptionsDlg 对话框

class COptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsDlg)

public:
	COptionsDlg(CWnd* pParent = nullptr); // 标准构造函数
	~COptionsDlg() override;

	SettingData m_data;
	BOOL OnInitDialog() override;
	// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_OPTIONS_DIALOG };
#endif

private:
	CEdit m_editBox_token;
	CEdit m_editBox_ip_address;

public:
	CListBox m_clist;

protected:
	void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	//virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickedOk();


	void loadlist();
};
