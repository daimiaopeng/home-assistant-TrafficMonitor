#pragma once
#include <string>
#include <vector>

#include "PluginInterface.h"
#include "DataManager.h"
#include "OptionsDlg.h"
#include "PluginItem.h"
class homeAssistantPlugin : public ITMPlugin
{
private:
	homeAssistantPlugin();

public:
	static homeAssistantPlugin& Instance();

	virtual IPluginItem* GetItem(int index) override;
	virtual const wchar_t* GetTooltipInfo() override;
	virtual void DataRequired() override;
	virtual OptionReturn ShowOptionsDialog(void* hParent) override;
	virtual const wchar_t* GetInfo(PluginInfoIndex index) override;
	virtual void OnExtenedInfo(ExtendedInfoIndex index, const wchar_t* data) override;

private:
	static homeAssistantPlugin m_instance;
	std::wstring m_tooltip_info;
	vector<PluginItem> m_items;

};

#ifdef __cplusplus
extern "C" {
#endif
	__declspec(dllexport) ITMPlugin* TMPluginGetInstance();

#ifdef __cplusplus
}
#endif
