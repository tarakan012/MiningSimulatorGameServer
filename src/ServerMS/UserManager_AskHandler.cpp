#include "ServerMS/UserManager.h"

using namespace Log;

namespace ServerMS {

    bool CUserManager::UserAskLogin(GCToGS::AskLogin &rLogin, SUserNetInfo netinfo) {
        SUserCombineKey sUserCombineKey(rLogin.namegoogleplus(), rLogin.id_gps());
        auto iter = m_AllUserNameIDMap.find(sUserCombineKey);
        if (iter != m_AllUserNameIDMap.end()) {
            LogPrintDebug("User Account Exist");
            INT32 n32DBGameUserID = iter->second;
            GSToDB::QueryUserReq sQueryUser;
            //sQueryUser.set_logininfo(rLogin.SerializeAsString());
            sQueryUser.set_gcnetid(netinfo.n32GCConnID);
            sQueryUser.set_gudbid(n32DBGameUserID);
            GetNowWorkActor().EncodeAndSendToDBThread(sQueryUser, sQueryUser.msgid());
        } else {
            LogPrintDebug("User Account No Exist");
            UserPtr pUser{new CUser()};
            auto pcInventory = boost::make_shared<CInventory>(pUser);
            pUser->SetInventory(pcInventory);
            INT32 guid = CombineGameUserID();
            m_AllUserNameIDMap.insert(std::make_pair(sUserCombineKey, guid));
            SUserDBData sUserDBData = CConfigManager::GetInstance().GetStartSet();
            sUserDBData.sPODUserDBData.n32DBId = guid;
            INT32 comp_dbid = CombineComputerID();
            auto pcComp = boost::make_shared<CComputer>(pUser);
            pcComp->SetDbId(comp_dbid);
            pUser->InsertComputer(pcComp);
            /* TODO only one computer*/
            auto &comp = sUserDBData.CompInfoMap[0];
            comp.n32DBId = comp_dbid;
            for (auto &item_pair : comp.ItemRecordMap) {
                auto &item = item_pair.second;
                INT32 item_dbid = GenerateItemDBID();
                item.n32DBId = item_dbid;
                switch (item.n32Type) {
                    case eTypeItem_CPU: {
                        auto cpu = boost::make_shared<SCCCpu>(item);
                        pcComp->InstallCpu(cpu);
                        break;
                    }
                    case eTypeItem_GPU: {
                        auto gpu = boost::make_shared<SCCGpu>(item);
                        pcComp->InstallGpu(gpu);
                        break;
                    }
                    case eTypeItem_Matherboard: {
                        auto matherboard = boost::make_shared<SCCMatherboard>(item);
                        pcComp->ReplaceMatherbord(matherboard);
                        break;
                    }
                    default:
                        LogPrintDebug("Invalid Item Type");
                        break;
                }
            }
            pUser->LoadDBData(sUserDBData);
            AddUser(pUser);
            InsertNewUserToDB(rLogin, pUser, guid);
            pUser->OnOnline(netinfo);
        }
    }

}
