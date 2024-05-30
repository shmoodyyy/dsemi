#include "dsemi/core/dsemiwindows.h"
#include "dsemi/util/logger.h"

#define NODEBUG

namespace dsemi {
	std::queue<logger::log_entry_internal>  logger::_log_queue;
	std::string                             logger::_format;
	std::wostringstream                     logger::_stream;

	logger::level logger::_log_level = logger::level::LOG_LEVEL_INFO;

	std::thread             logger::_thread;
	bool                    logger::_active = false;
	std::mutex              logger::_m;
	std::condition_variable logger::_cv;



	void logger::_thread_begin()
	{
		while (_active) {
			_process_queue();
		}
	}
	void logger::_process_queue()
	{
		std::unique_lock<std::mutex> lock(_m);
		log_entry_internal entry;

		_cv.wait(lock);

		while(!_log_queue.empty())
		{
			entry = _log_queue.front();
			_log_queue.pop();

			_process_entry(entry);
		}
	}
	void logger::_process_entry(logger::log_entry_internal entry)
	{
		_stream << L"[";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
		switch (entry.log_level)
		{
		case LOG_LEVEL_DEBUG:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
			_stream << L"DEBUG";
			break;
		case LOG_LEVEL_TRACE:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			_stream << L"TRACE";
			break;
		case LOG_LEVEL_INFO:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			_stream << L"INFO";
			break;
		case LOG_LEVEL_WARN:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			_stream << L"WARN";
			break;
		case LOG_LEVEL_ERROR:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
			_stream << L"ERROR";
			break;
		case LOG_LEVEL_CRITICAL:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
			_stream << L"CRITICAL";
			break;
		}
		_stream << L"] - ";
		std::wstring str;
		str = _stream.str();
		_stream.str(L"");
		_stream.clear();
		wprintf(L"%s%s\n", str.c_str(), entry.msg.c_str());
	}
}
