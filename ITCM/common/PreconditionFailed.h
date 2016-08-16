#ifndef ITCM_COMMON_PRECONDITION_FAILED_H
#define	ITCM_COMMON_PRECONDITION_FAILED_H

#include <ITCM/common/ItcmException.h>
#include <string>

namespace ITCM
{
namespace Common
{

/**
 * Signals that a generic precondition has failed.
 */
class PreconditionFailed : public ItcmException
{
public:
    /**
     * Constructs a generic precondition failed exception.
     * @param valueName The text of the value expression
     * @param value The value of the value expression
     * @param expectedName The text of the expected expression
     * @param expected The value of the expected expression
     * @param message Detail on why the exception was thrown
     */
    explicit PreconditionFailed(const std::string& valueName,
                                const std::string& value,
                                const std::string& expectedName,
                                const std::string& expected,
                                const std::string& message = "");

    /**
     * Destructor.
     */
    virtual ~PreconditionFailed() throw ()
    {
    }

    virtual std::string GetReason() const;

    void SetMessage(const std::string& message);
private:
    std::string m_valueName, m_value, m_expectedName, m_expected;
    std::string m_message;
};

}
}

#endif	/* ITCM_COMMON_PRECONDITION_FAILED_H */

