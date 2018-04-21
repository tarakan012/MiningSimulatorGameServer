#ifndef CONNECTION_H
#define CONNECTION_H

#include "GCInfo.h"

#include "boost/asio.hpp"
#include "boost/array.hpp"
#include "boost/bind.hpp"
#include "boost/shared_ptr.hpp"

#include <stdio.h>

namespace MS {
    namespace Server {

#ifdef __cplusplus
extern "C" {
#endif

        class CConnection
        {
        public:
            CConnection(boost::asio::io_service &  io_service);
            ~CConnection();
            void Start();
            void Stop()
            {
                m_Socket.close();
            }
            void Send(const char * pBuffer, int n32Len);
            void SetConnID(int n32ConnID);
            int GetConnID();
            boost::asio::ip::tcp::socket & GetSocket()
            {
                return m_Socket;
            }
            void HandleRead(const boost::system::error_code& error,
        std::size_t bytes_transferred);
            void HandleWrite( const boost::system::error_code& error, size_t length);


        private:
            boost::asio::ip::tcp::socket m_Socket;
            boost::array<char, 8192> m_Buffer;
            CGCInfo * m_pGCInfo;
        };

        typedef boost::shared_ptr<CConnection> ConnectionPtr;



#ifdef __cplusplus
}
#endif

    }
}

#endif /* CONNECTION_H */

