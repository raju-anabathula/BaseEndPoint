/*
 * Fifo.cpp
 * Created on: 19-Jul-2016
 * Author: Raju Anabathula
 * Purpose: This class implements the Unix named pipe (FIFO) mechanism by exploiting the default system calls.
 */

#include "Fifo.h"
#include "LOG/Log.h"
#include <sys/stat.h>
#include <iostream>
#include <fcntl.h>
#include <ITCM/common/ByteStream.h>
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
 * Function name	: initialize
 * Description		: This function initializes the file name needed for the read/write operations.
 * Parameters		: string - the name of the FIFO
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
 * Return value		: EmpFullMessage - data read from the FIFO
 ******************************************************************************************************/
EmpFullMessage* Fifo::readData()
{
	ssize_t bytesRead, totalBytesRead = 0;

	boost::uint8_t buffer[MAX_EMP_MESSAGE_LENGTH];
	boost::uint32_t m_dataLength;
	boost::uint8_t m_varHeaderSize;
	boost::uint32_t m_dataIntegrity;

	if(m_RWFD == -1)
	{
		Log::error(__FILE__, __LINE__, "Fifo is not opened in read/write mode.");
		return NULL;
	}

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

	boost::shared_array<boost::uint8_t> messageBytes(new boost::uint8_t[totalBytesRead]);
	memcpy(messageBytes.get(), buffer, totalBytesRead);
	EmpFullMessage *message = new EmpFullMessage(messageBytes.get(), totalBytesRead);

	if(bytesRead != -1)
		return message;
	else
		return NULL;
}

/****************************************************************************************************
 * Function name	: writeData
 * Description		: This function is used write the data to FIFO
 * Parameters		: EmpFullMessage - data to write to the FIFO
 * Return value		: void
 ******************************************************************************************************/
void Fifo::writeData(EmpFullMessage *message)
{
	if(m_RWFD == -1)
		Log::error(__FILE__, __LINE__, "Fifo is not opened in read/write mode.");

	boost::shared_array<boost::uint8_t> bytes(new boost::uint8_t[message->GetMessageSize()]);
	message->AsBytes(bytes.get(), message->GetMessageSize());

	if(message->GetMessageSize() > MAX_EMP_MESSAGE_LENGTH)
	{
		Log::trace(__FILE__, __LINE__, "Cannot send message. Message size exceeds maximum permitted length");
		return;
	}

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

/****************************************************************************************************
 * Function name 	: getCurrentState
 * Description		: This function is used to to get the current state of the object, true or false. Every thing is OK or not
 * Parameters		: void
 * Return value		: bool
 ****************************************************************************************************/
bool Fifo::getCurrentState()
{
	return true;
}
