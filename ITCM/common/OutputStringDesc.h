#ifndef ITCM_COMMON_OUTPUTSTRINGDESC_H
#define	ITCM_COMMON_OUTPUTSTRINGDESC_H

#include <ITCM/common/ByteStream.h>

namespace ITCM
{
namespace Common
{
/**
 * Tag for controlling how strings are serialized
 */
class OutputString
{
public:
    /**
     * Constructs an output string that serializes data according to supplied options
     * @param data The string to serialize
     * @param nullTerminated Whether or not to null terminate the string
     */
    OutputString(const std::string& data, bool nullTerminated)
    :m_data(data), m_nullTerminated(nullTerminated)
    {
    }

private:
    const std::string& m_data;
    bool m_nullTerminated;

    friend ByteStream& operator<< (ByteStream& input, const OutputString& data);
};

/**
 * Serializes a string to a ByteStream
 * @param output The stream to serialize to
 * @param data The tag for the associated string
 * @return The stream data was written to
 */
ByteStream& operator<<(ByteStream& output, const OutputString& data);
}
}

#endif	/* ITCM_COMMON_OUTPUTSTRINGDESC_H */

