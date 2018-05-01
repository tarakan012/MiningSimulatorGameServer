#include "ServerMS/UserManager.h"
#include "ServerMS/ParseProto.h"
#include "ServerMS/AllErrorCode.h"

using namespace Log;

namespace ServerMS {

    INT32 CUserManager::OnMSgFromGC_AskLogin(const CHAR *pMsg, INT32 n32MsgLength, SUserNetInfo netinfo) {
        std::string lstrMsg(pMsg, n32MsgLength);
        GCToGS::AskLogin lMsg;
        if (!ParseProtoMsg(pMsg, n32MsgLength, lMsg)) {
            LogPrint(LogFlags::ALL, "Login Fail with Msg Analysis Error\n");
            return 0;
        }
        INT32 n32RetFlag = UserAskLogin(lMsg, netinfo);
        if (eNormal != n32RetFlag) {

        }
    }

    INT32 CUserManager::OnMsgFromGC_AskBuyItem(const CHAR *pMsg, INT32 n32MsgLength, const SUserNetInfo netinfo) {
        UserPtr pcUser = CheckAndGetUserByNetInfo(netinfo);

        if (pcUser == NULL) return 0;
        GCToGS::AskBuyItem sMsg;
        if (!ParseProtoMsg(pMsg, n32MsgLength, sMsg)) return 0;

        INT32 n32RetFlag = pcUser->AskBuyItem(sMsg);
        PostMsgToGC_AskReturn(netinfo, sMsg.msgid(), n32RetFlag);

        return 1;
    }

    INT32
    CUserManager::OnMSgFromGC_AskStartMining(const CHAR *pMsg, INT32 n32MsgLength, const SUserNetInfo netinfo) {
        UserPtr pcUser = CheckAndGetUserByNetInfo(netinfo);

        if (pcUser == NULL) {

            return 0;
        }
        GCToGS::AskStartMining sMsg;
        if (!ParseProtoMsg(pMsg, n32MsgLength, sMsg)) {
            return 0;
        }

        INT32 n32RetFlag = pcUser->AskStartMining(sMsg);
        PostMsgToGC_AskReturn(netinfo, sMsg.msgid(), n32RetFlag);
        if (n32RetFlag == eNormal) {
            pcUser->SynPayInfo();
        }
        return 1;
    }

    INT32
    CUserManager::OnMSgFromGC_AskInstallCCInComp(const CHAR *pMsg, INT32 n32MsgLength, const SUserNetInfo netinfo) {
        UserPtr pcUser = CheckAndGetUserByNetInfo(netinfo);

        if (pcUser == NULL) {

            return 0;
        }
        GCToGS::AskInstallCCInComp sMsg;
        if (!ParseProtoMsg(pMsg, n32MsgLength, sMsg)) {
            return 0;
        }
        INT32 n32RetFlag = pcUser->AskInstallCCInComp(sMsg);
        PostMsgToGC_AskReturn(netinfo, sMsg.msgid(), n32RetFlag);

        return 1;
    }

}