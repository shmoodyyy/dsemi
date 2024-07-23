#include "Dsemi/Core/DsemiWindows.h"
#include <ios>
#include <sstream>

namespace dsemi {

	hresult_exception::hresult_exception(int line, const char* file, HRESULT hr, const char* type /* = "HRESULT Failure Exception." */)
		:dsemi_exception(line, file), _hr(hr), _type(type)
	{
	}

	const char* hresult_exception::what() const
	{
		std::ostringstream oss;
		oss << type() << '\n'
			<< "[Error Code] 0x" << std::hex << std::uppercase << error_code() << std::dec << " (" << (unsigned long)error_code() << ")" << '\n'
			<< "[Description] " << error_string() << '\n'
			<< origin_string();
		_what_buf = oss.str();
		return _what_buf.c_str();
	}

	const char* hresult_exception::type() const
	{
		return _type.c_str();
	}

	std::string hresult_exception::translate_error_code(HRESULT hr) const
	{
		char* msg_buf = nullptr;
		DWORD msg_len = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			hr,
			MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&msg_buf),
			0,
			nullptr
		);
		if (msg_len == 0)
		{
			return "Unknown Error Code. Could not format message";
		}

		std::string msg_string = msg_buf;
		LocalFree(msg_buf);
		return msg_string;
	}

	HRESULT hresult_exception::error_code() const
	{
		return _hr;
	}

	std::string hresult_exception::error_string() const
	{
		return translate_error_code(_hr);
	}
}
