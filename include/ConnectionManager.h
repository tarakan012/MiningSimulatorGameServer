#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <set>
#include "boost/atomic.hpp"
#include "Connection.h"

namespace MS {
    namespace Server {

#ifdef __cplusplus
        extern "C" {
#endif

        class CConnectionManager {
        public:
            CConnectionManager();

            void Start(ConnectionPtr shpCon);

            void SendMsgToConnection(int n32ConnID, const char *cpBuffer, int n32BufferLen);

            void Stop(ConnectionPtr shpCon);

            static boost::atomic_int n32CurNunConn;
        private:
            std::set<ConnectionPtr> m_setConnections;
        };


#ifdef __cplusplus
        }
#endif

    }
}

#endif /* CONNECTIONMANAGER_H */

