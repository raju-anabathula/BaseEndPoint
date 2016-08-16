#ifndef ITCM_COMMON_NOT_NULL_POINTER_H
#define	ITCM_COMMON_NOT_NULL_POINTER_H

#include <ITCM/common/ItcmException.h>
#include <string>


namespace ITCM
{
namespace Common
{

/**
 * Exception signals that a value was not-null when it was expected to be null.
 */
class NotNullPointer : public ItcmException
{
public:
    /**
     * Constructs a NotNullPointer
     * @param valueName The expression which evaluated to not null
     * @param message Additional information on the nature of the error
     */
    explicit NotNullPointer(const std::string& valueName,
                            const std::string& message = "");

    /**
     * Destructor.
     */
    virtual ~NotNullPointer() throw ()
    {
    }

    virtual std::string GetReason() const;

private:
    std::string m_valueName;
    std::string m_message;
};

}
}

#endif	/* ITCM_COMMON_NOT_NULL_POINTER_H */

