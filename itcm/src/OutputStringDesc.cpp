
#include "../common/OutputStringDesc.h"

namespace ITCM
{

namespace Common
{

ByteStream& operator<<(ByteStream& output, const OutputString& data)
{
    output.WriteBuffer(reinterpret_cast<const boost::uint8_t*>(&(*data.m_data.begin())), data.m_data.size());
    if (data.m_nullTerminated)
    {
        output << (boost::uint8_t)'\0';
    }

    return output;
}

}
}
