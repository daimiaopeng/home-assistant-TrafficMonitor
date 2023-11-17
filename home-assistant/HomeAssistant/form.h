#pragma once


// form 窗体视图

class form : public CFormView
{
	DECLARE_DYNCREATE(form)

protected:
	form(); // 动态创建所使用的受保护的构造函数
	~form() override;

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPTIONS_DIALOG };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
