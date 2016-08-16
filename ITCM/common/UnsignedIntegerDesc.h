#ifndef ITCM_COMMON_UNSIGNEDINTEGERDESC_H
#define	ITCM_COMMON_UNSIGNEDINTEGERDESC_H

#include <iostream>
#include <boost/static_assert.hpp>
#include <ITCM/common/ByteStream.h>
//#include "LoggingMacros.h"

namespace ITCM
{

namespace Common
{

template <unsigned int bytes, typename UnsignedType>
class UnsignedIntegerDesc;

template <unsigned int bytes, typename UnsignedType>
ByteStream& operator>>(ByteStream& input, const UnsignedIntegerDesc<bytes, UnsignedType>& desc);

template <unsigned int bytes, typename UnsignedType>
std::ostream& operator<<(std::ostream& stream, const UnsignedIntegerDesc<bytes, UnsignedType> & value);

template <unsigned int bytes, typename UnsignedType>
class UnsignedIntegerDesc
{
    BOOST_STATIC_ASSERT(std::numeric_limits<UnsignedType>::digits >= 0);
    BOOST_STATIC_ASSERT((unsigned int)std::numeric_limits<UnsignedType>::digits >= bytes);
public:
    explicit UnsignedIntegerDesc(UnsignedType& data)
    :m_data(data)
    {
    }

    const UnsignedType& GetData() const
    {
        return m_data;
    }
    
private:
    /*mutable*/ UnsignedType& m_data;
    friend std::ostream& operator<<<bytes, UnsignedType>(std::ostream& stream, const UnsignedIntegerDesc<bytes, UnsignedType> & value);
    friend ByteStream& operator>> <bytes, UnsignedType>(ByteStream& input, const UnsignedIntegerDesc<bytes, UnsignedType>& desc);
};

template<unsigned int bytes, typename UnsignedType>
UnsignedIntegerDesc<bytes, UnsignedType> UnsignedInteger(UnsignedType& data)
{
    return UnsignedIntegerDesc<bytes, UnsignedType>(data);
}

template <unsigned int bytes, typename UnsignedType>
ByteStream& operator<<(ByteStream& output, const UnsignedIntegerDesc<bytes, UnsignedType>& desc)
{
//    LOG_FUNCTION_ENTRY_AND_EXIT

    boost::uint8_t staging[bytes];
    UnsignedType data = desc.GetData();

    for (boost::uint32_t i = 1; i <= bytes; ++i)
    {
        staging[bytes - i] = data % 256;
        data = data >> 8;
    }

    output.WriteBuffer(staging, sizeof(staging));

    return output;
}

template <unsigned int bytes, typename UnsignedType>
ByteStream& operator>>(ByteStream& input, const UnsignedIntegerDesc<bytes, UnsignedType>& desc)
{
//    LOG_FUNCTION_ENTRY_AND_EXIT

    boost::uint8_t staging[bytes];
    boost::uint32_t bytesRead = input.ReadBuffer(staging, bytes);
    REQUIRE_EQUAL(bytesRead, bytes, "Buffer must contain enough data for read");

    desc.m_data = 0;
    for (boost::uint32_t i = 0; i < bytes; ++i)
    {
        desc.m_data = desc.m_data << 8;
        desc.m_data += staging[i];
    }

    return input;
}


}
}

#endif	/* ITCM_COMMON_UNSIGNEDINTEGERDESC_H */

