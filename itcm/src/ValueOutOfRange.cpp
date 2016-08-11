#include "../common/ValueOutOfRange.h"

#include <string>
#include <boost/format.hpp>

namespace ITCM
{
namespace Common
{

ValueOutOfRange::ValueOutOfRange(const std::string& valueName,
                                 const std::string& value,
                                 const std::string& lowerBoundName,
                                 const std::string& lowerBound,
                                 const std::string& upperBoundName,
                                 const std::string& upperBound,
                                 const std::string& message)
: m_valueName(valueName),
m_value(value),
m_lowerBoundName(lowerBoundName),
m_lowerBound(lowerBound),
m_upperBoundName(upperBoundName),
m_upperBound(upperBound),
m_message(message)
{
}

std::string ValueOutOfRange::GetReason() const
{
    return (boost::str(boost::format("Value (%1%=%2%) out of range [%3%=%4%, %5%=%6%]: %7% ")
                       % m_valueName % m_value
                       % m_lowerBoundName % m_lowerBound
                       % m_upperBoundName % m_upperBound
                       % m_message));
}

}
}

