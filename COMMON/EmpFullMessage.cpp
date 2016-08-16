#include "EmpFullMessage.h"

#include <cstddef>
#include <sstream>
#include <string>
#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_array.hpp>
#include "ITCM/Configuration/ConfigConstants.h"
#include <ITCM/common/ByteStream.h>
#include <ITCM/common/Preconditions.h>
#include <ITCM/Configuration/ConfigConstants.h>
#include <ITCM/Errors/InvalidMessage.h>

using namespace std;

namespace ITCM
{

using namespace ConfigConstants;

namespace
{
const size_t TTL_BYTES = 2;
const size_t QOS_BYTES = 2;

const size_t CLASS_BITS = 3;
const size_t PRIORITY_BITS = 3;
const size_t SPECIAL_HANDLING_BITS = 4;

const boost::uint8_t QOS_CLASS_MAX_VALUE = 1 << CLASS_BITS;
const boost::uint8_t QOS_PRIORITY_MAX_VALUE = 1 << PRIORITY_BITS;
const boost::uint8_t QOS_NETWORK_PREFERENCE_MAX_VALUE = 1 << NETWORK_PREFERENCE_BITS;
const boost::uint8_t QOS_SPECIAL_HANDLING_MAX_VALUE = 1 << SPECIAL_HANDLING_BITS;
const boost::uint8_t QOS_SERVICE_REQUEST_MAX_VALUE = 1 << SERVICE_REQUEST_BITS;
const size_t SOURCE_ADDRESS_MAX_LENGTH = 64;
const size_t DESTINATION_ADDRESS_MAX_LENGTH = 64;

const boost::uint8_t SERV_REQUEST_MASK = 0x07;
const boost::uint8_t SPEC_HANDLING_MASK = 0x0F;
const boost::uint8_t NET_PREF_MASK = 0x07;
const boost::uint8_t PRIORITY_MASK = 0x07;
const boost::uint8_t CLASS_MASK = 0x07;

const char NULL_TERMINATOR = '\0';
}

EmpFullMessage::EmpFullMessage()
: m_protocolVersion(0), m_messageType(0),
m_messageVersion(0), m_flags(0),
m_dataLength(0), m_messageNumber(0),
m_messageTime(0), m_varHeaderSize(0),
m_ttl(0), m_qosClass(0), m_qosPriority(0),
m_qosNetworkPreference(0), m_qosSpecialHandling(0),
m_qosServiceRequest(0), m_dataIntegrity(0)
{
}

EmpFullMessage::EmpFullMessage(boost::uint8_t const* bytes, boost::uint32_t messageSize)
: m_ttl(0), m_qosClass(0),
m_qosPriority(0), m_qosNetworkPreference(0),
m_qosSpecialHandling(0), m_qosServiceRequest(0)
{
    Common::ByteStream stream(bytes, messageSize);

    char* currentField = "Protocol Version";
    try
    {
        stream >> m_protocolVersion;
        SetProtocolVersion(m_protocolVersion);

        currentField = "Message Type";
        stream >> m_messageType;
        SetMessageType(m_messageType);

        currentField = "Message Version";
        stream >> m_messageVersion;
        SetMessageVersion(m_messageVersion);

        currentField = "Flags";
        stream >> m_flags;
        SetFlags(m_flags);

        currentField = "Data Length";
        stream >> Common::UnsignedInteger<DATA_LENGTH_BYTES > (m_dataLength);

        currentField = "Message Number";
        stream >> m_messageNumber;
        SetMessageNumber(m_messageNumber);

        currentField = "Message Time";
        stream >> m_messageTime;

        currentField = "Variable Header Size";
        stream >> m_varHeaderSize;

        if (HasVariableHeader())
        {
            currentField = "Time-To-Live";
            stream >> m_ttl;

            currentField = "Quality of Service";
            stream >> m_qosBits;

            SetQoSBits(m_qosBits);

            currentField = "Addresses";
            boost::uint8_t remainingVarBytes = m_varHeaderSize - TTL_BYTES - QOS_BYTES;
            stream 
                >> Common::InputString(m_sourceAddress, SOURCE_ADDRESS_MAX_LENGTH, true)
                >> Common::InputString(m_destinationAddress, DESTINATION_ADDRESS_MAX_LENGTH, true);

            // +1 to each string for the null terminator
            boost::uint8_t varBytesUsed = m_sourceAddress.length() + 1
                    + m_destinationAddress.length() + 1;

            if (varBytesUsed != remainingVarBytes)
            {
                InvalidMessage e("More bytes specified in the variable header than used; " + boost::lexical_cast<string > (remainingVarBytes - varBytesUsed) + " bytes remaining in buffer");
                THROW(e);
            }
        }

        currentField = "Body";
        m_body.reset(new boost::uint8_t[m_dataLength]);
        boost::uint32_t bytesRead = stream.ReadBuffer(m_body.get(), m_dataLength);
        REQUIRE_EQUAL(bytesRead, m_dataLength, "Invalid Body size specified");

        currentField = "Data Integrity";
        stream >> m_dataIntegrity;
    }
    catch (Common::PreconditionFailed&)
    {
        InvalidMessage e("Message construction failed while parsing: " + string(currentField));
        THROW(e);
    }

    if (stream.GetBufferLength() > 0)
    {
        // throw error
        InvalidMessage e("More bytes specified than used; " + boost::lexical_cast<string > (stream.GetBufferLength()) + " bytes remaining in buffer. Current message is: " + AsString());
        THROW(e);
    }
}

EmpFullMessage::~EmpFullMessage() throw ()
{
}

void EmpFullMessage::AsBytes(boost::uint8_t* buffer, boost::uint32_t expectedLength)
{
    boost::uint32_t messageSize = GetMessageSize();
    REQUIRE_EQUAL(expectedLength, messageSize, "Buffer provided to AsBytes() must be size expected of message");

    Common::ByteStream stream(messageSize);
    stream << m_protocolVersion;

    stream << m_messageType << m_messageVersion
            << m_flags;

    stream << Common::UnsignedInteger<DATA_LENGTH_BYTES > (m_dataLength);

    stream << m_messageNumber << m_messageTime
            << m_varHeaderSize;

    if (HasVariableHeader())
    {
        stream << m_ttl << GetQoSBits();
        stream << Common::OutputString(m_sourceAddress, true) << Common::OutputString(m_destinationAddress, true);
    }

    stream.WriteBuffer(m_body.get(), m_dataLength);
    stream << m_dataIntegrity;

    boost::uint32_t totalBytes = stream.GetBufferLength();
    REQUIRE_EQUAL(messageSize, totalBytes, "MessageSize and Serialized message must be same size");    

    stream.ReadBuffer(buffer, totalBytes);
}

boost::uint32_t EmpFullMessage::GetMessageSize() const{
    boost::uint32_t messageSize = 0;
    messageSize += sizeof (m_protocolVersion)
            + sizeof (m_messageType)
            + sizeof (m_messageVersion)
            + sizeof (m_flags)
            + DATA_LENGTH_BYTES
            + sizeof (m_messageNumber)
            + sizeof (m_messageTime)
            + sizeof (m_varHeaderSize)
            + m_varHeaderSize
            + m_dataLength
            + sizeof (m_dataIntegrity);

    return messageSize;
}

string EmpFullMessage::AsString() const
{
    stringstream ss;
    ss << "Emp Message: \n"
            << "\tProtocol Version: " << static_cast<int> (m_protocolVersion) << "\n"
            << "\tMessage Type: " << m_messageType << "\n"
            << "\tMessage Version: " << static_cast<int> (m_messageVersion) << "\n"
            << "\tFlags: " << hex << showbase << static_cast<int> (m_flags) << std::dec << "\n"
            << "\tData Length: " << m_dataLength << "\n"
            << "\tMessage Number: " << m_messageNumber << "\n"
            << "\tMessage Time: " << m_messageTime << "\n"
            << "\tVariable Header Size: " << static_cast<int> (m_varHeaderSize) << "\n";

    if (HasVariableHeader())
    {
        ss << "\tTTL: " << m_ttl << '\n'
                << "\tQoS Class: " << static_cast<int> (m_qosClass) << '\n'
                << "\tQoS Priority: " << static_cast<int> (m_qosPriority) << '\n'
                << "\tQoS Network Preference: " << static_cast<int> (m_qosNetworkPreference) << '\n'
                << "\tQoS Special Handling: " << static_cast<int> (m_qosSpecialHandling) << '\n'
                << "\tQoS Service Request: " << static_cast<int> (m_qosServiceRequest) << '\n'
                << "\tSource Address: " << m_sourceAddress << '\n'
                << "\tDestination Address: " << m_destinationAddress << '\n';
    }

    // body
    //	ss << "\tBody: " << hex << noshowbase << "\n\t";
    //	const static boost::uint8_t WIDTH = 6;
    //	for (boost::uint32_t i = 0; i < m_dataLength; ++i)
    //	{
    //		if (((i % WIDTH) == 0) && (i != 0))
    //		{
    //			ss << "\n\t";
    //		}
    //		else
    //		{
    //			ss << ' ';
    //		}
    //
    //		ss << setw(2) << setfill('0') << static_cast<int>(m_body[i]);
    //	}
    //	ss << '\n';

    ss << "\tData Integrity: " << m_dataIntegrity;
    return ss.str();
}

boost::uint8_t EmpFullMessage::GetProtocolVersion() const
{
    return m_protocolVersion;
}

void EmpFullMessage::SetProtocolVersion(boost::uint8_t protocolVersion)
{
    m_protocolVersion = protocolVersion;
}

boost::uint16_t EmpFullMessage::GetMessageType() const
{
    return m_messageType;
}

void EmpFullMessage::SetMessageType(boost::uint16_t messageType)
{
    m_messageType = messageType;
}

boost::uint8_t EmpFullMessage::GetMessageVersion() const
{
    return m_messageVersion;
}

void EmpFullMessage::SetMessageVersion(boost::uint8_t messageVersion)
{
    m_messageVersion = messageVersion;
}

boost::uint8_t EmpFullMessage::GetFlags() const
{
    return m_flags;
}

void EmpFullMessage::SetFlags(boost::uint8_t flags)
{
    m_flags = flags;
}

boost::uint32_t EmpFullMessage::GetDataLength() const
{
    return m_dataLength;
}

boost::uint32_t EmpFullMessage::GetMessageNumber() const
{
    return m_messageNumber;
}

void EmpFullMessage::SetMessageNumber(boost::uint32_t messageNumber)
{
    m_messageNumber = messageNumber;
}

boost::uint32_t EmpFullMessage::GetMessageTime() const
{
    return m_messageTime;
}

void EmpFullMessage::SetMessageTime(boost::uint32_t messageTime)
{
    m_messageTime = messageTime;
}

boost::uint8_t EmpFullMessage::GetVariableHeaderSize() const
{
    return m_varHeaderSize;
}

void EmpFullMessage::ClearVariableHeader()
{
    m_qosClass = 0;
    m_qosPriority = 0;
    m_qosNetworkPreference = 0;
    m_qosSpecialHandling = 0;
    m_qosServiceRequest = 0;

    m_sourceAddress.clear();
    m_destinationAddress.clear();

    m_varHeaderSize = 0;
}

boost::uint16_t EmpFullMessage::GetTtl() const
{
    if (HasVariableHeader())
    {
        return m_ttl;
    }

    return 0;
}

void EmpFullMessage::SetTtl(boost::uint16_t ttl)
{
    m_ttl = ttl;

    if (!HasVariableHeader())
    {
        CalculateVariableHeaderSize();
    }
}

boost::uint8_t EmpFullMessage::GetQoSClass() const
{
    return m_qosClass;
}

void EmpFullMessage::SetQoSClass(boost::uint8_t qosClass)
{
    REQUIRE_TRUE(qosClass < QOS_CLASS_MAX_VALUE, "qosClass is larger than the maximum allowed");

    m_qosClass = qosClass;

    if (!HasVariableHeader())
    {
        CalculateVariableHeaderSize();
    }
}

boost::uint8_t EmpFullMessage::GetQoSPriority() const
{
    return m_qosPriority;
}

void EmpFullMessage::SetQoSPriority(boost::uint8_t qosPriority)
{
    REQUIRE_TRUE(qosPriority < QOS_PRIORITY_MAX_VALUE, "qosPriority is larger than the maximum allowed");

    m_qosPriority = qosPriority;

    if (!HasVariableHeader())
    {
        CalculateVariableHeaderSize();
    }
}

boost::uint8_t EmpFullMessage::GetQoSNetworkPreference() const
{
    return m_qosNetworkPreference;
}

void EmpFullMessage::SetQoSNetworkPreference(boost::uint8_t qosNetworkPreference)
{
    REQUIRE_TRUE(qosNetworkPreference < QOS_NETWORK_PREFERENCE_MAX_VALUE, "qosNetworkPreference is larger than the maximum allowed");

    m_qosNetworkPreference = qosNetworkPreference;

    if (!HasVariableHeader())
    {
        CalculateVariableHeaderSize();
    }
}

boost::uint8_t EmpFullMessage::GetQoSSpecialHandling() const
{
    return m_qosSpecialHandling;
}

void EmpFullMessage::SetQoSSpecialHandling(boost::uint8_t qosSpecialHandling)
{
    REQUIRE_TRUE(qosSpecialHandling < QOS_SPECIAL_HANDLING_MAX_VALUE, "qosSpecialHandling is larger than the maximum allowed");

    m_qosSpecialHandling = qosSpecialHandling;

    if (!HasVariableHeader())
    {
        CalculateVariableHeaderSize();
    }
}

boost::uint8_t EmpFullMessage::GetQoSServiceRequest() const
{
    return m_qosServiceRequest;
}

void EmpFullMessage::SetQoSServiceRequest(boost::uint8_t qosServiceRequest)
{
    REQUIRE_TRUE(qosServiceRequest < QOS_SERVICE_REQUEST_MAX_VALUE, "qosServiceRequest is larger than the maximum allowed");

    m_qosServiceRequest = qosServiceRequest;

    if (!HasVariableHeader())
    {
        CalculateVariableHeaderSize();
    }
}

const string& EmpFullMessage::GetSourceAddress() const
{
    return m_sourceAddress;
}

void EmpFullMessage::SetSourceAddress(const string& address)
{
    REQUIRE_TRUE(address.length() < SOURCE_ADDRESS_MAX_LENGTH, "Source address is too long");

    m_sourceAddress = address;

    CalculateVariableHeaderSize();
}

const string& EmpFullMessage::GetDestinationAddress() const
{
    return m_destinationAddress;
}

void EmpFullMessage::SetDestinationAddress(const string& address)
{
    REQUIRE_TRUE(address.length() < DESTINATION_ADDRESS_MAX_LENGTH, "Destination address is too long");

    m_destinationAddress = address;

    CalculateVariableHeaderSize();
}

boost::shared_array<boost::uint8_t> EmpFullMessage::GetBody() const
{
    return m_body;
}

void EmpFullMessage::SetBody(boost::shared_array<boost::uint8_t> bytes, boost::uint32_t length)
{
    REQUIRE_TRUE(length < MAX_BODY_LENGTH, "Body Size too large");
    m_body = bytes;
    m_dataLength = length;
}

boost::uint32_t EmpFullMessage::GetDataIntegrity() const
{
    return m_dataIntegrity;
}

void EmpFullMessage::SetDataIntegrity(boost::uint32_t dataIntegrity)
{
    m_dataIntegrity = dataIntegrity;
}

boost::uint16_t EmpFullMessage::GetQoSBits() const
{
    boost::uint16_t qosBits = 0;
	qosBits |= m_qosServiceRequest;
	
	qosBits <<= SPECIAL_HANDLING_BITS;
	qosBits |= m_qosSpecialHandling;

	qosBits <<= NETWORK_PREFERENCE_BITS;
	qosBits |= m_qosNetworkPreference;

	qosBits <<= PRIORITY_BITS;
	qosBits |= m_qosPriority;

	qosBits <<= CLASS_BITS;
	qosBits |= m_qosClass;

    return qosBits;
}

void EmpFullMessage::SetQoSBits(boost::uint16_t bits)
{
	m_qosClass = bits & CLASS_MASK;
	bits >>= CLASS_BITS;

	m_qosPriority = bits & PRIORITY_MASK;
	bits >>= PRIORITY_BITS;

	m_qosNetworkPreference = bits & NET_PREF_MASK;
	bits >>= NETWORK_PREFERENCE_BITS;

	m_qosSpecialHandling  = bits & SPEC_HANDLING_MASK;
	bits >>= SPECIAL_HANDLING_BITS;

	m_qosServiceRequest = bits & SERV_REQUEST_MASK;
}

bool EmpFullMessage::HasVariableHeader() const
{
    return m_varHeaderSize > 0;
}

void EmpFullMessage::CalculateVariableHeaderSize()
{
    // fixedSize + sourceSize + destinationSize
    m_varHeaderSize = (TTL_BYTES + QOS_BYTES) + (m_sourceAddress.length() + 1) + (m_destinationAddress.length() + 1);
}

}
