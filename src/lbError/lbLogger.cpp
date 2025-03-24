#include "lbLogger.h"

/***************************/
/* PUBLIC MEMBER FUNCTIONS */
/***************************/

/// <summary>
/// Deconstructor
/// </summary>
lb::Logger::~Logger()
{

}

/// <summary>
/// Sets logger to automatically output to console or not
/// </summary>
/// <param name="v_out">true for automated console output, false for no automated output to console</param>
void lb::Logger::SetConsoleOutput(bool v_out)
{
	this->vOutputConsole = v_out;
}

/// <summary>
/// Defaults for instance
/// </summary>
inline void lb::Logger::SetDefaults()
{
	this->vLastLog = "";

	this->vOutputConsole = false;
}

/// <summary>
/// Get the instance reference
/// </summary>
/// <returns>reference to static object instance</returns>
lb::Logger& lb::Logger::GetInstance()
{
	static lb::Logger vObjInstance;
	vObjInstance.SetDefaults();
	return vObjInstance;
}

/// <summary>
/// pushes text from << operator
/// </summary>
void lb::Logger::Push()
{
	this->Log(this->vPushLog.c_str());
	this->vPushLog = "";
}

/// <summary>
/// Basic log function
/// </summary>
/// <param name="p_strMsg">message to log</param>
/// <param name="v_newLine">append new line after message (default true)</param>
void lb::Logger::Log(const char* p_strMsg, bool v_newLine)
{
	this->LogAs("INFO", p_strMsg, v_newLine);
}

/// <summary>
/// Log function with category. Category is not case sensitive and will force uppercase.
/// </summary>
/// <param name="p_strCategory">category of message Ex: 'Exception'</param>
/// <param name="p_strMsg">message to log</param>
/// <param name="v_newLine">append new line after message (default true)</param>
void lb::Logger::LogAs(const char* p_strCategory, const char* p_strMsg, bool v_newLine)
{
	std::string vCategory = p_strCategory;

	std::transform(vCategory.begin(), vCategory.end(), vCategory.begin(), toupper);

	this->vLastLog = "[" + vCategory + "]  " + p_strMsg;

	if (v_newLine)
		this->vLastLog += "\n";

	if (this->vOutputConsole)
		std::cout << this->vLastLog;

	this->vLogMap.push_back({ vCategory, p_strMsg });
}

/// <summary>
/// Get the last item and category logged
/// </summary>
/// <param name="v_as">category if you want last log from specific category</param>
/// <returns>formated previous log and category</returns>
std::string lb::Logger::GetLastLog(const char* v_as)
{
	if (v_as)
	{
		std::string vCategory = v_as;
		std::transform(vCategory.begin(), vCategory.end(), vCategory.begin(), toupper);

		for (int i = this->vLogMap.size() - 1; i > 0; i--)
			if (this->vLogMap[i].first.compare(vCategory) == 0)
				return ("[" + this->vLogMap[i].first + "]  " + this->vLogMap[i].second + "\n");
	}
	else
		return this->vLastLog;
}

/// <summary>
/// Get entire log data
/// </summary>
/// <param name="v_as">get logs beloning to this category only (default nullptr for all categories)</param>
/// <returns>output stream as string</returns>
std::string lb::Logger::GetLog(const char* v_as)
{
	std::string vOut = "";
	std::string vCategory = "";

	if (v_as)
	{
		std::string vCategory = v_as;
		std::transform(vCategory.begin(), vCategory.end(), vCategory.begin(), toupper);
		for (auto& i : this->vLogMap)
			if (i.first.compare(vCategory) == 0)
				vOut += "[" + i.first + "]  " + i.second + "\n";
	}
	else 	
		for (auto& i : this->vLogMap)
			vOut += "[" + i.first + "]  " + i.second + "\n";

	return vOut;
}

/// <summary>
/// Clear output buffer
/// </summary>
void lb::Logger::Clear()
{
	this->vLastLog = "";
	this->vLogMap.clear();
}

/// <summary>
/// Format the message for logging. Use LB_FORMAT_LOG instead for most cases.
/// </summary>
/// <param name="v_file">file name __FILE__</param>
/// <param name="v_func">function name __func__</param>
/// <param name="v_line">line in file __LINE__</param>
/// <param name="v_msg">custom message to append to log</param>
/// <returns></returns>
std::string lb::Logger::Format(std::string v_file, std::string v_func, std::string v_line, std::string v_msg)
{
	std::string vStrTmp = "(" + v_file + "," + v_func + "," + v_line + "): " + v_msg;
	return vStrTmp;
}

/// <summary>
/// Stores strings for one log request in multiple lines. Use 'Push()' to log this string
/// </summary>
/// <param name="v_str">string to append to log message</param>
/// <returns>reference to logger for chaining</returns>
lb::Logger& lb::Logger::operator<<(const char* v_str)
{
	this->vPushLog += v_str;
	return *this;
}
