#ifndef ITCM_COMMON_TYPEDEFS_H
#define ITCM_COMMON_TYPEDEFS_H

#include <boost/cstdint.hpp>

namespace ITCM
{
namespace Common
{
typedef boost::uint32_t Cost;
typedef boost::uint16_t Crc;
typedef boost::uint16_t FragmentNumber;
typedef boost::uint16_t FragmentSize;
typedef boost::uint8_t MessageClass;
typedef boost::uint8_t MessagePriority;
typedef boost::uint16_t MessageTtl;
typedef boost::uint32_t MessageVersion;
typedef boost::uint8_t SpecialHandlingCode;
typedef boost::uint64_t Timestamp;
}
}

#endif /* ITCM_COMMON_TYPEDEFS_H */
