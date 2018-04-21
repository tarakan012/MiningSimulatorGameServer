#include "UserManager.h"
#include "google/protobuf/message.h"
#include "ParseProto.h"
#include "AllErrorCode.h"

namespace MS {
    namespace Server {

        int CUserManager::OnMSgFromGC_AskLogin(const char *pMsg, int n32MsgLength, SUserNetInfo netinfo) {
            std::string lstrMsg(pMsg, n32MsgLength);
            GCToGS::AskLogin lMsg;
            if (!ParseProtoMsg(pMsg, n32MsgLength, lMsg)) {
                LogPrint(LogFlags::ALL, "Login Fail with Msg Analysis Error\n");
                return 0;
            }
            int n32RetFlag = UserAskLogin(lMsg, netinfo);
            if (eNormal != n32RetFlag) {

            }
        }

        int CUserManager::OnMsgFromGC_AskUpdateItemRet(const char *pMsg, int n32MsgLength, CConnection *pConn) {
/*            std::string lstrMsg(pMsg,n32MsgLength);
            GCToGS::AskUpdateItemRet lMsg;
            google::protobuf::util::JsonStringToMessage(lstrMsg,&lMsg);
            int userId = lMsg.userid();
            int itemId = lMsg.itemid();
            DBSynQueryAskUserItem(userId,itemId);*/
            //CUser user;
            //user.AskUpdateItem(lMsg,pConn);

        }

        int CUserManager::OnMSgFromGC_AskStartMining(const char *pMsg, int n32MsgLength, const SUserNetInfo netinfo) {

            CUser *pcUser = CheckAndGetUserByNetInfo(netinfo);

            if (pcUser == NULL) {

                return 0;
            }
            GCToGS::AskStartMining sMsg;
            if (!ParseProtoMsg(pMsg, n32MsgLength, sMsg)) {
                return 0;
            }

            int n32RetFlag = pcUser->StartMiningByComputer(sMsg);
            if (eNormal != n32RetFlag) {
                PostMsgToGC_AskReturn(netinfo, sMsg.msgid(), n32RetFlag);
            }
            return 1;
        }


    }
}

