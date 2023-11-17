#include "pch.h"
#include "homeAssistant.h"

homeAssistant::homeAssistant(string url, string token)
{
	_token = token;
	_states_url = url + "/api/states";
	_api_url = url + "/api/";
	_states_entity_id_url = url + "/api/states/";
}

bool homeAssistant::checkApi()
{
	auto r = Get(_api_url);
	if (r != "")
	{
		return true;
	}
	return false;
}

bool homeAssistant::update(string entity_id)
{
	if (entity_id == "")
	{
		return true;
	}
	auto text = Get(_states_entity_id_url + entity_id);
	if (text == "")
	{
		_result[entity_id] = L"Unauthorized or FAIL";
	}
	else
	{
		json r_json = json::parse(text);
		auto value = r_json.at("state").get<string>();
		string unit_of_measurement;
		try
		{
			unit_of_measurement = r_json.at("attributes").at("unit_of_measurement").get<string>();
		}
		catch (...)
		{
		}
		_result[entity_id] = CCommon::StrToUnicode(string(value + unit_of_measurement).c_str(), true);
	}
	return true;
}

map<string, wstring>& homeAssistant::getData()
{
	return _result;
}

json homeAssistant::getAllData()
{
	json j;
	auto text = Get(_states_url);

	if (text == "")
	{
		return j;
	}
	j = json::parse(text);
	return j;
}

string homeAssistant::Get(string url)
{
	CInternetSession* pSession{};
	CHttpFile* pfile{};
	string res;
	CString headers = "Content-Type: application/json\r\n";
	headers = headers + "Authorization: Bearer " + CString(_token.c_str());
	try
	{
		pSession = new CInternetSession();
		pfile = static_cast<CHttpFile*>(pSession->OpenURL(CString(url.c_str()), 1,
		                                                  INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_DONT_CACHE,
		                                                  headers,
		                                                  headers.GetLength()));
		DWORD dwStatusCode;

		pfile->QueryInfoStatusCode(dwStatusCode);
		if (dwStatusCode == HTTP_STATUS_OK)
		{
			CString content;
			CString data;
			while (pfile->ReadString(data))
			{
				content += data;
			}
			res = (const char*)content.GetString();
		}
		pfile->Close();
		delete pfile;
		pSession->Close();
	}
	catch (CInternetException* e)
	{
		if (pfile != nullptr)
		{
			pfile->Close();
			delete pfile;
		}
		if (pSession != nullptr)
			pSession->Close();
		e->Delete(); //没有这句会造成内存泄露
		SAFE_DELETE(pSession);
	}
	SAFE_DELETE(pSession);
	return res;
}
