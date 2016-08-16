
#include <string>
#include <boost/format.hpp>

#include <ITCM/common/ItcmException.h>

namespace ITCM
{
namespace Common
{

void ItcmException::SetFileName(const std::string& fileName) const
{
	m_fileName = fileName;
}

void ItcmException::SetLineNumber(boost::uint32_t lineNumber) const
{
	m_lineNumber = lineNumber;
}

const char* ItcmException::what() const throw()
{
	if (m_cachedWhat.empty())
	{
		m_cachedWhat = (boost::format("%1%, %2%: %3%") % m_fileName % m_lineNumber % GetReason()).str();
	}
	return m_cachedWhat.c_str();
}

}
}
