/*

 * TestIPCTechniques.cpp
 *
 *  Created on: 25-Jul-2016
 *      Author: shruthikac
 */

#include "../COMMON/EndPointFactory.h"
#include "../LOG/Log.h"
#include "../DYN_CONFIG/include/ConfigurationData.h"

#include <stdio.h>
#include <gtest/gtest.h>
#include <LOG/Log.h>

using namespace ipc;
using namespace utils;

EndPointFactory *factory = EndPointFactory::getEndPointFactory();
EndPoint *msgQueue = factory->createEndPointInstance(EndPointFactory::IPC_MQ);
EndPoint *fileLockingReadTest = factory->createEndPointInstance(EndPointFactory::IPC_FL);
EndPoint *fileLockingWriteTest = factory->createEndPointInstance(EndPointFactory::IPC_FL);

const boost::uint8_t m_protocolVersion = 2;		// 0x02
const boost::uint16_t m_messageType = 1;			// 0x00 0x01
const boost::uint8_t m_messageVersion = 3;		// 0x03
const boost::uint8_t m_flags = 9;				// 0x09
const boost::uint32_t m_dataLength = 4;			// 0x00, 0x00, 0x04
const boost::uint32_t m_messageNumber = 19;		// 0x00, 0x00, 0x00, 0x13
const boost::uint32_t m_messageTime = 5000;		// 0x00, 0x00, 0x13, 0x88
const boost::uint8_t m_varHeaderSize = 32;		// 0x20
const boost::uint16_t m_ttl = 99;				// 0x00, 0x63

const boost::uint8_t m_qosClass = 3;			// binary: 011
const boost::uint8_t m_qosPriority = 2;		// binary: 010
const boost::uint8_t m_qosNP = 1;				// binary: 001
const boost::uint8_t m_qosSH = 9;				// binary: 1001
const boost::uint8_t m_qosSvcReq = 7;			// binary: 111

const boost::uint16_t m_qos = 0xF253;

const string m_source = "SOURCE.ADDRESS";		// 0x53 4F 55 52 43 45 2E 41 44 44 52 45 53 53 00, size 15
const string m_destination = "DEST.ADDRESS";	// 0x44 45 53 54 2E 41 44 44 52 45 53 53 00, size 13

const boost::uint32_t m_dataIntegrity = 0;		// 0x00, 0x00, 0x00, 0x00

