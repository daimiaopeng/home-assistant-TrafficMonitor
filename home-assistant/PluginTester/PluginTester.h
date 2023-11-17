
// PluginTester.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
    #error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "CommonData.h"

// CPluginTesterApp: 
// �йش����ʵ�֣������ PluginTester.cpp
//

class CPluginTesterApp : public CWinApp
{
public:
    CPluginTesterApp();
    int DPI(int pixel);
    static CString LoadText(UINT id);
    HICON GetIcon(UINT id);

    Language m_language;
    std::wstring m_config_path;
    HICON m_plugin_icon;

private:
    void LoadConfig();
    void SaveConfig() const;

private:
    int m_dpi{ 96 };
    std::map<UINT, HICON> m_icons;

// ��д
public:
    virtual BOOL InitInstance();

// ʵ��

    DECLARE_MESSAGE_MAP()
    afx_msg void OnHelp();
};

extern CPluginTesterApp theApp;