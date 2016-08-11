#ifndef ITCM_COMMON_PRECONDITIONS_H
#define ITCM_COMMON_PRECONDITIONS_H

#include <string>
#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/preprocessor.hpp>

#include "ValueOutOfRange.h"
#include "NullPointer.h"
#include "NotNullPointer.h"
#include "PreconditionFailed.h"
#include "ThrowException.h"

namespace ITCM
{
namespace Common
{

/**
 * Checks if a value is within the defined range, inclusively, and throws an exception if not.
 *
 * Intended to be used via the REQUIRE_IN_RANGE macro
 *
 * @param fileName The name of the file the check is being performed in
 * @param lineNumber The line of the file the check is being performed on
 * @param valueName The expression which is being evaluated
 * @param value The value of the evaluated expression
 * @param lowerName The expression which forms the lower bound
 * @param lowerValue The value of the evaluated lower bound
 * @param upperName The expression that forms the upper bound
 * @param upperValue The value of the evaluated upper bound
 */
template<typename ValueType, typename LowerType, typename UpperType>
void RequireValueInRange(const std::string& fileName,
                         boost::uint32_t lineNumber,
                         const std::string& valueName,
                         const ValueType& value,
                         const std::string& lowerName,
                         const LowerType& lowerValue,
                         const std::string& upperName,
                         const UpperType& upperValue)
{
    if (value < lowerValue || value > upperValue)
    {
        ValueOutOfRange exc(valueName, boost::lexical_cast<std::string > (value),
                            lowerName, boost::lexical_cast<std::string > (lowerValue),
                            upperName, boost::lexical_cast<std::string > (upperValue),
                            ""
                            );
        ThrowException(fileName, lineNumber, exc);
    }
}

/**
 * Checks that a value is not NULL.
 *
 * Intended to be used via the REQUIRE_NOT_NULL macro.
 *
 * @param fileName The name of the file the check is being performed in
 * @param lineNumber The line of the file the check is being performed on
 * @param valueName The expression which when evaluated may not be NULL
 * @param message Additional information on this requirement
 */
inline void ThrowRequireNotNull(const std::string& fileName,
                    boost::uint32_t lineNumber,
                    const std::string& valueName,
                    const std::string& message = "")
{
    NullPointer exc(valueName, message);
    ThrowException(fileName, lineNumber, exc);
}

/**
 * Checks that a value is NULL.
 *
 * Intended to be used via the REQUIRE_NULL macro.
 *
 * @param fileName The name of the file the check is being performed in
 * @param lineNumber The line of the file the check is being performed on
 * @param valueName The expression which when evaluated should be NULL
 * @param message Additional information on this requirement
 */
inline void ThrowRequireNull(const std::string& fileName,
                 boost::uint32_t lineNumber,
                 const std::string& valueName,
                 const std::string& message = "")
{
    NotNullPointer exc(valueName, message);
    ThrowException(fileName, lineNumber, exc);
}

/**
 * Checks that a value equals an expected value.
 *
 * Intended to be used via the REQUIRE_EQUAL macro
 *
 * @param fileName The name of the file the check is being performed in
 * @param lineNumber The line of the file the check is being performed on
 * @param valueName The expression which will be evaluated
 * @param value The evaluated value of the expression
 * @param expectedName The expression which will be compared against
 * @param expectedValue The evaluated expected value
 */
template<typename ActualType, typename ExpectedType>
void RequireEqual(const std::string& fileName,
                  boost::uint32_t lineNumber,
                  const std::string& valueName,
                  const ActualType& value,
                  const std::string& expectedName,
                  const ExpectedType& expectedValue,
                  const std::string& message = "")
{
    if (value != expectedValue)
    {
        PreconditionFailed exc(valueName,
                               boost::lexical_cast<std::string > (value),
                               expectedName,
                               boost::lexical_cast<std::string > (expectedValue),
                               message);
        ThrowException(fileName, lineNumber, exc);
    }
}

/**
 * Checks that a value does not equal an unexpected value.
 *
 * Intended to be used via the REQUIRE_NOT_EQUAL macro
 *
 * @param fileName The name of the file the check is being performed in
 * @param lineNumber The line of the file the check is being performed on
 * @param valueName The expression which will be evaluated
 * @param value The evaluated value of the expression
 * @param unexpectedName The expression which will be compared against
 * @param unexpectedValue The evaluated expected value
 */
template<typename ActualType, typename ExpectedType>
void RequireNotEqual(const std::string& fileName,
                     boost::uint32_t lineNumber,
                     const std::string& valueName,
                     const ActualType& value,
                     const std::string& unexpectedName,
                     const ExpectedType& unexpectedValue)
{
    if (value == unexpectedValue)
    {
        PreconditionFailed exc(valueName,
                               boost::lexical_cast<std::string > (value),
                               "!(" + unexpectedName,
                               boost::lexical_cast<std::string > (unexpectedValue) + ")",
                               "");
        ThrowException(fileName, lineNumber, exc);
    }
}

}
}

