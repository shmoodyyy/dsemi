#ifndef HEADER_DSEMI_CORE_EXCEPTIONS_HRESULTEXCEPTION
#define HEADER_DSEMI_CORE_EXCEPTIONS_HRESULTEXCEPTION

#ifdef _WIN32
#include "dsemi/core/exceptions/dsemiexception.h"

#define HRESULT_EXCEPTION(hr) dsemi::hresult_exception(__LINE__, __FILE__, hr)
#define HRESULT_EXCEPTION_TYPE(hr, type) dsemi::hresult_exception(__LINE__, __FILE__, hr, type)

#define THROW_FAILED(fn) if(FAILED(hr = (fn))) { __debugbreak(); throw HRESULT_EXCEPTION(hr); }
#define THROW_FAILED_TYPE(fn, type) if(!SUCCEEDED((hr = fn))) { auto e = HRESULT_EXCEPTION_TYPE(hr, type); dsemi::display_except(e.what(), type); __debugbreak(); abort(); }

// specific throw macros

// DirectX / Direct3D throws
#define GFX_THROW_FAILED(fn) THROW_FAILED_TYPE(fn, "Direct3D Exception")

namespace dsemi {
	class hresult_exception : public dsemi_exception
	{
	public:
		hresult_exception(int line, const char* file, HRESULT hr, const char* type = "HRESULT Failure Exception.");

		virtual const char* what() const override;
		virtual const char* type() const override;

		HRESULT error_code() const;
		std::string error_string() const;
		std::string translate_error_code(HRESULT hr) const;

	private:
		std::string _type;
		HRESULT _hr;
	};
}
#endif

#endif
