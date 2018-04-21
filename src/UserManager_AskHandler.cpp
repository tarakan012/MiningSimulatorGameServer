#include "UserManager.h"

namespace MS {
    namespace Server {

        bool CUserManager::UserAskLogin(GCToGS::AskLogin &rLogin, SUserNetInfo netinfo) {
            std::cout << "UserLogin call" << std::endl;
            SUserCombineKey sUserCombineKey(rLogin.namegoogleplus(), rLogin.id_gps());

            auto iter = m_AllUserNameIDMap.find(sUserCombineKey);
            if (iter == m_AllUserNameIDMap.end()) {
                LogPrint(LogFlags::ALL, "UserLogin exist\n");
                int n32DBGameUserID = iter->second;
                GSToDB::QueryUserReq sQueryUser;
                //sQueryUser.set_logininfo(rLogin.SerializeAsString());
                sQueryUser.set_gcnetid(netinfo.n32GCConnID);
                sQueryUser.set_gudbid(n32DBGameUserID);
                GetNowWorkActor().EncodeAndSendToDBThread(sQueryUser, sQueryUser.msgid());
            } else {
                std::cout << "UserLogin New" << std::endl;
                CUser *pUser = new CUser();
                int guid = CombineGameUserID();
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
