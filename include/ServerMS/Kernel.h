#ifndef KERNEL_H
#define KERNEL_H

#define BOOST_SIGNALS_NAMESPACE boost_signals
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include "ServerMS/Network/ConnectionManager.h"
#include "boost/signal.hpp"
#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/thread.hpp"

namespace ServerMS {

    class CKernel {
    public:
        static CKernel &GetInstance();

        bool HandleMsgFromGC(const CHAR *pMsg, INT32 n32MsgLength, SUserNetInfo netinfo);

        void MainLoop();

        void NetServiceRun();

        void PostMsgToGC(INT32 n32ConnID, google::protobuf::MessageLite &rMsg, INT32 n32MsgID);

        void HandleStop();

        Network::CConnectionManager &GetConnectionMgr() { return m_ConnectionManager; };

        void InitNetService(INT32 n32ThreadCount);

        bool Initialize();

    private:
        CKernel();

        void PrepareForNextAccept();

        void HandleAccept(const boost::system::error_code &ec);

    private:
        boost::shared_ptr<boost::asio::ip::tcp::acceptor> m_pAcceptor;
        boost::shared_ptr<boost::asio::ip::tcp::socket> m_pSocket;
        Network::ConnectionPtr m_shpNewConnecion;
        boost::thread_group m_NetServiceThreadGroup;
        boost::shared_ptr<boost::asio::io_service> m_pIOService;
        boost::atomic_int m_n32AsioThreadCount;
        Network::CConnectionManager m_ConnectionManager;
        GCMsgHandlerMap m_GCMsgHandlerMap;
        boost::shared_ptr<boost::asio::signal_set> m_pSignalQuit;
    };

}

#endif /* KERNEL_H */