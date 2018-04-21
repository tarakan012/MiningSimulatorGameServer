#include "Kernel.h"
#include "UserManager.h"
//#include <codecvt>
#include "boost/shared_array.hpp"

namespace MS {
    namespace Server {

        void CKernel::MainLoop() {
            while (true) {
                CUserManager::GetInstance().OnHeartBeatImmediately();
                sleep(1);
            }
        }

        void CKernel::HandleStop() {
            m_pAcceptor->close();
        }


        void CKernel::PostMsgToGC(int n32ConnID, google::protobuf::MessageLite &rMsg, int n32MsgID) {
            int n32MsgLen = rMsg.ByteSize();
            int n32Len = n32MsgLen + 2 * sizeof(int);
            char *lshBuffer(new char[n32Len]);
            std::memcpy(lshBuffer + 0 * sizeof(int), (char *) &n32Len, sizeof(int));
            std::memcpy(lshBuffer + 1 * sizeof(int), (char *) &n32MsgID, sizeof(int));
            bool res = rMsg.SerializeToArray(lshBuffer + 2 * sizeof(int), n32MsgLen);
            for (int i = 0; i < n32Len; i++) {
                std::cout << (int) lshBuffer[i] << "\t";
            }
            std::cout << std::endl;
            LogPrint(LogFlags::NONE, "Msg to GC: %s\n", lshBuffer);
            m_ConnectionManager.SendMsgToConnection(n32ConnID, lshBuffer, n32Len);
            delete lshBuffer;
        }

    }

}

