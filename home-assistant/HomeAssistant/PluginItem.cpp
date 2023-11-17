#include "pch.h"
#include "DataManager.h"
#include "homeAssistantPlugin.h"

const wchar_t* PluginItem::GetItemName() const
{
	string itemName = g_data.m_setting_data.index_entity[index];
	if (itemName == "") itemName = "HomeAssistant";
	return CCommon::StrToUnicode(itemName.c_str()).c_str();
}

const wchar_t* PluginItem::GetItemId() const
{
	//TODO: 在此返回插件的唯一ID，建议只包含字母和数字
	return L"daimiaopeng";
}

const wchar_t* PluginItem::GetItemLableText() const
{
	return L"";
}

const wchar_t* PluginItem::GetItemValueText() const
{
	string entity = g_data.m_setting_data.index_entity[index];
	return g_data.m_homeAssistant.getData()[entity].c_str();
}

const wchar_t* PluginItem::GetItemValueSampleText() const
{
	string entity = g_data.m_setting_data.index_entity[index];
	return g_data.m_homeAssistant.getData()[entity].c_str();
}

bool PluginItem::IsCustomDraw() const
{
	//TODO: 根据是否由插件自绘返回对应的值
	return false;
}

int PluginItem::GetItemWidthEx(void* hDC) const
{
	//绘图句柄
	CDC* pDC = CDC::FromHandle(static_cast<HDC>(hDC));
	//TODO: 如果插件需要自绘，则在此修改显示区域的宽度
	return 60;
}

void PluginItem::DrawItem(void* hDC, int x, int y, int w, int h, bool dark_mode)
{
	//绘图句柄
	CDC* pDC = CDC::FromHandle(static_cast<HDC>(hDC));
	//矩形区域
	CRect rect(CPoint(x, y), CSize(w, h));
	//TODO: 在此添加绘图代码
}
