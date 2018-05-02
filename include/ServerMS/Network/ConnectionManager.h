#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <set>
#include "ServerMS/Network/Connection.h"

namespace ServerMS {
    class CKernel;
}

namespace Network {

    class CConnection;

    class CConnectionManager : public boost::enable_shared_from_this<CConnectionManager>, private boost::noncopyable {
    public:
        CConnectionManager(boost::shared_ptr<ServerMS::CKernel> pKernel);

        void Start(boost::shared_ptr<CConnection> shpCon);

        boost::shared_ptr<ServerMS::CKernel> &GetKernel() { return m_pKernel; }

        void SendMsgToConnection(INT32 n32ConnID, const CHAR *cpBuffer, INT32 n32BufferLen);

        void Stop(boost::shared_ptr<CConnection> shpCon);

        static boost::atomic_int n32CurNunConn;
    private:
        std::set<boost::shared_ptr<CConnection>> m_ConnectionSet;
        boost::shared_ptr<ServerMS::CKernel> m_pKernel;
    };

}

#endif /* CONNECTIONMANAGER_H */