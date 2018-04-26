#include "UserManager.h"
#include "google/protobuf/message.h"
#include "ParseProto.h"
#include "AllErrorCode.h"
#include "ShopManager.h"

namespace MS {
    namespace Server {

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
            CUser *pcUser = CheckAndGetUserByNetInfo(netinfo);

            if (pcUser == NULL) return 0;
            GCToGS::AskBuyItem sMsg;
            if (!ParseProtoMsg(pMsg, n32MsgLength, sMsg)) return 0;

            INT32 n32RetFlag = CShopManager::GetInstance().Buy(pcUser, sMsg.itemid());
            if (eNormal != n32RetFlag) {
                PostMsgToGC_AskReturn(netinfo, sMsg.msgid(), n32RetFlag);
            }
            return 1;
        }

        INT32 CUserManager::OnMsgFromGC_AskUpdateItemRet(const CHAR *pMsg, INT32 n32MsgLength, CConnection *pConn) {
        }

        INT32
        CUserManager::OnMSgFromGC_AskStartMining(const CHAR *pMsg, INT32 n32MsgLength, const SUserNetInfo netinfo) {
            CUser *pcUser = CheckAndGetUserByNetInfo(netinfo);

            if (pcUser == NULL) {

                return 0;
            }
            GCToGS::AskStartMining sMsg;
            if (!ParseProtoMsg(pMsg, n32MsgLength, sMsg)) {
                return 0;
            }

            INT32 n32RetFlag = pcUser->StartMiningByComputer(sMsg);
            if (eNormal != n32RetFlag) {
                PostMsgToGC_AskReturn(netinfo, sMsg.msgid(), n32RetFlag);
            }
            return 1;
        }
    }
}