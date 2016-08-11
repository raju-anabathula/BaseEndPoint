#ifndef ITCM_COMMON_OUTPUTBUFFERDESC_H
#define	ITCM_COMMON_OUTPUTBUFFERDESC_H

#include "ByteStream.h"

namespace ITCM
{

namespace Common
{

class OutputBuffer
{
public:

    OutputBuffer(const boost::uint8_t* const data, boost::uint32_t length)
    :m_data(data), m_length(length)
    {
    }

    const boost::uint8_t* const GetData() const;
    boost::uint32_t GetLength() const;

private:
    const boost::uint8_t* const m_data;
    boost::uint32_t m_length;
};

/**
 * Writes a buffer to a ByteStream
 * @param output The stream to write to
 * @param data Tag describing the data to be written
 * @return The stream written to
 */
ByteStream& operator<<(ByteStream& output, const OutputBuffer& data);

}

}


#endif	/* ITCM_COMMON_OUTPUTBUFFERDESC_H */

