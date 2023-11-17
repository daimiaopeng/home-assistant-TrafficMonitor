#pragma once
#include <afxinet.h>
#include "json.hpp"
#include "Common.h"

using json = nlohmann::json;
using namespace std;

class homeAssistant
{
public:
	homeAssistant() = default;
	homeAssistant(string url, string token);

	bool checkApi();

	bool update(string entity_id);

	map<string, wstring>& getData();

	json getAllData();

	string Get(string url);

private:
	string _url;
	string _token;
	map<string, wstring> _result;

	string _states_url;
	string _api_url;
	string _states_entity_id_url;
};
