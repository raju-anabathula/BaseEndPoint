#ifndef ITCM_COMMON_INPUTSTRINGDESC_H
#define	ITCM_COMMON_INPUTSTRINGDESC_H

#include <iostream>
#include <boost/scoped_array.hpp>
#include "ByteStream.h"
//#include "LoggingMacros.h"

namespace ITCM
{
namespace Common
{

template <typename LengthType>
class InputStringDesc;

template <typename LengthType>
ByteStream& operator>>(ByteStream& input, const InputStringDesc<LengthType>& data);

template <typename LengthType>
std::ostream& operator<<(std::ostream& stream, const InputStringDesc<LengthType>& value);

/**
 * Tag for controlling how strings are deserialized
 */
template <typename LengthType>
class InputStringDesc
{
public:
    /**
     * Constructs a InputStringDesc which will save read values in data up to maxLength or the first '\0'.
     *
     * If nullTerminated, then the string must end with a null character and no firther characters will be read.
     * If not nullTerminated, then the entire length will be read.  The range may be null terminated, but no addiontal data
     * may be present after the first null character.
     * @param data Location to store result
     * @param maxLength Maximum number of characters to read
     * @param nullTerminated Whether or not a nullTermination is required
     */
    InputStringDesc(std::string& data, const LengthType& maxLength, bool nullTerminated)
    :m_data(data), m_maxLength(maxLength), m_nullTerminated(nullTerminated)
    {
    }

private:
    /*mutable*/ std::string& m_data;
    const LengthType& m_maxLength;
    bool m_nullTerminated;

    friend std::ostream& operator<<<LengthType>(std::ostream& stream, const InputStringDesc<LengthType>& value);
    friend ByteStream& operator>> <LengthType>(ByteStream& input, const InputStringDesc<LengthType>& data);
};

/**
 * Tags a string with metadata that controls its deserialization.
 *
 * @param data Location to store result
 * @param maxLength Maximum number of bytes to read into data
 * @return The Desc object that describes this operation
 */
template<typename LengthType>
InputStringDesc<LengthType> InputString(std::string& data, const LengthType& maxLength, bool nullTerminated)
{
    return InputStringDesc<LengthType>(data, maxLength, nullTerminated);
}

/**
 * Deserializes a string from a ByteStream
 * @param input Stream to deserialize from
 * @param data Tag for location to store retrieved data
 * @return The stream data was read from
 * @throw ItcmException Thrown when stream does not contain either a null delimited string or a full string
 */
template <typename LengthType>
ByteStream& operator>>(ByteStream& input, const InputStringDesc<LengthType>& data)
{
//    LOG_FUNCTION_ENTRY_AND_EXIT

    REQUIRE_NOT_EQUAL(input.GetBufferLength(), 0U, "Buffer must hold data");

    if (data.m_nullTerminated)
    {
        boost::uint32_t maxReadLength = std::min(static_cast<std::size_t>(data.m_maxLength), input.GetBufferLength());
        ByteStream::const_iterator readTill = std::find(input.Begin(), input.Begin() + maxReadLength, '\0');
        REQUIRE_FALSE(readTill == input.End(), "Null Terminated Strings must be null terminated");
        data.m_data.assign(input.Begin(), readTill);
       
        input.Discard(readTill - input.Begin() + 1);
    }
    else
    {
        data.m_data.resize(data.m_maxLength);     
        boost::uint32_t bytesRead = input.ReadBuffer(reinterpret_cast<boost::uint8_t*>(&data.m_data[0]), data.m_maxLength);
        REQUIRE_EQUAL(bytesRead, data.m_maxLength, "Non-null terminated strings must be exactly maxLength long");

        data.m_data.resize(std::find(data.m_data.begin(), data.m_data.end(), '\0') - data.m_data.begin());
    }

    return input;
}


}

}

#endif	/* ITCM_COMMON_INPUTSTRINGDESC_H */

