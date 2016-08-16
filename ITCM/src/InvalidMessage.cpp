#include <ITCM/Errors/InvalidMessage.h>

using namespace std;

namespace ITCM
{

InvalidMessage::InvalidMessage(const string& reason)
: m_reason(reason)
{

}

InvalidMessage::~InvalidMessage() throw()
{

}

string InvalidMessage::GetReason() const
{
	return m_reason;
}

}
