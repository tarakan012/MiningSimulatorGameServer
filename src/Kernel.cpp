#include "Kernel.h"
#include "Logging.h"
#include "ConfigManager.h"
#include "UserManager.h"
#include "ScriptManager.h"
#include "Json.h"
#include "google/protobuf/util/json_util.h"
#include "google/protobuf/message.h"
#include <string>
#include <tinyformat.h>
#include "GCToGS.pb.h"
#include "boost/thread.hpp"
#include "boost/atomic.hpp"

namespace MS {
    namespace Server {


        static CKernel *pKInstance = nullptr;
        static boost::asio::io_service *g_stcpIOService = nullptr;
        static boost::thread *g_stcNetServiceThread[64] = {0};
        static boost::atomic_int g_stcAsioThreadCount(0);

        CKernel &CKernel::GetInstance() {
            if (NULL == pKInstance) {
                pKInstance = new CKernel;
            }
            return *pKInstance;
        }

        CKernel::CKernel()
                : m_pAcceptor(NULL),
                  m_pSocket(NULL),
                  m_pSignalQuit(NULL) {
            g_stcpIOService = new boost::asio::io_service;
        }

        void NetServiceRun() {
            g_stcAsioThreadCount++;
            g_stcpIOService->run();
        }

        void CKernel::InitNetService(INT32 n32ThreadCount) {
            for (INT32 i = 0; i < n32ThreadCount; ++i) {
                g_stcNetServiceThread[i] = new boost::thread(&NetServiceRun);
            }
        }

        bool CKernel::Initialize() {
            bool ret = CConfigManager::GetInstance().Initialize();
            if (ret) {
                LogPrint(LogFlags::ALL, "Config Initialize Success\n");
            } else {

            }
            CUserManager::GetInstance().RegisterMsgHandle(m_GCMsgHandlerMap);
            CUserManager::GetInstance().Initialize();
            CScriptManager::GetInstance().Initialize();
            InitNetService(1);
            m_pAcceptor = new boost::asio::ip::tcp::acceptor(*g_stcpIOService);
            m_pSignalQuit = new boost::asio::signal_set(*g_stcpIOService);
            m_pSignalQuit->add(SIGQUIT);
            m_pSignalQuit->async_wait(boost::bind(&CKernel::HandleStop, this));
            m_pSocket = new boost::asio::ip::tcp::socket(*g_stcpIOService);

            boost::asio::ip::tcp::resolver resolver(*g_stcpIOService);
            boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({"127.0.0.1", "1973"});
            m_pAcceptor->open(endpoint.protocol());
            m_pAcceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
            m_pAcceptor->bind(endpoint);
            m_pAcceptor->listen();
            PrepareForNextAccept();
            MainLoop();
        }

        void CKernel::PrepareForNextAccept() {
            m_shpNewConnecion.reset(new CConnection(*g_stcpIOService));
            m_pAcceptor->async_accept(m_shpNewConnecion->GetSocket(),
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
                m_ConnectionManager.Start(m_shpNewConnecion);
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
}