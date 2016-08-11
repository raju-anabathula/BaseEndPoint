#ifndef ITCM_COMMON_NULL_POINTER_H
#define	ITCM_COMMON_NULL_POINTER_H

#include "ItcmException.h"

#include <string>

namespace ITCM
{
namespace Common
{

/**
 * Exception for when a value is null but should be not-null
 */
class NullPointer : public ItcmException
{
public:
    /**
     * Constructs an exception which signals a value was null when not allowed.
     * @param valueName The text of the expression which was null
     * @param message Detail on why the exception was thrown
     */
    explicit NullPointer(const std::string& valueName,
                         const std::string& message = "");

    /**
     * Destructor.
     */
    virtual ~NullPointer() throw ()
    {
    }

    virtual std::string GetReason() const;

private:
    std::string m_valueName;
    std::string m_message;
};

}
}

#endif	/* ITCM_COMMON_NULL_POINTER_H */

