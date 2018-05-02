#include "ServerMS/Network/Connection.h"
#include "ServerMS/Network/ConnectionManager.h"
#include "ServerMS/Log/Logging.h"
#include "ServerMS/UserManager.h"
#include "ServerMS/Kernel.h"

using namespace Log;

namespace Network {

    CConnection::CConnection(boost::shared_ptr<CConnectionManager> pConnMgr, boost::asio::io_service &io_service)
            : m_Socket(io_service), m_pConnMgr(pConnMgr) {
    }

    CConnection::~CConnection() {

    }

    void CConnection::SetConnID(int n32ConnID) {
        m_sUserNerInfo.n32GCConnID = n32ConnID;
    }

    int CConnection::GetConnID() {
        return m_sUserNerInfo.n32GCConnID;
    }

    void CConnection::Send(const char *pBuffer, int n32Len) {
        m_Socket.async_send(boost::asio::buffer(pBuffer, n32Len),
                            boost::bind(&CConnection::HandleWrite, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
    }

    void CConnection::Start() {
        Read();
    }

    void CConnection::Read() {
        m_Socket.async_read_some(boost::asio::buffer(m_Buffer),
                                 boost::bind(&CConnection::HandleRead, this,
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
    }

    void CConnection::HandleRead(const boost::system::error_code &error, std::size_t bytes_transferred) {
        if (!error) {
            bool ret = m_pConnMgr->GetKernel()->HandleMsgFromGC(m_Buffer.data(),
                                                                bytes_transferred, m_sUserNerInfo);
        } else {
            LogPrintError("Handle Read Fail, error: %s", error.message());
            ServerMS::UserPtr user = m_pConnMgr->GetKernel()->GetUsrMgr()->GetUserByNetInfo(m_sUserNerInfo);
            if (user) {
                m_pConnMgr->GetKernel()->GetUsrMgr()->RemoveUser(user);
                m_pConnMgr->GetKernel()->GetUsrMgr()->OnUserOffline(user);
                m_pConnMgr->Stop(shared_from_this());
            }
        }
    }

    void CConnection::HandleWrite(const boost::system::error_code &error, size_t length) {
        if (!error) {
            LogPrintDebug("Send Msg Succes, size %d", length);
        } else {
            LogPrintDebug("Send Msg Fail, error %d", error);
        }
        Read();
    }
}