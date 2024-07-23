#ifndef HEADER_DSEMI_CORE_EXCEPTIONS_DSEMIEXCEPTION
#define HEADER_DSEMI_CORE_EXCEPTIONS_DSEMIEXCEPTION

#include <string>
#define DEFAULT_EXCEPT() dsemi::dsemi_exception(__LINE__, __FILE__)
#define CUSTOM_EXCEPT(comment) dsemi::custom_exception(__LINE__, __FILE__, comment)

namespace dsemi {
	void display_except(const std::string& text, const std::string& caption);

	class dsemi_exception : public std::exception
	{
	public:
		dsemi_exception(int line, const char* file);

		virtual const char* what() const override;
		virtual const char* type() const;

		int line() const;
		const std::string& file() const;
		std::string origin_string() const;

	private:
		int _line;
		std::string _file;

	protected:
		mutable std::string _what_buf;
	};

	class custom_exception : public dsemi_exception
	{
	public:
		custom_exception(int line, const char* file, const char* comment);

		virtual const char* what() const override;
		virtual const char* type() const override;

		const std::string& comment() const;

	private:
		std::string _comment;
	};
}

#endif
