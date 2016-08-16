#ifndef ITCM_COMMON_THROW_EXCEPTION_H
#define ITCM_COMMON_THROW_EXCEPTION_H

#include <string>
#include <boost/cstdint.hpp>
#include <boost/filesystem/path.hpp>

namespace ITCM
{
namespace Common
{

/**
 * Throws an ITCM Exception, after setting the file and line number.
 *
 * Intended to be used via the THROW macro
 * @param fileName The name of the file the exception is being thrown in
 * @param lineNumber The line number the exxception is being thrown on
 * @param e The exception to be thrown
 */
template <typename ExceptionType>
void ThrowException(const std::string& fileName,
                    boost::uint32_t lineNumber,
                    const ExceptionType& e)
{
//    e.SetFileName(boost::filesystem::path(fileName).leaf());
    e.SetLineNumber(lineNumber);
    throw e;
}

}
}

/** 
 *  THROW macro sets filename, linenumber, and throws the exception
 */
#define THROW(exception) \
    exception.SetLineNumber(__LINE__); \
    throw exception;
// exception.SetFileName(boost::filesystem::path(__FILE__).leaf());
#endif /* ITCM_COMMON_THROW_EXCEPTION_H */
