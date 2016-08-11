#include "../common/NotNullPointer.h"

#include <string>

namespace ITCM
{
namespace Common
{

NotNullPointer::NotNullPointer(const std::string& valueName, const std::string& message)
: m_valueName(valueName), m_message(message)
{
}

std::string NotNullPointer::GetReason() const
{
    return "Null Pointer: " + m_valueName + " is not null, " + m_message;
}

}
}

