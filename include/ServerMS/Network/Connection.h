#ifndef CONNECTION_H
#define CONNECTION_H

#include "boost/asio.hpp"
#include "boost/array.hpp"
#include "boost/bind.hpp"
#include "boost/noncopyable.hpp"
#include "boost/enable_shared_from_this.hpp"

namespace Network {

    class CConnectionManager;
    class CConnection : public boost::enable_shared_from_this<CConnection>, private boost::noncopyable {
    public:
        CConnection(boost::shared_ptr<CConnectionManager> pConnMgr, boost::asio::io_service &io_service);

        ~CConnection();

        void Start();

        void Stop() {
            m_Socket.close();
        }

        void Send(const char *pBuffer, INT32 n32Len);

        void Read();

        void SetConnID(INT32 n32ConnID);

        INT32 GetConnID();

        boost::asio::ip::tcp::socket &GetSocket() {
            return m_Socket;
        }

        void HandleRead(const boost::system::error_code &error,
                        std::size_t bytes_transferred);

        void HandleWrite(const boost::system::error_code &error, size_t length);

    private:
        boost::asio::ip::tcp::socket m_Socket;
        boost::array<CHAR, 8192> m_Buffer;
        boost::shared_ptr<CConnectionManager> m_pConnMgr;
        SUserNetInfo m_sUserNerInfo;
    };

    typedef boost::shared_ptr<CConnection> ConnectionPtr;

}

#endif /* CONNECTION_H */