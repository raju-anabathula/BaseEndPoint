#ifndef ITCM_COMMON_VALUE_OUT_OF_RANGE_H
#define	ITCM_COMMON_VALUE_OUT_OF_RANGE_H

#include "ItcmException.h"

namespace ITCM
{
namespace Common
{

/**
 * Signifies that a value was out of range.
 */
class ValueOutOfRange: public ItcmException
{
public:
    /**
     * Constructs a value out of range exception.
     * @param valueName The text of the value expression
     * @param value The value of the value expression
     * @param lowerBoundName The text of the lower bound expression
     * @param lowerBound The value of the lower bound expression
     * @param upperBoundName The text of the upper bound expression
     * @param upperBound The value of the upper bound expression
     * @param message Detail regarding why the exception was thrown
     */
	explicit ValueOutOfRange(const std::string& valueName,
            const std::string& value,
            const std::string& lowerBoundName, 
            const std::string& lowerBound, 
            const std::string& upperBoundName, 
            const std::string& upperBound, 
            const std::string& message = ""
            );
	/**
	 * Destructor.
	 */
    virtual ~ValueOutOfRange() throw() {}

    virtual std::string GetReason() const;
    
private:
    std::string m_valueName, m_value, m_lowerBoundName, m_lowerBound, m_upperBoundName, m_upperBound;
    std::string m_message;
};

}
}

#endif	/* ITCM_COMMON_VALUE_OUT_OF_RANGE_H */

