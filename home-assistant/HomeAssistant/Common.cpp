#include "pch.h"
#include "Common.h"

std::wstring CCommon::StrToUnicode(const char* str, bool utf8)
{
	if (str == nullptr)
		return std::wstring();
	std::wstring result;
	int size;
	size = MultiByteToWideChar((utf8 ? CP_UTF8 : CP_ACP), 0, str, -1, nullptr, 0);
	if (size <= 0) return std::wstring();
	auto str_unicode = new wchar_t[size + 1];
	MultiByteToWideChar((utf8 ? CP_UTF8 : CP_ACP), 0, str, -1, str_unicode, size);
	result.assign(str_unicode);
	delete[] str_unicode;
	return result;
}

std::string CCommon::UnicodeToStr(const wchar_t* wstr, bool utf8)
{
	if (wstr == nullptr)
		return std::string();
	std::string result;
	int size{0};
	size = WideCharToMultiByte((utf8 ? CP_UTF8 : CP_ACP), 0, wstr, -1, nullptr, 0, nullptr, nullptr);
	if (size <= 0) return std::string();
	auto str = new char[size + 1];
	WideCharToMultiByte((utf8 ? CP_UTF8 : CP_ACP), 0, wstr, -1, str, size, nullptr, nullptr);
	result.assign(str);
	delete[] str;
	return result;
}
