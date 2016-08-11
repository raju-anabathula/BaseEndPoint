#include "../common/NullPointer.h"

#include <string>

namespace ITCM
{
namespace Common
{

NullPointer::NullPointer(const std::string& valueName, const std::string& message)
: m_valueName(valueName), m_message(message)
{
}

std::string NullPointer::GetReason() const
{
    return "Null Pointer: " + m_valueName + " is null, " + m_message;
}

}
}