int main(int argc, char **argv)
{
	printf("Running main() from TestIPCTechniques.cpp\n");
	//  utils::Log::init("./testlog");

	EndPoint::initConfigItems("/home/araju/workspace/BaseEndPoint/XML/config.xml");

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


/*void fillMessageStruct(struct message_struct *msg_t, int actualMessageId, int messageId, string strData, int sequenceNumber)
{
	msg_t->ActualMessageId = actualMessageId;
	msg_t->MessageId = messageId;
	msg_t->seq_nbr = sequenceNumber;

	strcpy(msg_t->data, strData.c_str());
}*/

bool compareMessages(EmpFullMessage *srcMessage, EmpFullMessage *destMessage)
{
	/*cout << "ActualMessageId:" << srcMessage->ActualMessageId << "::" << destMessage->ActualMessageId << endl;
	cout << "MessageId:" << srcMessage->MessageId << "::" << destMessage->MessageId << endl;
	cout << "seq_nbr:" << srcMessage->seq_nbr << "::" << destMessage->seq_nbr << endl;
	cout << "data:" << srcMessage->data << "::" << destMessage->data << endl;*/

	if(srcMessage->GetProtocolVersion() 	== destMessage->GetProtocolVersion() 	&&
	   srcMessage->GetMessageNumber() 		== destMessage->GetMessageNumber() 		&&
	   srcMessage->GetMessageVersion() 		== destMessage->GetMessageVersion() 	&&
	   strcmp((const char*)srcMessage->GetBody().get(), (const char*)destMessage->GetBody().get()) == 0 &&
	   srcMessage->GetFlags() 				== destMessage->GetFlags() 				&&
	   srcMessage->GetDataLength() 			== destMessage->GetDataLength() 		&&
	   srcMessage->GetMessageTime() 		== destMessage->GetMessageTime() 		&&
	   srcMessage->GetVariableHeaderSize() 	== 0 									&&
	   srcMessage->GetDataIntegrity() 		== destMessage->GetDataIntegrity())
	{
		return true;
	}
	else
	{
		return false;
	}
}

/****************************** Test program for Message queue ******************************************/
/*
void *sender(void * i)
{
	sleep(1);
	msgQueue->writeData("1");
	msgQueue->writeData("2");
	msgQueue->writeData("3");
	msgQueue->writeData("4");
	pthread_exit(NULL);
}

void *receiver(void * i)
{
	EXPECT_EQ(msgQueue->readData(),"1");
	EXPECT_EQ(msgQueue->readData(),"2");
	EXPECT_EQ(msgQueue->readData(),"3");
	EXPECT_EQ(msgQueue->readData(),"4");
	pthread_exit(NULL);
}
*/

TEST(IPCFactory,MessageQueue)
{
	/*EndPoint *msgQueueTechnique = factory->createEndPointInstance(EndPointFactory::IPC_MQ);
	msgQueueTechnique->setName("/myqueue1123");
	msgQueueTechnique->initialize();

	pthread_t p[2];
	pthread_create(&p[0],NULL,sender,NULL);
	pthread_join(p[0],NULL);
	pthread_create(&p[1],NULL,receiver,NULL);
	pthread_join(p[1],NULL);

	msgQueueTechnique->destroy();*/
}



/******************************* Test program for Pipe **************************************************/
/*TEST(IPCFactory, Pipe)
{
	EndPoint *pipeEndpoint = factory->createEndPointInstance(EndPointFactory::IPC_PIPE);
	pipeEndpoint->initialize("");

	cout << "init finished" << endl;

	struct message_struct msgToWrite;
	fillMessageStruct(&msgToWrite, 1, 1, "Pipe Test Message", 1);

	cout << "write finished" << endl;

	pipeEndpoint->writeData(&msgToWrite);

	EXPECT_TRUE(compareMessages(pipeEndpoint->readData(), &msgToWrite));

	pipeEndpoint->destroy();
}*/


/**************************** Test programs for FIFO ******************************************/

TEST(IPCFactory, NamedPipe)
{
	EndPoint *fifoEndPoint = factory->createEndPointInstance(EndPointFactory::IPC_FIFO);
	fifoEndPoint->initialize("FIFO_NAME");

	string body = "";
	int bodyLength = body.length() + 1;

	boost::shared_array<boost::uint8_t> messageBody(new boost::uint8_t[bodyLength]);
	memcpy(messageBody.get(), body.c_str(), bodyLength);

	EmpFullMessage message;

	message.SetProtocolVersion(m_protocolVersion);
	message.SetMessageType(m_messageType);
	message.SetMessageVersion(m_messageVersion);
	message.SetFlags(m_flags);
	message.SetMessageNumber(m_messageNumber);
	message.SetMessageTime(m_messageTime);
	message.SetBody(messageBody, bodyLength);

	fifoEndPoint->writeData(&message);

	EXPECT_TRUE(compareMessages(fifoEndPoint->readData(), &message));

	fifoEndPoint->destroy();
}

/*TEST(IPCFactory, NamedPipeMultiProcess)
{
	pid_t childPID;

	EndPoint *fifo1 = EndPointFactory::getEndPointFactory()->createEndPointInstance(EndPointFactory::IPC_FIFO);
	fifo1->initialize("FIFO1");

	EndPoint *fifo2 = EndPointFactory::getEndPointFactory()->createEndPointInstance(EndPointFactory::IPC_FIFO);
	fifo2->initialize("FIFO2");

	struct message_struct *messageFromClient, *messageFromServer;
	struct message_struct msgToServer, msgToClient;

	fillMessageStruct(&msgToServer, 1, 1, "Hello server", 1);
	fillMessageStruct(&msgToClient, 1, 1, "Hello client", 1);

	childPID = fork();

	if(childPID == 0)
	{
		while(true)
		{
			static int dClientMsgIndex = 0;
			sleep(1);

			fifo1->writeData(&msgToServer);

			messageFromServer = fifo2->readData();
			if(messageFromServer != NULL)
			{
				cout << "messageFromServer:" << messageFromServer->data << endl;
				EXPECT_TRUE(strcmp(messageFromServer->data, "Hello client") == 0);
			}

			dClientMsgIndex ++;
			if(dClientMsgIndex == 5)
				break;
		}
	}
	else
	{
		while(true)
		{
			static int dServerMsgIndex = 0;
			sleep(1);

			messageFromClient = fifo1->readData();

			if(messageFromClient != NULL)
			{
				cout << "messageFromClient:" << messageFromClient->data << endl;
				EXPECT_TRUE(strcmp(messageFromClient->data, "Hello server") == 0);
			}
			fifo2->writeData(&msgToClient);

			dServerMsgIndex ++;
			if(dServerMsgIndex == 5)
				break;
		}
	}

	fifo1->destroy();
	fifo2->destroy();
}*/



/********************* Test program for Shared memory **************************************/

/*TEST(IPCFactory, SharedMemory)
{
	EndPoint *shmMemoryTechnique = factory->createEndPointInstance(EndPointFactory::IPC_SM);

	shmMemoryTechnique->initialize("SHMOBJ_PATH");
	shmMemoryTechnique->writeData("Hello 123");
	shmMemoryTechnique->writeData("write shared memory");
	EXPECT_EQ(shmMemoryTechnique->readData(),"write shared memory");
}*/



/******************** Test programs for File locking **************************************/

/*void *fileRead(void * i)
{
	unsigned int thread_id = (unsigned int)pthread_self();
	EXPECT_TRUE(fileLockingReadTest->readData().find("locking provides"));
	Log::info(__FILE__, __LINE__, ((string)("Current Thread id " + thread_id)).c_str());
	pthread_exit(NULL);
}


void *fileWrite(void * i)
{

	unsigned int thread_id = (unsigned int)pthread_self();
	Log::info(__FILE__, __LINE__, ((string)("Current Thread id " + thread_id)).c_str());
	fileLockingWriteTest->writeData(" IPC file locking Technique");
	pthread_exit(NULL);
}


TEST(IPCFactory, FileRead)
{
	fileLockingReadTest->initialize("FILE_PATH");

	pthread_t read[2];
	pthread_create(&read[0],NULL,fileRead,NULL);
	pthread_create(&read[1],NULL,fileRead,NULL);
	pthread_join(read[0],NULL);
	pthread_join(read[1],NULL);
}

// Test program for File locking using multiple threads
TEST(IPCFactory, FileWrite)
{
	string outString;
	fileLockingWriteTest->initialize("FILE_WRITE");
	pthread_t write[2];
	pthread_create(&write[0],NULL,fileWrite,NULL);
	pthread_create(&write[1],NULL,fileWrite,NULL);
	pthread_join(write[0],NULL);
	pthread_join(write[1],NULL);
	outString=fileLockingWriteTest->readData();
	EXPECT_TRUE(outString.find("IPC file locking Technique"));
}

// Test program for File locking using multiple processes
TEST(IPCFactory, FileWriteMultipleProcess)
{
	string outString;
	EndPoint *fileLockingProcessTest = factory->createEndPointInstance(EndPointFactory::IPC_FL);

	fileLockingProcessTest->initialize("FILE_WRITE");

	pid_t pid=fork();
	if(pid ==0)
	{
		fileLockingProcessTest->writeData(" child process");
	}
	else
	{
		fileLockingProcessTest->writeData(" Parent process");
	}
	outString=fileLockingProcessTest->readData();
	EXPECT_TRUE(outString.find("child process"));
}*/

