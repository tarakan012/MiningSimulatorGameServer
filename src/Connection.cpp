#include "Kernel.h"
#include <iostream>
#include "util.h"

namespace MS {
    namespace Server {

        CConnection::CConnection(boost::asio::io_service &io_service)
                : m_Socket(io_service), m_pGCInfo(new CGCInfo()) {

        }

        CConnection::~CConnection() {
            delete m_pGCInfo;
        }

        void CConnection::SetConnID(int n32ConnID) {
            m_pGCInfo->SetGCID(n32ConnID);
        }

        int CConnection::GetConnID() {
            return m_pGCInfo->GetGCID();
        }

        void CConnection::Send(const char *pBuffer, int n32Len) {
            m_Socket.async_send(boost::asio::buffer(pBuffer, n32Len),
                                boost::bind(&CConnection::HandleWrite, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
        }

        void CConnection::Start() {
            m_Socket.async_read_some(boost::asio::buffer(m_Buffer),
                                     boost::bind(&CConnection::HandleRead, this,
                                                 boost::asio::placeholders::error,
                                                 boost::asio::placeholders::bytes_transferred));
        }

        void CConnection::HandleRead(const boost::system::error_code &error, std::size_t bytes_transferred) {
            if (!error) {
                bool ret = CKernel::GetInstance().HandleMsgFromGC(m_Buffer.data(),
                                                                  bytes_transferred, m_pGCInfo);
                //assert(ret);
            } else {

            }
        }

        void CConnection::HandleWrite(const boost::system::error_code &error, size_t length) {
            if (!error) {
                LogPrint(LogFlags::ALL, "Send Msg Succes, size %d\n", length);
            } else {
                LogPrint(LogFlags::ALL, "Send Msg Fail, error %d\n", error);
            }
        }
    }
}