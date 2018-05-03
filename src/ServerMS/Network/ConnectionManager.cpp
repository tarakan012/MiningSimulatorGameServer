#include "ServerMS/Network/ConnectionManager.h"
#include "ServerMS/Network/Connection.h"
#include "ServerMS/Kernel.h"

namespace Network {

    boost::atomic_int CConnectionManager::n32CurNunConn(0);

    CConnectionManager::CConnectionManager(boost::shared_ptr<ServerMS::CKernel> pKernel)
            : m_pKernel(pKernel) {
        m_ConnectionSet.clear();
    }

    void CConnectionManager::Start(boost::shared_ptr<CConnection> shpCon) {

        shpCon->SetConnID(n32CurNunConn);
        n32CurNunConn++;
        m_ConnectionSet.insert(shpCon);
        shpCon->Start();
    }

    void CConnectionManager::Stop(boost::shared_ptr<CConnection> shpCon) {
        m_ConnectionSet.erase(shpCon);
        shpCon->Stop();
    }

    void CConnectionManager::SendMsgToConnection(int n32ConnID, const char *cpBuffer, int n32BufferLen) {
        for (auto &conn : m_ConnectionSet) {
            if (n32ConnID == conn->GetConnID()) {
                conn->Send(cpBuffer, n32BufferLen);
                return;
            }
        }
    }

}