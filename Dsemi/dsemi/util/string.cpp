#include "dsemi/util/string.h"

namespace dsemi {
	std::vector<std::string> String::split(const std::string& str, const std::string& splitter)
	{
		std::vector<std::string> out;
		out.resize(sizeof(char) * str.length());

		size_t start = 0;
		size_t i = 0;
		i = str.find(splitter, start);
		while (i != std::string::npos)
		{
			out.push_back(str.substr(start, i - start));
			start = i + 1;
			i = str.find(splitter, start);
		}

		out.push_back(str.substr(start, str.length() - start));
		out.shrink_to_fit();

		return out;
	}

	std::vector<std::wstring> String::split(const std::wstring& str, const std::wstring& splitter)
	{
		std::vector<std::wstring> out;

		auto i = str.find(splitter);
		while (i != -1)
		{
			out.push_back(str.substr(i + 1, splitter.length()));
			i = str.find(splitter);
		}

		return out;
	}
}
