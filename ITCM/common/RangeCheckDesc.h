#ifndef ITCM_COMMON_RANGECHECKDESC_H
#define	ITCM_COMMON_RANGECHECKDESC_H

#include <ITCM/common/ByteStream.h>
#include <iostream>

namespace ITCM
{
namespace Common
{
template <typename RangeType>
class RangeCheckDesc;

template <typename RangeType>
std::ostream& operator<<(std::ostream& stream, const RangeCheckDesc<RangeType>& value);


/**
 * Tag which verifies that a deserialized value is within an expected range
 */
template <typename RangeType>
class RangeCheckDesc
{
	friend std::ostream& operator<<<RangeType>(std::ostream& stream, const RangeCheckDesc<RangeType>& value);
public:
    /**
     * Constructs a tag which verifies a deserialized value is in an expected range
     * @param value Location to store deserialized value
     * @param lower Lower bound (inclusive) of the value
     * @param upper Upper bound (inclusive) of the value
     */
    RangeCheckDesc(RangeType& value, const RangeType& lower, const RangeType& upper)
    :m_lowerBound(lower), m_upperBound(upper), m_value(value)
    {
    }

    const RangeType& GetLowerBound() const
    {
        return m_lowerBound;
    }
    const RangeType& GetUpperBound() const
    {
        return m_upperBound;
    }

    RangeType& GetValue() const
    {
        return m_value;
    }

private:
    const RangeType& m_lowerBound;
    const RangeType& m_upperBound;
    /*mutable*/ RangeType& m_value;
};

/**
 * Deserializes a value and performs a range check as described by data.
 *
 * @param input Stream to read data from
 * @param data Tag which describes location to store data and bounds
 * @return Stream data was read from
 * @throw ItcmException Thrown when deserialized value is out of range
 */
template <typename RangeType>
ByteStream& operator>>(ByteStream& input, const RangeCheckDesc<RangeType>& data)
{
    input >> data.GetValue();
    REQUIRE_IN_RANGE(data.GetValue(), data.GetLowerBound(), data.GetUpperBound(), "");

    return input;
}

/**
 * Constructs a RangeCheckDesc
 * @param value The location to store a deserialized value
 * @param lower The lower bound (inclusive) of the value
 * @param upper The upper bound (inclusive) of the value
 * @return A tag representing the selected options
 */
template <typename RangeType>
RangeCheckDesc<RangeType> RangeCheck(RangeType& value, const RangeType& lower, const RangeType& upper)
{
    return RangeCheckDesc<RangeType>(value, lower, upper);
}

/**
 * Outputs RangeCheckDesc in human-readable format for debugging purposes
 * @param stream Stream object
 * @param value The RangeCheckDesc to output
 * @return stream Stream object
 */
template <typename RangeType>
std::ostream& operator<<(std::ostream& stream, const RangeCheckDesc<RangeType>& value)
{
	/*qpid::types::Variant::Map map;
	map["lowerBound"] = value.m_lowerBound;
	map["upperBound"] = value.m_upperBound;
	map["value"] = value.m_value;
	stream << map;
	return stream;*/

	return stream;
}

}
}

#endif	/* ITCM_COMMON_RANGECHECKDESC_H */

