#ifndef ITCM_COMMON_ITCM_EXCEPTION_H
#define	ITCM_COMMON_ITCM_EXCEPTION_H

#include <stdexcept>
#include <string>

#include <boost/cstdint.hpp>

namespace ITCM
{
namespace Common
{

/**
 * Standard base class for exceptions thrown by ITCM components.  This exception
 * records the file and line number of errors.  The THROW macro defined in
 * Preconditions.h should be used to automatically fill out these fields.
 */
class ItcmException: public std::exception
{
public:
	/**
	 * Destructor.
	 */
    virtual ~ItcmException() throw(){};
    /**
     * @return reason exception was thrown
     */
    virtual const char* what() const throw();

    /**
     * Virtual function the must be defined to specify the reason for a failure.
     * @return string
     */
    virtual std::string GetReason() const = 0;

    /**
     * Sets the file name associated with this error
     * @param fileName
     */
    void SetFileName(const std::string& fileName) const;

    /**
     * Sets the line number associated with this error.
     * @param lineNumber
     */
    void SetLineNumber(boost::uint32_t lineNumber) const;

private:

    mutable std::string m_fileName;
    mutable boost::uint32_t m_lineNumber;

    mutable std::string m_cachedWhat;
};

}    
}

#endif	/* ITCM_COMMON_ITCM_EXCEPTION_H */

