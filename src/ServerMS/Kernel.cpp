#include "ServerMS/Kernel.h"
#include "ServerMS/Log/Logging.h"
#include "ServerMS/ShopManager.h"
#include "ServerMS/UserManager.h"
#include "ServerMS/Network/ConnectionManager.h"
#include "ServerMS/Network/Connection.h"
#include "GCToGS.pb.h"

using namespace Log;

namespace ServerMS {

    CKernel::CKernel()
            : m_pAcceptor(NULL),
              m_pSocket(NULL),
              m_pSignalQuit(NULL),
              m_pIOService(NULL) {

    }

    void CKernel::NetServiceRun() {
        m_n32AsioThreadCount++;
        m_pIOService->run();
    }

    void CKernel::InitNetService(INT32 n32ThreadCount) {
        for (INT32 i = 0; i < n32ThreadCount; ++i) {
            m_NetServiceThreadGroup.create_thread(boost::bind(&CKernel::NetServiceRun, this));
        }
    }

    bool CKernel::Initialize() {
        m_pConfMgr = boost::make_shared<CConfigManager>();
        bool ret = m_pConfMgr->Initialize();
        if (ret) {
            LogPrint(LogFlags::ALL, "Config Initialize Success\n");
        } else {

        }
        m_pUsrMgr = boost::make_shared<CUserManager>(shared_from_this());
        m_pUsrMgr->Initialize();
        m_pUsrMgr->RegisterMsgHandle(m_GCMsgHandlerMap);
        m_pShopMgr = boost::make_shared<CShopManager>(m_pUsrMgr);
        m_pConnMgr = boost::make_shared<Network::CConnectionManager>(shared_from_this());
        m_pIOService = boost::make_shared<boost::asio::io_service>();
        m_pAcceptor = boost::make_shared<boost::asio::ip::tcp::acceptor>(*m_pIOService);
        m_pSignalQuit = boost::make_shared<boost::asio::signal_set>(*m_pIOService);
        m_pSignalQuit->add(SIGQUIT);
        m_pSignalQuit->async_wait(boost::bind(&CKernel::HandleStop, this));
        m_pSocket = boost::make_shared<boost::asio::ip::tcp::socket>(*m_pIOService);

        boost::asio::ip::tcp::resolver resolver(*m_pIOService);
        boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({"127.0.0.1", "1973"});
        m_pAcceptor->open(endpoint.protocol());
        m_pAcceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        m_pAcceptor->bind(endpoint);
        m_pAcceptor->listen();
        PrepareForNextAccept();
        InitNetService(1);
    }

    void CKernel::PrepareForNextAccept() {
        m_pNewConn.reset(new Network::CConnection(m_pConnMgr, *m_pIOService));
        m_pAcceptor->async_accept(m_pNewConn->GetSocket(),
                                  boost::bind(&CKernel::HandleAccept, this,
                                              boost::asio::placeholders::error));
    }

    void CKernel::HandleAccept(const boost::system::error_code &ec) {
        if (!m_pAcceptor->is_open()) {
            LogPrintDebug("Acceptor Close");
            return;
        }
        if (!ec) {
            LogPrintDebug("Handle Accept Succes");
            m_pConnMgr->Start(m_pNewConn);
        } else {
            LogPrintDebug("Handle Accept Error: %s", ec.message());
        }
        PrepareForNextAccept();

    }

    bool CKernel::HandleMsgFromGC(const CHAR *pMsg, INT32 n32MsgLength, SUserNetInfo netinfo) {
        INT32 n32MsgLen = n32MsgLength - 2 * sizeof(INT32);
        INT32 n32MsgId = *(INT32 *) (pMsg + 1 * sizeof(INT32));
        LogPrintDebug("Message ID: %d", n32MsgId);
        if (!GCToGS::MsgId_IsValid(n32MsgId)) return false;
        auto iter = m_GCMsgHandlerMap.find(n32MsgId);
        if (iter != m_GCMsgHandlerMap.end()) {
            LogPrintDebug("Handle Message Find");
            (iter->second)(pMsg + 2 * sizeof(INT32), n32MsgLen, netinfo);
            return true;
        } else {
            LogPrintDebug("Handle Message No Find");
            return false;
        }
    }

}
