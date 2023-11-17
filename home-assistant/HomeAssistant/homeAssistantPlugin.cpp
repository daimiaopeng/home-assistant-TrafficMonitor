#include "pch.h"
#include "homeAssistantPlugin.h"

using namespace std;
homeAssistantPlugin homeAssistantPlugin::m_instance;
#define devices_ITEM_MAX 20

homeAssistantPlugin::homeAssistantPlugin()
{
	m_items = vector<PluginItem>(devices_ITEM_MAX);
	fill(m_items.begin(), m_items.end(), PluginItem());
	for (int index = 0; index < m_items.size(); index++)
	{
		m_items[index].index = index;
	}
}

homeAssistantPlugin& homeAssistantPlugin::Instance()
{
	return m_instance;
}

IPluginItem* homeAssistantPlugin::GetItem(int index)
{
	size_t item_size = m_items.size();
	if (g_data.m_setting_data.index_entity.size() < item_size)
	{
		item_size = g_data.m_setting_data.index_entity.size();
	}
	if (item_size == 0)
	{
		item_size = 1;
	}
	if (index >= item_size)
	{
		return nullptr;
	}
	return &(m_items[index]);
}

const wchar_t* homeAssistantPlugin::GetTooltipInfo()
{
	return m_tooltip_info.c_str();
}

void homeAssistantPlugin::DataRequired()
{
	//TODO: 在此添加获取监控数据的代码
	g_data.update();
}

ITMPlugin::OptionReturn homeAssistantPlugin::ShowOptionsDialog(void* hParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CWnd* pParent = CWnd::FromHandle(static_cast<HWND>(hParent));
	COptionsDlg dlg(pParent);
	dlg.m_data = g_data.m_setting_data;
	if (dlg.DoModal() == IDOK)
	{
		g_data.m_setting_data = dlg.m_data;
		g_data.SaveConfig();
		g_data.LoadConfig(g_data.m_config_dir);
		return OR_OPTION_CHANGED;
	}
	return OR_OPTION_CHANGED;
}

const wchar_t* homeAssistantPlugin::GetInfo(PluginInfoIndex index)
{
	static CString str;
	switch (index)
	{
	case TMI_NAME:
		return g_data.StringRes(IDS_PLUGIN_NAME).GetString();
	case TMI_DESCRIPTION:
		return g_data.StringRes(IDS_PLUGIN_DESCRIPTION).GetString();
	case TMI_AUTHOR:
		//TODO: 在此返回作者的名字
		return L"daimiaopeng";
	case TMI_COPYRIGHT:
		//TODO: 在此返回版权信息
		return L"Copyright (C) by daimiaopeng 2023";
	case TMI_URL:
		//TODO: 在此返回URL
		return L"https://github.com/daimiaopeng/home-assistant-TrafficMonitor";
		break;
	case TMI_VERSION:
		//TODO: 在此修改插件的版本
		return L"1.00";
	default:
		break;
	}
	return L"";
}

void homeAssistantPlugin::OnExtenedInfo(ExtendedInfoIndex index, const wchar_t* data)
{
	switch (index)
	{
	case EI_CONFIG_DIR:
		//从配置文件读取配置
		g_data.LoadConfig(std::wstring(data));
		break;
	default:
		break;
	}
}

ITMPlugin* TMPluginGetInstance()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return &homeAssistantPlugin::Instance();
}
