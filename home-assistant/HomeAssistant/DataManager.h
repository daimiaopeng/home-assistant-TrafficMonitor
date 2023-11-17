#pragma once
#include <string>
#include <map>
#include "resource.h"
#include "homeAssistant.h"
#define g_data CDataManager::Instance()


class SettingData
{
public:
	SettingData()
	{
	};
	//TODO: 在此添加选项设置的数据
	map<int, string> index_entity;
	map<string, bool> entity;
	CString token;
	CString baseUrl;

	vector<string> split(string str, string pattern)
	{
		string::size_type pos;
		vector<string> result;
		str += pattern; //扩展字符串以方便操作
		int size = str.size();
		for (int i = 0; i < size; i++)
		{
			pos = str.find(pattern, i);
			if (pos < size)
			{
				if (pos == 0)
				{
					i = pos + pattern.size() - 1;
					continue;
				}
				string s = str.substr(i, pos - i);
				result.push_back(s);
				i = pos + pattern.size() - 1;
			}
		}
		return result;
	}

	void setupByCodeStr(CString codeStr)
	{
		index_entity.clear();
		entity.clear();
		vector<string> codes = split(CCommon::UnicodeToStr(codeStr), ";");
		if (codes.size() > 0)
		{
			index_entity.clear();
			for (int i = 0; i < codes.size(); i++)
			{
				index_entity[i] = codes[i];
				entity[codes[i]] = true;
			}
		}
	}
};

class CDataManager
{
private:
	CDataManager();
	~CDataManager();

public:
	static CDataManager& Instance();

	void LoadConfig(const wstring& config_dir);
	void SaveConfig() const;
	const CString& StringRes(UINT id); //根据资源id获取一个字符串资源
	void DPIFromWindow(CWnd* pWnd);
	int DPI(int pixel);
	float DPIF(float pixel);
	int RDPI(int pixel);
	HICON GetIcon(UINT id);

	SettingData m_setting_data;
	wstring data;

	homeAssistant m_homeAssistant;

	void update();


	wstring m_config_dir;

private:
	static CDataManager m_instance;
	wstring m_config_path;
	map<UINT, CString> m_string_table;
	map<UINT, HICON> m_icons;
	int m_dpi{96};
};
