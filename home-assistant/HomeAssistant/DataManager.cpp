#include "pch.h"
#include "DataManager.h"
#include "Common.h"
#include <vector>
#include <sstream>

CDataManager CDataManager::m_instance;

CDataManager::CDataManager()
{
	//初始化DPI
	HDC hDC = GetDC(HWND_DESKTOP);
	m_dpi = GetDeviceCaps(hDC, LOGPIXELSY);
	ReleaseDC(HWND_DESKTOP, hDC);
}

CDataManager::~CDataManager()
{
}

CDataManager& CDataManager::Instance()
{
	return m_instance;
}

static void WritePrivateProfileInt(const wchar_t* app_name, const wchar_t* key_name, int value,
                                   const wchar_t* file_path)
{
	wchar_t buff[16];
	swprintf_s(buff, L"%d", value);
	WritePrivateProfileString(app_name, key_name, buff, file_path);
}

void CDataManager::LoadConfig(const std::wstring& config_dir)
{
	m_config_path = config_dir + L"homeAssistant.ini";
	//TODO: 在此添加载入配置的代码
	m_config_dir = config_dir;
	CString codeStr{};
	::GetPrivateProfileString(L"config", L"entity_id_list", L"", codeStr.GetBuffer(MAX_PATH), MAX_PATH,
	                          m_config_path.c_str());
	m_setting_data.setupByCodeStr(codeStr);

	::GetPrivateProfileString(L"config", L"token", L"", codeStr.GetBuffer(MAX_PATH), MAX_PATH, m_config_path.c_str());
	m_setting_data.token = codeStr;

	::GetPrivateProfileString(L"config", L"url", L"", codeStr.GetBuffer(MAX_PATH), MAX_PATH, m_config_path.c_str());
	m_setting_data.baseUrl = codeStr;

	m_homeAssistant = homeAssistant(CCommon::UnicodeToStr(m_setting_data.baseUrl),
	                                CCommon::UnicodeToStr(m_setting_data.token));
}

void CDataManager::SaveConfig() const
{
	if (m_config_path.empty())
	{
		return;
	}
	string entity_str;
	for (auto id : m_setting_data.entity)
	{
		if (id.second && id.first != "")
		{
			entity_str = entity_str + id.first.c_str() + ";";
		}
	}
	::WritePrivateProfileString(L"config", L"entity_id_list", CCommon::StrToUnicode(entity_str.c_str()).c_str(),
	                            m_config_path.c_str());
	::WritePrivateProfileString(L"config", L"token", m_setting_data.token, m_config_path.c_str());
	::WritePrivateProfileString(L"config", L"url", m_setting_data.baseUrl, m_config_path.c_str());
}

const CString& CDataManager::StringRes(UINT id)
{
	auto iter = m_string_table.find(id);
	if (iter != m_string_table.end())
	{
		return iter->second;
	}
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_string_table[id].LoadString(id);
	return m_string_table[id];
}

void CDataManager::DPIFromWindow(CWnd* pWnd)
{
	CWindowDC dc(pWnd);
	HDC hDC = dc.GetSafeHdc();
	m_dpi = GetDeviceCaps(hDC, LOGPIXELSY);
}

int CDataManager::DPI(int pixel)
{
	return m_dpi * pixel / 96;
}

float CDataManager::DPIF(float pixel)
{
	return m_dpi * pixel / 96;
}

int CDataManager::RDPI(int pixel)
{
	return pixel * 96 / m_dpi;
}

HICON CDataManager::GetIcon(UINT id)
{
	auto iter = m_icons.find(id);
	if (iter != m_icons.end())
	{
		return iter->second;
	}
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	auto hIcon = static_cast<HICON>(LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(id), IMAGE_ICON, DPI(16), DPI(16),
	                                          0));
	m_icons[id] = hIcon;
	return hIcon;
}

void CDataManager::update()
{
	for (auto& [index, entity_id] : m_setting_data.index_entity)
	{
		m_homeAssistant.update(entity_id);
	}
}
