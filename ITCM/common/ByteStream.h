#ifndef ITCM_COMMON_BYTESTREAM_H
#define	ITCM_COMMON_BYTESTREAM_H

#include <cstddef>
#include <iosfwd>
#include <limits>
#include <string>
#include <vector>
#include <boost/cstdint.hpp>
#include <boost/noncopyable.hpp>

#include <ITCM/common/Preconditions.h>

namespace ITCM
{
namespace Common
{

/**
 * Manages a buffer form which data can be serialized and deserialized.
 */
class ByteStream : private boost::noncopyable
{
    friend std::ostream& ITCM::Common::operator<<(std::ostream& stream, const ByteStream& value);
public:
    typedef std::vector<boost::uint8_t>::const_iterator const_iterator;

    /**
     * Default constructor which creates an empty buffer
     */
    explicit ByteStream();

    /**
     * Constructs buffer with preallocated space
     */
    explicit ByteStream(std::size_t reserve);

    /**
     * Constructor which copies length bytes from data into the internal buffer
     * @param data Location of the data to copy
     * @param length The number of bytes to copy
     */
    explicit ByteStream(const boost::uint8_t* data, std::size_t length);

    /**
     * Writes a single byte to the buffer
     * @param data Byte to write
     */
    void WriteByte(boost::uint8_t data);
    /**
     * Reads a single byte from the buffer, and throws if no data is available
     * @return The byte which was read
     */
    boost::uint8_t ReadByte();

    /**
     * Writes length bytes from data into the buffer
     * @param data Location of the data to copy
     * @param length The number of bytes to copy
     */
    void WriteBuffer(const boost::uint8_t * const data, std::size_t length);
    /**
     * Reads up to maxLength bytes into data
     * @param data Destination to store data
     * @param maxLength Maximum number of bytes to copy to data
     * @return The number of bytes read
     */
    boost::uint32_t ReadBuffer(boost::uint8_t* data, std::size_t maxLength);

    /**
     * Discards up to length bytes
     * @param length Number of bytes to discard
     */
    void Discard(std::size_t length);

    /**
     * Gets an iterator to the start of the buffer
     */
    const_iterator Begin() const;
    const_iterator begin() const; //Synonym for gtest print support

    /**
     * Gets an iterator to one past the end of the buffer
     */
    const_iterator End() const;
    const_iterator end() const; //synonym for gtets printer support

    /**
     * Gets a pointer to the internal buffer
     * @return pointer to internal buffer
     */
    const boost::uint8_t* GetBuffer() const;
    /**
     * Gets the length of the internal buffer
     * @return length of internal buffer
     */
    std::size_t GetBufferLength() const;

private:
    std::vector<boost::uint8_t> m_buffer;
    std::size_t m_readPosition;
};

/**
 * Serializes data to output
 * @param output The stream to serialize to
 * @param data The data to serialize, in host byte order
 * @return The stream serialized to
 */
ByteStream & operator<<(ByteStream& output, boost::uint8_t data);
/**
 * Serializes data to output
 * @param output The stream to serialize to
 * @param data The data to serialize, in host byte order
 * @return The stream serialized to
 */
ByteStream & operator<<(ByteStream& output, boost::uint16_t data);
/**
 * Serializes data to output
 * @param output The stream to serialize to
 * @param data The data to serialize, in host byte order
 * @return The stream serialized to
 */
ByteStream & operator<<(ByteStream& output, boost::uint32_t data);
/**
 * Serializes data to output
 * @param output The stream to serialize to
 * @param data The data to serialize, in host byte order
 * @return The stream serialized to
 */
ByteStream & operator<<(ByteStream& output, boost::uint64_t data);

/**
 * Deserializes from an input stream into the reference location.
 *
 * Throws if insufficient data in in the buffer
 * @param input Stream to read data from
 * @param data Location to store result
 * @return Stream data was read from
 * @throw ItcmException when stream contains less than 1 byte
 */
ByteStream & operator>>(ByteStream& input, boost::uint8_t& data);

/**
 * Deserializes from an input stream into the reference location.
 *
 * Throws if insufficient data in in the buffer
 * @param input Stream to read data from
 * @param data Location to store result
 * @return Stream data was read from
 * @throw ItcmException when stream contains less than 2 bytes
 */
ByteStream & operator>>(ByteStream& input, boost::uint16_t& data);

/**
 * Deserializes from an input stream into the reference location.
 *
 * Throws if insufficient data in in the buffer
 * @param input Stream to read data from
 * @param data Location to store result
 * @return Stream data was read from
 * @throw ItcmException when stream contains less than 4 bytes
 */
ByteStream & operator>>(ByteStream& input, boost::uint32_t& data);

/**
 * Deserializes from an input stream into the reference location.
 *
 * Throws if insufficient data in in the buffer
 * @param input Stream to read data from
 * @param data Location to store result
 * @return Stream data was read from
 * @throw ItcmException when stream contains less than 8 bytes
 */
ByteStream & operator>>(ByteStream& input, boost::uint64_t& data);

/**
 * Outputs ByteStream in human-readable format for debugging purposes
 * @param stream Stream object
 * @param value The ByteStream to output
 * @return stream Stream object
 */
std::ostream & operator<<(std::ostream& stream, const ByteStream& value);


} // END namespace Common
} // END namespace ITCM

#include <ITCM/common/InputStringDesc.h>
#include <ITCM/common/OutputStringDesc.h>
#include <ITCM/common/InputBufferDesc.h>
#include <ITCM/common/OutputBufferDesc.h>
#include <ITCM/common/RangeCheckDesc.h>
#include <ITCM/common/UnsignedIntegerDesc.h>

#endif	/* ITCM_COMMON_BYTESTREAM_H */

