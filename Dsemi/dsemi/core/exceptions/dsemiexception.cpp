#include "dsemi/core/dsemiwindows.h"
#include "dsemiexception.h"
#include <sstream>

namespace dsemi {

	void display_except(const std::string& text, const std::string& caption) noexcept
	{
		MessageBoxA(nullptr, text.c_str(), caption.c_str(), MB_OK | MB_ICONEXCLAMATION);
	}

	dsemi_exception::dsemi_exception(int line, const char* file) noexcept
		:_line(line), _file(file)
	{

	}

	const char* dsemi_exception::what() const noexcept
	{
		std::ostringstream oss;
		oss << type() << std::endl
			<< origin_string();
		_what_buf = oss.str();
		return _what_buf.c_str();
	}

	const char* dsemi_exception::type() const noexcept
	{
		return "Default Dsemi Exception";
	}

	int dsemi_exception::line() const noexcept
	{
		return _line;
	}

	const std::string& dsemi_exception::file() const noexcept
	{
		return _file;
	}

	std::string dsemi_exception::origin_string() const noexcept
	{
		std::ostringstream oss;
		oss << "[File] " << _file << std::endl
			<< "[Line] " << _line;
		return oss.str();
	}

	custom_exception::custom_exception(int line, const char* file, const char* comment)
		: dsemi_exception(line, file), _comment(comment)
	{
	}

	const char* custom_exception::what() const noexcept
	{
		std::ostringstream oss;
		oss << type() << std::endl
			<< comment() << std::endl
			<< origin_string();
		_what_buf = oss.str();
		return _what_buf.c_str();
	}

	const char* custom_exception::type() const noexcept
	{
		return "Default Custom Exception";
	}

	const std::string& custom_exception::comment() const noexcept
	{
		return _comment;
	}

}
