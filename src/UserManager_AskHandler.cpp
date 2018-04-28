#include "UserManager.h"

namespace MS {
    namespace Server {

        bool CUserManager::UserAskLogin(GCToGS::AskLogin &rLogin, SUserNetInfo netinfo) {
            SUserCombineKey sUserCombineKey(rLogin.namegoogleplus(), rLogin.id_gps());
            auto iter = m_AllUserNameIDMap.find(sUserCombineKey);
            if (iter != m_AllUserNameIDMap.end()) {
                LogPrintDebug("User Account Exist\n");
                INT32 n32DBGameUserID = iter->second;
                GSToDB::QueryUserReq sQueryUser;
                //sQueryUser.set_logininfo(rLogin.SerializeAsString());
                sQueryUser.set_gcnetid(netinfo.n32GCConnID);
                sQueryUser.set_gudbid(n32DBGameUserID);
                GetNowWorkActor().EncodeAndSendToDBThread(sQueryUser, sQueryUser.msgid());
            } else {
                LogPrintDebug("User Account No Exist\n");
                UserPtr pUser{new CUser()};
                INT32 guid = CombineGameUserID();
                m_AllUserNameIDMap.insert(std::make_pair(sUserCombineKey, guid));
                SUserDBData &sUserDBData = CConfigManager::GetInstance().GetStartSet();
                sUserDBData.sPODUserDBData.n32DBId = guid;
                AddUser(pUser);
                pUser->LoadDBData(sUserDBData);
                InsertNewUserToDB(rLogin, pUser, guid);
                pUser->OnOnline(netinfo);
            }
        }
    }
}
