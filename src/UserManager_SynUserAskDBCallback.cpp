#include <tinyformat.h>
#include "UserManager.h"
#include "UserDbDataMgr.h"

namespace MS {
    namespace Server {

        void CUserManager::SynUserAskDBCallBack() {
            while (!m_DBCallbackQueue.empty()) {
                CBuffer *pBuffer = m_DBCallbackQueue.back();
                LogPrint(LogFlags::ALL, "SynUserAskDBCallBack pBuffer->LogLvl: %d\n", pBuffer->m_LogLvl);
                switch (pBuffer->m_LogLvl) {
                    case DBToGS::eQueryAllAccount_CallBack : {
                        SynHandleAllAccountCallback(pBuffer);
                        m_DBCallbackQueue.pop();
                        break;
                    }
                    case DBToGS::eQueryUser_DBCallBack : {
                        SynHandleQueryUserCallback(pBuffer);
                        m_DBCallbackQueue.pop();
                        break;
                    }
                    default : {
                        break;
                    }
                }
            }
        }

        void CUserManager::SynHandleQueryUserCallback(CBuffer *pBuffer) {

            boost::shared_ptr<DBToGS::QueryUser> sQueryUser = ParseProtoMsgInThread<DBToGS::QueryUser>(
                    pBuffer->GetCurData(), pBuffer->GetDataLength());
            CUser *lpUser = new CUser();
            AddUser(lpUser);
            SUserNetInfo sUserNetInfo(sQueryUser->gcnetid());
            SUserDBData sUserDBData;
            for (INT32 i = 0; i < sQueryUser->computer_size(); ++i) {
                auto comp = sQueryUser->mutable_computer(i);
                INT32 dbid = comp->id();
                sUserDBData.CompInfoMap[i].n32DBId = dbid;
                for (INT32 j = 0; j < comp->item_size(); ++j) {
                    sUserDBData.CompInfoMap[i].ItemRecordMap[j].n32DBId = comp->mutable_item(j)->id();
                    sUserDBData.CompInfoMap[i].ItemRecordMap[j].n32MiningGold = comp->mutable_item(j)->mining_gold();
                    //sUserDBData.CompInfoMap[i].ItemRecordMap[j].szName = comp->mutable_item(i)->name();
                }
            }
            std::memcpy(&sUserDBData, sQueryUser->db().c_str(), sQueryUser->db().size());
            lpUser->LoadDBData(sUserDBData);
            lpUser->OnOnline(sUserNetInfo);
        }

        void CUserManager::DBAsyn_QueryWhenThreadBegin() {
            CDBConnector *pDBConnector = m_UserCacheDBActiveWrapper->GetDBConnector();
            pDBConnector->ExecQuery("select max(id) from (select unnest(comp_id) as id from game_user) as foo");
            pDBConnector->GetQueryFieldData("max", m_n32MaxComputerID);
            pDBConnector->CloseQuery();
            pDBConnector->ExecQuery("select max(id) from (select unnest(item_id) as id from computer_user) as foo");
            pDBConnector->GetQueryFieldData("max", m_n32MaxItemDBID);
            pDBConnector->CloseQuery();
            LogPrint(LogFlags::ALL, "MaxComputerID: %d\n", m_n32MaxComputerID);
        }

        bool CUserManager::DBPoster_UpdateUser(CUser *pUser) {
            SUserDBData &rsUserDBData = pUser->GetUserDBData();
            CUserDbDataMgr::UpdateUserDbData(rsUserDBData, m_SaveUserStream);
            if (!m_SaveUserStream.str().empty()) {
                GSToDB::UserUpdate sUpdateUser;

                sUpdateUser.set_sql(m_SaveUserStream.str());
                m_UserCacheDBActiveWrapper->EncodeAndSendToDBThread(sUpdateUser, sUpdateUser.msgid());
            }
        }

        void CUserManager::GPSThreadBeginCallback() {
            CDBConnector *pGameDBConnector = m_LoginDBWrapper->GetDBConnector();
            std::stringstream SqlStr;
            SqlStr << "select id, name_googleplus, id_gps, nickname  from account_user";
            pGameDBConnector->ExecQuery(SqlStr.str());
            INT32 row = pGameDBConnector->GetQueryFieldRowNum();
            DBToGS::QueryAllAccount sQueryAllAccount;
            for (INT32 i = 0; i < row; ++i) {
                auto pAccount = sQueryAllAccount.add_account();

                INT32 id = 0;
                pGameDBConnector->GetQueryFieldData("id", id);
                pAccount->set_id(id);
                std::string name_googleplus;
                pGameDBConnector->GetQueryFieldData("name_googleplus", name_googleplus);
                pAccount->set_namegoogleplus(name_googleplus);
                INT32 id_gps = 0;
                pGameDBConnector->GetQueryFieldData("id_gps", id_gps);
                pAccount->set_id_gps(id_gps);
                std::string nickname;
                pGameDBConnector->GetQueryFieldData("nickname", nickname);
                pAccount->set_nickname(nickname);
                pGameDBConnector->QueryNext();
            }
            pGameDBConnector->CloseQuery();
            if (row > 0) {
                EncodeAndSendToLogicThread(sQueryAllAccount, sQueryAllAccount.id());
            }

        }

        void CUserManager::PostSaveCmd() {

        }
    }
}

