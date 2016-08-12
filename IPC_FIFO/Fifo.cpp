/*
 * Fifo.cpp
 * Created on: 19-Jul-2016
 * Author: Raju Anabathula
 * Purpose: This class implements the Unix named pipe (FIFO) mechanism by exploiting the default system calls.
 */

#include "Fifo.h"
#include "LOG/Log.h"
#include "itcm/common/ByteStream.h"

#include <sys/stat.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

using namespace ipc;
using namespace utils;
using namespace ITCM::ConfigConstants;
using namespace ITCM::Common;

#define FIFO_CONFIG_GROUP "FIFO"

Fifo::Fifo()
{
	m_RWFD = -1;
}

Fifo::~Fifo()
{
}

/****************************************************************************************************
 * Function name	: init
 * Description		: This function initializes the file name needed for the read/write operations.
 * Parameters		: void
 * Return value		: void
 ******************************************************************************************************/
void Fifo::initialize(string strName)
{
	m_strFifoName = objConfigurationData->getStringTypeItemValue(FIFO_CONFIG_GROUP, strName, "fifo");

	if(mknod(m_strFifoName.c_str(), S_IFIFO | 0666, 0) < 0)
		Log::error(__FILE__, __LINE__, "cannot create fifo");

	m_RWFD = open(m_strFifoName.c_str(), O_RDWR | O_NONBLOCK);
}

/****************************************************************************************************
 * Function name	: readData
 * Description		: This function is used read the written data from the FIFO
 * Parameters		: void
 * Return value		: string - data read from the FIFO
 ******************************************************************************************************/
