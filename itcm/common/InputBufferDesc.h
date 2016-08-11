#ifndef ITCM_COMMON_INPUTBUFFERDESC_H
#define	ITCM_COMMON_INPUTBUFFERDESC_H

#include "ByteStream.h"
//#include <qpid/types/Variant.h>
#include <boost/lexical_cast.hpp>
#include <boost/cstdint.hpp>
#include <iostream>
#include <string>

namespace ITCM
{

namespace Common
{

template <typename LengthType>
class InputBufferDesc;

template <typename LengthType>
ByteStream& operator>>(ByteStream& input, const InputBufferDesc<LengthType>& data);

template <typename LengthType>
std::ostream& operator<<(std::ostream& output, const InputBufferDesc<LengthType>& data);

/**
 * Tag for controlling how buffers are deserialized
 */
template <typename LengthType>
class InputBufferDesc
{
public:
    /**
     * Constructs a tag which will read length bytes from a stream into data
     * @param data Destination for retrieved data
     * @param length Number of bytes to read
     */
    InputBufferDesc(boost::uint8_t* data, LengthType& length)
    :m_data(data), m_length(length)
    {
    }

private:
    boost::uint8_t* m_data;
    LengthType& m_length;
    friend std::ostream& operator<< <LengthType>(std::ostream& output, const InputBufferDesc<LengthType>& data);
    friend ByteStream& operator>> <LengthType>(ByteStream& output, const InputBufferDesc<LengthType>& data);
};

/**
 * Reads a buffer from a ByteStream
 * @param input The stream to read from
 * @param data Tag describing the data to be read
 * @return The stream to read from
 * @throw ItcmException Thrown when insufficient data is left in buffer
 */
template <typename LengthType>
ByteStream& operator>>(ByteStream& input, const InputBufferDesc<LengthType>& data)
{
    REQUIRE_TRUE(input.GetBufferLength() >= data.m_length, "Buffer must contain enough data for read");
    input.ReadBuffer(data.m_data, data.m_length);

    return input;
}

/**
 * Creates an InputBuffer tag
 * @param data Location to store data
 * @param length Number of bytes to read
 * @return Tag which represent settings
 */
template <typename LengthType>
InputBufferDesc<LengthType> InputBuffer(boost::uint8_t* data, LengthType& length)
{
    return InputBufferDesc<LengthType>(data, length);
}

/**
 * Outputs InputBufferDesc in human-readable format for debugging purposes
 * @param stream Stream object
 * @param value The InputBufferDesc to output
 * @return stream Stream object
 */
/*template <typename LengthType>
std::ostream& operator<<(std::ostream& stream, const InputBufferDesc<LengthType>& value)
{
	qpid::types::Variant::Map map;
	for (boost::uint64_t i = 0U; i < sizeof(value.m_data) && i < value.m_length; ++i)
	{
		map["data[" + boost::lexical_cast<std::string>(i)+"]"] = value.m_data[i];
	}
	map["length"] = value.m_length;
	stream << map;
	return stream;
}*/

}
}

#endif	/* ITCM_COMMON_INPUTBUFFERDESC_H */