/**
 * Output the file name of the current file.
 */
#define __FILENAME__ __FILE__

#define INJECT_EXCEPTION_MESSAGE(statement, message) do { \
    try { \
        (statement); \
    } catch(::ITCM::Common::PreconditionFailed& e) \
    { e.SetMessage(message); throw e;} \
    } while (false)

/**
 * Requires that a value is between [lower, upper], inclusive, as defined by the less than ang greater than operators
 */
#define REQUIRE_IN_RANGE(value, lower, upper, message) INJECT_EXCEPTION_MESSAGE(::ITCM::Common::RequireValueInRange(__FILENAME__, __LINE__, BOOST_PP_STRINGIZE(value), (value), BOOST_PP_STRINGIZE(lower), (lower), BOOST_PP_STRINGIZE(upper), (upper)), (message))

/**
 * Requires that the values is not null.  Works with any value which can be compared against null.
 */
#define REQUIRE_NOT_NULL(value, message) if ((value) == NULL) ::ITCM::Common::ThrowRequireNotNull(__FILENAME__, __LINE__, BOOST_PP_STRINGIZE(value), (message))

/**
 * Requires that the values is null.  Works with any value which can be compared against null.
 */
#define REQUIRE_NULL(value, message) if ((value) != NULL) ::ITCM::Common::ThrowRequireNull(__FILENAME__, __LINE__, BOOST_PP_STRINGIZE(value), (message))

/**
 * Requires that two values be equal as defined by operator==
 */
#define REQUIRE_EQUAL(value, expected, message)  INJECT_EXCEPTION_MESSAGE(::ITCM::Common::RequireEqual(__FILENAME__, __LINE__, BOOST_PP_STRINGIZE(value), (value), BOOST_PP_STRINGIZE(expected), (expected)), (message))

/**
 * Requires that the value be true
 */
#define REQUIRE_TRUE(value, message) if (!(value)) ::ITCM::Common::RequireEqual(__FILENAME__, __LINE__, BOOST_PP_STRINGIZE(value), false, "true", true, (message))

/**
 * Rewuires that the value be false
 */
#define REQUIRE_FALSE(value, message) if (value) ::ITCM::Common::RequireEqual(__FILENAME__, __LINE__, BOOST_PP_STRINGIZE(value), true, "false", false, (message))

/**
 * Requires that the value not equal an unexpected value
 */
#define REQUIRE_NOT_EQUAL(value, unexpected, message) INJECT_EXCEPTION_MESSAGE(::ITCM::Common::RequireNotEqual(__FILENAME__, __LINE__, BOOST_PP_STRINGIZE(value), (value), BOOST_PP_STRINGIZE(unexpected), (unexpected)), (message))

#endif /* ITCM_COMMON_PRECONDITIONS_H */
