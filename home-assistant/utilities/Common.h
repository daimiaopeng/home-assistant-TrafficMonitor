#pragma once
#include <string>
#include <vector>

namespace utilities
{
    class CCommon
    {
    public:
        CCommon();
        ~CCommon();

        static std::wstring StrToUnicode(const char* str, bool utf8 = false);

        static std::string UnicodeToStr(const wchar_t* wstr, bool utf8 = false);

        //��ȡ�ļ�����
        static bool GetFileContent(const wchar_t* file_path, std::string& contents_buff);

        //��ȡ�ļ�����
        //file_path: �ļ���·��
        //length: �ļ��ĳ���
        //����ֵ: ��ȡ�ɹ����ض�ȡ�����ļ����ݵ�const char���͵�ָ�룬��ʹ����Ϻ����ָ����Ҫ����ʹ��delete�ͷš���ȡʧ�ܷ���nullptr
        static const char* GetFileContent(const wchar_t* file_path, size_t& length);

        static void GetFiles(const wchar_t* path, std::vector<std::wstring>& files);

        template<class T>
        static void StringNormalize(T& str)
        {
            if (str.empty()) return;

            int size = str.size();  //�ַ����ĳ���
            if (size < 0) return;
            int index1 = 0;     //�ַ����е�1�����ǿո������ַ���λ��
            int index2 = size - 1;  //�ַ��������һ�����ǿո������ַ���λ��
            while (index1 < size && str[index1] >= 0 && str[index1] <= 32)
                index1++;
            while (index2 >= 0 && str[index2] >= 0 && str[index2] <= 32)
                index2--;
            if (index1 > index2)    //���index1 > index2��˵���ַ���ȫ�ǿո������ַ�
                str.clear();
            else if (index1 == 0 && index2 == size - 1) //���index1��index2��ֵ�ֱ�Ϊ0��size - 1��˵���ַ���ǰ��û�пո������ַ���ֱ�ӷ���
                return;
            else
                str = str.substr(index1, index2 - index1 + 1);
        }

        //��һ���ַ����ָ�����ɸ��ַ���ģ������ֻ��Ϊstring��wstring��
        //str: ԭʼ�ַ���
        //div_ch: ���ڷָ���ַ�
        //result: ���շָ��Ľ��
        template<class T>
        static void StringSplit(const T& str, wchar_t div_ch, std::vector<T>& results, bool skip_empty = true, bool trim = true)
        {
            results.clear();
            size_t split_index = -1;
            size_t last_split_index = -1;
            while (true)
            {
                split_index = str.find(div_ch, split_index + 1);
                T split_str = str.substr(last_split_index + 1, split_index - last_split_index - 1);
                if (trim)
                    StringNormalize(split_str);
                if (!split_str.empty() || !skip_empty)
                    results.push_back(split_str);
                if (split_index == std::wstring::npos)
                    break;
                last_split_index = split_index;
            }
        }

        template<class T>
        static void StringSplit(const T& str, const T& div_str, std::vector<T>& results, bool skip_empty = true, bool trim = true)
        {
            results.clear();
            size_t split_index = 0 - div_str.size();
            size_t last_split_index = 0 - div_str.size();
            while (true)
            {
                split_index = str.find(div_str, split_index + div_str.size());
                T split_str = str.substr(last_split_index + div_str.size(), split_index - last_split_index - div_str.size());
                if (trim)
                    StringNormalize(split_str);
                if (!split_str.empty() || !skip_empty)
                    results.push_back(split_str);
                if (split_index == std::wstring::npos)
                    break;
                last_split_index = split_index;
            }
        }

    };
}

