#include "ServerMS/Kernel.h"
#include "ServerMS/UserManager.h"

namespace ServerMS {

    void CKernel::MainLoop() {
        while (true) {
            CUserManager::GetInstance().OnHeartBeatImmediately();
            sleep(1);
        }
    }

    void CKernel::HandleStop() {
        m_pAcceptor->close();
    }

    void CKernel::PostMsgToGC(INT32 n32ConnID, google::protobuf::MessageLite &rMsg, INT32 n32MsgID) {
        INT32 n32MsgLen = rMsg.ByteSize();
        INT32 n32Len = n32MsgLen + 2 * sizeof(INT32);
        CHAR *lshBuffer(new CHAR[n32Len]);
        std::memcpy(lshBuffer + 0 * sizeof(INT32), (CHAR *) &n32Len, sizeof(INT32));
        std::memcpy(lshBuffer + 1 * sizeof(INT32), (CHAR *) &n32MsgID, sizeof(INT32));
        bool res = rMsg.SerializeToArray(lshBuffer + 2 * sizeof(INT32), n32MsgLen);
        m_ConnectionManager.SendMsgToConnection(n32ConnID, lshBuffer, n32Len);
        delete lshBuffer;
    }

}