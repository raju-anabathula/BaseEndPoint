
#include <vector>
#include <algorithm>

#include "../common/ByteStream.h"
#include "../common/Preconditions.h"
//#include <qpid/types/Variant.h>
#include <iostream>

#include <netinet/in.h>
#include <boost/static_assert.hpp>


namespace ITCM
{

namespace Common
{

ByteStream::ByteStream()
:m_readPosition(0)
{
}

ByteStream::ByteStream(std::size_t reserve)
:m_readPosition(0)
{
    m_buffer.reserve(reserve);
}

ByteStream::ByteStream(const boost::uint8_t* data, std::size_t length)
:m_buffer(data, data + length), m_readPosition(0)
{

}

void ByteStream::WriteByte(boost::uint8_t data)
{
    m_buffer.push_back(data);
}

boost::uint8_t ByteStream::ReadByte()
{
    REQUIRE_NOT_EQUAL(m_readPosition, m_buffer.size(), "Cannot read past end of buffer");
    std::size_t position = m_readPosition;
    ++m_readPosition;
    return m_buffer[position];
}

void ByteStream::WriteBuffer(const boost::uint8_t* const data, std::size_t length)
{
    m_buffer.insert(m_buffer.end(), data, data+length);
}

boost::uint32_t ByteStream::ReadBuffer(boost::uint8_t* data, std::size_t maxLength)
{
    std::size_t copyLength = std::min(maxLength, m_buffer.size() - m_readPosition);
    
    std::vector<boost::uint8_t>::const_iterator start = m_buffer.begin() + m_readPosition;
    std::copy(start, start + copyLength, data);

    m_readPosition += copyLength;
    return copyLength;
}

void ByteStream::Discard(std::size_t length)
{
    m_readPosition += length;
    m_readPosition = std::min(m_readPosition, m_buffer.size());
}

ByteStream::const_iterator ByteStream::Begin() const
{
    return m_buffer.begin() + m_readPosition;
}

ByteStream::const_iterator ByteStream::End() const
{
    return m_buffer.end();
}

ByteStream::const_iterator ByteStream::begin() const
{
    return Begin();
} 

ByteStream::const_iterator ByteStream::end() const
{
    return End();
}

const boost::uint8_t* ByteStream::GetBuffer() const
{
    return (m_buffer.begin() + m_readPosition).base();
}

std::size_t ByteStream::GetBufferLength() const
{
    return m_buffer.size() - m_readPosition;
}

ByteStream& operator<<(ByteStream& output, boost::uint8_t data)
{
    output.WriteByte(data);

    return output;
}

ByteStream& operator<<(ByteStream& output, boost::uint16_t data)
{
    data = htons(data);
    output.WriteBuffer(reinterpret_cast<boost::uint8_t*>(&data), (size_t)sizeof(uint16_t));

    return output;
}

ByteStream& operator<<(ByteStream& output, boost::uint32_t data)
{
    data = htonl(data);
    output.WriteBuffer(reinterpret_cast<boost::uint8_t*>(&data), (size_t)sizeof(uint32_t));

    return output;
}

ByteStream& operator<<(ByteStream& output, boost::uint64_t data)
{
    boost::uint8_t staging[sizeof(boost::uint64_t)];
    
    for (boost::uint32_t i = 1; i <= sizeof(boost::uint64_t); ++i)
    {
        staging[sizeof(boost::uint64_t) - i] = data % 256;
        data = data >> 8;
    }
    
    output.WriteBuffer(staging, sizeof(staging));

    return output;
}


ByteStream& operator>>(ByteStream& input, boost::uint8_t& data)
{
    data = input.ReadByte();

    return input;
}

ByteStream& operator>>(ByteStream& input, boost::uint16_t& data)
{
    boost::uint32_t bytesRead = input.ReadBuffer(reinterpret_cast<boost::uint8_t*>(&data), sizeof(boost::uint16_t));
    REQUIRE_EQUAL(bytesRead, sizeof(boost::uint16_t), "Buffer must contain enough data for read");

    data = ntohs(data);

    return input;
}

ByteStream& operator>>(ByteStream& input, boost::uint32_t& data)
{
    boost::uint32_t bytesRead = input.ReadBuffer(reinterpret_cast<boost::uint8_t*>(&data), sizeof(boost::uint32_t));
    REQUIRE_EQUAL(bytesRead, sizeof(boost::uint32_t), "Buffer must contain enough data for read");

    data = ntohl(data);

    return input;
}

ByteStream& operator>>(ByteStream& input, boost::uint64_t& data)
{
    boost::uint8_t staging[sizeof(boost::uint64_t)];
    boost::uint32_t bytesRead = input.ReadBuffer(staging, sizeof(boost::uint64_t));
    REQUIRE_EQUAL(bytesRead, sizeof(boost::uint64_t), "Buffer must contain enough data for read");

    for (boost::uint32_t i = 0; i < sizeof(boost::uint64_t); ++i)
    {
        data = data << 8;
        data += staging[i];
    }

    return input;
}

/*std::ostream& operator<<(std::ostream& stream, const ByteStream& value)
{
	qpid::types::Variant::Map map;
	for (std::vector<boost::uint8_t>::const_iterator bufferItr = (value.m_buffer.begin()); bufferItr != value.m_buffer.end(); ++bufferItr)
	{
		boost::uint64_t iteration = bufferItr-value.m_buffer.begin();
		map["buffer[" +boost::lexical_cast<std::string>(iteration)+"]"] = *bufferItr;
	}
	map["readPosition"] = value.m_readPosition;
	stream << map;
	return stream;
}*/


}

}
