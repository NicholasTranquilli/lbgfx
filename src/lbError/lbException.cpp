#include "lbException.h"

/****************************/
/* PRIVATE MEMBER FUNCTIONS */
/****************************/

void lb::Exception::ThrowExcept(lb::Exception v_exc)
{
	lb::LB_LOG_NAME.LogAs("EXCEPTION", v_exc.vExcStr.c_str());
	throw v_exc;
}

void lb::Exception::LogInfo(lb::Exception v_exc)
{
	lb::LB_LOG_NAME.LogAs("INFO", v_exc.vExcStr.c_str());
}

void lb::Exception::LogError(lb::Exception v_exc)
{
	lb::LB_LOG_NAME.LogAs("ERROR", v_exc.vExcStr.c_str());
}

/***************************/
/* PUBLIC MEMBER FUNCTIONS */
/***************************/

lb::Exception::Exception()
{
}

lb::Exception::Exception(std::string v_excStr)
{
	this->vExcStr = v_excStr;
}

std::string lb::Exception::GetLastException() const
{
	return lb::logger.GetLastLog("EXCEPTION");
}

std::string lb::Exception::GetExceptionLog() const
{
	return lb::logger.GetLog("EXCEPTION");
}
