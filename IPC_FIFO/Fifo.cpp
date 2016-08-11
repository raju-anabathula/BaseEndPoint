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
#include <unistd.h>
#include <stdio.h>

using namespace ipc;
using namespace utils;
using namespace ITCM::ConfigConstants;

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


	if(m_RWFD == -1)
		Log::error(__FILE__, __LINE__, "Fifo is not opened in read/write mode.");

//	bytesRead = read(m_RWFD, buffer, 1024);

	// read data until length bytes
	struct emp_message_part_before_length *before_len_struct = (emp_message_part_before_length*) malloc(sizeof(emp_message_part_before_length));
	bytesRead = read(m_RWFD, before_len_struct, sizeof(emp_message_part_before_length));

	if(bytesRead != -1)
	{
		// copy to buffer
//		buffer = (boost::uint8_t*) malloc(sizeof(before_len_struct));
//		memcpy(buffer, before_len_struct, bytesRead);
		totalBytesRead += bytesRead;

		// read length bytes
		bytesRead = read(m_RWFD, &dataLength, DATA_LENGTH_BYTES);
//		memcpy(buffer + totalBytesRead, &dataLength, bytesRead);
		totalBytesRead += bytesRead;

		cout << "dataLength:" << dataLength << endl;
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


	}

	boost::shared_array<boost::uint8_t> messageBytes(new boost::uint8_t[totalBytesRead]);
//	memcpy(messageBytes.get(), buffer, bytesRead);
	EmpFullMessage *message = new EmpFullMessage(messageBytes.get(), bytesRead);

	cout<<"bytesRead: "<<bytesRead<<endl;

	if(bytesRead != -1)
	{
		cout<<"ProtocolVersion:" << message->GetProtocolVersion() << endl;
		cout<<"MessageType:" << message->GetMessageType() << endl;
		cout<<"MessageVersion:" << message->GetMessageVersion() << endl;
		cout<<"Flags:" << message->GetFlags() << endl;
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
