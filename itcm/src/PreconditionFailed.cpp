#include "../common/PreconditionFailed.h"

#include <string>
#include <boost/format.hpp>

namespace ITCM
{
namespace Common
{

PreconditionFailed::PreconditionFailed(const std::string& valueName,
                                       const std::string& value,
                                       const std::string& expectedName,
                                       const std::string& expected,
                                       const std::string& message)
: m_valueName(valueName), m_value(value), m_expectedName(expectedName), m_expected(expected), m_message(message)
{
}

std::string PreconditionFailed::GetReason() const
{
    return (boost::str(boost::format("Precondition Failed.  %1%=%2%, but the expected value is %3%=%4%. %5%") %
                       m_valueName %
                       m_value %
                       m_expectedName %
                       m_expected %
                       m_message));
}

void PreconditionFailed::SetMessage(const std::string& message)
{
    m_message = message;
}

}
}

