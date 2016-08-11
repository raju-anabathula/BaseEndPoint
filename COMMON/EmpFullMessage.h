#ifndef ITCM_EMP_FULL_MESSAGE_H
#define ITCM_EMP_FULL_MESSAGE_H

#include <string>
#include <boost/cstdint.hpp>
#include <boost/shared_array.hpp>

namespace ITCM
{

class EmpFullMessage
{
public:
    explicit EmpFullMessage();
    explicit EmpFullMessage(boost::uint8_t const* bytes, boost::uint32_t messageSize);

    ~EmpFullMessage() throw ();

    void AsBytes(boost::uint8_t* buffer, boost::uint32_t expectedLength);
    boost::uint32_t GetMessageSize() const;

    std::string AsString() const;

    boost::uint8_t GetProtocolVersion() const;
    void SetProtocolVersion(boost::uint8_t protocolVersion);

    boost::uint16_t GetMessageType() const;
    void SetMessageType(boost::uint16_t messageType);

    boost::uint8_t GetMessageVersion() const;
    void SetMessageVersion(boost::uint8_t messageVersion);

    boost::uint8_t GetFlags() const;
    void SetFlags(boost::uint8_t flags);

    boost::uint32_t GetDataLength() const;

    boost::uint32_t GetMessageNumber() const;
    void SetMessageNumber(boost::uint32_t messageNumber);

    boost::uint32_t GetMessageTime() const;
    void SetMessageTime(boost::uint32_t messageTime);

    boost::uint8_t GetVariableHeaderSize() const;
    void ClearVariableHeader();

    boost::uint16_t GetTtl() const;
    void SetTtl(boost::uint16_t ttl);

    boost::uint8_t GetQoSClass() const;
    void SetQoSClass(boost::uint8_t qosClass);

    boost::uint8_t GetQoSPriority() const;
    void SetQoSPriority(boost::uint8_t qosPriority);

    boost::uint8_t GetQoSNetworkPreference() const;
    void SetQoSNetworkPreference(boost::uint8_t qosNetworkPreference);

    boost::uint8_t GetQoSSpecialHandling() const;
    void SetQoSSpecialHandling(boost::uint8_t qosSpecialHandling);

    boost::uint8_t GetQoSServiceRequest() const;
    void SetQoSServiceRequest(boost::uint8_t qosServiceRequest);

    const std::string& GetSourceAddress() const;
    void SetSourceAddress(const std::string& address);

    const std::string& GetDestinationAddress() const;
    void SetDestinationAddress(const std::string& address);

    boost::shared_array<boost::uint8_t> GetBody() const;
    void SetBody(boost::shared_array<boost::uint8_t> bytes, boost::uint32_t length);

    boost::uint32_t GetDataIntegrity() const;
    void SetDataIntegrity(boost::uint32_t dataIntegrity);

    bool HasVariableHeader() const;

    boost::uint16_t GetQoSBits() const;

protected:
    void SetQoSBits(boost::uint16_t bits);
    void CalculateVariableHeaderSize();

    static std::string ExtractStringFromBuffer(const char* buffer, size_t bufferSize, size_t offset = 0);

private:
    boost::uint8_t m_protocolVersion;
    boost::uint16_t m_messageType;
    boost::uint8_t m_messageVersion;
    boost::uint8_t m_flags;
    boost::uint32_t m_dataLength;
    boost::uint32_t m_messageNumber;
    boost::uint32_t m_messageTime;
    boost::uint8_t m_varHeaderSize;
    boost::uint16_t m_ttl;

    // QoS fields
    boost::uint16_t m_qosBits;
    boost::uint8_t m_qosClass;
    boost::uint8_t m_qosPriority;
    boost::uint8_t m_qosNetworkPreference;
    boost::uint8_t m_qosSpecialHandling;
    boost::uint8_t m_qosServiceRequest;

    std::string m_sourceAddress;
    std::string m_destinationAddress;

    boost::shared_array<boost::uint8_t> m_body;

    boost::uint32_t m_dataIntegrity;
};

}


#endif /* ITCM_EMP_FULL_MESSAGE_H */
