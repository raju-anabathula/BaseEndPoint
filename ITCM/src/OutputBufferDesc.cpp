
#include <ITCM/common/OutputBufferDesc.h>

namespace ITCM
{

namespace Common
{

ByteStream& operator<<(ByteStream& output, const OutputBuffer& data)
{
    output.WriteBuffer(data.GetData(), data.GetLength());

    return output;
}

const boost::uint8_t* const OutputBuffer::GetData() const
{
    return m_data;
}

boost::uint32_t OutputBuffer::GetLength() const
{
    return m_length;
}

}
}
