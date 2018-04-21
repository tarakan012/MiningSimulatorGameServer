#ifndef KERNEL_H
#define KERNEL_H

#define BOOST_SIGNALS_NAMESPACE boost_signals
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include "UserNetInfo.h"
#include "ConnectionManager.h"
#include "stdafx.h"

#include "boost/signal.hpp"
#include "boost/asio.hpp"
#include "boost/bind.hpp"

#include <cstdlib>
#include <map>

#include "google/protobuf/message.h"

namespace MS {
    namespace Server {

#ifdef __cplusplus
        extern "C" {
#endif

        class CKernel {
        public:
            static CKernel &GetInstance();

            bool HandleMsgFromGC(const char *pMsg, int n32MsgLength, SUserNetInfo netinfo);

            void MainLoop();

            void PostMsgToGC(int n32ConnID, google::protobuf::MessageLite &rMsg, int n32MsgID);

            void HandleStop();

            void InitNetService(int n32ThreadCount);

            bool Initialize();
            //~Ckernel();
        private:
            CKernel();

            void PrepareForNextAccept();

            void HandleAccept(const boost::system::error_code &ec);

        private:
            boost::asio::ip::tcp::acceptor *m_pAcceptor;
            boost::asio::ip::tcp::socket *m_pSocket;
            ConnectionPtr m_shpNewConnecion;
            CConnectionManager m_ConnectionManager;
            GCMsgHandlerMap m_GCMsgHandlerMap;
            boost::asio::signal_set *m_pSignalQuit;
        };


#ifdef __cplusplus
        }
#endif

    }
}

#endif /* KERNEL_H */

