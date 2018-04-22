#include "ConnectionManager.h"

namespace MS {
    namespace Server {

        boost::atomic_int CConnectionManager::n32CurNunConn(0);

        CConnectionManager::CConnectionManager() {
            m_setConnections.clear();
        }

        void CConnectionManager::Start(ConnectionPtr shpCon) {

            shpCon->SetConnID(n32CurNunConn);
            n32CurNunConn++;
            m_setConnections.insert(shpCon);
            shpCon->Start();
        }

        void CConnectionManager::Stop(ConnectionPtr shpCon) {
            m_setConnections.erase(shpCon);
            shpCon->Stop();
        }

        void CConnectionManager::SendMsgToConnection(int n32ConnID, const char *cpBuffer, int n32BufferLen) {
            for (auto &conn : m_setConnections) {
                if (n32ConnID == conn->GetConnID()) {
                    conn->Send(cpBuffer, n32BufferLen);
                    return;
                }
            }
        }
    }
}