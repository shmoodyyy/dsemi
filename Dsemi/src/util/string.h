#pragma once

#include <string>
#include <queue>

// TODO: implement own string class
namespace dsemi {
	class String
	{
		std::string m_string;
	public:
		String split(const String& delimiter);

		static std::vector<std::string> split(const std::string& str, const std::string& splitter);
		static std::vector<std::wstring> split(const std::wstring& str, const std::wstring& splitter);
	};
}