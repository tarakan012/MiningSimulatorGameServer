#include "UserManager.h"

namespace MS {
    namespace Server {

        void CUserManager::DBAsynQueryUserCallback(CBuffer *pBuffer, CDBConnector *pConnector) {
            LogPrint(LogFlags::ALL, "DBAsynQueryUserCallback\n");
            GSToDB::QueryUserReq msg;
            ParseProtoMsg(pBuffer->GetCurData(), pBuffer->GetDataLength(), msg);
            DBToGS::QueryUser sQueryUser;
            sQueryUser.set_gcnetid(msg.gcnetid());
            SUserDBData sUserDBData;
            sUserDBData.sPODUserDBData.n32DBId = msg.gudbid();
            DBAsynQueryUser(sUserDBData, sQueryUser, pConnector);
            EncodeAndSendToLogicThread(sQueryUser, sQueryUser.msgid());
        }


    }
}
