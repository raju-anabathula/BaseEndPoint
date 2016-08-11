#ifndef ITCM_INVALID_MESSAGE_H
#define ITCM_INVALID_MESSAGE_H

#include <itcm/common/ItcmException.h>

namespace ITCM
{

class InvalidMessage : public Common::ItcmException
{
public:
	/**
	 * Constructor for ConnectionFailedException
	 * @param connId ID of the connection that failed
	 */
	explicit InvalidMessage(const std::string& reason);
	/**
	 * Destructor for ConnectionFailedException
	 */
	virtual ~InvalidMessage() throw();
	/**
	 * @return Exception's message text
	 */
	virtual std::string GetReason() const;
private:
	const std::string m_reason;
};


}

#endif /* ITCM_INVALID_MESSAGE_H */
