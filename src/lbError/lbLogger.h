#pragma once

#include <string>
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <comdef.h>

#ifndef LB_LOG_NAME
#define LB_LOG_NAME logger
#endif 

#define LB_FORMAT_LOCATION std::string(__FILE__ + ", " + __func__ + ", " + std::to_string(__LINE__))
#define LB_FORMAT_LOG(v_msg) lb::Logger::Format(__FILE__, __func__, std::to_string(__LINE__), v_msg)

#define LB_LOGAS_INFO(v_msg) lb::LB_LOG_NAME.LogAs("INFO", LB_FORMAT_LOG(v_msg).c_str())
#define LB_LOGAS_ERROR(v_msg) lb::LB_LOG_NAME.LogAs("ERROR", LB_FORMAT_LOG(v_msg).c_str())
#define LB_LOGAS_EXCEPTION(v_msg) { lb::LB_LOG_NAME.LogAs("EXCEPTION", LB_FORMAT_LOG(v_msg).c_str()); throw lb::Exception(v_msg); }

// Quick catch all for debugging
#pragma region LB_CATCH_ALL Macro

#define LB_CATCH_ALL							\
catch (const lb::Exception& v_exc)				\
{												\
	std::cout << v_exc.GetExceptionLog();		\
	lb::Terminate();							\
}												\
catch (const std::exception& v_exc)				\
{												\
	std::cout << v_exc.what();					\
}												\
catch (...)										\
{												\
	std::cout << "Unknown Exception Thrown\n";	\
	std::cout << "printing log...\n";			\
	std::cout << lb::LB_LOG_NAME.GetLog();		\
}

#pragma endregion

namespace lb
{
	class Exception;

	class Logger
	{
	private:
		std::vector<std::pair<std::string, std::string>> vLogMap;
		std::string vPushLog;
		std::string vLastLog;
		bool vOutputConsole;

		Logger() = default;

	public:
		Logger(Logger&) = delete;
		Logger(const Logger&) = delete;

		~Logger();

		void SetConsoleOutput(bool v_out);

		void SetDefaults();
		static Logger& GetInstance();

		void Push();
		void Log(const char* p_str, bool v_newLine = true);
		void LogAs(const char* p_strCategory, const char* p_strMsg, bool v_newLine = true);
		std::string GetLog(const char* v_as = nullptr);
		std::string GetLastLog(const char* v_as = nullptr);
		void Clear();

		template <typename _T>
		inline static std::string ValToString(_T v_val);
		static std::string Format(std::string v_file, std::string v_func, std::string v_line, std::string v_msg);

		lb::Logger& operator<<(const char* v_str);
	};
	
	static Logger& LB_LOG_NAME = Logger::GetInstance();
}

/// <summary>
/// Formats structs, pointers, and primitive data to string.
/// </summary>
/// <typeparam name="_T">Any type</typeparam>
/// <param name="v_val">any input</param>
/// <returns>string form of any input provided (or as close as possible)</returns>
template<typename _T>
inline std::string lb::Logger::ValToString(_T v_val)
{
	std::ostringstream vOss;
	if (typeid(_T) == typeid(HRESULT))
	{
		_com_error err(HRESULT_CODE(v_val));
		vOss << std::string(err.ErrorMessage());
	}
	else 
	{
		vOss << v_val;
	}
	return vOss.str();
}
