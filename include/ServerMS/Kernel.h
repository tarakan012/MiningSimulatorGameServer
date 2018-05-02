#ifndef KERNEL_H
#define KERNEL_H

#define BOOST_SIGNALS_NAMESPACE boost_signals
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include "boost/signal.hpp"
#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/thread.hpp"

namespace Network {
    class CConnectionManager;

    class CConnection;
}

namespace ServerMS {

    class CUserManager;

    class CConfigManager;

    class CShopManager;

    class CKernel : public boost::enable_shared_from_this<CKernel>, private boost::noncopyable {
    public:

        bool HandleMsgFromGC(const CHAR *pMsg, INT32 n32MsgLength, SUserNetInfo netinfo);

        void MainLoop();

        void NetServiceRun();

        void PostMsgToGC(INT32 n32ConnID, google::protobuf::MessageLite &rMsg, INT32 n32MsgID);

        void HandleStop();

        boost::shared_ptr<CConfigManager> &GetConfMgr() { return m_pConfMgr; }

        boost::shared_ptr<CShopManager> &GetShopMgr() { return m_pShopMgr; }

        boost::shared_ptr<CUserManager> &GetUsrMgr() { return m_pUsrMgr; }

        boost::shared_ptr<Network::CConnectionManager> &GetConnMgr() { return m_pConnMgr; };

        void InitNetService(INT32 n32ThreadCount);

        bool Initialize();

        CKernel();


    private:

        void PrepareForNextAccept();

        void HandleAccept(const boost::system::error_code &ec);

    private:
        boost::shared_ptr<boost::asio::signal_set> m_pSignalQuit;
        boost::shared_ptr<boost::asio::ip::tcp::acceptor> m_pAcceptor;
        boost::shared_ptr<boost::asio::ip::tcp::socket> m_pSocket;
        boost::shared_ptr<boost::asio::io_service> m_pIOService;
        boost::thread_group m_NetServiceThreadGroup;
        boost::atomic_int m_n32AsioThreadCount;
        boost::shared_ptr<Network::CConnection> m_pNewConn;
        GCMsgHandlerMap m_GCMsgHandlerMap;
        boost::shared_ptr<Network::CConnectionManager> m_pConnMgr;
        boost::shared_ptr<CConfigManager> m_pConfMgr;
        boost::shared_ptr<CUserManager> m_pUsrMgr;
        boost::shared_ptr<CShopManager> m_pShopMgr;
    };

}

#endif /* KERNEL_H */