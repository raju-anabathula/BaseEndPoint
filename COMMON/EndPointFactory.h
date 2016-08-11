
#ifndef END_POINT_FACTORY_H_
#define END_point_FACTORY_H_

#include "EndPoint.h"

namespace ipc
{

class EndPointFactory
{
	private:
		EndPointFactory();
		virtual ~EndPointFactory();

		static EndPointFactory *m_EndPointFactoryInstance;

	public:
		enum EndPointType
		{
			IPC_PIPE,
			IPC_FIFO,
			IPC_SM,
			IPC_MQ,
			IPC_FL,
			TCP_IP,
			IPC_UDS,
			EMP_CLASSD,
		};

		static EndPointFactory* getEndPointFactory();

		EndPoint* createEndPointInstance(EndPointFactory::EndPointType endPointType);
		EndPoint* createSocketEndPointInstance(EndPointFactory::EndPointType endPointType, EndPoint::ProtocolType protocolType, EndPoint::SocketType socketType);
};

}
#endif /* END_point_FACTORY_H_ */
