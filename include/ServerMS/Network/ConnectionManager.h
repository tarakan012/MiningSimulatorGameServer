#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <set>
#include "boost/atomic.hpp"
#include "ServerMS/PreDefine.h"
#include "ServerMS/Network/Connection.h"

namespace Network {

    class CConnectionManager {
    public:
        CConnectionManager();

        void Start(Network::ConnectionPtr shpCon);

        void SendMsgToConnection(INT32 n32ConnID, const CHAR *cpBuffer, INT32 n32BufferLen);

        void Stop(Network::ConnectionPtr shpCon);

        static boost::atomic_int n32CurNunConn;
    private:
        std::set<ConnectionPtr> m_setConnections;
    };

}

#endif /* CONNECTIONMANAGER_H */