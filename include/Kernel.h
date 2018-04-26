#ifndef KERNEL_H
#define KERNEL_H

#define BOOST_SIGNALS_NAMESPACE boost_signals
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include "stdafx.h"
#include "PreDefine.h"
#include "UserNetInfo.h"
#include "ConnectionManager.h"

#include "google/protobuf/message_lite.h"
#include "boost/signal.hpp"
#include "boost/asio.hpp"
#include "boost/bind.hpp"

namespace MS {
    namespace Server {

#ifdef __cplusplus
        extern "C" {
#endif
        //struct SUserNetInfo;
        class CKernel {
        public:
            static CKernel &GetInstance();

            bool HandleMsgFromGC(const CHAR *pMsg, INT32 n32MsgLength, SUserNetInfo netinfo);

            void MainLoop();

            void PostMsgToGC(INT32 n32ConnID, google::protobuf::MessageLite &rMsg, INT32 n32MsgID);

            void HandleStop();

            CConnectionManager &GetConnectionMgr() { return m_ConnectionManager; };

            void InitNetService(INT32 n32ThreadCount);

            bool Initialize();

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