EmpFullMessage* Fifo::readData()
{
	ssize_t bytesRead, totalBytesRead = 0;

	boost::uint32_t dataLength;
	boost::uint8_t buffer[1024];
	boost::uint8_t varHeader;
	boost::uint8_t *body;
	boost::uint32_t integrity;


	boost::uint8_t m_protocolVersion;
	boost::uint16_t m_messageType;
	boost::uint8_t m_messageVersion;
	boost::uint8_t m_flags;
	boost::uint32_t m_dataLength;
	boost::uint32_t m_messageNumber;
	boost::uint32_t m_messageTime;
	boost::uint8_t m_varHeaderSize;
	boost::uint32_t m_dataIntegrity;

	if(m_RWFD == -1)
		Log::error(__FILE__, __LINE__, "Fifo is not opened in read/write mode.");

	bytesRead = read(m_RWFD, buffer, sizeof(emp_message_part_before_length) - 1);
	totalBytesRead += bytesRead;

	bytesRead = read(m_RWFD, buffer + totalBytesRead, DATA_LENGTH_BYTES);

	ByteStream lengthStream(buffer + totalBytesRead, bytesRead);
	lengthStream >> Common::UnsignedInteger<DATA_LENGTH_BYTES > (m_dataLength);

	cout << "data length:" << static_cast<int>(m_dataLength) << endl;
	totalBytesRead += bytesRead;

	bytesRead = read(m_RWFD, buffer + totalBytesRead, sizeof(emp_message_part_after_length));
	totalBytesRead += bytesRead;

	bytesRead = read(m_RWFD, buffer + totalBytesRead, sizeof(m_varHeaderSize));

	ByteStream varHeaderStream(buffer + totalBytesRead, bytesRead);
	varHeaderStream >> m_varHeaderSize;

	cout << "var header size:" << static_cast<int>(m_varHeaderSize) << endl;
	totalBytesRead += bytesRead;

	bytesRead = read(m_RWFD, buffer + totalBytesRead, m_varHeaderSize + m_dataLength + sizeof(m_dataIntegrity));
	totalBytesRead += bytesRead;

	cout << "total bytes:" << totalBytesRead << endl;


//	bytesRead = read(m_RWFD, buffer, 1024);
//	bytesRead = read(m_RWFD, &m_protocolVersion, sizeof(m_protocolVersion));
//
//	bytesRead = read(m_RWFD, &m_messageType, sizeof(m_messageType));
//
//	bytesRead = read(m_RWFD, &m_messageVersion, sizeof(m_messageVersion));
//
//	bytesRead = read(m_RWFD, &m_flags, sizeof(m_flags));



	// read data until length bytes
//	struct emp_message_part_before_length *before_len_struct = (emp_message_part_before_length*) malloc(sizeof(emp_message_part_before_length));
//	bytesRead = read(m_RWFD, buffer, sizeof(emp_message_part_before_length));
	//memcpy(before_len_struct, buffer, bytesRead);

	/*if(bytesRead != -1)
	{
		cout << "flags:" << before_len_struct->flags << endl;
		cout << "messageType:" << before_len_struct->messageType << endl;
		cout << "messageVersion:" << before_len_struct->messageVersion << endl;
		cout << "protocolVersion:" << before_len_struct->protocolVersion << endl;

		// copy to buffer
//		buffer = (boost::uint8_t*) malloc(sizeof(before_len_struct));
//		memcpy(buffer, before_len_struct, bytesRead);
		totalBytesRead += bytesRead;

		// read length bytes
		bytesRead = read(m_RWFD, buffer + totalBytesRead, DATA_LENGTH_BYTES);
//		memcpy(buffer + totalBytesRead, &dataLength, bytesRead);
		totalBytesRead += bytesRead;

//		cout << "dataLength:" << dataLength << endl;
		// read data after length until variable header size
		struct emp_message_part_after_length *after_len_struct = (emp_message_part_after_length*) malloc(sizeof(emp_message_part_after_length));
		bytesRead = read(m_RWFD, after_len_struct, sizeof(emp_message_part_after_length));

		// append to buffer
//		memcpy((buffer + totalBytesRead), after_len_struct, bytesRead);
		totalBytesRead += bytesRead;

		if(after_len_struct->varHeaderSize > 0)
		{
			bytesRead = read(m_RWFD, &varHeader, after_len_struct->varHeaderSize);
			totalBytesRead += bytesRead;
		}

		// read body part
		body = (boost::uint8_t*) malloc(dataLength);
		bytesRead = read(m_RWFD, body, dataLength);
		totalBytesRead += bytesRead;


	}*/

	boost::shared_array<boost::uint8_t> messageBytes(new boost::uint8_t[totalBytesRead]);
	memcpy(messageBytes.get(), buffer, totalBytesRead);
	EmpFullMessage *message = new EmpFullMessage(messageBytes.get(), totalBytesRead);

	cout<<"bytesRead: "<<bytesRead<<endl;

	if(bytesRead != -1)
	{
		cout<<"ProtocolVersion:" << static_cast<int>(message->GetProtocolVersion()) << endl;
		cout<<"MessageType:" << message->GetMessageType() << endl;
		cout<<"MessageVersion:" << static_cast<int>(message->GetMessageVersion()) << endl;
		cout<<"Flags:" << static_cast<int>(message->GetFlags()) << endl;
		cout<<"MessageNumber:" << message->GetMessageNumber() << endl;
		cout<<"MessageTime:" << message->GetMessageTime() << endl;
		cout<<"Body:" << message->GetBody().get() << endl;
		cout<<"DataLength:" << message->GetDataLength() << endl;

		return message;
	}
	else
		return NULL;
}

/****************************************************************************************************
 * Function name	: writeData
 * Description		: This function is used write the data to FIFO
 * Parameters		: string - data to write to the FIFO
 * Return value		: void
 ******************************************************************************************************/
void Fifo::writeData(EmpFullMessage *message)
{
	if(m_RWFD == -1)
		Log::error(__FILE__, __LINE__, "Fifo is not opened in read/write mode.");

	boost::shared_array<boost::uint8_t> bytes(new boost::uint8_t[message->GetMessageSize()]);
	message->AsBytes(bytes.get(), message->GetMessageSize());

	if(write(m_RWFD, bytes.get(), message->GetMessageSize()) == -1)
	{
		Log::error(__FILE__, __LINE__, "cannot write to the fifo.");
	}
	else
	{
		Log::trace(__FILE__, __LINE__, "Written to fifo:");
	}
}

/****************************************************************************************************
 * Function name	: destroy
 * Description		: This function is used to deallocate the resources used in the FIFO
 * Parameters		: void
 * Return value		: void
 ******************************************************************************************************/
void Fifo::destroy()
{
	unlink(m_strFifoName.c_str());
}

bool Fifo::getCurrentState()
{
	return true;
}
