#pragma once

#include <exception>
#include "lbLogger.h"

// Does not have to inherit Exception
//#define LB_LOGAS_EXCEPTION(v_msg) lb::LB_LOG_NAME.LogAs("EXCEPTION", LB_FORMAT_LOG(v_msg).c_str()); \
//throw lb::Exception(v_msg)

// Class must inherit from lb::Exception for this Macro to work
#define LB_LOG_VALUE(v_val) this->LogInfo(lb::Exception(LB_FORMAT_LOG((std::string)#v_val + " = " + lb::Logger::ValToString(v_val))))
#define LB_LOG_VALUE_EX(v_val, v_name) this->LogInfo(lb::Exception(LB_FORMAT_LOG((std::string)v_name + " = " + lb::Logger::ValToString(v_val))))

// Class must inherit from lb::Exception for this Macro to work
#define LB_LOG_ERROR(v_msg) this->LogError(lb::Exception(LB_FORMAT_LOG((std::string)v_msg)))

// Class must inherit from lb::Exception for this Macro to work
// If v_operation is true, throw exception.
// Else, LB_LOG_VALUE(v_val)
#define LB_CHECK_AND_THROW(v_operation, v_val)																				\
	if (v_operation)																										\
		this->ThrowExcept(lb::Exception(LB_FORMAT_LOG((std::string)#v_val + " was " + lb::Logger::ValToString(v_val))));	\
	else LB_LOG_VALUE(v_val)

#define LB_CHECK_AND_THROW_EX(v_operation, v_val, v_name)																	\
	if (v_operation)																										\
		this->ThrowExcept(lb::Exception(LB_FORMAT_LOG((std::string)v_name + " was " + lb::Logger::ValToString(v_val))));	\
	else LB_LOG_VALUE_EX(v_val, v_name)

// Class must inherit from lb::Exception for this Macro to work
// Checks if v_val is nullptr
// If true, throws exception
// Else, log normally
#define LB_CHECK_NP_AND_THROW(v_val) LB_CHECK_AND_THROW(v_val == nullptr, v_val)
#define LB_CHECK_NP_AND_THROW_EX(v_val, v_name) LB_CHECK_AND_THROW_EX(v_val == nullptr, v_val, v_name)

// Class must inherit from lb::Exception for this Macro to work
// Checks if v_val != 0 (S_OK for HRESULT)
// If true, throw exception
// Else, log normally
#define LB_CHECK_HR_AND_THROW(v_val) LB_CHECK_AND_THROW(v_val != 0, v_val)
#define LB_CHECK_HR_AND_THROW_EX(v_val, v_name) LB_CHECK_AND_THROW_EX(v_val != 0, v_val, v_name)

namespace lb
{
	class Exception
	{
	private:
		std::string vExcStr;

	protected:
		static void ThrowExcept(lb::Exception v_exc);
		static void LogInfo(lb::Exception v_exc);
		static void LogError(lb::Exception v_exc);

	public:
		Exception();
		Exception(std::string v_excStr);

		std::string GetLastException() const;
		std::string GetExceptionLog() const;
	};
}