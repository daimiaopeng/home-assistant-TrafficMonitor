// form.cpp: 实现文件
//

#include "pch.h"
#include "homeAssistantPlugin.h"
#include "form.h"


// form

IMPLEMENT_DYNCREATE(form, CFormView)

form::form()
	: CFormView(IDD_OPTIONS_DIALOG)
{
}

form::~form()
{
}

void form::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(form, CFormView)
END_MESSAGE_MAP()


// form 诊断

#ifdef _DEBUG
void form::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void form::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// form 消息处理程序
